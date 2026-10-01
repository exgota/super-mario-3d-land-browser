#!/usr/bin/env python3
import hashlib
import io
import json
import re
import struct

from elftools.elf.elffile import ELFFile
from elftools.elf.enums import ENUM_SH_TYPE_ARM
from elftools.elf.relocation import RelocationSection

from tools.pypstem._utils import *
from tools.pypstem.callProcess import do_archive, do_link
from tools.pypstem.manSetup import setup_compiler
from tools.low.glob import *
from tools.low.genScatter import gen_scatter
from tools.low.getSection import typeToSection
from tools.low.readSymMap import MapFmt, read_sym_file
from tools.low.buildProvenance import verify_build_output

def find_scaffold_data_import(record, section_index, mapped_data):
    """Identify a complete, unaccepted virtual table for the scaffold only."""
    section = record["elf"].get_section(section_index)
    if not section["sh_flags"] & 2 or section["sh_flags"] & 4:
        return None
    definitions = [symbol for symbol in record["symbols"]
                   if symbol["st_shndx"] == section_index
                   and symbol["st_info"]["type"] != "STT_SECTION"
                   and (symbol["st_info"]["bind"] != "STB_LOCAL"
                        or symbol["st_info"]["type"] in ("STT_OBJECT", "STT_FUNC"))]
    if len(definitions) != 1:
        return None
    symbol = definitions[0]
    rows = mapped_data.get(symbol.name, [])
    if len(rows) != 1 or not symbol.name.startswith("_ZTV"):
        return None
    row = rows[0]
    size = row[MapFmt.End] - row[MapFmt.Start]
    section_name = row[MapFmt.SectionName] or typeToSection(row[MapFmt.Type], symbol.name)
    if (row[MapFmt.Rank] != "U" or "c" not in row[MapFmt.Type]
            or symbol["st_info"]["type"] != "STT_OBJECT"
            or symbol["st_info"]["bind"] not in ("STB_GLOBAL", "STB_WEAK") or symbol["st_value"] != 0
            or size < 8 or size % 4
            or section["sh_size"] < 8 or section["sh_size"] % 4
            or symbol["st_size"] != section["sh_size"]
            or section.name != section_name):
        return None
    return {"symbol": symbol.name, "section": section_name,
            "original_start": row[MapFmt.Start], "original_end": row[MapFmt.End],
            "size": size, "native_table_size": section["sh_size"],
            "table_bytes_accepted": False}

def write_compact_object(record, retained, output):
    """Project compiler sections for the scaffold, never for matching checks."""
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
        if isinstance(symbol["st_shndx"], int):
            if symbol["st_shndx"] in retained:
                section_index = section_indices[symbol["st_shndx"]]
            else:
                discarded_section = elf.get_section(symbol["st_shndx"])
                if not discarded_section["sh_flags"] & 4:
                    identity = record.get("scaffold_data_imports", {}).get(symbol["st_shndx"])
                    if identity is None or (symbol["st_info"]["type"] != "STT_SECTION"
                                            and symbol.name != identity["symbol"]):
                        raise ValueError("A discarded data definition has no established scaffold import: " + discarded_section.name)
                    name = identity["symbol"]
                elif symbol["st_info"]["type"] == "STT_SECTION":
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

def project_compact_archives():
    """Keep enrolled functions and their data/helper closure in separate archives."""
    mapped_symbols = read_sym_file()
    roots = {symbol[MapFmt.Symbol] or f"fn_{symbol[MapFmt.Start]:08X}"
             for symbol in mapped_symbols if "f" in symbol[MapFmt.Type] and symbol[MapFmt.Rank] != "U"}
    mapped_functions = {symbol[MapFmt.Symbol] or f"fn_{symbol[MapFmt.Start]:08X}"
                        for symbol in mapped_symbols if "f" in symbol[MapFmt.Type]}
    mapped_data = {}
    for symbol in mapped_symbols:
        if "d" in symbol[MapFmt.Type] and symbol[MapFmt.Symbol]:
            mapped_data.setdefault(symbol[MapFmt.Symbol], []).append(symbol)
    records = []
    definitions = {}
    for module_path, module in cfg.modules.items():
        if module.get("name") == getStubsLibName():
            continue
        source_root = getModSrc(module_path, module)
        selected_sources = module.get("source_files")
        if selected_sources is not None:
            selected_sources = {source_root / source for source in selected_sources}
        extensions = module.get("extensions", cfg.extensions)
        for source in sorted(source_root.rglob("*")):
            if not source.is_file() or source.suffix.lstrip(".") not in extensions:
                continue
            if selected_sources is not None and source not in selected_sources:
                continue
            path = getFileBuildPath(source)
            if not path.is_file():
                raise ValueError("Canonical compiler object is missing: " + str(path))
            content = path.read_bytes()
            elf = ELFFile(io.BytesIO(content))
            if elf.elfclass != 32 or not elf.little_endian or elf["e_machine"] != "EM_ARM" or elf["e_type"] != "ET_REL":
                raise ValueError("Compact projection requires canonical ELF32 ARM objects: " + str(path))
            table = elf.get_section_by_name(".symtab")
            if table is None or table["sh_entsize"] != 16:
                raise ValueError("Canonical object has no supported symbol table: " + str(path))
            symbols = list(table.iter_symbols())
            relocations = {}
            for section in elf.iter_sections():
                if isinstance(section, RelocationSection):
                    if section["sh_type"] != "SHT_REL":
                        raise ValueError("Compact projection requires ARM REL relocations.")
                    relocations.setdefault(section["sh_info"], []).extend(section.iter_relocations())
            index = len(records)
            record = {"elf": elf, "table": table, "symbols": symbols, "relocations": relocations,
                      "module": module["name"], "source": source, "path": path,
                      "sha256": hashlib.sha256(content).hexdigest(), "scaffold_data_imports": {}}
            provenance = path.with_suffix(".provenance.json")
            record["provenance_sha256"] = hashlib.sha256(provenance.read_bytes()).hexdigest() if provenance.exists() else None
            records.append(record)
            for symbol in symbols:
                if isinstance(symbol["st_shndx"], int) and symbol["st_shndx"] and symbol["st_info"]["bind"] != "STB_LOCAL":
                    definitions.setdefault(symbol.name, []).append((index, symbol))

    retained = {index: set() for index in range(len(records))}
    inline_helpers = {}
    verified_inline_inputs = {}
    pending = [(index, symbol["st_shndx"]) for name in roots for index, symbol in definitions.get(name, [])]
    while pending:
        index, section_index = pending.pop()
        if section_index in retained[index]:
            continue
        record = records[index]
        section = record["elf"].get_section(section_index)
        if not section["sh_flags"] & 2:
            raise ValueError("A compact root or dependency is not allocated compiler content: " + section.name)
        retained[index].add(section_index)
        for linked_index, linked in enumerate(record["elf"].iter_sections()):
            if linked["sh_flags"] & 0x80 and linked["sh_link"] == section_index:
                pending.append((index, linked_index))
        for relocation in record["relocations"].get(section_index, []):
            symbol = record["symbols"][relocation["r_info_sym"]]
            targets = [(index, symbol)] if isinstance(symbol["st_shndx"], int) else definitions.get(symbol.name, [])
            for target_index, target in targets:
                target_record = records[target_index]
                target_section = target_record["elf"].get_section(target["st_shndx"])
                data_import = find_scaffold_data_import(target_record, target["st_shndx"], mapped_data)
                if data_import is not None:
                    # The native table is not accepted data. Its existing map
                    # identity selects an explicitly zero-filled scaffold object.
                    target_record["scaffold_data_imports"][target["st_shndx"]] = data_import
                    continue
                if target_section["sh_flags"] & 4:
                    # Other mapped implementations remain ordinary imports for
                    # the existing scaffold stubs. Local/weak compiler helpers
                    # stay only when the retained code actually references them.
                    functions = [item for item in records[target_index]["symbols"]
                                 if item["st_shndx"] == target["st_shndx"] and item["st_info"]["type"] == "STT_FUNC"]
                    helper = functions and all(item["st_info"]["bind"] in ("STB_LOCAL", "STB_WEAK")
                                               and item.name not in mapped_functions for item in functions)
                    enrolled = any(item.name in roots for item in functions)
                    if not enrolled and not helper:
                        standalone = (functions and all(item["st_value"] == 0 for item in functions)
                                      and all(item.name not in mapped_functions for item in functions)
                                      and relocation["r_info_type"] in (1, 28, 29))
                        if not standalone:
                            continue
                        for item in functions:
                            providers = {(provider, definition["st_shndx"])
                                         for provider, definition in definitions.get(item.name, [])}
                            if len(providers) != 1:
                                raise ValueError("An unmapped C++ inline helper has ambiguous definitions: " + item.name)
                        for provider in (index, target_index):
                            provider_record = records[provider]
                            if provider not in verified_inline_inputs:
                                provenance = verify_build_output(provider_record["path"])
                                if provenance["object_sha256"] != provider_record["sha256"]:
                                    raise ValueError("A C++ inline closure object changed during projection.")
                                verified_inline_inputs[provider] = provenance
                        if (verified_inline_inputs[index]["compiler"]
                                != verified_inline_inputs[target_index]["compiler"]):
                            raise ValueError("An unmapped inline helper uses a different configured compiler.")
                        inline_helpers[(target_index, target["st_shndx"])] = {
                            "canonical_object": str(records[target_index]["path"]),
                            "section": target_section.name,
                            "symbols": sorted(item.name for item in functions),
                            "sha256": hashlib.sha256(target_section.data()).hexdigest(),
                            "provenance": verified_inline_inputs[target_index]}
                if target_section["sh_flags"] & 2:
                    pending.append((target_index, target["st_shndx"]))

    projection_root = getBuildPath() / "compact_link"
    library_root = projection_root / "lib"
    library_root.mkdir(parents=True, exist_ok=True)
    report = {"purpose": "Compact scaffold link only. These projected objects are ineligible for matching checks.",
              "roots": sorted(roots), "objects": [],
              "inline_helpers": list(inline_helpers.values()),
              "inline_inputs": [{"canonical_object": str(records[index]["path"]),
                                 "provenance": provenance,
                                 "provenance_sha256": records[index]["provenance_sha256"]}
                                for index, provenance in sorted(verified_inline_inputs.items())],
              "canonical_inputs": [
                  {"object": str(record["path"]), "sha256": record["sha256"],
                   "provenance_sha256": record["provenance_sha256"]} for record in records]}
    archives = {}
    for index, record in enumerate(records):
        if not retained[index]:
            continue
        output = projection_root / "obj" / record["source"].relative_to(getProjDir()).with_suffix(".o")
        evidence = write_compact_object(record, retained[index], output)
        evidence.update({"source": str(record["source"].relative_to(getProjDir())),
                         "canonical_object": str(record["path"]), "canonical_sha256": record["sha256"],
                         "canonical_provenance_sha256": record["provenance_sha256"],
                         "scaffold_data_imports": list(record["scaffold_data_imports"].values())})
        if hashlib.sha256(record["path"].read_bytes()).hexdigest() != record["sha256"]:
            raise ValueError("The canonical compiler object changed during projection.")
        provenance = record["path"].with_suffix(".provenance.json")
        current_provenance = hashlib.sha256(provenance.read_bytes()).hexdigest() if provenance.exists() else None
        if current_provenance != record["provenance_sha256"]:
            raise ValueError("The canonical build provenance changed during projection.")
        report["objects"].append(evidence)
        archives.setdefault(record["module"], []).append(output)
    for module, objects in archives.items():
        archive = library_root / f"lib{module}.a"
        if archive.exists():
            archive.unlink()
        via = projection_root / f"{module}.via"
        via.write_text("\n".join('"' + str(path) + '"' for path in objects) + "\n")
        do_archive(["-rsc", str(archive), f"--via={via}"])
    for record in records:
        provenance = record["path"].with_suffix(".provenance.json")
        current_provenance = hashlib.sha256(provenance.read_bytes()).hexdigest() if provenance.exists() else None
        if (hashlib.sha256(record["path"].read_bytes()).hexdigest() != record["sha256"]
                or current_provenance != record["provenance_sha256"]):
            raise ValueError("A canonical object or provenance record changed while creating compact archives.")
    for index, provenance in verified_inline_inputs.items():
        if verify_build_output(records[index]["path"]) != provenance:
            raise ValueError("An inline closure input changed during archive projection.")
    (projection_root / "projection.json").write_text(json.dumps(report, indent=2) + "\n")
    return [library_root / f"lib{module}.a" for module in archives]

def verify_inline_helpers_removed():
    """Keep unmapped strong helpers only when the scaffold linker inlines them."""
    projection = getBuildPath() / "compact_link/projection.json"
    if not projection.exists():
        return
    report = json.loads(projection.read_text())
    helpers = report.get("inline_helpers", [])
    if not helpers:
        return
    memory_map = getOutMapFile().read_text().partition("Memory Map of the image")[2]
    if not memory_map:
        raise ValueError("The compact linker map has no allocated-section evidence.")
    entries = []
    for line in memory_map.splitlines():
        fields = line.split()
        if (len(fields) >= 4 and fields[0].startswith("0x")
                and fields[1].startswith("0x") and fields[2] in ("Code", "Data", "Zero")):
            entries.append(line)
    for helper in helpers:
        if any(re.search(r"\s" + re.escape(helper["section"]) + r"\s", line) for line in entries):
            raise ValueError("An unmapped source helper remained allocated in the compact link: " + helper["section"])
    for record in report.get("inline_inputs", []):
        path = Path(record["canonical_object"])
        if (verify_build_output(path) != record["provenance"]
                or hashlib.sha256(path.with_suffix(".provenance.json").read_bytes()).hexdigest()
                != record["provenance_sha256"]):
            raise ValueError("A C++ inline closure input changed during the compact link.")


def exec_link():
    echo ("Generating ldscript")
    gen_scatter()

    echo ("Preparing to link ...", "\r")

    if getElfFile().exists():
        getElfFile().unlink()
    if getOutMapFile().exists():
        getOutMapFile().unlink()

    setup_compiler(cfg.compiler)
    projected_archives = None if cfg.split else project_compact_archives()

    flags = default_flags_link[:]
    flags.extend(cfg.flags_link)
    if not cfg.split:
        # Importing an archive member must not retain its unrelated functions.
        # Keep only the map functions explicitly enrolled in the compact build.
        functions = [symbol[MapFmt.Symbol] or f"fn_{symbol[MapFmt.Start]:08X}"
                     for symbol in read_sym_file()
                     if "f" in symbol[MapFmt.Type] and symbol[MapFmt.Rank] != "U"]
        flags.extend(f"--keep={function}" for function in functions)
    flags.append(f"--list={str(getOutMapFile())}")
    flags.append(f"--output={str(getElfFile())}")
    flags.append(f"--scatter={str(getOutScatterFile())}")

    depend_file = getSplitDependFile() or getBuildDependFile()
    if not depend_file:
        fail("No depend file found! (internal error)")
    flags.append(str(depend_file))

    flags.append(f"--userlibpath={str(getBuildLibPath())}")

    if not cfg.modules or len(cfg.modules) <= 0:
        echo ("No modules are specified.")
    else:
        for mod_path_name, mod_data in cfg.modules.items():
            my_name = str(mod_data.get("name"))
            if my_name == getStubsLibName():
                continue
            if not (getBuildLibPath() / f"lib{my_name}.a").exists(): # header-only module
                continue
            if cfg.split:
                flags.append(f"--library={my_name}")
    if projected_archives is not None:
        flags.extend(str(path) for path in projected_archives)

    if cfg.split:
        flags.append(f"--library={getSplitLibName()}")
    else:
        flags.append(f"--library={getStubsLibName()}")


    echo (f"Linking {getElfFile().name}")

    do_link (flags)
    if not cfg.split:
        verify_inline_helpers_removed()
