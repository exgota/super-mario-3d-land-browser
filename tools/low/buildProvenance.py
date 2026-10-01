"""Record and verify direct ARMCC output from committed repository C++ inputs."""

import hashlib
import json
from pathlib import Path
import shlex
import subprocess

from tools.low.glob import getProjDir, getCompilerPath, getBuildObjPath, getVersion
from tools.pypstem._utils import getFileBuildPath


def provenance_path(object_path):
    return Path(object_path).with_suffix(".provenance.json")


def _sha256(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def snapshot_build_inputs(source):
    root = getProjDir()
    tracked = subprocess.check_output(
        ["git", "ls-files", "-z", "--", "Game", "lib", "data/config.json"], cwd=root
    ).decode().split("\0")
    paths = [root / name for name in tracked if name and (root / name).is_file()]
    paths.append(Path(source))
    return {str(path.relative_to(root)): _sha256(path) for path in paths}


def record_build_output(source, object_path, command, before):
    root = getProjDir()
    source = Path(source).resolve()
    object_path = Path(object_path).resolve()
    dependencies = {source, root / "data/config.json"}
    for line in object_path.with_suffix(".d").read_text().splitlines():
        fields = shlex.split(line)
        if len(fields) >= 2:
            path = Path(fields[1]).resolve()
            if path.is_relative_to(root) and not path.is_relative_to(root / "data/compilers"):
                dependencies.add(path)
    inputs = {str(path.relative_to(root)): _sha256(path) for path in dependencies}
    stable = all(before.get(name) == digest for name, digest in inputs.items())
    compiler = getCompilerPath() / "bin/armcc.exe"
    record = {"schema": 1, "build_step": "tools.pypstem.stepBuild",
              "language": "C++", "source": str(source.relative_to(root)),
              "object": str(object_path.relative_to(root)), "version": getVersion(),
              "compiler": str(compiler.relative_to(root)), "compiler_sha256": _sha256(compiler),
              "command": list(map(str, command)), "inputs": inputs, "inputs_stable": stable,
              "object_sha256": _sha256(object_path)}
    provenance_path(object_path).write_text(json.dumps(record, indent=2) + "\n")


def verify_build_output(object_path):
    """Return verified source identity, or raise ValueError without changing ranks."""
    root = getProjDir()
    object_path = Path(object_path).resolve()
    if not object_path.is_relative_to(getBuildObjPath().resolve()):
        raise ValueError("The object is outside the project build output directory.")
    try:
        record = json.loads(provenance_path(object_path).read_text())
    except (OSError, ValueError) as error:
        raise ValueError("The object has no valid project ARMCC build provenance.") from error
    if record.get("schema") != 1 or record.get("build_step") != "tools.pypstem.stepBuild" or record.get("language") != "C++":
        raise ValueError("The object was not recorded as direct project ARMCC C++ output.")
    source = (root / record["source"]).resolve()
    if not source.is_relative_to(root) or source.suffix not in (".cpp", ".cc", ".cxx"):
        raise ValueError("The object does not identify repository C++ source.")
    if object_path != getFileBuildPath(source).resolve() or record["object"] != str(object_path.relative_to(root)):
        raise ValueError("The object path is not the configured source's project output.")
    config = json.loads((root / "data/config.json").read_text())
    module = next((data for name, data in config["modules"].items()
                   if source.is_relative_to(root / name / data.get("source_dir", "."))), None)
    if module is None:
        raise ValueError("The source is outside the configured build modules.")
    compiler_version = module.get("compiler", config["compiler"])
    compiler = root / "data/compilers" / compiler_version / "bin/armcc.exe"
    if record.get("compiler") != str(compiler.relative_to(root)) or record.get("compiler_sha256") != _sha256(compiler):
        raise ValueError("The recorded ARMCC compiler differs from the configured compiler.")
    command = record.get("command", [])
    if str(compiler) not in command or "-c" not in command or "-S" in command or str(source) not in command:
        raise ValueError("The recorded command is not a direct ARMCC source-to-object compilation.")
    if "-o" not in command or command[command.index("-o") + 1] != str(object_path):
        raise ValueError("The recorded compiler command writes a different object.")
    if record.get("version") != getVersion() or not record.get("inputs_stable"):
        raise ValueError("The recorded build version or input stability is invalid.")
    if record.get("object_sha256") != _sha256(object_path):
        raise ValueError("The compiler object changed after the project build.")
    inputs = record.get("inputs", {})
    if str(source.relative_to(root)) not in inputs or "data/config.json" not in inputs:
        raise ValueError("The build record omits its source or configuration input.")
    tree = subprocess.check_output(
        ["git", "ls-tree", "-r", "-z", "HEAD", "--", *inputs], cwd=root
    ).decode().split("\0")
    committed = {}
    for entry in tree:
        if entry:
            attributes, name = entry.split("\t", 1)
            mode, kind, digest = attributes.split()
            if kind == "blob" and mode in ("100644", "100755"):
                committed[name] = digest
    algorithm = subprocess.check_output(["git", "rev-parse", "--show-object-format"], cwd=root, text=True).strip()
    for name, digest in inputs.items():
        path = (root / name).resolve()
        if not path.is_relative_to(root) or not path.is_file():
            raise ValueError("A recorded build input is missing or outside the repository.")
        content = path.read_bytes()
        if hashlib.sha256(content).hexdigest() != digest:
            raise ValueError(f"The build input changed after compilation: {name}.")
        blob = f"blob {len(content)}\0".encode() + content
        if committed.get(name) != hashlib.new(algorithm, blob).hexdigest():
            raise ValueError(f"The build input is untracked or differs from committed source: {name}.")
    return {"source": record["source"], "compiler": compiler_version,
            "object_sha256": record["object_sha256"]}
