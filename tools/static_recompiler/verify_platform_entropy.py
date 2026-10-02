#!/usr/bin/env python3
"""Exercise the real wasm LibreSSL archive and its actual entropy-failure path."""
import argparse
import json
import os
from pathlib import Path
import subprocess
import time

from audit_webassembly_platform import digest

ROOT = Path(__file__).resolve().parents[2]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("platform", type=Path, help="pinned platform builder output")
    parser.add_argument("output", type=Path)
    parser.add_argument("--emsdk", type=Path, required=True)
    parser.add_argument("--node", type=Path, required=True)
    args = parser.parse_args()
    platform, output = args.platform.resolve(), args.output.resolve()
    if not output.is_relative_to(ROOT / "build") or output.exists():
        raise RuntimeError("output must be an absent child of this checkout's ignored build")
    archive = platform / "compiled/externals/libressl/crypto/libcrypto.a"
    archive_hash = digest(archive)
    source = ROOT / "tools/static_recompiler/verify_platform_entropy.c"
    output.mkdir(parents=True)
    module = output / "verify_platform_entropy.js"
    environment = os.environ.copy()
    environment["EMSDK_NODE"] = str(args.node.resolve())
    commands = []

    def run(command, expected_status):
        started = time.monotonic()
        number = len(commands)
        stdout = output / f"command_{number}_stdout.log"
        stderr = output / f"command_{number}_stderr.log"
        with stdout.open("w") as out, stderr.open("w") as err:
            try:
                result = subprocess.run([str(value) for value in command], env=environment,
                                        stdout=out, stderr=err, timeout=60)
                status, timed_out = result.returncode, False
            except subprocess.TimeoutExpired:
                status, timed_out = None, True
        commands.append({"command": [str(value) for value in command], "status": status,
                         "expected_status": expected_status, "elapsed_seconds": time.monotonic() - started,
                         "timeout_seconds": 60, "timed_out": timed_out,
                         "stdout": str(stdout), "stderr": str(stderr)})
        (output / "commands.json").write_text(json.dumps(commands, indent=2) + "\n")
        if timed_out or status != expected_status:
            raise RuntimeError(f"entropy command {number} returned {status}, timed_out={timed_out}; see {stderr}")
        return subprocess.CompletedProcess(command, status, stdout.read_text(), stderr.read_text())

    run([args.emsdk.resolve() / "upstream/emscripten/emcc", "-std=c11", "-O1", "-pthread",
         "-sPTHREAD_POOL_SIZE=2", "-sENVIRONMENT=node,worker", "-sASSERTIONS=1", "-sEXIT_RUNTIME=1",
         "-Wl,--wrap=getentropy", f"-I{platform / 'azahar_source/externals/libressl/include'}",
         f"-I{platform / 'compiled/include'}", source, archive, "-o", module], 0)
    healthy = []
    for _ in range(2):
        result = run([args.node.resolve(), module], 0)
        record = json.loads(result.stdout)
        if not (record["complete"] and record["getentropy_success_calls"] == 2
                and record["getentropy_range_refused"] and record["refused_buffer_unchanged"]
                and record["sha256_known_vector"] and record["threads"] == 2
                and record["random_samples"] == 65536 and record["random_bytes"] == 4194304
                and record["duplicate_samples"] == 0 and record["zero_samples"] == 0
                and record["observed_entropy_calls"] >= 5 and record["injected_failures"] == 0):
            raise RuntimeError("real entropy/concurrent generator result differs from the contract")
        healthy.append(record)
    c_failure = run([args.node.resolve(), module, "--force-entropy-failure"], 1)
    failure_records = [json.loads(line) for line in c_failure.stdout.splitlines()]
    if [record.get("stage") for record in failure_records] != ["before_random_call", "entropy_source_refused"]:
        raise RuntimeError("controlled C entropy source did not refuse the actual generator")
    provider_failure = run([
        args.node.resolve(), "-e",
        'require("node:crypto").randomFillSync = function() {throw new Error("controlled_node_entropy_failure");}; '
        'require(process.argv[1]);', module], 1)
    if provider_failure.stdout or "controlled_node_entropy_failure" not in provider_failure.stderr:
        raise RuntimeError("actual Node entropy provider did not refuse before success output")
    if digest(archive) != archive_hash:
        raise RuntimeError("protected real crypto archive changed")
    receipt = {"passed": True, "commands": commands, "healthy_runs": healthy,
               "source_sha256": digest(source), "crypto_archive_sha256": archive_hash,
               "javascript_sha256": digest(module), "wasm_sha256": digest(module.with_suffix(".wasm")),
               "c_entropy_failure_refused": True, "node_entropy_provider_failure_refused": True,
               "scope": "Two real Node entropy/concurrent generator executions and two real failure controls. "
                        "No statistical cryptographic-quality, TLS/network, browser Web Crypto, "
                        "allocation-failure or mutex-failure certification."}
    (output / "result.json").write_text(json.dumps(receipt, indent=2) + "\n")
    print(json.dumps({"passed": True, "healthy_runs": len(healthy), "failure_controls": 2}))


if __name__ == "__main__":
    main()
