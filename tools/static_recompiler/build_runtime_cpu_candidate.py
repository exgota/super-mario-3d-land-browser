#!/usr/bin/env python3
"""Build the bounded R2 page-table candidate from the sealed live link recipe."""

import argparse
import datetime
import difflib
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import time


BASELINE_MANIFEST_SHA256 = "188b6cb68f99f0f50c3e56f9fc87b1a8d382063be3b5acf2b6e10206c5bce84f"
ORIGINAL_ADAPTER_SHA256 = "b0a9533abfb2c681ee819bad757351f3b4ad5ef8d048f0b63f7aafd107446d3d"
ORIGINAL_HEADER_SHA256 = "411cd7664b03bc96b7c13d9e966a43224c0cad3bee74bfc36d61a26e4e239ac2"
BASELINE_MODULE_SHA256 = "34b63c89ea3b140653fd4d860e1dbc66561cb6e5928dedaa053b617452d3e92a"
BASELINE_WASM_SHA256 = "92df445a692a30e4e24ffca7c7fba573a3253a21e9e0db90e4a4e37747acd22f"
BASELINE_RECEIPT_SHA256 = "8c8626446661973aa2582ea24c136027fb3a1e1533c2b8e5ce68925fd7c73383"
COORDINATION_SCRIPT_SHA256 = "87120b74aa9624cddca830e16750b4968ecaa4d13a05b4bb55cc71601c3c5e08"


def sha256(path):
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def identity(path):
    return {"path": str(path), "bytes": path.stat().st_size, "sha256": sha256(path)}


def require_digest(path, expected):
    actual = sha256(path)
    if actual != expected:
        raise ValueError(f"Sealed input changed: {path}: {actual} != {expected}")


def replace_once(source, original, replacement):
    if source.count(original) != 1:
        raise ValueError(f"Expected exactly one source anchor: {original!r}")
    return source.replace(original, replacement, 1)


def save_receipt(path, receipt):
    temporary = path.with_suffix(".temporary")
    temporary.write_text(json.dumps(receipt, indent=2) + "\n")
    os.replace(temporary, path)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--baseline-manifest", type=Path, required=True)
    parser.add_argument("--baseline-receipt", type=Path, required=True)
    parser.add_argument("--recovery-receipt", type=Path, required=True)
    parser.add_argument("--coordination-script", type=Path, required=True)
    parser.add_argument("--reservation-token", required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    repository = Path(__file__).resolve().parents[2]
    output = args.output.resolve()
    if not output.is_relative_to(repository / "build") or output.exists():
        raise ValueError("Output must be an absent directory inside this lane's build directory")
    if os.getpriority(os.PRIO_PROCESS, 0) < 5:
        raise ValueError("Build admission requires nice priority 5 or greater")
    require_digest(args.coordination_script.resolve(), COORDINATION_SCRIPT_SHA256)
    require_digest(args.baseline_receipt.resolve(), BASELINE_RECEIPT_SHA256)
    status = json.loads(subprocess.check_output(
        [sys.executable, str(args.coordination_script.resolve()), "status"], text=True))
    reservation = status["reservation"]
    if not reservation or reservation.get("token") != args.reservation_token:
        raise ValueError("The supplied token does not own the shared reservation")
    if reservation.get("lane") != "R2" or reservation.get("kind") != "build":
        raise ValueError("The shared reservation must belong to an R2 build")
    if shutil.disk_usage(output.parent).free < 12_000_000_000:
        raise ValueError("Free disk is below the shared 12 GB admission limit")
    output.mkdir()
    builder_source = output / "builder_source_at_execution.py"
    shutil.copy2(Path(__file__).resolve(), builder_source)
    receipt_path = output / "build_manifest.json"
    receipt = {
        "passed": False,
        "utc_start": datetime.datetime.now(datetime.timezone.utc).isoformat(),
        "scope": "One exact-old-ABI adapter object. Original generated translations, scheduling, floating point and all non-CPU link inputs remain sealed. Gameplay and regression equivalence require separate observations.",
        "cpu_provider": "static translated ARM",
        "reservation": reservation,
        "commands": [],
        "browser_execution_verified": False,
        "default_capture_equivalence_verified": False,
        "builder_source": identity(builder_source),
    }
    environment = dict(os.environ, EMSDK_NODE="/opt/homebrew/bin/node")

    def run(command, label):
        log = output / f"{len(receipt['commands']):02d}_{label}.log"
        record = {"command": command, "cwd": str(repository), "log": str(log)}
        receipt["commands"].append(record)
        save_receipt(receipt_path, receipt)
        started = time.monotonic()
        with log.open("xb") as stream:
            result = subprocess.run(command, cwd=repository, env=environment,
                                    stdout=stream, stderr=subprocess.STDOUT, check=False)
        record.update(status=result.returncode, seconds=time.monotonic() - started,
                      log_identity=identity(log))
        save_receipt(receipt_path, receipt)
        if result.returncode:
            raise RuntimeError(f"{label} failed with status {result.returncode}; see {log}")
        return log

    try:
        baseline_path = args.baseline_manifest.resolve()
        require_digest(baseline_path, BASELINE_MANIFEST_SHA256)
        baseline = json.loads(baseline_path.read_text())
        baseline_module = Path(baseline["module"])
        baseline_wasm = Path(baseline["wasm"])
        require_digest(baseline_module, BASELINE_MODULE_SHA256)
        require_digest(baseline_wasm, BASELINE_WASM_SHA256)
        recovery_path = args.recovery_receipt.resolve()
        recovery = json.loads(recovery_path.read_text())
        compile_records = [record for record in recovery["commands"]
                           if any(str(argument).endswith("/StaticArmBackend.cpp")
                                  for argument in record["command"])]
        if len(compile_records) != 1 or compile_records[0]["status"] != 0:
            raise ValueError("Expected one successful original adapter compile record")
        compile_command = list(compile_records[0]["command"])
        original_source = Path(compile_command[compile_command.index("-c") + 1])
        original_header = original_source.with_suffix(".h")
        require_digest(original_source, ORIGINAL_ADAPTER_SHA256)
        require_digest(original_header, ORIGINAL_HEADER_SHA256)
        authored_source = repository / "runtime/port/StaticArmBackend.cpp"
        authored_header = authored_source.with_suffix(".h")
        if "    void RefreshMemoryPages();" not in authored_header.read_text():
            raise ValueError("The authored adapter header must declare RefreshMemoryPages")
        authored_text = authored_source.read_text()
        method_start = authored_text.index("void StaticArmBackend::RefreshMemoryPages() {")
        method_end = authored_text.index("\nvoid StaticArmBackend::Run() {", method_start)
        method = authored_text[method_start:method_end]
        method = replace_once(method, "trace || write_observation || execution_observation", "trace")
        original_text = original_source.read_text()
        candidate_text = replace_once(original_text, "void StaticArmBackend::Run() {",
                                      method + "\nvoid StaticArmBackend::Run() {")
        candidate_text = replace_once(candidate_text, "    reschedule = false;",
                                      "    RefreshMemoryPages();\n    reschedule = false;")
        candidate_text = replace_once(candidate_text, "            svc->CallSVC(context.svc);\n",
                                      "            svc->CallSVC(context.svc);\n            RefreshMemoryPages();\n")
        header_text = original_header.read_text()
        candidate_header = replace_once(header_text, "    static u8 Read8(Context*, u32);",
                                        "    void RefreshMemoryPages();\n    static u8 Read8(Context*, u32);")
        sources = output / "sources"
        sources.mkdir()
        candidate_source_path = sources / original_source.name
        candidate_header_path = sources / original_header.name
        candidate_source_path.write_text(candidate_text)
        candidate_header_path.write_text(candidate_header)
        differences = []
        for name, before, after in ((original_source.name, original_text, candidate_text),
                                    (original_header.name, header_text, candidate_header)):
            differences.extend(difflib.unified_diff(before.splitlines(True), after.splitlines(True),
                                                    fromfile=f"original/{name}", tofile=f"candidate/{name}"))
        patch_path = output / "adapter_source.patch"
        patch_path.write_text("".join(differences))
        link_command = list(baseline["commands"][-1]["command"])
        cpu_inputs = [Path(argument) for argument in link_command
                      if Path(argument).name == "libstatic_arm_webassembly.a"]
        if len(cpu_inputs) != 1:
            raise ValueError("Expected exactly one original CPU adapter archive")
        cpu_archive = cpu_inputs[0]
        protected_paths = {baseline_path, baseline_module, baseline_wasm, recovery_path,
                           original_source, original_header, authored_source, authored_header,
                           args.baseline_receipt.resolve(), args.coordination_script.resolve(),
                           Path(__file__).resolve()}
        protected_paths.update(Path(argument) for argument in link_command
                               if Path(argument).is_absolute() and Path(argument).is_file())
        for key in ("protected_inputs", "live_baseline_sources"):
            for filename, digest in baseline.get(key, {}).items():
                path = Path(filename)
                require_digest(path, digest)
                protected_paths.add(path)
        protected_paths.update(path for path in original_source.parent.iterdir()
                               if path.is_file() and path.suffix == ".h")
        before = {str(path): identity(path) for path in sorted(protected_paths)}
        receipt.update(
            protected_inputs=before, baseline_manifest=identity(baseline_path),
            common_baseline_receipt=identity(args.baseline_receipt.resolve()),
            baseline_inventory=baseline, original_compile_record=compile_records[0],
            candidate_sources=[identity(candidate_source_path), identity(candidate_header_path)],
            candidate_patch=identity(patch_path),
            adapter_abi="Original constructors, data members and virtual methods retained. One private nonvirtual method added. Context/Host/Entry ABI unchanged.",
            observer_behavior="Original trace presence keeps all-null callback pages. Committed current adapter additionally guards both newer observers, which are absent from the historical caller closure.")
        save_receipt(receipt_path, receipt)
        objects = output / "objects"
        objects.mkdir()
        object_path = objects / "StaticArmBackend.cpp.o"
        compile_command[compile_command.index("-c") + 1] = str(candidate_source_path)
        compile_command[compile_command.index("-o") + 1] = str(object_path)
        run(compile_command, "compile_adapter")
        candidate_archive = output / cpu_archive.name
        run(["/bin/cp", "-c", str(cpu_archive), str(candidate_archive)], "clone_cpu_archive")
        archive_tool = str(Path(compile_command[0]).with_name("emar"))
        members_before = run([archive_tool, "t", str(candidate_archive)], "archive_members_before")
        if members_before.read_text().splitlines().count(object_path.name) != 1:
            raise ValueError("The original archive must contain exactly one adapter member")
        run([archive_tool, "r", str(candidate_archive), str(object_path)], "replace_adapter_member")
        members_after = run([archive_tool, "t", str(candidate_archive)], "archive_members_after")
        if members_after.read_text() != members_before.read_text():
            raise ValueError("Archive member names/order changed")
        link_command[link_command.index(str(cpu_archive))] = str(candidate_archive)
        module = output / "root_port_browser_capture.mjs"
        link_command[link_command.index("-o") + 1] = str(module)
        run(link_command, "link_module")
        licenses = output / "licenses"
        licenses.mkdir()
        for name, digest in baseline["licenses"].items():
            source = baseline_path.parent / "licenses" / name
            require_digest(source, digest)
            shutil.copy2(source, licenses / name)
        after = {str(path): identity(path) for path in sorted(protected_paths)}
        if before != after:
            raise ValueError("A sealed input changed during the build")
        module_identity = identity(module)
        wasm_identity = identity(module.with_suffix(".wasm"))
        receipt.update(passed=True, module=str(module), module_identity=module_identity,
                       module_sha256=module_identity["sha256"],
                       wasm=str(module.with_suffix(".wasm")),
                       wasm_identity=wasm_identity, wasm_sha256=wasm_identity["sha256"],
                       wasm_bytes=wasm_identity["bytes"],
                       gameplay_session_supported=baseline["gameplay_session_supported"],
                       adapter_object=identity(object_path), adapter_archive=identity(candidate_archive),
                       sealed_inputs_unchanged=True, licenses=baseline["licenses"])
    except Exception as error:
        receipt["failure"] = f"{type(error).__name__}: {error}"
        raise
    finally:
        receipt["utc_end"] = datetime.datetime.now(datetime.timezone.utc).isoformat()
        receipt["final_free_bytes"] = shutil.disk_usage(output).free
        save_receipt(receipt_path, receipt)
        print(json.dumps({"passed": receipt["passed"], "manifest": str(receipt_path),
                          "failure": receipt.get("failure")}))


if __name__ == "__main__":
    main()
