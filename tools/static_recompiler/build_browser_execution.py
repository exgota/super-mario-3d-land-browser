#!/usr/bin/env python3
"""Link a worker-only browser profile from sealed real platform archives."""
import argparse
import json
import os
from pathlib import Path
import shlex
import shutil
import subprocess
import time

from audit_webassembly_platform import digest

ROOT = Path(__file__).resolve().parents[2]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("platform", type=Path, help="successful pinned platform builder output")
    parser.add_argument("output", type=Path)
    parser.add_argument("--platform-verification", type=Path, required=True)
    parser.add_argument("--emsdk", type=Path, required=True)
    parser.add_argument("--node", type=Path, required=True)
    args = parser.parse_args()
    platform, output = args.platform.resolve(), args.output.resolve()
    if not output.is_relative_to(ROOT / "build") or output.exists():
        raise RuntimeError("output must be an absent child of this checkout's ignored build")
    verification_path = args.platform_verification.resolve()
    verification = json.loads(verification_path.read_text())
    if verification.get("passed") is not True:
        raise RuntimeError("actual platform verification is unsuccessful")
    verification_root = verification_path.parent.parent
    for relative, expected in verification["receipts_sha256"].items():
        if digest(verification_root / relative) != expected:
            raise RuntimeError(f"protected platform receipt changed: {relative}")
    receipt_paths = list(verification["receipts_sha256"])
    execution_paths = [path for path in receipt_paths if path.endswith("_execution_receipt.json")]
    audit_paths = [path for path in receipt_paths if path.endswith("_audit.json")]
    entropy_paths = [path for path in receipt_paths if "entropy/" in path and path.endswith("result.json")]
    if len(execution_paths) != 1 or len(audit_paths) != 1 or len(entropy_paths) != 1:
        raise RuntimeError("verified real execution and archive audit are unavailable or ambiguous")
    execution = json.loads((verification_root / execution_paths[0]).read_text())
    audit = json.loads((verification_root / audit_paths[0]).read_text())
    entropy = json.loads((verification_root / entropy_paths[0]).read_text())
    if not (execution.get("exit_code") == 0 and execution.get("protected_inputs_unchanged") is True
            and execution["before"] == execution["after"] and audit.get("passed") is True
            and entropy.get("passed") is True):
        raise RuntimeError("verified actual platform closure is unsuccessful")
    manifest = json.loads((platform / "build_manifest.json").read_text())
    if not (manifest.get("passed") is True and manifest["wasm_sha256"] == verification["wasm_sha256"]
            and digest(manifest["wasm"]) == manifest["wasm_sha256"]
            and digest(manifest["javascript"]) == manifest["javascript_sha256"]):
        raise RuntimeError("actual platform artifact seal differs")
    build = platform / "compiled"
    inventory_path = build / "compile_commands.json"
    if digest(inventory_path) != audit["compile_commands_sha256"]:
        raise RuntimeError("verified platform compile inventory changed")
    inventory = json.loads(inventory_path.read_text())
    entries = [entry for entry in inventory if entry["file"].endswith("/AzaharCompiledExecution.cpp")]
    if len(entries) != 1:
        raise RuntimeError("real platform main compile command is unavailable or ambiguous")
    entry = entries[0]
    original_main = Path(entry["file"])
    original_root = original_main.parents[2]
    # A new profile may reuse only byte-identical accepted host interfaces.
    runtime_seals = {}
    for original in sorted((original_root / "runtime/port").rglob("*")):
        if original.is_file() and original.suffix in (".h", ".cpp", ".c"):
            relative = original.relative_to(original_root)
            expected = digest(original)
            if digest(ROOT / relative) != expected:
                raise RuntimeError(f"accepted host interface changed: {relative}")
            runtime_seals[str(relative)] = expected
    link_path = build / "src/root_port_capture/CMakeFiles/root_port_webassembly_capture.dir/link.txt"
    link = shlex.split(link_path.read_text())
    compiler = args.emsdk.resolve() / "upstream/emscripten/em++"
    if Path(link[0]).resolve() != compiler or Path(shlex.split(entry["command"])[0]).resolve() != compiler:
        raise RuntimeError("selected compiler differs from the actual platform toolchain")
    link_directory = Path(entry["directory"])
    protected = {str(verification_path): digest(verification_path),
                 str(platform / "build_manifest.json"): digest(platform / "build_manifest.json"),
                 str(inventory_path): digest(inventory_path), str(link_path): digest(link_path),
                 str(Path(manifest["wasm"])): manifest["wasm_sha256"],
                 str(Path(manifest["javascript"])): manifest["javascript_sha256"]}
    for relative, expected in runtime_seals.items():
        protected[str(ROOT / relative)] = expected
        protected[str(original_root / relative)] = expected
    entry_source = ROOT / "runtime/port/browser/BrowserExecutionEntry.cpp"
    identity_source = ROOT / "runtime/port/browser/BrowserInputIdentity.cpp"
    for source in (entry_source, identity_source, Path(__file__).resolve()):
        protected[str(source)] = digest(source)
    for name, expected in manifest["licenses"].items():
        notice = platform / "licenses" / name
        if Path(name).name != name or digest(notice) != expected:
            raise RuntimeError(f"verified platform notice changed: {name}")
        protected[str(notice)] = expected
    for token in link:
        if token.endswith((".a", ".o")):
            path = Path(token)
            path = path if path.is_absolute() else link_directory / path
            path = path.resolve()
            actual = digest(path)
            if token.endswith(".a"):
                sealed = execution["before"].get(str(path))
                audited = audit["archives"].get(str(path.relative_to(build))) if path.is_relative_to(build) else None
                if sealed and (sealed["sha256"] != actual or sealed["bytes"] != path.stat().st_size):
                    raise RuntimeError(f"verified translation provider archive changed: {path}")
                if audited and audited["sha256"] != actual:
                    raise RuntimeError(f"verified platform provider archive changed: {path}")
                if path == build / "externals/libressl/crypto/libcrypto.a" and entropy["crypto_archive_sha256"] != actual:
                    raise RuntimeError("verified input-identity crypto provider archive changed")
            protected[str(path)] = actual
    output.mkdir(parents=True)
    environment = os.environ.copy()
    environment["EMSDK_NODE"] = str(args.node.resolve())
    commands = []

    def run(command):
        index = len(commands)
        log = output / f"command_{index}.log"
        started = time.monotonic()
        with log.open("wb") as stream:
            result = subprocess.run([str(value) for value in command], cwd=ROOT,
                                    env=environment, stdout=stream, stderr=subprocess.STDOUT)
        commands.append({"command": [str(value) for value in command], "status": result.returncode,
                         "elapsed_seconds": time.monotonic() - started, "log": str(log)})
        (output / "commands.json").write_text(json.dumps(commands, indent=2) + "\n")
        if result.returncode:
            raise RuntimeError(f"browser command {index} refused; see {log}")

    # Reproduce the entire verified Node wasm from the complete current link
    # closure. Its byte identity binds providers outside the four-archive audit,
    # rather than labeling newly observed archive hashes as historical seals.
    reproduction = output / "platform_reproduction"
    reproduction.mkdir()
    reproduced_module = reproduction / Path(manifest["javascript"]).name
    reproduction_link = []
    for token in link:
        if token.endswith((".a", ".o")):
            path = Path(token)
            token = str((path if path.is_absolute() else link_directory / path).resolve())
        reproduction_link.append(token)
    reproduction_link[reproduction_link.index("-o") + 1] = str(reproduced_module)
    run(reproduction_link)
    reproduced_wasm = reproduced_module.with_suffix(".wasm")
    if digest(reproduced_wasm) != manifest["wasm_sha256"]:
        raise RuntimeError("complete current provider closure cannot reproduce the verified platform wasm")

    flags = shlex.split(entry["command"])[1:]
    for option in ("-o", "-c"):
        position = flags.index(option)
        del flags[position:position + 2]
    old_runtime = str(original_root / "runtime/port")
    flags = [flag.replace(old_runtime, str(ROOT / "runtime/port")) for flag in flags]
    main_object = output / "AzaharCompiledExecution.o"
    entry_object = output / "BrowserExecutionEntry.o"
    identity_object = output / "BrowserInputIdentity.o"
    run([compiler, *flags, "-Dmain=RunStaticCompiledExecution",
         "-c", ROOT / "runtime/port/AzaharCompiledExecution.cpp", "-o", main_object])
    run([compiler, *flags, "-c", entry_source, "-o", entry_object])
    identity_flags = [flag for flag in flags if flag != "-w"]
    run([compiler, *identity_flags, "-Wall", "-Wextra", "-Werror",
         f"-I{platform / 'azahar_source/externals/libressl/include'}", f"-I{build / 'include'}",
         "-c", identity_source, "-o", identity_object])
    if link.count("-sENVIRONMENT=node,worker") != 1 or link.count("-sNODERAWFS=1") != 1:
        raise RuntimeError("finite Node link profile differs from the reviewed source")
    output_index = link.index("-o")
    del link[output_index:output_index + 2]
    module = output / "root_port_browser_capture.mjs"
    browser_link = [str(compiler)]
    for token in link[1:]:
        if token in ("-sENVIRONMENT=node,worker", "-sNODERAWFS=1") or token.endswith(".o"):
            continue
        if token.endswith(".a"):
            path = Path(token)
            token = str((path if path.is_absolute() else link_directory / path).resolve())
        browser_link.append(token)
    browser_link += [str(main_object), str(entry_object), str(identity_object),
                     "-sENVIRONMENT=worker", "-sMODULARIZE=1", "-sEXPORT_ES6=1",
                     "-sEXPORT_NAME=createStaticPortModule", "-sINVOKE_RUN=0", "-sFILESYSTEM=1",
                     "-lworkerfs.js", "-sEXPORTED_FUNCTIONS=['_main','_BrowserInputIdentityValidateSha256']",
                     "-sEXPORTED_RUNTIME_METHODS=['FS','WORKERFS','ENV','callMain','ccall']",
                     "-sINCOMING_MODULE_JS_API=['noInitialRun','preRun','locateFile','print','printErr','onExit','onAbort','thisProgram']",
                     "-o", str(module)]
    # The main and identity objects must precede their static providers.
    objects = [str(main_object), str(entry_object), str(identity_object)]
    browser_link = [browser_link[0], *objects, *[token for token in browser_link[1:] if token not in objects]]
    run(browser_link)
    notices = output / "licenses"
    shutil.copytree(platform / "licenses", notices)
    for name, expected in manifest["licenses"].items():
        if digest(notices / name) != expected:
            raise RuntimeError(f"copied platform notice differs: {name}")
    for path, expected in protected.items():
        if digest(path) != expected:
            raise RuntimeError(f"protected actual platform input changed: {path}")
    wasm = module.with_suffix(".wasm")
    receipt = {"passed": True, "platform_manifest_sha256": digest(platform / "build_manifest.json"),
               "platform_verification_sha256": digest(verification_path), "runtime_sources": runtime_seals,
               "reproduced_platform_wasm": str(reproduced_wasm),
               "reproduced_platform_wasm_sha256": digest(reproduced_wasm),
               "browser_entry_source_sha256": digest(entry_source),
               "input_identity_source_sha256": digest(identity_source), "protected_inputs": protected,
               "licenses": manifest["licenses"],
               "commands": commands, "module": str(module), "module_sha256": digest(module),
               "wasm": str(wasm), "wasm_sha256": digest(wasm), "wasm_bytes": wasm.stat().st_size,
               "browser_execution_verified": False,
               "scope": "Actual worker-only module link from protected real platform/translation/arithmetic archives, "
                        "after byte-identical reproduction of the entire verified Node wasm. "
                        "No browser startup, mounted-file identity, GPU replay or rendered frame is certified by linking."}
    (output / "build_manifest.json").write_text(json.dumps(receipt, indent=2) + "\n")
    print(json.dumps({key: receipt[key] for key in ("passed", "wasm_bytes", "wasm_sha256", "scope")}))


if __name__ == "__main__":
    main()
