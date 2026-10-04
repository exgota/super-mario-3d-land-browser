#!/usr/bin/env python3
"""Build the bounded descriptor-cache and block-accounting CPU candidate."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import time


def main():
    repository = Path(__file__).resolve().parents[2]
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", required=True, type=Path)
    args = parser.parse_args()
    output = args.output.resolve()
    if not output.is_relative_to(repository / "build") or output.exists():
        raise ValueError("Output must be absent and inside this checkout's build directory")
    if os.getpriority(os.PRIO_PROCESS, 0) < 5:
        raise ValueError("Small functional builds require nice5")
    if shutil.disk_usage(repository).free < 12_000_000_000:
        raise ValueError("Free disk is below12GB")
    comparator = repository / "build/runtime_translated_code/cpu_optimization_candidate"
    previous = json.loads((comparator / "build_manifest.json").read_text())
    if not previous.get("passed"):
        raise ValueError("The O3 comparator did not link")
    output.mkdir()
    (output / "sources").mkdir()
    (output / "objects").mkdir()
    # Keep the tested historical constructor/caller closure. Only the two owned
    # function bodies are transferred from the observer-aware authored adapter.
    adapter = (comparator / "sources/StaticArmBackend.cpp").read_text()
    authored = (repository / "runtime/port/StaticArmBackend.cpp").read_text()
    old_dispatch = adapter[adapter.index("Code StaticArmBackend::FindCode("):].rsplit("\n}", 1)[0]
    new_dispatch = authored[authored.index("Code StaticArmBackend::FindCode("):].rsplit("\n}", 1)[0]
    adapter = adapter.replace(old_dispatch, new_dispatch, 1)
    marker = "    current_instruction = address;"
    if adapter.count(marker) != 1:
        raise ValueError("Historical charge entry differs")
    adapter = adapter.replace(marker, marker + "\n    if (!trace && instructions == 1 && schedule->ContinueInstruction()) return;", 1)
    (output / "sources/StaticArmBackend.cpp").write_text(adapter)
    shutil.copy2(comparator / "sources/StaticArmBackend.h", output / "sources/StaticArmBackend.h")
    for name in ["NativeBlockSchedule.cpp", "NativeBlockSchedule.h"]:
        shutil.copy2(repository / "runtime/port" / name, output / "sources" / name)
    manifest = dict(scope="Two old-ABI O3 CPU objects. Validated descriptor indices, active-block admission, block instruction totals. Original translations, timing ABI, Context/Host/Entry, numerical flags and link recipe preserved.",
                    comparator=str(comparator), passed=False, commands=[], load_start=os.getloadavg())
    manifest_path = output / "build_manifest.json"
    environment = dict(os.environ, EMSDK_NODE="/opt/homebrew/bin/node")

    def run(command, cwd):
        started = time.monotonic()
        log = output / f"command_{len(manifest['commands']):02}.log"
        with log.open("xb") as stream:
            result = subprocess.run(command, cwd=cwd, env=environment, stdout=stream,
                                    stderr=subprocess.STDOUT, timeout=60)
        manifest["commands"].append(dict(command=command, cwd=str(cwd), status=result.returncode,
                                         seconds=time.monotonic() - started, log=str(log)))
        manifest_path.write_text(json.dumps(manifest, indent=2) + "\n")
        if result.returncode:
            raise RuntimeError(f"Command failed; see {log}")

    for item in previous["compilation"]:
        command = list(item["command"])
        if "-O3" not in command or "-ffp-contract=off" not in command:
            raise ValueError("Comparator CPU flags differ")
        command[command.index("-c") + 1] = str(output / "sources" / Path(item["member"]).stem)
        command[command.index("-o") + 1] = str(output / "objects" / item["member"])
        run(command, item["cwd"])
    archive = output / "libstatic_arm_webassembly.a"
    run(["/bin/cp", "-c", previous["candidate_cpu_archive"], str(archive)], repository)
    archive_tool = str(Path(previous["compilation"][0]["command"][0]).with_name("emar"))
    run([archive_tool, "r", str(archive), *[str(output / "objects" / item["member"])
                                             for item in previous["compilation"]]], repository)
    command = list(previous["link"]["command"])
    command[command.index(previous["candidate_cpu_archive"])] = str(archive)
    command[command.index("-o") + 1] = str(output / "root_port_browser_capture.mjs")
    run(command, previous["link"]["cwd"])
    manifest.update(passed=True, candidate_cpu_archive=str(archive),
                    archive_sha256=hashlib.sha256(archive.read_bytes()).hexdigest(),
                    source_commit=subprocess.check_output(["git", "rev-parse", "HEAD"],
                                                          cwd=repository, text=True).strip(),
                    unchanged_translation_archive=previous["unchanged_translation_archive"],
                    load_end=os.getloadavg(), browser_performance_verified=False)
    manifest_path.write_text(json.dumps(manifest, indent=2) + "\n")


if __name__ == "__main__":
    main()
