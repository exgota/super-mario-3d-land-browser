#!/usr/bin/env python3
"""Cross-compile sealed timed translation sources into an ignored wasm32 archive."""
import argparse
import csv
import hashlib
import json
import os
from pathlib import Path
import shutil
import re
import subprocess

from build_port import REPLACEMENTS

ROOT = Path(__file__).resolve().parents[2]


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", type=Path, help="sealed timed native module directory")
    parser.add_argument("output", type=Path)
    parser.add_argument("--softfloat-source", type=Path, required=True)
    parser.add_argument("--emsdk", type=Path, required=True)
    parser.add_argument("--node", type=Path, required=True)
    parser.add_argument("--cmake", default="cmake")
    args = parser.parse_args()
    source, output = args.source.resolve(), args.output.resolve()
    if not output.is_relative_to(ROOT / "build") or output.exists():
        raise RuntimeError("output must be an absent child of this checkout's ignored build")
    manifest = json.loads((source / "build_manifest.json").read_text())
    if digest(manifest["library"]) != manifest["library_sha256"]:
        raise RuntimeError("sealed native module library changed")
    if manifest.get("native_timing_revision") != 2 or manifest.get("floating_point_operations_rewritten", 0) == 0:
        raise RuntimeError("translation requires revision-two timing and explicit guest arithmetic")
    output.mkdir(parents=True)
    sources = output / "sources"
    sources.mkdir()
    for relative, expected in manifest["sources"].items():
        original = source / Path(relative).name
        if digest(original) != expected:
            raise RuntimeError(f"translation source seal failed: {original.name}")
        shutil.copy2(original, sources / original.name)
    for name in ("NativeFloatingPoint.h", "NativeFloatingPoint.c", "NativeTiming.h", "NativeTiming.c"):
        if digest(sources / name) != digest(ROOT / "runtime/port" / name):
            raise RuntimeError(f"sealed translation differs from accepted runtime source: {name}")
    for relative, expected in manifest["replacement_sources"].items():
        if digest(ROOT / relative) != expected:
            raise RuntimeError(f"decompiled replacement source differs: {relative}")
    frozen_main = subprocess.check_output(["git", "rev-parse", "main"], cwd=ROOT, text=True).strip()
    map_text = subprocess.check_output(["git", "show", f"{frozen_main}:data/ver/eu/map.csv"], cwd=ROOT, text=True)
    rows = [{key.strip(): value.strip() for key, value in row.items()} for row in csv.DictReader(map_text.splitlines())]
    if set(manifest["replacement_addresses"]) != set(REPLACEMENTS):
        raise RuntimeError("source replacement registry differs from the reviewed translation")
    for address, (symbol, relative) in REPLACEMENTS.items():
        matches = [row for row in rows if int(row["Start"], 16) == address]
        if len(matches) != 1 or matches[0]["Rank"] != "O" or matches[0]["Symbol"] != symbol:
            raise RuntimeError(f"source replacement is not rank O on frozen main: {symbol}")
        committed = subprocess.check_output(["git", "show", f"{frozen_main}:{relative}"], cwd=ROOT)
        if hashlib.sha256(committed).hexdigest() != manifest["replacement_sources"][relative]:
            raise RuntimeError(f"source replacement differs from frozen main: {symbol}")
    declared_counts = re.findall(r"recomp_entry_count\s*=\s*(\d+)\s*;", (sources / "entries.c").read_text())
    if len(declared_counts) != 1:
        raise RuntimeError("entry count declaration is unavailable or ambiguous")
    environment = os.environ.copy()
    environment["EMSDK_NODE"] = str(args.node.resolve())
    build = output / "compiled"
    commands = [
        [str(args.emsdk.resolve() / "upstream/emscripten/emcmake"), str(args.cmake),
         "-S", str(ROOT / "runtime/port/webassembly_translation"), "-B", str(build),
         "-DCMAKE_BUILD_TYPE=Release", "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
         f"-DROOT_PORT_TRANSLATED_DIRECTORY={sources}",
         f"-DROOT_PORT_SOFTFLOAT_SOURCE_DIRECTORY={args.softfloat_source.resolve()}"],
        [str(args.cmake), "--build", str(build), "--target", "static_arm_translation", "-j", "1"],
    ]
    for index, command in enumerate(commands):
        with (output / f"command_{index}.log").open("wb") as log:
            subprocess.run(command, env=environment, stdout=log, stderr=subprocess.STDOUT, check=True)
    archive = build / "libstatic_arm_translation.a"
    receipt = {"frozen_main": frozen_main,
               "native_source_manifest_sha256": digest(source / "build_manifest.json"),
               "source_library_sha256": manifest["library_sha256"], "commands": commands,
               "sources": {name: digest(sources / Path(name).name) for name in manifest["sources"]},
               "archive": str(archive), "archive_sha256": digest(archive),
               "declared_instruction_entries": int(declared_counts[0]), "native_recompiled_bytes": manifest["recompiled_bytes"],
               "new_linked_wasm_game_bytes": 0, "runtime_verified": False,
               "scope": "Cross-compiled archive only. Final module linkage and actual guest entry execution remain unverified."}
    (output / "build_manifest.json").write_text(json.dumps(receipt, indent=2) + "\n")
    print(json.dumps({key: receipt[key] for key in ("archive", "archive_sha256", "new_linked_wasm_game_bytes")}))


if __name__ == "__main__":
    main()
