#!/usr/bin/env python3
"""Audit the configured sources, real archives and imports of a wasm32 platform."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess

ROOT = Path(__file__).resolve().parents[2]
FORBIDDEN_SYMBOLS = re.compile(
    r"\b(dlopen|dlsym|dlclose|dlerror|dladdr|dl_iterate_phdr|LoadLibrary[A-Z]*|GetProcAddress)\b"
    r"|Common::DynamicLibrary|ARM_DynCom|ARM_Dynarmic|ARM_FastInterp|Dynarmic::")
FORBIDDEN_SOURCES = re.compile(
    r"/arm/(dyncom|dynarmic|fastinterp|skyeye_common)/"
    r"|/shader/shader_jit[^/]*\.cpp$"
    r"|/dynamic_library/(dynamic_library|ffmpeg)\.cpp$"
    r"|/dumping/ffmpeg_backend\.cpp$"
    r"|/linux/gamemode\.cpp$|/renderer_(opengl|vulkan)/")
FORBIDDEN_IMPORTS = re.compile(
    r"dlopen|dlsym|dlclose|dlerror|dladdr|dl_iterate_phdr|LoadLibrary|GetProcAddress"
    r"|Dynarmic|DynCom|FastInterp|^gl[A-Z]|emscripten_webgl")
ARCHIVES = (
    "static_arm_webassembly/libstatic_arm_webassembly.a",
    "src/core/libcitra_core.a", "src/common/libcitra_common.a",
    "src/root_port_capture/libroot_port_headless_capture.a")


def digest(path):
    checksum = hashlib.sha256()
    with Path(path).open("rb") as source:
        for block in iter(lambda: source.read(1024 * 1024), b""):
            checksum.update(block)
    return checksum.hexdigest()


class WasmReader:
    def __init__(self, data):
        self.data = data
        self.position = 0

    def take(self, size):
        end = self.position + size
        if end > len(self.data):
            raise ValueError("truncated WebAssembly section")
        value = self.data[self.position:end]
        self.position = end
        return value

    def byte(self):
        return self.take(1)[0]

    def integer(self):
        value = 0
        for shift in range(0, 35, 7):
            byte = self.byte()
            value |= (byte & 127) << shift
            if byte < 128:
                if value > 0xffffffff:
                    break
                return value
        raise ValueError("invalid wasm32 unsigned integer")

    def name(self):
        return self.take(self.integer()).decode("utf-8")

    def limits(self):
        flags = self.integer()
        if flags & ~3 or flags & 2 and not flags & 1:
            raise ValueError("unsupported wasm32 memory/table limits")
        minimum = self.integer()
        maximum = self.integer() if flags & 1 else None
        if maximum is not None and maximum < minimum:
            raise ValueError("inverted WebAssembly limits")
        return {"flags": flags, "minimum": minimum, "maximum": maximum}


def read_imports(path):
    reader = WasmReader(path.read_bytes())
    if reader.take(8) != b"\0asm\x01\0\0\0":
        raise ValueError("expected a WebAssembly version 1 module")
    imports = []
    found = False
    while reader.position < len(reader.data):
        section = reader.byte()
        payload = reader.take(reader.integer())
        if section != 2:
            continue
        if found:
            raise ValueError("duplicate WebAssembly import section")
        found = True
        body = WasmReader(payload)
        for _ in range(body.integer()):
            item = {"module": body.name(), "name": body.name(), "kind": body.byte()}
            kind = item["kind"]
            if kind == 0:
                item["type_index"] = body.integer()
            elif kind == 1:
                item["reference_type"] = body.byte()
                item["limits"] = body.limits()
            elif kind == 2:
                item["limits"] = body.limits()
            elif kind == 3:
                item["value_type"], item["mutable"] = body.byte(), body.byte()
            elif kind == 4:
                item["attribute"], item["type_index"] = body.integer(), body.integer()
            else:
                raise ValueError(f"unknown WebAssembly import kind: {kind}")
            imports.append(item)
        if body.position != len(payload):
            raise ValueError("trailing bytes in WebAssembly import section")
    return imports


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("build", type=Path)
    parser.add_argument("report", type=Path)
    parser.add_argument("--llvm-nm", type=Path, required=True)
    args = parser.parse_args()
    build, report = args.build.resolve(), args.report.resolve()
    if not report.is_relative_to(ROOT / "build") or report.exists():
        raise RuntimeError("report must be an absent child of this checkout's ignored build")
    inventory_path = build / "compile_commands.json"
    inventory = json.loads(inventory_path.read_text())
    source_hits = [entry["file"] for entry in inventory if FORBIDDEN_SOURCES.search(entry["file"])]
    archives = {}
    for relative in ARCHIVES:
        archive = build / relative
        command = [str(args.llvm_nm.resolve()), "--undefined-only", "--demangle", str(archive)]
        result = subprocess.run(command, text=True, capture_output=True, check=True)
        hits = [line for line in result.stdout.splitlines() if FORBIDDEN_SYMBOLS.search(line)]
        archives[relative] = {"sha256": digest(archive), "command": command,
                              "forbidden_undefined_symbols": hits}
    wasm = build / "bin/Release/root_port_webassembly_capture.wasm"
    imports = read_imports(wasm)
    import_hits = [item for item in imports if FORBIDDEN_IMPORTS.search(item["name"])]
    memories = [item for item in imports if item["kind"] == 2]
    expected_memory = {"flags": 3, "minimum": 16384, "maximum": 32768}
    memory_matches = len(memories) == 1 and memories[0]["limits"] == expected_memory
    passed = not source_hits and not import_hits and memory_matches and all(
        not item["forbidden_undefined_symbols"] for item in archives.values())
    receipt = {"passed": passed, "configured_source_entries": len(inventory),
               "compile_commands_sha256": digest(inventory_path), "forbidden_source_hits": source_hits,
               "archives": archives, "wasm": str(wasm), "wasm_sha256": digest(wasm),
               "wasm_bytes": wasm.stat().st_size, "imports": imports,
               "forbidden_import_hits": import_hits, "expected_shared_memory_limits": expected_memory,
               "shared_memory_matches": memory_matches,
               "scope": "Configured source inventory, archive undefined symbols and final module imports. "
                        "Inventory entries are not a compiled-unit count. This audit does not execute guest code. "
                        "PICA shader interpretation and Teakra DSP are retained; guest ARM CPU fallback is excluded."}
    report.parent.mkdir(parents=True, exist_ok=True)
    report.write_text(json.dumps(receipt, indent=2) + "\n")
    print(json.dumps({"passed": passed, "wasm_bytes": receipt["wasm_bytes"],
                      "imports": len(imports), "configured_source_entries": len(inventory)}))
    if not passed:
        raise SystemExit(1)


if __name__ == "__main__":
    main()
