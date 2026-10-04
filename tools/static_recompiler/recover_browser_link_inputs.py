#!/usr/bin/env python3
"""Recover retired browser objects and archives from preserved exact build recipes."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shlex
import shutil
import subprocess
import time

from audit_webassembly_platform import digest

ROOT = Path(__file__).resolve().parents[2]


def identity(path):
    return {"bytes": Path(path).stat().st_size, "sha256": digest(path)}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("baseline", type=Path, help="historical successful browser build manifest")
    parser.add_argument("platform", type=Path, help="preserved platform compiled directory")
    parser.add_argument("translation", type=Path, help="preserved translation compiled directory")
    parser.add_argument("output", type=Path)
    parser.add_argument("--emsdk", type=Path, required=True)
    parser.add_argument("--node", type=Path, required=True)
    parser.add_argument("--reuse-recovery", type=Path,
                        help="reuse unchanged inputs from a complete recovery, then reproduce both wasm modules again")
    args = parser.parse_args()
    baseline_path, platform, translation, output = (path.resolve() for path in
                                                   (args.baseline, args.platform, args.translation, args.output))
    compiler = args.emsdk.resolve() / "upstream/emscripten/em++"
    baseline = json.loads(baseline_path.read_text())
    if not output.is_relative_to(ROOT / "build") or os.path.lexists(output):
        raise RuntimeError("Output must be absent under this checkout's ignored build")
    if baseline.get("passed") is not True or any(command["status"] != 0 for command in baseline["commands"]):
        raise RuntimeError("Historical browser link is unsuccessful")
    before = {str(baseline_path): identity(baseline_path), str(Path(__file__).resolve()): identity(__file__)}
    missing, mapping, recipes, archive_recipes = {}, {}, {}, {}
    for original, expected in baseline["protected_inputs"].items():
        if Path(original).is_file():
            actual = identity(original)
            if actual["sha256"] != expected:
                raise RuntimeError(f"Historical protected input changed: {original}")
            before[original] = actual
            mapping[original] = original
        else:
            missing[original] = expected
    if args.reuse_recovery:
        reuse_path = args.reuse_recovery.resolve()
        reuse = json.loads(reuse_path.read_text())
        if (reuse.get("passed") is not True or reuse.get("before") != reuse.get("after") or
                reuse.get("baseline_sha256") != digest(baseline_path) or
                reuse.get("status") != "complete_byte_identical_browser_link_recovery"):
            raise RuntimeError("Reusable closure is unsuccessful or refers to a different baseline")
        before[str(reuse_path)] = identity(reuse_path)
        for original, record in reuse["recovered"].items():
            actual = identity(record["path"])
            if actual != {"bytes": record["bytes"], "sha256": record["sha256"]} or (
                    original in missing and actual["sha256"] != missing[original]):
                raise RuntimeError(f"Reusable recovered input changed: {original}")
            mapping[original] = record["path"]
            before[record["path"]] = actual
    for build in (platform, translation):
        inventory_path = build / "compile_commands.json"
        before[str(inventory_path)] = identity(inventory_path)
        for entry in json.loads(inventory_path.read_text()):
            command = shlex.split(entry["command"])
            selected_compiler = Path(command[0]).resolve()
            if selected_compiler.parent != compiler.parent or selected_compiler.name not in ("em++", "emcc"):
                raise RuntimeError("Historical compiler differs")
            target = str((Path(entry["directory"]) / command[command.index("-o") + 1]).resolve())
            recipes[target] = (command, Path(entry["directory"]))
        for link_path in build.glob("**/link.txt"):
            commands = [shlex.split(line) for line in link_path.read_text().splitlines() if line.strip()]
            if not commands or Path(commands[0][0]).name != "emar":
                continue
            directory = link_path.parents[2]
            archive = str((directory / commands[0][2]).resolve())
            archive_recipes[archive] = (commands, directory, link_path)
    needed_objects, needed_archives = set(), []
    for original in missing:
        if original.endswith(".a"):
            commands, directory, link_path = archive_recipes[original]
            before[str(link_path)] = identity(link_path)
            needed_archives.append(original)
            needed_objects.update(str((directory / token).resolve()) for token in commands[0][3:])
        elif original.endswith(".o"):
            needed_objects.add(original)
        elif not original.endswith(".wasm"):
            raise RuntimeError(f"Unsupported retired input: {original}")
    browser_recipes = []
    for record in baseline["commands"]:
        command = record["command"]
        if "-c" not in command:
            continue
        if Path(command[0]).resolve() != compiler:
            raise RuntimeError("Historical browser compiler differs")
        original = command[command.index("-o") + 1]
        if original in mapping:
            continue
        if Path(original).is_file():
            before[original] = identity(original)
            mapping[original] = original
        else:
            browser_recipes.append((original, command))
    for original in needed_objects:
        command, _ = recipes[original]
        source = command[command.index("-c") + 1]
        before[source] = identity(source)
    for _, command in browser_recipes:
        source = command[command.index("-c") + 1]
        before[source] = identity(source)
    output.mkdir(parents=True)
    for directory in ("objects", "archives", "logs", "node_baseline", "browser_baseline"):
        (output / directory).mkdir()
    receipt = {"passed": False, "status": "running", "baseline": str(baseline_path),
               "baseline_sha256": digest(baseline_path), "before": before,
               "recovered": {name: {"path": path, **identity(path)} for name, path in mapping.items()}, "commands": [],
               "retired_inputs": missing, "scope": "Exact historical closure recovery only. No new runtime claim.",
               "bounds": {"compiler_parallelism": 1, "command_seconds": 600, "total_seconds": 7200,
                          "additional_monitored_bytes": 4 * 1024**3, "minimum_free_bytes": 16 * 1024**3}}
    environment = os.environ.copy()
    environment["EMSDK_NODE"] = str(args.node.resolve())
    started, initial_free = time.monotonic(), shutil.disk_usage(output).free

    def save():
        (output / "recovery_receipt.json").write_text(json.dumps(receipt, indent=2) + "\n")

    def run(command, directory):
        free = shutil.disk_usage(output).free
        if free < 16 * 1024**3 or initial_free - free > 4 * 1024**3 or time.monotonic() - started > 7200:
            raise RuntimeError("Browser recovery resource bound reached")
        log = output / "logs" / f"{len(receipt['commands']):04d}.log"
        began = time.monotonic()
        with log.open("wb") as stream:
            result = subprocess.run(command, cwd=directory, env=environment, stdout=stream,
                                    stderr=subprocess.STDOUT, timeout=600)
        receipt["commands"].append({"command": command, "cwd": str(directory), "status": result.returncode,
                                    "seconds": time.monotonic() - began, "log": str(log),
                                    "log_identity": identity(log)})
        save()
        if result.returncode:
            raise RuntimeError(f"Browser recovery command refused: {log}")

    def recovered(original, target, expected=None):
        actual = identity(target)
        if expected and actual["sha256"] != expected:
            raise RuntimeError(f"Recovered historical input differs: {original}")
        mapping[original] = str(target)
        receipt["recovered"][original] = {"path": str(target), **actual}
        save()

    try:
        save()
        for index, original in enumerate(sorted(needed_objects)):
            if original in mapping:
                recovered(original, Path(mapping[original]), missing.get(original))
                continue
            command, directory = recipes[original]
            command = list(command)
            target = output / "objects" / hashlib.sha256(original.encode()).hexdigest()[:16] / Path(original).name
            target.parent.mkdir()
            command[command.index("-o") + 1] = str(target)
            run(command, directory)
            recovered(original, target, missing.get(original))
            if index % 25 == 0:
                print(json.dumps({"compiled_objects": index + 1, "total_objects": len(needed_objects)}), flush=True)
        for original in needed_archives:
            if original in mapping:
                recovered(original, Path(mapping[original]), missing[original])
                continue
            commands, directory, _ = archive_recipes[original]
            target = output / "archives" / Path(original).name
            if target.exists():
                raise RuntimeError("Recovered archive basename collision")
            command = list(commands[0])
            command[2] = str(target)
            command[3:] = [mapping[str((directory / token).resolve())] for token in command[3:]]
            run(command, directory)
            for command in commands[1:]:
                run([str(target) if token.endswith(".a") else token for token in command], directory)
            recovered(original, target, missing[original])
        node_link_path = platform / "src/root_port_capture/CMakeFiles/root_port_webassembly_capture.dir/link.txt"
        before[str(node_link_path)] = identity(node_link_path)
        node_directory = node_link_path.parents[2]
        command = shlex.split(node_link_path.read_text())
        for index, token in enumerate(command):
            if token.endswith((".a", ".o")):
                original = str((node_directory / token).resolve())
                command[index] = mapping.get(original, original)
        node_module = output / "node_baseline/root_port_webassembly_capture.js"
        command[command.index("-o") + 1] = str(node_module)
        run(command, node_directory)
        original = str(platform / "bin/Release/root_port_webassembly_capture.wasm")
        recovered(original, node_module.with_suffix(".wasm"), baseline["protected_inputs"][original])
        for original, recipe in browser_recipes:
            command = list(recipe)
            target = output / "objects" / Path(original).name
            command[command.index("-o") + 1] = str(target)
            run(command, ROOT)
            recovered(original, target)
        command = [mapping.get(token, token) for token in baseline["commands"][-1]["command"]]
        module = output / "browser_baseline" / Path(baseline["module"]).name
        command[command.index("-o") + 1] = str(module)
        run(command, ROOT)
        if digest(module.with_suffix(".wasm")) != baseline["wasm_sha256"]:
            raise RuntimeError("Complete recovered browser wasm differs from its historical hash")
        receipt["browser_baseline"] = {"path": str(module.with_suffix(".wasm")), **identity(module.with_suffix(".wasm"))}
        receipt["after"] = {path: identity(path) for path in before}
        if before != receipt["after"]:
            raise RuntimeError("Protected original inputs changed during recovery")
        receipt.update({"passed": True, "status": "complete_byte_identical_browser_link_recovery"})
    except BaseException as error:
        receipt.update({"status": "failed_browser_link_recovery", "failure": type(error).__name__ + ": " + str(error)})
        raise
    finally:
        receipt.update({"seconds": time.monotonic() - started, "final_free_bytes": shutil.disk_usage(output).free})
        save()
    print(json.dumps({"passed": True, "receipt": str(output / "recovery_receipt.json")}), flush=True)


if __name__ == "__main__":
    main()
