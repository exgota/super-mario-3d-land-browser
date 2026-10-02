#!/usr/bin/env python3
"""Build a pinned, finite Node wasm32 capture host without guest ARM fallback."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

ROOT = Path(__file__).resolve().parents[2]
PACKAGE = ROOT / "tools/static_recompiler/webassembly_platform"


def digest(path):
    checksum = hashlib.sha256()
    with Path(path).open("rb") as source:
        for block in iter(lambda: source.read(1024 * 1024), b""):
            checksum.update(block)
    return checksum.hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("translation", type=Path, help="sealed wasm32 translation archive output")
    parser.add_argument("output", type=Path)
    parser.add_argument("--emsdk", type=Path, required=True)
    parser.add_argument("--node", type=Path, required=True)
    parser.add_argument("--translation-verification", type=Path, required=True,
                        help="successful actual linked translation execution receipt")
    parser.add_argument("--cmake", default="cmake")
    parser.add_argument("--reference-source", type=Path, help="optional local public Azahar Git object cache")
    args = parser.parse_args()
    translation, output = args.translation.resolve(), args.output.resolve()
    if not output.is_relative_to(ROOT / "build") or output.exists():
        raise RuntimeError("output must be an absent child of this checkout's ignored build")
    pins = json.loads((PACKAGE / "source_revisions.json").read_text())
    translated = json.loads((translation / "build_manifest.json").read_text())
    archive = Path(translated["archive"])
    if digest(archive) != translated["archive_sha256"]:
        raise RuntimeError("sealed translation archive changed")
    for relative, expected in translated["sources"].items():
        if digest(translation / "sources" / Path(relative).name) != expected:
            raise RuntimeError(f"sealed translation source changed: {relative}")
    arithmetic = archive.parent / "floating_point/libport_floating_point.a"
    arithmetic_sha256 = digest(arithmetic)
    verification_path = args.translation_verification.resolve()
    verification_hash = digest(verification_path)
    verification = json.loads(verification_path.read_text())
    if not (verification.get("passed") is True
            and verification.get("translation_archive_sha256") == translated["archive_sha256"]
            and verification.get("arithmetic_archive_sha256") == arithmetic_sha256):
        raise RuntimeError("linked execution receipt does not seal both actual input archives")
    softfloat_notice = arithmetic.parent / "SoftFloat-COPYING.txt"
    if digest(softfloat_notice) != pins["softfloat_notice_sha256"]:
        raise RuntimeError("official SoftFloat notice seal failed")
    output.mkdir(parents=True)
    commands = []
    environment = os.environ.copy()
    environment["EMSDK_NODE"] = str(args.node.resolve())

    def run(command, *, cwd=ROOT, expected_status=0, timeout=None):
        index = len(commands)
        started = time.monotonic()
        log = output / f"command_{index:03d}.log"
        with log.open("wb") as stream:
            try:
                result = subprocess.run([str(value) for value in command], cwd=cwd,
                                        env=environment, stdout=stream, stderr=subprocess.STDOUT,
                                        timeout=timeout)
                status, timed_out = result.returncode, False
            except subprocess.TimeoutExpired:
                status, timed_out = None, True
        record = {"command": [str(value) for value in command], "cwd": str(cwd),
                  "status": status, "expected_status": expected_status,
                  "timeout_seconds": timeout, "timed_out": timed_out,
                  "elapsed_seconds": time.monotonic() - started, "log": str(log)}
        commands.append(record)
        (output / "commands.json").write_text(json.dumps(commands, indent=2) + "\n")
        if timed_out or status != expected_status:
            raise RuntimeError(f"command {index} refused with status {status}, timed_out={timed_out}; see {log}")
        return log

    def cached_repository(path):
        if path is None or not path.exists():
            return None
        probe = subprocess.run(["git", "-C", str(path), "rev-parse", "--show-toplevel"],
                               text=True, stdout=subprocess.PIPE, stderr=subprocess.DEVNULL)
        return path if probe.returncode == 0 and Path(probe.stdout.strip()).resolve() == path.resolve() else None

    def clone(url, revision, destination, reference=None):
        repository = cached_repository(reference) or url
        run(["git", "clone", "--no-hardlinks", "--no-checkout", repository, destination])
        run(["git", "-C", destination, "checkout", "--detach", revision])
        actual = subprocess.check_output(["git", "-C", destination, "rev-parse", "HEAD"], text=True).strip()
        if actual != revision:
            raise RuntimeError(f"public source revision differs: {destination}")

    source = output / "azahar_source"
    reference = args.reference_source.resolve() if args.reference_source else None
    clone(pins["azahar_url"], pins["azahar_revision"], source, reference)
    configuration = subprocess.check_output(
        ["git", "config", "-f", str(source / ".gitmodules"), "--get-regexp", r"submodule\..*\.(path|url)"], text=True)
    submodules = {}
    for line in configuration.splitlines():
        key, value = line.split(None, 1)
        group, field = key.rsplit(".", 1)
        submodules.setdefault(group, {})[field] = value
    urls = {value["path"]: value["url"] for value in submodules.values()}
    for relative, revision in pins["dependencies"].items():
        gitlink = subprocess.check_output(["git", "-C", source, "ls-tree", "HEAD", relative], text=True).split()
        if len(gitlink) < 3 or gitlink[0] != "160000" or gitlink[2] != revision:
            raise RuntimeError(f"dependency pin differs from public Azahar gitlink: {relative}")
        clone(urls[relative], revision, source / relative, reference / relative if reference else None)
    robin_reference = reference / "externals/dynarmic/externals/robin-map" if reference else None
    clone(pins["robin_map_url"], pins["robin_map_revision"], source / "externals/robin-map", robin_reference)

    def apply(path, expected, destination):
        if digest(path) != expected:
            raise RuntimeError(f"public patch seal failed: {path.name}")
        run(["git", "apply", "--check", path], cwd=destination)
        run(["git", "apply", path], cwd=destination)

    for name, expected in pins["reference_patches"].items():
        apply(ROOT / "tools/static_recompiler/azahar_reference" / name, expected, source)
    for name, expected in pins["platform_patches"].items():
        apply(PACKAGE / name, expected, source)
    apply(PACKAGE / pins["libressl_patch"]["name"], pins["libressl_patch"]["sha256"], source / "externals/libressl")
    notices = output / "licenses"
    notices.mkdir()
    license_seals = {}
    for relative, seal in pins["licenses"].items():
        original = output / relative
        if digest(original) != seal["sha256"]:
            raise RuntimeError(f"official license seal failed: {relative}")
        name = "_".join(Path(relative).parts[1:])
        shutil.copy2(original, notices / name)
        license_seals[name] = seal["sha256"]
    shutil.copy2(softfloat_notice, notices / "SoftFloat-COPYING.txt")
    license_seals["SoftFloat-COPYING.txt"] = digest(softfloat_notice)

    build = output / "compiled"
    options = [
        "-DARCHITECTURE=GENERIC", "-DROOT_PORT_CAPTURE_MINIMAL_DEPENDENCIES=ON",
        "-DCMAKE_BUILD_TYPE=Release", "-DCMAKE_C_FLAGS_RELEASE=-O1 -DNDEBUG -pthread",
        "-DCMAKE_CXX_FLAGS_RELEASE=-O1 -DNDEBUG -pthread -fwasm-exceptions",
        "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON", "-DENABLE_QT_TRANSLATION=OFF",
        "-DENABLE_BUILTIN_KEYBLOB=OFF", "-DENABLE_LTO=OFF", "-DCITRA_USE_PRECOMPILED_HEADERS=OFF",
        "-DCITRA_WARNINGS_AS_ERRORS=OFF", "-DENABLE_DISCORD_RPC=OFF",
        "-DUSE_SYSTEM_FMT=OFF", "-DUSE_SYSTEM_OPENSSL=OFF", "-DUSE_SYSTEM_ZSTD=OFF",
        "-DUSE_SYSTEM_BOOST=OFF", "-DFMT_DOC=OFF", "-DFMT_TEST=OFF",
        "-DROOT_PORT_WEBASSEMBLY_PLATFORM=ON", "-DENABLE_SOFTWARE_RENDERER=ON",
        "-DENABLE_ASM=OFF", "-DCRYPTOPP_DISABLE_ASM=ON", "-DCMAKE_TRY_COMPILE_TARGET_TYPE=EXECUTABLE",
        f"-DROOT_PORT_SOURCE_DIRECTORY={ROOT}", f"-DROOT_PORT_TRANSLATED_DIRECTORY={translation / 'sources'}",
        f"-DROOT_PORT_TRANSLATED_LIBRARY_FILE={archive}", f"-DROOT_PORT_ARITHMETIC_LIBRARY_FILE={arithmetic}",
    ]
    options += [f"-D{name}=OFF" for name in
                ("ENABLE_QT", "ENABLE_SDL2", "ENABLE_OPENAL", "ENABLE_CUBEB", "ENABLE_LIBUSB",
                 "ENABLE_WEB_SERVICE", "ENABLE_SCRIPTING", "ENABLE_GDBSTUB", "ENABLE_TESTS",
                 "ENABLE_ROOM", "ENABLE_ROOM_STANDALONE", "ENABLE_OPENGL", "ENABLE_VULKAN")]
    run([args.emsdk.resolve() / "upstream/emscripten/emcmake", args.cmake,
         "-S", source, "-B", build, *options])
    run([args.cmake, "--build", build, "--target", "root_port_webassembly_capture", "--parallel", "1"])
    javascript = build / "bin/Release/root_port_webassembly_capture.js"
    wasm = javascript.with_suffix(".wasm")
    usage = run([args.node.resolve(), javascript], expected_status=2, timeout=60)
    if "usage: azahar_compiled_execution" not in usage.read_text():
        raise RuntimeError("finite Node host did not reach its real pre-guest argument refusal")
    if (digest(archive) != translated["archive_sha256"] or digest(arithmetic) != arithmetic_sha256
            or digest(verification_path) != verification_hash
            or digest(softfloat_notice) != pins["softfloat_notice_sha256"]):
        raise RuntimeError("protected translation/arithmetic input changed during platform build")
    receipt = {"passed": True, "source_revisions_sha256": digest(PACKAGE / "source_revisions.json"),
               "translation_manifest_sha256": digest(translation / "build_manifest.json"),
               "translation_verification_sha256": verification_hash,
               "translation_archive_sha256": translated["archive_sha256"], "arithmetic_archive_sha256": arithmetic_sha256,
               "javascript": str(javascript), "javascript_sha256": digest(javascript),
               "wasm": str(wasm), "wasm_sha256": digest(wasm), "wasm_bytes": wasm.stat().st_size,
               "commands": commands, "licenses": license_seals,
               "scope": "Complete finite Node filesystem capture host link and pre-guest refusal only.",
               "guest_execution_verified": False, "browser_execution_verified": False}
    (output / "build_manifest.json").write_text(json.dumps(receipt, indent=2) + "\n")
    print(json.dumps({key: receipt[key] for key in ("passed", "wasm_bytes", "wasm_sha256", "scope")}))


if __name__ == "__main__":
    main()
