#!/usr/bin/env python3
"""Link optional browser gameplay from a byte-identical recovered browser closure."""
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
OVERLAY = ROOT / "tools/static_recompiler/azahar_reference/azahar_browser_gameplay_session.patch"


def archive_members(path):
    """Bind all unmodified members when replacing selected observer objects."""
    members, names = {}, b""
    with Path(path).open("rb") as stream:
        if stream.read(8) != b"!<arch>\n":
            raise RuntimeError("Expected a regular archive")
        while header := stream.read(60):
            if len(header) != 60 or header[58:] != b"`\n":
                raise RuntimeError("Invalid archive member header")
            size = int(header[48:58])
            body = stream.read(size)
            if len(body) != size:
                raise RuntimeError("Truncated archive member")
            if size % 2:
                stream.read(1)
            name = header[:16].decode().strip()
            if name == "//":
                names = body
                continue
            if name in ("/", "/SYM64/"):
                continue
            if name.startswith("#1/"):
                length = int(name[3:])
                name, body = body[:length].rstrip(b"\0").decode(), body[length:]
            elif name.startswith("/"):
                name = names[int(name[1:]):].split(b"/\n", 1)[0].decode()
            else:
                name = name.removesuffix("/")
            # Upstream archives contain repeated basenames from different
            # source directories. Bind their original member order as well.
            members[f"{len(members)}:{name}"] = hashlib.sha256(body).hexdigest()
    return members


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("baseline", type=Path, help="historical accepted browser build manifest")
    parser.add_argument("recovery", type=Path, help="complete byte-identical recovery receipt")
    parser.add_argument("platform", type=Path, help="preserved pinned platform directory")
    parser.add_argument("output", type=Path)
    parser.add_argument("--emsdk", type=Path, required=True)
    parser.add_argument("--node", type=Path, required=True)
    args = parser.parse_args()
    baseline_path, recovery_path = args.baseline.resolve(), args.recovery.resolve()
    baseline, recovery = json.loads(baseline_path.read_text()), json.loads(recovery_path.read_text())
    platform, output = args.platform.resolve(), args.output.resolve()
    compiler = args.emsdk.resolve() / "upstream/emscripten/em++"
    archiver = compiler.with_name("emar")
    if not output.is_relative_to(ROOT / "build") or os.path.lexists(output):
        raise RuntimeError("Output must be absent under this checkout's ignored build")
    if not (baseline.get("passed") is True and recovery.get("passed") is True and
            recovery.get("status") == "complete_byte_identical_browser_link_recovery" and
            recovery.get("before") == recovery.get("after") and
            recovery.get("baseline_sha256") == digest(baseline_path) and
            all(command["status"] == 0 for command in baseline["commands"]) and
            all(command["status"] == 0 for command in recovery["commands"])):
        raise RuntimeError("The historical browser closure is not completely recovered")
    restored_browser = Path(recovery["browser_baseline"]["path"])
    if digest(restored_browser) != baseline["wasm_sha256"]:
        raise RuntimeError("Complete recovered browser wasm differs from its historical seal")
    protected = {str(baseline_path): digest(baseline_path), str(recovery_path): digest(recovery_path),
                 str(restored_browser): digest(restored_browser), str(OVERLAY): digest(OVERLAY),
                 str(Path(__file__).resolve()): digest(Path(__file__).resolve())}
    for path, expected in recovery["after"].items():
        if digest(path) != expected["sha256"]:
            raise RuntimeError(f"Protected recovered input changed: {path}")
        protected[path] = expected["sha256"]
    mapping = {}
    for original, record in recovery["recovered"].items():
        path = Path(record["path"])
        if digest(path) != record["sha256"] or path.stat().st_size != record["bytes"]:
            raise RuntimeError(f"Recovered provider changed: {path}")
        mapping[original] = str(path)
        protected[str(path)] = record["sha256"]
    for original, expected in baseline["protected_inputs"].items():
        path = Path(original) if Path(original).is_file() else Path(mapping[original])
        if digest(path) != expected:
            raise RuntimeError(f"Historical provider changed: {original}")
        protected[str(path)] = expected
    inventory_path = platform / "compiled/compile_commands.json"
    inventory = json.loads(inventory_path.read_text())
    protected[str(inventory_path)] = digest(inventory_path)
    selected_sources = [ROOT / "runtime/port/GameplaySession.cpp", ROOT / "runtime/port/GameplaySession.h"]
    for name in ("BrowserExecutionEntry", "BrowserButtonInput", "BrowserFrameOutput", "BrowserCirclePadInput",
                 "BrowserTouchInput", "BrowserInputIdentity"):
        selected_sources.append(ROOT / f"runtime/port/browser/{name}.cpp")
        header = ROOT / f"runtime/port/browser/{name}.h"
        if header.is_file():
            selected_sources.append(header)
    for path in selected_sources:
        protected[str(path)] = digest(path)
    output.mkdir(parents=True)
    (output / "objects").mkdir()
    (output / "archives").mkdir()
    source_overlay = output / "source_overlay"
    (source_overlay / "src/root_port_capture").mkdir(parents=True)
    for name in ("gpu_command_capture.cpp", "headless_capture.cpp", "audio_capture.cpp"):
        original = platform / "azahar_source/src/root_port_capture" / name
        protected[str(original)] = digest(original)
        shutil.copy2(original, source_overlay / "src/root_port_capture" / name)
    environment = os.environ.copy()
    environment["EMSDK_NODE"] = str(args.node.resolve())
    commands = []
    receipt = {"passed": False, "protected_inputs": protected,
               "baseline_manifest_sha256": digest(baseline_path),
               "recovery_receipt_sha256": digest(recovery_path),
               "historical_browser_wasm_sha256": digest(restored_browser),
               "cpu_provider": "Unchanged recovered historical static ARM archive and main object",
               "browser_execution_verified": False, "default_capture_equivalence_verified": False,
               "scope": "Optional gameplay frontend link only. No guest execution, performance, GPU equality, "
                        "World 1-1 goal or Section 7 claim."}

    def save():
        (output / "build_manifest.json").write_text(json.dumps(receipt, indent=2) + "\n")

    def run(command, directory=ROOT):
        if shutil.disk_usage(output).free < 16 * 1024**3:
            raise RuntimeError("Gameplay link reached its 16 GiB free-space floor")
        log = output / f"command_{len(commands)}.log"
        started = time.monotonic()
        with log.open("wb") as stream:
            result = subprocess.run([str(value) for value in command], cwd=directory, env=environment,
                                    stdout=stream, stderr=subprocess.STDOUT, timeout=600)
        commands.append({"command": [str(value) for value in command], "status": result.returncode,
                         "elapsed_seconds": time.monotonic() - started, "log": str(log)})
        receipt["commands"] = commands
        save()
        if result.returncode:
            raise RuntimeError(f"Gameplay link command refused; see {log}")

    def compile_recipe(record, source, target, additional=()):
        command = shlex.split(record["command"]) if isinstance(record["command"], str) else record["command"]
        if Path(command[0]).resolve() != compiler:
            raise RuntimeError("Recovered compile recipe selects a different compiler")
        flags = list(command[1:])
        for option in ("-o", "-c"):
            position = flags.index(option)
            del flags[position:position + 2]
        return [str(compiler), *flags, f"-I{ROOT / 'runtime/port'}", *additional,
                "-c", str(source), "-o", str(target)]

    try:
        save()
        destination = "--directory=" + str(source_overlay.relative_to(ROOT))
        run(["git", "apply", "--check", destination, str(OVERLAY)])
        run(["git", "apply", destination, str(OVERLAY)])
        for name in ("gpu_command_capture.cpp", "headless_capture.cpp", "audio_capture.cpp"):
            if "Port::GameplaySession" not in (source_overlay / "src/root_port_capture" / name).read_text():
                raise RuntimeError(f"Selected observer overlay was not applied: {name}")
        replacements = {}
        member_evidence = {}
        for archive_name, source_names in (("libcitra_core.a", ("gpu_command_capture.cpp",)),
                                           ("libaudio_core.a", ("audio_capture.cpp",)),
                                           ("libroot_port_headless_capture.a", ("headless_capture.cpp",))):
            candidates = [original for original in mapping if Path(original).name == archive_name]
            if len(candidates) != 1:
                raise RuntimeError(f"Ambiguous recovered archive: {archive_name}")
            original = candidates[0]
            target_archive = output / "archives" / archive_name
            before_members = archive_members(mapping[original])
            shutil.copy2(mapping[original], target_archive)
            for name in source_names:
                recipes = [record for record in inventory if record["file"].endswith("/root_port_capture/" + name)]
                if name == "headless_capture.cpp":
                    recipes = [record for record in recipes if
                               "CMakeFiles/root_port_headless_capture.dir/headless_capture.cpp.o" in record["command"]]
                if len(recipes) != 1:
                    raise RuntimeError(f"Ambiguous preserved compile recipe: {name}")
                record = recipes[0]
                target = output / "objects" / (name + ".o")
                run(compile_recipe(record, source_overlay / "src/root_port_capture" / name, target), record["directory"])
                run([archiver, "r", target_archive, target])
            after_members = archive_members(target_archive)
            expected_changes = {name + ".o" for name in source_names}
            changed = {name for name in before_members if before_members[name] != after_members.get(name)}
            changed_names = {name.split(":", 1)[1] for name in changed}
            if (set(before_members) != set(after_members) or changed_names != expected_changes or
                    len(changed) != len(expected_changes)):
                raise RuntimeError(f"Unexpected provider member change: {archive_name}")
            member_evidence[archive_name] = {"before": before_members, "after": after_members,
                                            "changed_members": sorted(changed)}
            replacements[original] = str(target_archive)
        objects = {}
        for record in baseline["commands"]:
            command = record["command"]
            if "-c" not in command:
                continue
            source = Path(command[command.index("-c") + 1])
            if source.name == "AzaharCompiledExecution.cpp":
                # The existing CPU/provider ABI stays sealed. A changed current
                # Backend header is not interchangeable with the old archive.
                continue
            original = command[command.index("-o") + 1]
            selected = ROOT / "runtime/port/browser" / source.name
            target = output / "objects" / Path(original).name
            run(compile_recipe(record, selected, target))
            objects[original] = str(target)
        recipe = next(record for record in inventory if record["file"].endswith("/root_port_capture/gpu_command_capture.cpp"))
        session_object = output / "objects/GameplaySession.o"
        run(compile_recipe(recipe, ROOT / "runtime/port/GameplaySession.cpp", session_object,
                           ("-Wall", "-Wextra", "-Werror")), recipe["directory"])
        module = output / "root_port_browser_capture.mjs"
        link = [objects.get(token, replacements.get(token, mapping.get(token, token)))
                for token in baseline["commands"][-1]["command"]]
        link.insert(1, str(session_object))
        exports = next(index for index, token in enumerate(link) if token.startswith("-sEXPORTED_FUNCTIONS="))
        link[exports] = link[exports][:-1] + "," + ",".join("'" + name + "'" for name in (
            "_BrowserGameplaySessionRequestStop", "_BrowserGameplaySessionIsActive",
            "_BrowserButtonInputSetButtonMask", "_BrowserButtonInputHeldButtonMask",
            "_BrowserButtonInputHasGameplayControls")) + "]"
        link[link.index("-o") + 1] = str(module)
        run(link)
        shutil.copytree(platform / "licenses", output / "licenses")
        for name, expected in baseline["licenses"].items():
            if digest(output / "licenses" / name) != expected:
                raise RuntimeError(f"Copied provider notice differs: {name}")
        for path, expected in protected.items():
            if digest(path) != expected:
                raise RuntimeError(f"Protected historical input changed: {path}")
        wasm = module.with_suffix(".wasm")
        receipt.update({"passed": True, "gameplay_session_supported": True,
                        "runtime_sources": {str(path.relative_to(ROOT)): digest(path)
                        for path in selected_sources}, "module": str(module), "module_sha256": digest(module),
                        "wasm": str(wasm), "wasm_sha256": digest(wasm), "wasm_bytes": wasm.stat().st_size,
                        "licenses": baseline["licenses"], "derived_archives": {
                            name: {"path": path, "sha256": digest(path)} for name, path in replacements.items()},
                        "archive_member_evidence": member_evidence})
    except BaseException as error:
        receipt["failure"] = type(error).__name__ + ": " + str(error)
        raise
    finally:
        save()
    print(json.dumps({key: receipt[key] for key in ("passed", "wasm_bytes", "wasm_sha256", "scope")}))


if __name__ == "__main__":
    main()
