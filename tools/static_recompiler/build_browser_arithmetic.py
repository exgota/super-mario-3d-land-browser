"""Link explicit native-order provider arithmetic without changing sealed gameplay inputs."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shlex
import shutil
import subprocess
import sys
import time

WORKTREE = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(WORKTREE / "tools/static_recompiler"))
from build_browser_gameplay_session import archive_members

OVERLAY = WORKTREE / "tools/static_recompiler/azahar_reference/azahar_native_arithmetic.patch"

def digest(path):
    with Path(path).open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("baseline", type=Path, help="sealed gameplay module directory")
parser.add_argument("platform", type=Path, help="preserved pinned platform directory")
parser.add_argument("output", type=Path)
parser.add_argument("--emsdk", type=Path, required=True)
parser.add_argument("--node", type=Path, required=True)
args = parser.parse_args()
baseline_directory = args.baseline.resolve()
baseline_path = baseline_directory / "build_manifest.json"
baseline = json.loads(baseline_path.read_text())
platform = args.platform.resolve()
inventory_path = platform / "compiled/compile_commands.json"
inventory = json.loads(inventory_path.read_text())
output = args.output.resolve()
if not output.is_relative_to(WORKTREE / "build") or not baseline.get("passed") or not baseline.get("gameplay_session_supported"):
    raise RuntimeError("An existing successful gameplay build and absent ignored output are required")
if output.exists():
    raise RuntimeError("Candidate output already exists")
protected = dict(baseline["protected_inputs"])
protected[str(baseline_path)] = digest(baseline_path)
protected[str(inventory_path)] = digest(inventory_path)
for key in ("module", "wasm"):
    protected[baseline[key]] = baseline[key + "_sha256"]
for record in baseline["derived_archives"].values():
    protected[record["path"]] = record["sha256"]
for path in (OVERLAY, Path(__file__).resolve()):
    protected[str(path)] = digest(path)
for path, expected in protected.items():
    if digest(path) != expected:
        raise RuntimeError("Protected input changed: " + path)
output.mkdir()
for name in ("objects", "archives", "source_overlay", "logs"):
    (output / name).mkdir()
receipt = {**baseline, "passed": False, "protected_inputs": protected,
           "arithmetic_overlay": "Explicit native-object gain ramp, stereo downmix, fog and depth FMA order",
           "actual_movie_correction_verified": False,
           "scope": "Isolated arithmetic candidate. Full identical-movie comparison pending."}
commands = []
def save():
    (output / "build_manifest.json").write_text(json.dumps(receipt, indent=2) + "\n")
def run(command, directory=WORKTREE):
    log = output / "logs" / f"command_{len(commands)}.log"
    if shutil.disk_usage(output).free < 16 * 1024**3:
        raise RuntimeError("Arithmetic link reached its 16 GiB free-space floor")
    started = time.monotonic()
    with log.open("wb") as stream:
        completed = subprocess.run([str(token) for token in command], cwd=directory,
                                   stdout=stream, stderr=subprocess.STDOUT, timeout=600)
    commands.append({"command": [str(token) for token in command], "status": completed.returncode,
                     "elapsed_seconds": time.monotonic()-started, "log": str(log)})
    receipt["commands"] = commands
    save()
    print("command", len(commands), "status", completed.returncode, "seconds", round(time.monotonic()-started, 3), flush=True)
    if completed.returncode:
        raise RuntimeError("Candidate command failed: " + str(log))

try:
    compiler = args.emsdk.resolve() / "upstream/emscripten/em++"
    archiver = compiler.with_name("emar")
    os.environ["EMSDK_NODE"] = str(args.node.resolve())
    selected = {"libaudio_core.a": ["audio_core/hle/source.cpp", "audio_core/hle/mixers.cpp"],
                "libvideo_core.a": ["video_core/renderer_software/sw_rasterizer.cpp"]}
    sources = {}
    for names in selected.values():
        for name in names:
            original = platform / "azahar_source/src" / name
            protected[str(original)] = digest(original)
            target = output / "source_overlay/src" / name
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(original, target)
    destination = "--directory=" + str((output / "source_overlay").relative_to(WORKTREE))
    run(["git", "apply", "--check", destination, str(OVERLAY)])
    run(["git", "apply", destination, str(OVERLAY)])
    for names in selected.values():
        for name in names:
            target = output / "source_overlay/src" / name
            original = platform / "azahar_source/src" / name
            sources[name] = {"path": str(target), "sha256": digest(target),
                             "original_sha256": digest(original)}
    receipt["candidate_patch_sha256"] = digest(OVERLAY)
    receipt["candidate_sources"] = sources
    replacements = {}
    archive_evidence = {}
    original_link = baseline["commands"][-1]["command"]
    for archive_name, names in selected.items():
        candidates = {token for token in original_link if Path(token).name == archive_name}
        assert len(candidates) == 1
        original_archive = Path(candidates.pop())
        protected[str(original_archive)] = digest(original_archive)
        target_archive = output / "archives" / archive_name
        shutil.copy2(original_archive, target_archive)
        before = archive_members(original_archive)
        for name in names:
            records = [record for record in inventory if record["file"].endswith("/src/"+name)]
            assert len(records) == 1
            record = records[0]
            command = shlex.split(record["command"])
            assert Path(command[0]).resolve() == compiler.resolve()
            target = output / "objects" / (Path(name).name + ".o")
            command[command.index("-c")+1] = sources[name]["path"]
            command[command.index("-o")+1] = str(target)
            run(command, record["directory"])
            run([archiver, "r", target_archive, target])
        after = archive_members(target_archive)
        changes = {name for name in before if before[name] != after.get(name)}
        assert set(before) == set(after)
        assert {name.split(":",1)[1] for name in changes} == {Path(name).name+".o" for name in names}
        archive_evidence[archive_name] = {"before": before, "after": after, "changed_members": sorted(changes)}
        replacements[str(original_archive)] = str(target_archive)
    module = output / "root_port_browser_capture.mjs"
    link = [replacements.get(token, token) for token in original_link]
    link[link.index("-o")+1] = str(module)
    run(link)
    shutil.copytree(baseline_directory / "licenses", output / "licenses")
    for path, expected in protected.items():
        if digest(path) != expected:
            raise RuntimeError("Protected input changed during build: " + path)
    wasm = module.with_suffix(".wasm")
    receipt.update({"passed": True, "module": str(module), "module_sha256": digest(module),
                    "wasm": str(wasm), "wasm_sha256": digest(wasm), "wasm_bytes": wasm.stat().st_size,
                    "archive_member_evidence": archive_evidence,
                    "derived_archives": {original: {"path": path, "sha256": digest(path)} for original,path in replacements.items()}})
except BaseException as error:
    receipt["failure"] = type(error).__name__ + ": " + str(error)
    raise
finally:
    save()
print(json.dumps({name: receipt[name] for name in ("passed", "wasm_bytes", "wasm_sha256")}))
