#!/usr/bin/env python3
"""Exercise the real browser C stdio/LibreSSL identity ABI with small readonly fixtures."""
import argparse
import json
from pathlib import Path
import subprocess
import time
from urllib.parse import urljoin, urlsplit
from urllib.request import urlopen

from audit_webassembly_platform import digest
from browser_session_policy import BrowserSession, reject_keep_open

ROOT = Path(__file__).resolve().parents[2]
WORKER_SOURCE = r"""
self.onmessage = async event => {
    const url = event.data.module_url;
    const factory = (await import(url)).default;
    const module = await factory({noInitialRun: true, locateFile: name => new URL(name, url).href,
        preRun: [instance => {
            instance.FS.mkdir('/owned');
            instance.FS.mount(instance.WORKERFS,
                {files: [new File(['abc'], 'abc'), new File([], 'empty')]}, '/owned');
        }], print: () => {}, printErr: () => {}});
    const valid = 'ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad';
    const empty = 'e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855';
    const cases = [
        ['exact', '/owned/abc', valid, 3, 0],
        ['empty', '/owned/empty', empty, 0, 0],
        ['prefix', '/owned/abc', valid, 2, 5],
        ['short_file', '/owned/abc', valid, 4, 5],
        ['wrong_hash', '/owned/abc', '0'.repeat(64), 3, 8],
        ['malformed_hash', '/owned/abc', 'x', 3, 2],
        ['uppercase_hash', '/owned/abc', valid.toUpperCase(), 3, 2],
        ['missing_file', '/owned/missing', valid, 3, 3],
        ['relative_path', 'owned/abc', valid, 3, 1],
        ['unbounded_path', '/' + 'a'.repeat(4095), valid, 3, 1]
    ];
    const check = (path, hash, size) => module.ccall('BrowserInputIdentityValidateSha256', 'number',
        ['string', 'string', 'number'], [path, hash, size]);
    const results = cases.map(([name, path, hash, size, expected]) =>
        ({name, expected, status: check(path, hash, size)}));
    let writeDenied = false;
    try { module.FS.writeFile('/owned/abc', new Uint8Array([0])); }
    catch { writeDenied = true; }
    const unchanged = check('/owned/abc', valid, 3) === 0;
    self.postMessage({passed: results.every(record => record.expected === record.status) && writeDenied && unchanged,
        results, readonly_write_denied: writeDenied, readonly_bytes_unchanged: unchanged});
    self.close();
};
"""


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("url", help="local browser execution server")
    parser.add_argument("output", type=Path)
    parser.add_argument("--headed", action="store_true")
    args = parser.parse_args()
    reject_keep_open(getattr(args, "keep_open", False))
    url, output = urlsplit(args.url), args.output.resolve()
    if (url.scheme != "http" or url.hostname != "127.0.0.1" or url.username or url.password
            or url.query or url.fragment or url.path not in ("", "/")):
        raise ValueError("Use the local preview server origin")
    if not output.is_relative_to(ROOT / "build") or output.exists():
        raise ValueError("Output must be an absent child of this checkout's ignored build")
    source_path = Path(__file__).resolve()
    source_hash = digest(source_path)
    with urlopen(urljoin(args.url, "/configuration.json"), timeout=10) as response:
        payload = response.read(16 * 1024 * 1024 + 1)
    if len(payload) > 16 * 1024 * 1024:
        raise ValueError("Local configuration exceeds its finite bound")
    configuration = json.loads(payload)
    module_url = urljoin(args.url, configuration["module_url"])
    if configuration["schema_version"] != 1 or urlsplit(module_url).netloc != url.netloc or urlsplit(module_url).scheme != "http":
        raise ValueError("Local module origin/schema differs")
    output.mkdir(parents=True)
    with BrowserSession(output, headed=args.headed) as browser:
        run = browser.run
        (output / "worker_source.mjs").write_text(WORKER_SOURCE)
        run(["open", args.url])
        callback = "async (page) => await page.evaluate(async ({source, module_url}) => { "
        callback += "if (!crossOriginIsolated) throw new Error('Browser isolation is unavailable'); "
        callback += "const url = URL.createObjectURL(new Blob([source], {type: 'text/javascript'})); "
        callback += "const worker = new Worker(url, {type: 'module', name: 'browser-input-identity-verification'}); "
        callback += "try { return await new Promise((resolve, reject) => { "
        callback += "const deadline = setTimeout(() => reject(new Error('Identity probe timed out')), 60000); "
        callback += "worker.onmessage = event => { clearTimeout(deadline); resolve(event.data); }; "
        callback += "worker.onerror = event => { clearTimeout(deadline); reject(new Error(event.message)); }; "
        callback += "worker.postMessage({module_url}); }); } finally { worker.terminate(); URL.revokeObjectURL(url); } }, "
        callback += json.dumps({"source": WORKER_SOURCE, "module_url": module_url}) + ")"
        text = run(["run-code", callback])
        result = json.JSONDecoder().raw_decode(text.split("### Result\n", 1)[1].lstrip())[0]
        if result.get("passed") is not True or digest(source_path) != source_hash:
            raise RuntimeError("Actual input-identity ABI controls refused or verification source changed")
        result.update({"module_url": module_url, "source_sha256": source_hash,
                       "scope": "Actual browser WORKERFS/C stdio/LibreSSL ABI with small synthetic fixtures. "
                                "No owned dump or guest main is loaded; fixture-worker termination earns no capture-shutdown claim."})
        cleanup = browser.close()
        result["browser_session_policy"] = {"passed": cleanup["passed"],
                                             "cleanup_sha256": digest(output / "cleanup.json"),
                                             "registration_sha256": digest(output / "registration.json")}
    (output / "result.json").write_text(json.dumps(result, indent=2) + "\n")
    print(json.dumps(result))


if __name__ == "__main__":
    main()
