#!/usr/bin/env python3
"""Translate local game code and link it natively. All output must remain ignored."""
import argparse
import csv
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys
from datetime import datetime, timezone

ROOT = Path(__file__).resolve().parents[2]
GENERATOR_REVISION = "83e6920784baff6ebc5888ed5cfdbf9735dfbf1c"
ORIGINAL_DIGEST = "e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64"
REPLACEMENTS = {0x0010766C: ("_ZN2nn2os6detail27ConvertSvcToLibraryPriorityEi",
                            "lib/CtrSDK/sources/os_Priority.cpp")}


def run(arguments, *, environment=None, output=None):
    result = subprocess.run([str(v) for v in arguments], cwd=ROOT, env=environment,
                            text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if output:
        Path(output).write_text(result.stdout + result.stderr)
    if result.returncode:
        raise RuntimeError(f"command failed ({result.returncode}): {arguments}\n{result.stderr}")
    return result.stdout


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, default=ROOT / "build/native_port")
    parser.add_argument("--optimization", choices=("0", "1", "2"), default="1")
    parser.add_argument("--without-replacements", action="store_true")
    args = parser.parse_args()
    output = args.output.resolve()
    # Never write translated instructions outside this repository's ignored build tree.
    if not output.is_relative_to(ROOT / "build") or output == ROOT / "build":
        raise RuntimeError("output must be a child of the ignored build directory")
    if output.exists():
        raise RuntimeError("output exists; choose a fresh directory to preserve earlier evidence")
    output.mkdir(parents=True)
    if run(["git", "check-ignore", str(output / "payload.c")]).strip() == "":
        raise RuntimeError("generated payload is not ignored")
    base = run(["git", "rev-parse", "main"]).strip()
    map_text = run(["git", "show", f"{base}:data/ver/eu/map.csv"])
    map_file = output / "function_map.csv"
    map_file.write_text(map_text)
    rows = [{k.strip(): v.strip() for k, v in row.items()}
            for row in csv.DictReader(map_text.splitlines())]
    replacements = ROOT / "runtime/port/ExactFunctionReplacements.cpp"
    replacement_source = replacements.read_text()
    declarations = {int(v, 16) for v in re.findall(r"RECOMP_OVERRIDE\((0x[0-9A-Fa-f]+)\)", replacement_source)}
    if declarations != set(REPLACEMENTS):
        raise RuntimeError("replacement declarations disagree with the reviewed source registry")
    source_hashes = {}
    for address, (symbol, source) in REPLACEMENTS.items():
        matches = [r for r in rows if int(r["Start"], 16) == address]
        if len(matches) != 1 or matches[0]["Rank"] != "O" or matches[0]["Symbol"] != symbol:
            raise RuntimeError(f"replacement {symbol} is not rank O on frozen main")
        committed = subprocess.check_output(["git", "show", f"{base}:{source}"], cwd=ROOT)
        if (ROOT / source).read_bytes() != committed:
            raise RuntimeError(f"native replacement source differs from frozen main: {source}")
        source_hashes[source] = hashlib.sha256(committed).hexdigest()
    code = ROOT / "data/ver/eu/code.bin"
    header = ROOT / "data/ver/eu/exh.bin"
    if digest(code) != ORIGINAL_DIGEST:
        raise RuntimeError("original executable digest disagrees with the approved EU oracle")
    selected = output / "replacement.cpp"
    selected.write_text("" if args.without_replacements else replacement_source)
    environment = os.environ.copy()
    environment.setdefault("CARGO_HOME", str(ROOT / "build/port_tools/cargo"))
    environment.setdefault("RUSTUP_HOME", str(ROOT / "build/port_tools/rustup"))
    cargo = environment.get("CARGO", str(Path(environment["CARGO_HOME"]) / "bin/cargo"))
    environment["PATH"] = str(Path(cargo).parent) + os.pathsep + environment.get("PATH", "")
    target = ROOT / "build/port_tools/static_recompiler_target"
    run([cargo, "build", "--locked", "--release", "--jobs", "1", "--manifest-path",
         ROOT / "tools/static_recompiler/Cargo.toml", "--target-dir", target],
        environment=environment, output=output / "generator_build.log")
    run([target / "release/static-recompiler", code, header, map_file, selected, output],
        output=output / "generation.log")
    # Serial compilation respects the root lane's resource allocation.
    compiler = environment.get("CC", "clang")
    objects = []
    sources = sorted(output.glob("*.c"))
    for index, source in enumerate(sources):
        object_file = source.with_suffix(".o")
        run([compiler, f"-O{args.optimization}", "-ffp-contract=off", "-fno-math-errno",
             "-fPIC", "-fvisibility=hidden", "-I", output, "-c", source, "-o", object_file],
            output=output / f"compile_{index:03}.log")
        objects.append(object_file)
        print(f"compiled {index + 1}/{len(sources)}", flush=True)
    if not args.without_replacements:
        object_file = output / "replacement.o"
        run([environment.get("CXX", "clang++"), "-std=c++20", f"-O{args.optimization}",
             "-ffp-contract=off", "-fPIC", "-I", ROOT, "-I", output,
             "-c", selected, "-o", object_file], output=output / "replacement_build.log")
        objects.append(object_file)
    library = output / ("translated.dylib" if sys.platform == "darwin" else "translated.so")
    run([compiler, "-shared", *objects, "-lm", "-o", library], output=output / "link.log")
    executable = output / "native_execution"
    run([environment.get("CXX", "clang++"), "-std=c++20", f"-O{args.optimization}",
         "-ffp-contract=off", "-Wall", "-Wextra", "-Werror", "-Wno-unused-function",
         "-I", output, ROOT / "runtime/port/NativeExecution.cpp", "-o", executable,
         *([] if sys.platform == "darwin" else ["-ldl"])], output=output / "host_build.log")
    native_symbols = run(["nm", "-a", library])
    (output / "native_symbols.txt").write_text(native_symbols)
    names = {v for source in sources for v in re.findall(r"void ([A-Za-z0-9_]+)\(Context \*ctx\) \{", source.read_text())}
    linked_names = {line.split()[-1].removeprefix("_") for line in native_symbols.splitlines() if line.split()}
    missing = sorted(names - linked_names)
    if missing:
        raise RuntimeError(f"translated functions absent from native link: {missing[:8]}")
    fallback = {int(v, 16) for source in sources for v in
                re.findall(r"INTERPRET\(0x([0-9A-Fa-f]+)u,", source.read_text())}
    candidates = {int(r["address"]): int(r["mode"])
                  for r in csv.DictReader((output / "instruction_coverage.csv").open())}
    ranges = [(int(r["Start"], 16), int(r["Pool"] or r["End"], 16))
              for r in rows if "f" in r["Type"]]
    # Count a union, so shared code contributes once. Literal pools, fallback instructions,
    # bytes outside map functions, and functions replaced by decompiled source contribute zero.
    eligible = set()
    for start, end in ranges:
        if not args.without_replacements and start in REPLACEMENTS:
            continue
        for at in range(start, end, 4):
            if candidates.get(at) == 4 and at not in fallback:
                eligible.update(range(at, at + 4))
    manifest = {
        "updated": datetime.now(timezone.utc).isoformat(), "frozen_main": base,
        "generator_revision": GENERATOR_REVISION, "original_sha256": digest(code),
        "header_sha256": digest(header), "function_map_sha256": digest(map_file),
        "replacement_sources": source_hashes if not args.without_replacements else {},
        "replacement_addresses": sorted(REPLACEMENTS) if not args.without_replacements else [],
        "translated_functions_linked": len(names), "fallback_instruction_sites": len(fallback),
        "recompiled_bytes": len(eligible), "coverage_unit": "unique map function instruction bytes; literal pools and fallback sites excluded",
        "library": str(library), "library_sha256": digest(library),
        "sources": {str(p.relative_to(ROOT)): digest(p) for p in sources},
        "compiler": run([compiler, "--version"]).splitlines()[0],
        "optimization": args.optimization, "runtime_verified": False,
        "gpu_frame_verified": False,
    }
    (output / "build_manifest.json").write_text(json.dumps(manifest, indent=2) + "\n")
    print(json.dumps({k: manifest[k] for k in ("translated_functions_linked", "recompiled_bytes", "fallback_instruction_sites", "runtime_verified", "gpu_frame_verified")}))


if __name__ == "__main__":
    main()
