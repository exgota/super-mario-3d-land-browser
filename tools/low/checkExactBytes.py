#!/usr/bin/env python3
"""Link one source-generated ARM section at its unchanged original address.

Only compiler-object bytes and symbol addresses from map.csv enter the link.
The target executable is read for hash verification and final comparison only.
"""

import sys

# Direct execution must not let this directory's glob.py shadow the standard
# library module imported by pathlib on newer Python releases.
if __name__ == "__main__" and sys.path:
    sys.path.pop(0)

import argparse
import csv
import hashlib
import io
import json
import os
import re
import struct
import subprocess
import tempfile
from pathlib import Path

from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
from elftools.common.exceptions import ELFError

sys.path.insert(0, str(Path(__file__).resolve().parents[2]))
from tools.low.getSection import typeToSection


class ExactByteError(Exception):
    pass


def _read_map(path):
    rows = []
    with path.open(newline="") as stream:
        for source in csv.DictReader(stream):
            row = {key.strip(): value.strip() for key, value in source.items()}
            if not row["Start"].startswith("0x"):
                continue
            row["Start"] = int(row["Start"], 16)
            row["End"] = int(row["End"], 16)
            rows.append(row)
    return rows


def _resolve_symbol(symbol, source_section, rows):
    name = symbol.name
    matches = [row for row in rows if row["Symbol"] == name and name]
    if not matches and symbol["st_info"]["type"] == "STT_SECTION":
        section_name = source_section.name if source_section is not None else name
        matches = []
        for row in rows:
            mapped_name = row["Symbol"]
            if not mapped_name:
                mapped_name = ("fn_" if "f" in row["Type"] else "dat_") + f'{row["Start"]:08X}'
            names = {row["SectionName"], typeToSection(row["Type"], mapped_name)}
            if section_name in names:
                matches.append(row)
    if not matches:
        # These names encode an address, but it must already have a map row.
        address_name = re.fullmatch(r"(fn|dat)_([0-9A-Fa-f]{8})", name)
        if address_name:
            address = int(address_name.group(2), 16)
            kind = "f" if address_name.group(1) == "fn" else "d"
            matches = [row for row in rows if row["Start"] == address and kind in row["Type"]]
    if not matches:
        raise ExactByteError(f"No established original address for {name or '[unnamed symbol]'}.")
    addresses = {row["Start"] for row in matches}
    kinds = {"A" if "f" in row["Type"] else "D" for row in matches}
    if len(addresses) != 1 or len(kinds) != 1:
        raise ExactByteError(f"Ambiguous original address or kind for {name}.")
    address = next(iter(addresses))
    if symbol["st_info"]["type"] == "STT_SECTION":
        address += symbol["st_value"]
    return address, next(iter(kinds)), matches[0]


def _write_object(path, elf, code_section, symbol_data, string_data, relocation_data, local_count):
    # ELF32 section indices: selected code 1, symbols 2, strings 3, relocations 4.
    sections = [
        ("", 0, 0, b"", 0, 0, 0, 0),
        (code_section.name, 1, code_section["sh_flags"] & ~0x200, code_section.data(),
         0, 0, max(4, code_section["sh_addralign"]), code_section["sh_entsize"]),
        (".symtab", 2, 0, symbol_data, 3, local_count, 4, 16),
        (".strtab", 3, 0, string_data, 0, 0, 1, 0),
        (".rel" + code_section.name, 9, 0, relocation_data, 2, 1, 4, 8),
    ]
    attributes = [section for section in elf.iter_sections() if section["sh_type"] == "SHT_ARM_ATTRIBUTES"]
    if len(attributes) > 1:
        raise ExactByteError("Multiple ARM attribute sections are unsupported.")
    if attributes:
        sections.append((".ARM.attributes", 0x70000003, 0, attributes[0].data(), 0, 0, 1, 0))
    name_data = bytearray(b"\0")
    name_offsets = {}
    for name in [section[0] for section in sections] + [".shstrtab"]:
        name_offsets[name] = len(name_data)
        name_data.extend(name.encode() + b"\0")
    sections.append((".shstrtab", 3, 0, bytes(name_data), 0, 0, 1, 0))

    output = bytearray(52)
    headers = []
    for index, (name, kind, flags, data, link, info, alignment, entry_size) in enumerate(sections):
        if index == 0:
            headers.append(bytes(40))
            continue
        output.extend(bytes((-len(output)) % alignment))
        offset = len(output)
        output.extend(data)
        headers.append(struct.pack("<10I", name_offsets[name], kind, flags, 0, offset,
                                   len(data), link, info, alignment, entry_size))
    output.extend(bytes((-len(output)) % 4))
    section_offset = len(output)
    output.extend(b"".join(headers))
    output[:16] = b"\x7fELF\x01\x01\x01" + bytes(9)
    output[16:52] = struct.pack("<HHIIIIIHHHHHH", 1, 40, 1, 0, 0, section_offset,
                               elf["e_flags"], 52, 0, 0, 40, len(sections), len(sections) - 1)
    path.write_bytes(output)


def _isolate_function(object_path, symbol_name, rows, candidate_path):
    original_object = object_path.read_bytes()
    elf = ELFFile(io.BytesIO(original_object))
    if elf.elfclass != 32 or not elf.little_endian or elf["e_machine"] != "EM_ARM" or elf["e_type"] != "ET_REL":
        raise ExactByteError("The compiled input must be a little-endian ELF32 ARM relocatable object.")
    symbols = elf.get_section_by_name(".symtab")
    if symbols is None or symbols["sh_entsize"] != 16:
        raise ExactByteError("The compiled object has no supported symbol table.")
    selected = [symbol for symbol in symbols.iter_symbols()
                if symbol.name == symbol_name and isinstance(symbol["st_shndx"], int)]
    if len(selected) != 1:
        raise ExactByteError("The requested symbol has no unique compiled definition.")
    selected = selected[0]
    section_index = selected["st_shndx"]
    code_section = elf.get_section(section_index)
    if selected["st_value"] != 0 or selected["st_info"]["type"] != "STT_FUNC":
        raise ExactByteError("The requested function must start at offset zero in its own section.")
    if code_section["sh_type"] != "SHT_PROGBITS" or code_section["sh_flags"] & 6 != 6:
        raise ExactByteError("The selected section is not allocated executable compiler code.")
    if code_section["sh_addralign"] > 4:
        raise ExactByteError("Selected section alignment above four bytes is unsupported.")
    for symbol in symbols.iter_symbols():
        if symbol["st_shndx"] == section_index and symbol["st_info"]["type"] == "STT_FUNC" and symbol["st_value"] != 0:
            # ARMCC marks its switch-table base as a local, zero-size function
            # label. It belongs to this section and keeps its original value
            # for the compiler's ABS32 case-table relocations.
            if (symbol.name == "__switch$$" and symbol["st_info"]["bind"] == "STB_LOCAL"
                    and symbol["st_size"] == 0 and symbol["st_value"] % 4 == 0
                    and symbol["st_value"] < code_section["sh_size"]):
                continue
            raise ExactByteError(f"The selected section is shared with {symbol.name}.")

    relocations = []
    for section in elf.iter_sections():
        if not isinstance(section, RelocationSection) or section["sh_info"] != section_index:
            continue
        if section["sh_type"] != "SHT_REL" or section["sh_link"] != elf.get_section_index(".symtab"):
            raise ExactByteError("Only ARM REL relocations against the original symbol table are supported.")
        relocations.extend(section.iter_relocations())
    required = {0}
    required.update(relocation["r_info_sym"] for relocation in relocations)
    required.update(index for index, symbol in enumerate(symbols.iter_symbols()) if symbol["st_shndx"] == section_index)
    entries = []
    imports = []
    string_data = bytearray(b"\0")
    for original_index in sorted(required):
        symbol = symbols.get_symbol(original_index)
        source = struct.unpack_from("<IIIBBH", symbols.data(), original_index * 16)
        _, value, size, information, other, original_section = source
        if original_index == 0:
            entries.append((original_index, 0, 0, 0, 0, 0, 0))
            continue
        name = symbol.name
        if not name and symbol["st_info"]["type"] == "STT_SECTION" and isinstance(symbol["st_shndx"], int):
            name = elf.get_section(symbol["st_shndx"]).name
        if any(character.isspace() for character in name) or not name:
            raise ExactByteError("A required symbol has an empty or unsupported name.")
        name_offset = len(string_data)
        string_data.extend(name.encode() + b"\0")
        if symbol["st_shndx"] == section_index:
            new_section = 1
        else:
            source_section = elf.get_section(symbol["st_shndx"]) if isinstance(symbol["st_shndx"], int) else None
            address, kind, map_row = _resolve_symbol(symbol, source_section, rows)
            imports.append({"symbol": name, "address": address, "kind": kind,
                            "map_symbol": map_row["Symbol"], "map_start": map_row["Start"],
                            "map_end": map_row["End"],
                            "defined_in_input": isinstance(symbol["st_shndx"], int)})
            value, size, other, new_section = 0, 0, 0, 0
            # A section symbol cannot remain STT_SECTION after becoming external.
            symbol_type = 0 if symbol["st_info"]["type"] == "STT_SECTION" else information & 15
            information = 0x10 | symbol_type
        entries.append((original_index, name_offset, value, size, information, other, new_section))
    entries.sort(key=lambda entry: entry[4] >> 4 != 0)
    new_indices = {entry[0]: index for index, entry in enumerate(entries)}
    local_count = sum(entry[4] >> 4 == 0 for entry in entries)
    symbol_data = b"".join(struct.pack("<IIIBBH", *entry[1:]) for entry in entries)
    relocation_data = bytearray()
    for relocation in relocations:
        offset = relocation["r_offset"]
        if offset % 4 or offset + 4 > code_section["sh_size"]:
            raise ExactByteError("A required relocation lies outside the selected section or is unaligned.")
        information = (new_indices[relocation["r_info_sym"]] << 8) | relocation["r_info_type"]
        relocation_data.extend(struct.pack("<II", offset, information))
    _write_object(candidate_path, elf, code_section, symbol_data, bytes(string_data), bytes(relocation_data), local_count)
    return code_section.name, code_section.data(), imports


def check_exact_bytes(symbol_name, object_path, version="eu", compiler_version="4.1/791", output_directory=None):
    """Return {exact, reason, evidence}; never change ranks, maps, or source files."""
    evidence = {"symbol": symbol_name, "object": str(object_path), "version": version,
                "compiler": compiler_version}
    try:
        root = Path(__file__).resolve().parents[2]
        if not re.fullmatch(r"[A-Za-z0-9_]+", version):
            raise ExactByteError("The version must name a repository version directory.")
        if not re.fullmatch(r"[0-9]+\.[0-9]+/[0-9]+", compiler_version):
            raise ExactByteError("The compiler version must name an installed ARMCC release/build.")
        object_path = Path(object_path).resolve()
        map_path = root / "data/ver" / version / "map.csv"
        rows = _read_map(map_path)
        selected = [row for row in rows if row["Symbol"] == symbol_name]
        if len(selected) != 1 or "f" not in selected[0]["Type"]:
            raise ExactByteError("The requested symbol has no unique original function interval.")
        selected = selected[0]
        start, end = selected["Start"], selected["End"]
        expected_size = end - start
        if expected_size <= 0 or start % 4:
            raise ExactByteError("The original function interval is empty or unaligned.")
        evidence.update({"original_start": start, "original_end": end,
                         "original_size": expected_size,
                         "map_sha256": hashlib.sha256(map_path.read_bytes()).hexdigest(),
                         "object_sha256": hashlib.sha256(object_path.read_bytes()).hexdigest()})
        build_root = (root / "build").resolve()
        if output_directory is None:
            parent = build_root / "exact_checks" / version
            parent.mkdir(parents=True, exist_ok=True)
            output_directory = Path(tempfile.mkdtemp(prefix=f"function_{start:08X}_", dir=parent))
        else:
            output_directory = Path(output_directory).resolve()
            if not output_directory.is_relative_to(build_root):
                raise ExactByteError("Generated outputs must remain under the ignored build directory.")
            output_directory.mkdir(parents=True, exist_ok=True)
        evidence["output_directory"] = str(output_directory)
        candidate_path = output_directory / "candidate.o"
        section_name, compiled_bytes, imports = _isolate_function(object_path, symbol_name, rows, candidate_path)
        evidence.update({"compiled_section": section_name, "compiled_section_size": len(compiled_bytes),
                         "compiled_section_sha256": hashlib.sha256(compiled_bytes).hexdigest(),
                         "isolated_object_sha256": hashlib.sha256(candidate_path.read_bytes()).hexdigest(),
                         "imports": imports})
        if len(compiled_bytes) != expected_size:
            raise ExactByteError("The complete compiled section, including its literal pool, has a different size from the original interval.")
        symbol_file = output_directory / "original_symbols.sym"
        symbol_file.write_text("#<SYMDEFS>#\n" + "".join(
            f'0x{entry["address"]:08X} {entry["kind"]} {entry["symbol"]}\n' for entry in imports))
        scatter_path = output_directory / "candidate.sct"
        scatter_path.write_text(f"CANDIDATE_LOAD 0x{start:08X}\n{{\n"
                               f"    CANDIDATE_CODE 0x{start:08X} 0x{expected_size:X}\n    {{\n"
                               f"        candidate.o ({section_name}, +FIRST)\n    }}\n}}\n")
        linker = root / "data/compilers" / compiler_version / "bin/armlink.exe"
        command = [str(linker)]
        if sys.platform != "win32":
            command.insert(0, str(root / "data/compilers/wibo"))
        command.extend(["--cpu=MPCore", "--fpu=VFPv2", "--arm_only", "--no_exceptions", "--inline",
                        "--datacompressor=off", "--no_debug", "--no_scanlib", "--mangled", "--symbols", "--map",
                        f"--entry={symbol_name}", f"--keep={symbol_name}", f"--scatter={scatter_path}",
                        f"--output={output_directory / 'candidate.axf'}",
                        f"--list={output_directory / 'candidate.map'}", str(candidate_path), str(symbol_file)])
        evidence["link_command"] = command
        environment = os.environ.copy()
        environment["TMP"] = "/tmp"
        result = subprocess.run(command, cwd=root, env=environment, stdout=subprocess.PIPE,
                                stderr=subprocess.STDOUT, text=True, timeout=60)
        (output_directory / "link.log").write_text(result.stdout)
        if result.returncode != 0:
            raise ExactByteError(f"Original-address link failed with exit {result.returncode}; see link.log.")
        with (output_directory / "candidate.axf").open("rb") as stream:
            linked_elf = ELFFile(stream)
            allocated = [section for section in linked_elf.iter_sections()
                         if section["sh_flags"] & 2 and section["sh_size"]]
            if len(allocated) != 1 or allocated[0].name != "CANDIDATE_CODE":
                raise ExactByteError("The linked image contains unexpected allocated sections.")
            linked_section = allocated[0]
            if linked_section["sh_type"] != "SHT_PROGBITS" or linked_section["sh_addr"] != start or linked_section["sh_size"] != expected_size:
                raise ExactByteError("The linked section's address or full size differs from the original interval.")
            linked_bytes = linked_section.data()
        config = json.loads((root / "data/config.json").read_text())
        target = (root / "data/ver" / version / "code.bin").read_bytes()
        target_hash = hashlib.sha256(target).hexdigest()
        evidence["target_sha256"] = target_hash
        if config["versions"].get(version) != target_hash:
            raise ExactByteError("The target executable does not match the unchanged configured version hash.")
        header = (root / "data/ver" / version / "exh.bin").read_bytes()
        load_base = struct.unpack_from("<I", header, 0x10)[0]
        offset = start - load_base
        if offset < 0 or offset + expected_size > len(target):
            raise ExactByteError("The original interval lies outside the verified target executable.")
        original_bytes = target[offset:offset + expected_size]
        different = [index for index, (original, current) in enumerate(zip(original_bytes, linked_bytes))
                     if original != current]
        evidence.update({"linked_section_sha256": hashlib.sha256(linked_bytes).hexdigest(),
                         "different_bytes": len(different),
                         "first_differences": [start + index for index in different[:8]]})
        exact = not different
        reason = "The complete source-generated function interval matches byte for byte." if exact else "The linked candidate differs from the unchanged original interval."
        return {"exact": exact, "reason": reason, "evidence": evidence}
    except (ExactByteError, ELFError, OSError, ValueError, KeyError, struct.error, subprocess.TimeoutExpired) as error:
        return {"exact": False, "reason": str(error), "evidence": evidence}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("symbol")
    parser.add_argument("object", type=Path)
    parser.add_argument("--version", default="eu")
    parser.add_argument("--compiler", default="4.1/791")
    arguments = parser.parse_args()
    result = check_exact_bytes(arguments.symbol, arguments.object, arguments.version, arguments.compiler)
    print(json.dumps(result, indent=2))
    return 0 if result["exact"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
