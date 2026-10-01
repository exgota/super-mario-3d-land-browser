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
import time
from datetime import datetime, timezone
from pathlib import Path

from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
from elftools.common.exceptions import ELFError
from elftools.elf.enums import ENUM_SH_TYPE_ARM

sys.path.insert(0, str(Path(__file__).resolve().parents[2]))
from tools.low.getSection import typeToSection
from tools.low.buildProvenance import provenance_path, verify_build_output
from tools.low.glob import getBuildObjPath, getVersion


class ExactByteError(Exception):
    pass


class UnresolvedOriginalSymbol(ExactByteError):
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
        raise UnresolvedOriginalSymbol(f"No established original address for {name or '[unnamed symbol]'}.")
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


def _write_closure_object(record, retained, output, external_names):
    """Preserve source-generated sections and relocations for an isolated link."""
    elf = record["elf"]
    symbols = record["symbols"]
    table = record["table"]
    required = {0}
    for index, symbol in enumerate(symbols):
        if symbol["st_shndx"] in retained:
            required.add(index)
    for section_index in retained:
        required.update(relocation["r_info_sym"] for relocation in record["relocations"].get(section_index, []))

    ordered = sorted(retained)
    section_indices = {index: destination for destination, index in enumerate(ordered, 1)}
    strings = bytearray(b"\0")
    entries = []
    discarded = []
    for index in sorted(required):
        symbol = symbols[index]
        _, value, size, information, other, section_index = struct.unpack_from("<IIIBBH", table.data(), index * 16)
        if index == 0:
            entries.append((index, 0, 0, 0, 0, 0, 0))
            continue
        name = symbol.name
        if index in external_names:
            name = external_names[index]
            value, size, other, section_index = 0, 0, 0, 0
            information = 0x10 | (2 if symbol["st_info"]["type"] == "STT_FUNC" else 0)
        elif isinstance(symbol["st_shndx"], int):
            if symbol["st_shndx"] in retained:
                section_index = section_indices[symbol["st_shndx"]]
            else:
                if symbol["st_info"]["type"] == "STT_SECTION":
                    functions = [item for item in symbols
                                 if item["st_shndx"] == symbol["st_shndx"]
                                 and item["st_info"]["type"] == "STT_FUNC" and item["st_value"] == 0]
                    names = {item.name for item in functions if item.name}
                    if len(names) != 1:
                        raise ValueError("A discarded section has no unique function import: " + elf.get_section(symbol["st_shndx"]).name)
                    name = next(iter(names))
                if not name or value:
                    raise ValueError("A discarded definition has no supported import identity: " + name)
                discarded.append(name)
                value, size, other, section_index = 0, 0, 0, 0
                information = 0x10 | (2 if symbol["st_info"]["type"] == "STT_FUNC" else 0)
        if not name and symbol["st_info"]["type"] == "STT_SECTION":
            name = elf.get_section(symbol["st_shndx"]).name
        name_offset = len(strings)
        strings.extend(name.encode() + b"\0")
        entries.append((index, name_offset, value, size, information, other, section_index))
    entries.sort(key=lambda entry: entry[4] >> 4 != 0)
    symbol_indices = {entry[0]: index for index, entry in enumerate(entries)}
    local_count = sum(entry[4] >> 4 == 0 for entry in entries)

    sections = [{"name": "", "type": 0, "flags": 0, "data": b"", "size": 0,
                 "link": 0, "info": 0, "alignment": 0, "entry_size": 0}]
    retained_evidence = []
    for index in ordered:
        source = elf.get_section(index)
        kind = ENUM_SH_TYPE_ARM[source["sh_type"]]
        data = source.data() if source["sh_type"] != "SHT_NOBITS" else b""
        link = section_indices[source["sh_link"]] if source["sh_flags"] & 0x80 else 0
        sections.append({"name": source.name, "type": kind, "flags": source["sh_flags"] & ~0x200,
                         "data": data, "size": source["sh_size"], "link": link, "info": 0,
                         "alignment": source["sh_addralign"], "entry_size": source["sh_entsize"]})
        retained_evidence.append({"name": source.name, "source_index": index, "size": source["sh_size"],
                                  "sha256": hashlib.sha256(data).hexdigest()})
    attributes = [section for section in elf.iter_sections() if section["sh_type"] == "SHT_ARM_ATTRIBUTES"]
    for source in attributes:
        sections.append({"name": source.name, "type": 0x70000003, "flags": 0, "data": source.data(),
                         "size": source["sh_size"], "link": 0, "info": 0, "alignment": 1, "entry_size": 0})
    symbol_section = len(sections)
    symbol_data = b"".join(struct.pack("<IIIBBH", *entry[1:]) for entry in entries)
    sections.append({"name": ".symtab", "type": 2, "flags": 0, "data": symbol_data, "size": len(symbol_data),
                     "link": symbol_section + 1, "info": local_count, "alignment": 4, "entry_size": 16})
    sections.append({"name": ".strtab", "type": 3, "flags": 0, "data": bytes(strings), "size": len(strings),
                     "link": 0, "info": 0, "alignment": 1, "entry_size": 0})
    for index in ordered:
        relocations = record["relocations"].get(index, [])
        if not relocations:
            continue
        data = b"".join(struct.pack("<II", relocation["r_offset"],
                                    symbol_indices[relocation["r_info_sym"]] << 8 | relocation["r_info_type"])
                        for relocation in relocations)
        sections.append({"name": ".rel" + elf.get_section(index).name, "type": 9, "flags": 0,
                         "data": data, "size": len(data), "link": symbol_section,
                         "info": section_indices[index], "alignment": 4, "entry_size": 8})
    names = bytearray(b"\0")
    for section in sections[1:]:
        section["name_offset"] = len(names)
        names.extend(section["name"].encode() + b"\0")
    name_offset = len(names)
    names.extend(b".shstrtab\0")
    sections.append({"name": ".shstrtab", "name_offset": name_offset, "type": 3, "flags": 0,
                     "data": bytes(names), "size": len(names), "link": 0, "info": 0,
                     "alignment": 1, "entry_size": 0})
    content = bytearray(52)
    headers = [bytes(40)]
    for section in sections[1:]:
        alignment = max(1, section["alignment"])
        content.extend(bytes((-len(content)) % alignment))
        offset = len(content)
        content.extend(section["data"])
        headers.append(struct.pack("<10I", section["name_offset"], section["type"], section["flags"], 0,
                                   offset, section["size"], section["link"], section["info"],
                                   section["alignment"], section["entry_size"]))
    content.extend(bytes((-len(content)) % 4))
    section_offset = len(content)
    content.extend(b"".join(headers))
    content[:16] = b"\x7fELF\x01\x01\x01" + bytes(9)
    content[16:52] = struct.pack("<HHIIIIIHHHHHH", 1, 40, 1, 0, 0, section_offset,
                               elf["e_flags"], 52, 0, 0, 40, len(sections), len(sections) - 1)
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_bytes(content)
    projected = ELFFile(io.BytesIO(content))
    for destination, source_index in enumerate(ordered, 1):
        original = elf.get_section(source_index)
        actual = projected.get_section(destination)
        if original["sh_size"] != actual["sh_size"] or original.data() != actual.data():
            raise ValueError("A compiler-generated section changed during compact projection: " + original.name)
    return {"projected_object": str(output), "projected_sha256": hashlib.sha256(content).hexdigest(),
            "retained_sections": retained_evidence, "discarded_definition_imports": sorted(set(discarded))}


def _read_closure_object(path):
    provenance = verify_build_output(path)
    content = path.read_bytes()
    if hashlib.sha256(content).hexdigest() != provenance['object_sha256']:
        raise ExactByteError('A canonical object changed while its provenance was being read.')
    elf = ELFFile(io.BytesIO(content))
    if elf.elfclass != 32 or not elf.little_endian or elf['e_type'] != 'ET_REL' or (elf['e_machine'] != 'EM_ARM'):
        raise ExactByteError('A closure input is not an ELF32 ARM relocatable object.')
    table = elf.get_section_by_name('.symtab')
    if table is None or table['sh_entsize'] != 16:
        raise ExactByteError('A closure input has no supported symbol table.')
    relocations = {}
    for section in elf.iter_sections():
        if isinstance(section, RelocationSection):
            if section['sh_type'] != 'SHT_REL' or section['sh_link'] != elf.get_section_index('.symtab'):
                raise ExactByteError('A closure input has unsupported relocation metadata.')
            relocations.setdefault(section['sh_info'], []).extend(section.iter_relocations())
    return {'path': path, 'elf': elf, 'table': table,
            'symbols': list(table.iter_symbols()), 'relocations': relocations,
            'provenance': provenance, 'sha256': hashlib.sha256(content).hexdigest(),
            'provenance_sha256': hashlib.sha256(provenance_path(path).read_bytes()).hexdigest()}

def _validate_closure_function(record, section_index):
    section = record['elf'].get_section(section_index)
    functions = [s for s in record['symbols'] if s['st_shndx'] == section_index and s['st_info']['type'] == 'STT_FUNC']
    if section['sh_type'] != 'SHT_PROGBITS' or section['sh_flags'] & 6 != 6 or section['sh_addralign'] > 4 or (not functions):
        raise ExactByteError('A reached closure section is not supported executable compiler code.')
    for symbol in functions:
        if symbol['st_value'] == 0:
            continue
        if (symbol.name == '__switch$$' and symbol['st_info']['bind'] == 'STB_LOCAL'
                and symbol['st_size'] == 0 and symbol['st_value'] % 4 == 0
                and symbol['st_value'] < section['sh_size']):
            continue
        raise ExactByteError('A reached closure section is shared with another function: ' + symbol.name)
    return section

def _check_inline_closure(symbol_name, object_path, helper_paths, version, compiler_version, output):
    repository = Path(__file__).resolve().parents[2]
    evidence = {'symbol': symbol_name, 'root': str(object_path), 'helpers': list(map(str, helper_paths)), 'started_utc': datetime.now(timezone.utc).isoformat()}
    began = time.monotonic()
    try:
        if not re.fullmatch('[A-Za-z0-9_]+', version) or not re.fullmatch('[0-9]+\\.[0-9]+/[0-9]+', compiler_version):
            raise ExactByteError('The closure version or compiler identifier is invalid.')
        if output is None:
            parent = repository / 'build/exact_checks' / version
            parent.mkdir(parents=True, exist_ok=True)
            output = Path(tempfile.mkdtemp(prefix='inline_closure_', dir=parent))
        else:
            output = Path(output).resolve()
        object_path = Path(object_path).resolve()
        helper_paths = [Path(path).resolve() for path in helper_paths]
        paths = [object_path.resolve()] + [p.resolve() for p in helper_paths]
        if len(paths) != len(set(paths)):
            raise ExactByteError('Closure inputs must be distinct canonical objects.')
        records = [_read_closure_object(p) for p in paths]
        compiler = records[0]['provenance']['compiler']
        if compiler != compiler_version or version != getVersion():
            raise ExactByteError('Closure objects must use the requested configured compiler and build version.')
        if any((r['provenance']['compiler'] != compiler for r in records)):
            raise ExactByteError('Closure objects must use the same configured compiler build.')
        map_path = repository / 'data/ver' / version / 'map.csv'
        map_hash = hashlib.sha256(map_path.read_bytes()).hexdigest()
        rows = _read_map(map_path)
        evidence['map_sha256'] = map_hash
        selected = [r for r in rows if r['Symbol'] == symbol_name and 'f' in r['Type']]
        if len(selected) != 1:
            raise ExactByteError('The root requires one established original function interval.')
        original = selected[0]
        start, end = (original['Start'], original['End'])
        if end <= start or start % 4:
            raise ExactByteError('The original function interval is empty or unaligned.')
        roots = [s for s in records[0]['symbols'] if s.name == symbol_name and isinstance(s['st_shndx'], int)]
        if len(roots) != 1 or roots[0]['st_value'] != 0 or roots[0]['st_info']['type'] != 'STT_FUNC':
            raise ExactByteError('The root requires one function at the start of its own compiler section.')
        root_section = _validate_closure_function(records[0], roots[0]['st_shndx'])
        evidence['compiled_root_size'] = root_section['sh_size']
        definitions = {}
        for index, record in enumerate(records):
            for symbol in record['symbols']:
                if symbol.name and isinstance(symbol['st_shndx'], int) and symbol['st_shndx'] and (symbol['st_info']['bind'] != 'STB_LOCAL'):
                    definitions.setdefault(symbol.name, []).append((index, symbol))
        retained = {index: set() for index in range(len(records))}
        external_names = {index: {} for index in range(len(records))}
        imports = {}
        helpers = []
        pending = [(0, roots[0]['st_shndx'])]
        while pending:
            index, section_index = pending.pop()
            if section_index in retained[index]:
                continue
            record = records[index]
            section = _validate_closure_function(record, section_index)
            retained[index].add(section_index)
            if (index, section_index) != (0, roots[0]['st_shndx']):
                helpers.append({'object': str(record['path']), 'section': section.name,
                                'size': section['sh_size'],
                                'sha256': hashlib.sha256(section.data()).hexdigest()})
            for relocation in record['relocations'].get(section_index, []):
                offset = relocation['r_offset']
                if offset % 4 or offset + 4 > section['sh_size']:
                    raise ExactByteError('A closure relocation lies outside its section or is unaligned.')
                symbol = record['symbols'][relocation['r_info_sym']]
                if symbol['st_shndx'] == section_index:
                    continue
                source = record['elf'].get_section(symbol['st_shndx']) if isinstance(symbol['st_shndx'], int) else None
                try:
                    address, kind, mapped = _resolve_symbol(symbol, source, rows)
                except UnresolvedOriginalSymbol:
                    local = isinstance(symbol['st_shndx'], int) and symbol['st_info']['bind'] == 'STB_LOCAL'
                    targets = [(index, symbol)] if local else definitions.get(symbol.name, [])
                    targets = [(i, s) for i, s in targets if s['st_info']['type'] == 'STT_FUNC' and s['st_value'] == 0]
                    if len(targets) != 1:
                        raise ExactByteError('A source helper has no unique supplied C++ definition: ' + symbol.name)
                    target_index, target = targets[0]
                    _validate_closure_function(records[target_index], target['st_shndx'])
                    pending.append((target_index, target['st_shndx']))
                    continue
                name = symbol.name or (source.name if source is not None else '')
                if not name or any((c.isspace() for c in name)):
                    raise ExactByteError('A runtime import has no supported identity.')
                prior = imports.get(name)
                if prior and (prior['address'], prior['kind']) != (address, kind):
                    raise ExactByteError('A runtime import has ambiguous original addresses.')
                imports[name] = {'symbol': name, 'address': address, 'kind': kind,
                                 'map_symbol': mapped['Symbol'],
                                 'map_start': mapped['Start'], 'map_end': mapped['End']}
                external_names[index][relocation['r_info_sym']] = name
        if any((not retained[index] for index in range(1, len(records)))):
            raise ExactByteError('A supplied helper object is unrelated to the selected source closure.')
        for index, record in enumerate(records):
            for section_index in retained[index]:
                for relocation in record['relocations'].get(section_index, []):
                    symbol = record['symbols'][relocation['r_info_sym']]
                    targets = definitions.get(symbol.name, [])
                    if any((target['st_shndx'] in retained[target_index]
                            and (target_index, target['st_shndx']) != (0, roots[0]['st_shndx'])
                            for target_index, target in targets)):
                        if relocation['r_info_type'] not in [1, 28, 29]:
                            raise ExactByteError('A source helper is referenced by a non-branch relocation.')
        if not output.resolve().is_relative_to(repository / 'build'):
            raise ExactByteError('Closure outputs must remain in ignored build storage.')
        output.mkdir(parents=True, exist_ok=True)
        objects = []
        projections = []
        for index, record in enumerate(records):
            if not retained[index]:
                continue
            target = output / f'input{index}.o'
            projections.append(_write_closure_object(record, retained[index], target, external_names[index]))
            objects.append(target)
        symbol_file = output / 'original_symbols.sym'
        symbol_file.write_text('#<SYMDEFS>#\n' + ''.join((f"0x{item['address']:08X} {item['kind']} {name}\n" for name, item in sorted(imports.items()))))
        scatter = output / 'candidate.sct'
        scatter.write_text(f'CANDIDATE_LOAD 0x{start:08X}\n{{\n'
                           f' CANDIDATE_CODE 0x{start:08X}\n {{\n'
                           f'  input0.o ({root_section.name}, +FIRST)\n  *(+RO)\n }}\n}}\n')
        command = [str(repository / 'data/compilers/wibo'),
                   str(repository / 'data/compilers' / compiler / 'bin/armlink.exe'),
                   '--cpu=MPCore', '--fpu=VFPv2', '--arm_only', '--no_exceptions',
                   '--inline', '--datacompressor=off', '--no_debug', '--no_scanlib',
                   '--mangled', '--symbols', '--map', f'--entry={symbol_name}',
                   f'--keep={symbol_name}', f'--scatter={scatter}',
                   f"--output={output / 'candidate.axf'}",
                   f"--list={output / 'candidate.map'}"] + list(map(str, objects)) + [str(symbol_file)]
        result = subprocess.run(command, cwd=repository, env=dict(os.environ, TMP='/tmp'), capture_output=True, text=True, timeout=60)
        (output / 'link.log').write_text(result.stdout + result.stderr)
        evidence.update(compiler=compiler, original_start=start, original_end=end,
                        helpers_reached=helpers, imports=list(imports.values()),
                        objects=projections, link_command=command, link_exit=result.returncode)
        for record in records:
            after = verify_build_output(record['path'])
            if after != record['provenance'] or hashlib.sha256(provenance_path(record['path']).read_bytes()).hexdigest() != record['provenance_sha256']:
                raise ExactByteError('A closure object or provenance record changed during the check.')
        if result.returncode:
            raise ExactByteError('The source closure link failed.')
        with (output / 'candidate.axf').open('rb') as stream:
            elf = ELFFile(stream)
            allocated = [s for s in elf.iter_sections() if s['sh_flags'] & 2 and s['sh_size']]
            evidence['allocated_sections'] = [{'name': s.name, 'address': s['sh_addr'], 'size': s['sh_size']} for s in allocated]
            if len(allocated) != 1 or allocated[0].name != 'CANDIDATE_CODE' or allocated[0]['sh_addr'] != start or (allocated[0]['sh_size'] != end - start):
                raise ExactByteError('Residual helper code or another allocated extent remains after inlining.')
            linked = allocated[0].data()
        linker_map = (output / 'candidate.map').read_text()
        global_symbols = linker_map.partition('Global Symbols')[2].partition('===')[0]
        root_symbol_pattern = (r'^\s*' + re.escape(symbol_name)
                               + r'\s+(0x[0-9a-fA-F]+)\s+ARM Code\s+(\d+)\s+input0\.o\('
                               + re.escape(root_section.name) + r'\)\s*$')
        root_symbols = re.findall(root_symbol_pattern, global_symbols, re.MULTILINE)
        if (len(root_symbols) != 1 or int(root_symbols[0][0], 16) != start
                or not 0 < int(root_symbols[0][1]) <= end - start
                or int(root_symbols[0][1]) % 4):
            raise ExactByteError('The selected linked function has no unique original start and valid ARM code size.')
        # ARM Code symbol sizes exclude trailing literal pools. The allocated
        # section and sole root memory entry enforce the complete extent.
        evidence['linked_root_symbol'] = {'name': symbol_name, 'address': start,
                                          'size': end - start, 'section': root_section.name,
                                          'code_size': int(root_symbols[0][1]),
                                          'object': 'input0.o'}
        memory_map = linker_map.partition('Memory Map of the image')[2]
        memory_entries = []
        for line in memory_map.splitlines():
            fields = line.split()
            if len(fields) >= 4 and fields[0].startswith('0x') and fields[1].startswith('0x') and (fields[2] in ('Code', 'Data', 'Zero')):
                memory_entries.append(line)
        if len(memory_entries) != 1 or not re.search(re.escape(root_section.name) + r"\s+input0\.o\s*$", memory_entries[0]):
            raise ExactByteError('The linker retained helper sections or another source extent.')
        evidence['root_memory_entry'] = memory_entries[0].strip()
        root_fields = memory_entries[0].split()
        evidence['linked_root_extent'] = {'address': int(root_fields[0], 16),
                                          'size': int(root_fields[1], 16)}
        if evidence['linked_root_extent'] != {'address': start, 'size': end - start}:
            raise ExactByteError('The selected root memory entry has a different original extent.')
        if hashlib.sha256(map_path.read_bytes()).hexdigest() != map_hash:
            raise ExactByteError('The original symbol map changed during the closure check.')
        target = (repository / 'data/ver' / version / 'code.bin').read_bytes()
        target_hash = hashlib.sha256(target).hexdigest()
        if target_hash != json.loads((repository / 'data/config.json').read_text())['versions'][version]:
            raise ExactByteError('The unchanged target version hash is invalid.')
        header = (repository / 'data/ver' / version / 'exh.bin').read_bytes()
        load_base = struct.unpack_from('<I', header, 16)[0]
        offset = start - load_base
        if offset < 0 or offset + end - start > len(target):
            raise ExactByteError('The original interval lies outside the verified target executable.')
        expected = target[offset:offset + end - start]
        differences = [i for i, (a, b) in enumerate(zip(linked, expected)) if a != b]
        evidence.update(different_bytes=len(differences),
                        linked_sha256=hashlib.sha256(linked).hexdigest(),
                        target_sha256=target_hash, provenance=[r['provenance'] for r in records])
        result = {'exact': not differences, 'rejected': False,
                  'reason': 'The complete source-generated function interval matches byte for byte.'
                  if not differences else 'The linked candidate differs from the unchanged original interval.'}
    except (OSError, ValueError, KeyError, ExactByteError, ELFError, struct.error, subprocess.TimeoutExpired) as error:
        result = {'exact': False, 'reason': str(error), 'rejected': True}
    evidence['inline_closure'] = True
    evidence['seconds'] = time.monotonic() - began
    evidence['finished_utc'] = datetime.now(timezone.utc).isoformat()
    result['evidence'] = evidence
    return result


def _discover_inline_objects(object_path, symbol_name, rows):
    """Find canonical source definitions only for unresolved branch helpers."""
    root = _read_closure_object(Path(object_path).resolve())
    records = {root["path"]: root}
    definitions = {}
    for path in sorted(getBuildObjPath().rglob("*.o")):
        try:
            provenance_record = json.loads(provenance_path(path).read_text())
        except (OSError, ValueError):
            continue
        if Path(provenance_record.get("source", "")).suffix not in (".cpp", ".cc", ".cxx"):
            continue
        content = path.read_bytes()
        try:
            elf = ELFFile(io.BytesIO(content))
            table = elf.get_section_by_name(".symtab")
            if table is None:
                continue
            for symbol in table.iter_symbols():
                if (symbol.name and isinstance(symbol["st_shndx"], int)
                        and symbol["st_shndx"] and symbol["st_info"]["type"] == "STT_FUNC"
                        and symbol["st_info"]["bind"] != "STB_LOCAL" and symbol["st_value"] == 0):
                    definitions.setdefault(symbol.name, []).append(path.resolve())
        except (ELFError, ValueError, KeyError, struct.error):
            continue
    selected = [symbol for symbol in root["symbols"]
                if symbol.name == symbol_name and isinstance(symbol["st_shndx"], int)]
    if len(selected) != 1:
        raise ExactByteError("The root has no unique canonical function definition.")
    pending = [(root["path"], selected[0]["st_shndx"])]
    visited = set()
    while pending:
        path, section_index = pending.pop()
        if (path, section_index) in visited:
            continue
        visited.add((path, section_index))
        record = records[path]
        _validate_closure_function(record, section_index)
        for relocation in record["relocations"].get(section_index, []):
            symbol = record["symbols"][relocation["r_info_sym"]]
            if symbol["st_shndx"] == section_index:
                continue
            source = record["elf"].get_section(symbol["st_shndx"]) if isinstance(symbol["st_shndx"], int) else None
            try:
                _resolve_symbol(symbol, source, rows)
                continue
            except UnresolvedOriginalSymbol:
                pass
            if relocation["r_info_type"] not in (1, 28, 29):
                raise ExactByteError("An unresolved source helper is referenced by a non-branch relocation.")
            if isinstance(symbol["st_shndx"], int):
                if symbol["st_info"]["type"] != "STT_FUNC" or symbol["st_value"]:
                    raise ExactByteError("An unresolved reference is not a standalone C++ function helper.")
                if symbol["st_info"]["bind"] != "STB_LOCAL":
                    providers = set(definitions.get(symbol.name, []))
                    if providers != {path}:
                        raise ExactByteError("A source helper has ambiguous canonical C++ definitions: " + symbol.name)
                pending.append((path, symbol["st_shndx"]))
                continue
            providers = sorted(set(definitions.get(symbol.name, [])))
            if len(providers) != 1:
                raise ExactByteError("A source helper has no unique canonical C++ definition: " + symbol.name)
            provider = providers[0]
            if provider not in records:
                records[provider] = _read_closure_object(provider)
            targets = [item for item in records[provider]["symbols"]
                       if item.name == symbol.name and isinstance(item["st_shndx"], int)
                       and item["st_info"]["type"] == "STT_FUNC" and item["st_value"] == 0]
            if len(targets) != 1:
                raise ExactByteError("A canonical provider has an ambiguous helper definition.")
            pending.append((provider, targets[0]["st_shndx"]))
    return sorted(path for path in records if path != root["path"])


def check_exact_bytes(symbol_name, object_path, version="eu", compiler_version="4.1/791", output_directory=None, inline_objects=None):
    """Return {exact, reason, evidence}; never change ranks, maps, or source files."""
    if inline_objects is not None:
        return _check_inline_closure(symbol_name, Path(object_path), list(inline_objects), version, compiler_version, output_directory)
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
    except UnresolvedOriginalSymbol as error:
        try:
            helpers = _discover_inline_objects(object_path, symbol_name, rows)
            return _check_inline_closure(symbol_name, object_path, helpers, version, compiler_version, output_directory)
        except (ExactByteError, ELFError, OSError, ValueError, KeyError) as closure_error:
            evidence["inline_closure"] = True
            return {"exact": False, "rejected": True, "reason": str(closure_error), "evidence": evidence}
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
