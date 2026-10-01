#!/usr/bin/env python3
"""Link EU source and explicit placeholders at original addresses for diagnostic comparison."""
import argparse
import collections
import copy
import csv
import hashlib
import io
import json
import os
from pathlib import Path
import re
import struct
import subprocess
import sys
import time

ROOT = Path(__file__).resolve().parents[1]
sys.dont_write_bytecode = True
sys.path.insert(0, str(ROOT))
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
from tools.low.getSection import typeToSection
from tools.low.buildProvenance import verify_build_output
from tools.low.glob import setVersion
from tools.pypstem.stepLink import write_compact_object


def digest(content):
    return hashlib.sha256(content).hexdigest()


def save(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, indent=2) + '\n')


def checked(condition, message):
    if not condition:
        raise ValueError(message)


def map_rows(content):
    result = []
    for raw in csv.DictReader(io.StringIO(content.decode()), skipinitialspace=True):
        row = {key.strip(): value.strip() for key, value in raw.items()}
        row['start'], row['end'] = int(row['Start'], 16), int(row['End'], 16)
        row['size'] = row['end'] - row['start']
        row['name'] = row['Symbol'] or ('fn_' if 'f' in row['Type'] else 'dat_') + f"{row['start']:08X}"
        checked(row['size'] > 0, 'Invalid map interval: ' + row['Start'])
        result.append(row)
    return result


def read_object(path):
    content = path.read_bytes()
    elf = ELFFile(io.BytesIO(content))
    checked(elf.elfclass == 32 and elf.little_endian and elf['e_machine'] == 'EM_ARM'
            and elf['e_type'] == 'ET_REL', 'Unsupported canonical object: ' + str(path))
    table = elf.get_section_by_name('.symtab')
    checked(table is not None, 'Missing symbol table: ' + str(path))
    relocations = collections.defaultdict(list)
    for section in elf.iter_sections():
        if isinstance(section, RelocationSection):
            checked(section['sh_type'] == 'SHT_REL', 'Unsupported RELA relocation')
            relocations[section['sh_info']].extend(dict(item.entry) for item in section.iter_relocations())
    return {'elf': elf, 'table': table, 'symbols': list(table.iter_symbols()),
            'relocations': relocations, 'path': path, 'sha256': digest(content),
            'scaffold_data_imports': {}}


def snapshot(output, checkpoint):
    original_map = ROOT / 'data/ver/eu/map.csv'
    map_content = original_map.read_bytes()
    config_content = (ROOT / 'data/config.json').read_bytes()
    config = json.loads(config_content)
    setVersion('eu')
    verified_checkpoint = subprocess.check_output(['git', 'rev-parse', 'HEAD'], cwd=ROOT, text=True).strip()
    checked(re.fullmatch(r'[0-9a-f]{7,40}', checkpoint) is not None and verified_checkpoint.startswith(checkpoint), 'Checkpoint differs from committed HEAD')
    exh = (ROOT / 'data/ver/eu/exh.bin').read_bytes()
    captured = {'checkpoint_attested_by_root': checkpoint, 'verified_committed_checkpoint': verified_checkpoint,
                'map_sha256': digest(map_content),
                'config_sha256': digest(config_content), 'exheader_sha256': digest(exh),
                'canonical_objects': [], 'canonical_provenance_commit_validation':
                'Existing committed-source build provenance gate verified before snapshot. Frozen copies remain diagnostic-only.'}
    source_inputs = {}
    records = []
    for provenance_path in sorted((ROOT / 'build/eu/obj').rglob('*.provenance.json')):
        provenance_bytes = provenance_path.read_bytes()
        provenance = json.loads(provenance_bytes)
        source = ROOT / provenance.get('source', '')
        module = next((value for name, value in config['modules'].items()
                       if source.is_relative_to(ROOT / name / value.get('source_dir', '.'))), None)
        if module is None:
            continue
        checked(provenance.get('schema') == 1 and provenance.get('language') == 'C++'
                and provenance.get('build_step') == 'tools.pypstem.stepBuild'
                and provenance.get('inputs_stable') and provenance.get('version') == 'eu',
                'Invalid canonical provenance: ' + str(provenance_path))
        compiler_version = module.get('compiler', config['compiler'])
        compiler = ROOT / 'data/compilers' / compiler_version / 'bin/armcc.exe'
        checked(provenance['compiler'] == str(compiler.relative_to(ROOT))
                and digest(compiler.read_bytes()) == provenance['compiler_sha256'], 'Compiler provenance mismatch')
        object_path = ROOT / provenance['object']
        verify_build_output(object_path)
        content = object_path.read_bytes()
        checked(digest(content) == provenance['object_sha256'], 'Canonical object changed: ' + str(object_path))
        command = provenance['command']
        checked('-c' in command and '-S' not in command and str(source) in command
                and str(compiler) in command and '-o' in command
                and command[command.index('-o') + 1] == str(object_path), 'Noncanonical compile command')
        for name, expected in provenance['inputs'].items():
            path = ROOT / name
            content_input = path.read_bytes()
            checked(digest(content_input) == expected, 'Stale canonical input: ' + name)
            if name not in source_inputs:
                destination = output / 'snapshot/inputs' / name
                destination.parent.mkdir(parents=True, exist_ok=True)
                destination.write_bytes(content_input)
                source_inputs[name] = expected
        destination = output / 'snapshot/objects' / object_path.relative_to(ROOT / 'build/eu/obj')
        destination.parent.mkdir(parents=True, exist_ok=True)
        destination.write_bytes(content)
        destination.with_suffix('.provenance.json').write_bytes(provenance_bytes)
        record = read_object(destination)
        record['source'] = source
        record['original_object'] = object_path
        record['compiler_version'] = compiler_version
        records.append(record)
        captured['canonical_objects'].append({'original': str(object_path.relative_to(ROOT)),
                                              'snapshot': str(destination), 'sha256': digest(content),
                                              'provenance_sha256': digest(provenance_bytes)})
    checked(map_content == original_map.read_bytes(), 'Map changed during snapshot')
    checked(config_content == (ROOT / 'data/config.json').read_bytes(), 'Config changed during snapshot')
    for item in captured['canonical_objects']:
        checked(digest((ROOT / item['original']).read_bytes()) == item['sha256'], 'Object changed during snapshot')
        checked(digest((ROOT / item['original']).with_suffix('.provenance.json').read_bytes())
                == item['provenance_sha256'], 'Provenance changed during snapshot')
    for name, expected in source_inputs.items():
        checked(digest((ROOT / name).read_bytes()) == expected, 'Input changed during snapshot: ' + name)
    checked(subprocess.check_output(['git', 'rev-parse', 'HEAD'], cwd=ROOT, text=True).strip()
            == verified_checkpoint, 'Committed checkpoint changed during snapshot')
    captured['input_hashes'] = source_inputs
    captured['serializer_sha256'] = digest((ROOT / 'tools/pypstem/stepLink.py').read_bytes())
    (output / 'snapshot/map.csv').write_bytes(map_content)
    (output / 'snapshot/exh.bin').write_bytes(exh)
    save(output / 'snapshot/manifest.json', captured)
    regions = [(struct.unpack_from('<I', exh, offset)[0], struct.unpack_from('<I', exh, offset + 8)[0])
               for offset in (0x10, 0x20, 0x30)]
    bss_start = sum(regions[2])
    bss_end = bss_start + struct.unpack_from('<I', exh, 0x3c)[0]
    return map_rows(map_content), records, regions, (bss_start, bss_end), config, captured



def load_snapshot(snapshot_path):
    captured = json.loads((snapshot_path / 'manifest.json').read_text())
    map_content = (snapshot_path / 'map.csv').read_bytes()
    exh = (snapshot_path / 'exh.bin').read_bytes()
    config_content = (snapshot_path / 'inputs/data/config.json').read_bytes()
    checked(digest(map_content) == captured['map_sha256'], 'Frozen map hash changed')
    checked(digest(exh) == captured['exheader_sha256'], 'Frozen exheader hash changed')
    checked(digest(config_content) == captured['config_sha256'], 'Frozen configuration hash changed')
    for name, expected in captured['input_hashes'].items():
        checked(digest((snapshot_path / 'inputs' / name).read_bytes()) == expected, 'Frozen input changed: ' + name)
    records = []
    for item in captured['canonical_objects']:
        path = Path(item['snapshot'])
        checked(path.is_relative_to(snapshot_path / 'objects'), 'Object outside frozen snapshot')
        checked(digest(path.read_bytes()) == item['sha256'], 'Frozen canonical object hash changed')
        provenance_bytes = path.with_suffix('.provenance.json').read_bytes()
        checked(digest(provenance_bytes) == item['provenance_sha256'], 'Frozen provenance changed')
        provenance = json.loads(provenance_bytes)
        record = read_object(path)
        record['source'] = ROOT / provenance['source']
        record['original_object'] = ROOT / item['original']
        record['compiler_version'] = str(Path(provenance['compiler']).parent.parent.relative_to('data/compilers'))
        records.append(record)
    regions = [(struct.unpack_from('<I', exh, offset)[0], struct.unpack_from('<I', exh, offset + 8)[0])
               for offset in (0x10, 0x20, 0x30)]
    bss_start = sum(regions[2])
    bss = (bss_start, bss_start + struct.unpack_from('<I', exh, 0x3c)[0])
    return map_rows(map_content), records, regions, bss, json.loads(config_content), captured


def construct_projection(output, rows, records):
    normal = [row for row in rows if 'i' not in row['Type']]
    by_name = {row['name']: row for row in rows}
    by_start = {row['start']: row for row in normal}
    section_rows = collections.defaultdict(list)
    for row in normal:
        section_rows[row['SectionName'] or typeToSection(row['Type'], row['name'])].append(row)
    definitions = collections.defaultdict(list)
    for index, record in enumerate(records):
        for symbol in record['symbols']:
            if isinstance(symbol['st_shndx'], int) and symbol['st_shndx'] and symbol['st_info']['bind'] != 'STB_LOCAL':
                definitions[symbol.name].append((index, symbol))
    accepted = [row for row in normal if 'f' in row['Type'] and row['Rank'] == 'O']
    selected = {}
    root_sections = {}
    identical_weak_providers = []
    for row in accepted:
        providers = [item for item in definitions[row['name']] if item[1]['st_info']['type'] == 'STT_FUNC']
        checked(providers, 'Missing O source provider: ' + row['name'])
        unique = {(index, symbol['st_shndx']) for index, symbol in providers}
        if len(unique) > 1:
            fingerprints = set()
            for index, symbol in providers:
                checked(symbol['st_info']['bind'] == 'STB_WEAK', 'Ambiguous strong O provider: ' + row['name'])
                record = records[index]
                relocs = tuple((rel['r_offset'], rel['r_info_type'], record['symbols'][rel['r_info_sym']].name)
                               for rel in record['relocations'][symbol['st_shndx']])
                fingerprints.add((digest(record['elf'].get_section(symbol['st_shndx']).data()), relocs))
            checked(len(fingerprints) == 1, 'Different weak O provider bodies: ' + row['name'])
            identical_weak_providers.append({'symbol': row['name'], 'providers': len(unique),
                                            'selection': 'First frozen path among identical weak bodies and relocations'})
        index, symbol = providers[0]
        checked(symbol['st_value'] == 0, 'O root does not start its own section: ' + row['name'])
        key = (index, symbol['st_shndx'])
        checked(key not in root_sections, 'Two distinct intervals share one canonical section')
        root_sections[key] = row
        selected[row['start']] = {'record': index, 'section_index': symbol['st_shndx'],
                                  'section': records[index]['elf'].get_section(symbol['st_shndx']).name,
                                  'provider': f"diagnostic_source_{row['start']:08X}", 'canonical_symbol': row['name']}
    aliases = {row['name']: (row, 0) for row in rows}
    for key, row in root_sections.items():
        for symbol in records[key[0]]['symbols']:
            if symbol['st_shndx'] == key[1] and symbol['st_info']['type'] == 'STT_FUNC' and symbol['st_value'] == 0:
                checked(symbol.name not in aliases or aliases[symbol.name][0]['start'] == row['start'],
                        'Compiler alias disagrees with an existing mapped address')
                aliases[symbol.name] = (row, 0)
    imports = {}
    retained = collections.defaultdict(set)
    rewrite = collections.defaultdict(dict)
    pending = list(root_sections)
    identity_notes = []
    # Separate independently justified BSS row, already recorded in project/decisions.md.
    explicit_sections = {'.bss.Togezo.cpp': (0x0042FB10, 0xFC, 'project/decisions.md:345')}

    def identity(index, symbol):
        if symbol.name in aliases:
            return aliases[symbol.name]
        match = re.fullmatch(r'(fn|dat)_([0-9a-fA-F]{8})', symbol.name)
        if match:
            row = by_start.get(int(match[2], 16))
            if row is not None and (('f' in row['Type']) == (match[1] == 'fn')):
                return row, 0
        section_index = symbol['st_shndx']
        if not isinstance(section_index, int) or not section_index:
            return None
        record = records[index]
        section = record['elf'].get_section(section_index)
        if (index, section_index) in root_sections and symbol['st_value'] == 0:
            return root_sections[(index, section_index)], 0
        candidates = section_rows.get(section.name, [])
        if len(candidates) == 1:
            row = candidates[0]
            if 'f' in row['Type'] and section['sh_flags'] & 4 and symbol['st_value'] == 0:
                return row, 0
            native_objects = [item for item in record['symbols'] if item['st_shndx'] == section_index
                              and item['st_info']['type'] not in ('STT_SECTION',)
                              and (item['st_info']['bind'] != 'STB_LOCAL'
                                   or item['st_info']['type'] in ('STT_OBJECT', 'STT_FUNC'))]
            whole_table = (row['Rank'] == 'U' and 'c' in row['Type'] and row['name'].startswith('_ZTV')
                           and len(native_objects) == 1 and native_objects[0].name == row['name']
                           and native_objects[0]['st_value'] == 0
                           and native_objects[0]['st_size'] == section['sh_size']
                           and row['size'] >= 8 and row['size'] % 4 == 0
                           and section['sh_size'] >= 8 and section['sh_size'] % 4 == 0)
            if 'd' in row['Type'] and (section['sh_size'] <= row['size'] or whole_table) and symbol['st_value'] < row['size']:
                identity_notes.append({'source_section': section.name, 'map_name': row['name'],
                                       'original_start': row['start'], 'original_end': row['end'],
                                       'source_section_bytes': section['sh_size'],
                                       'native_table_size': section['sh_size'] if whole_table else None,
                                       'table_bytes_accepted': False,
                                       'basis': 'Exact existing native section spelling and mapped extent; opaque import only'})
                return row, symbol['st_value']
        if section.name in explicit_sections:
            start, extent, evidence = explicit_sections[section.name]
            row = by_start.get(start)
            checked(row is not None and row['size'] == extent and section['sh_size'] == extent,
                    'Independently identified shared BSS extent changed')
            checked(symbol['st_value'] < extent, 'BSS interior provider is outside its known row')
            identity_notes.append({'source_section': section.name, 'map_name': row['name'],
                                   'original_start': start, 'source_section_bytes': extent, 'basis': evidence})
            return row, symbol['st_value']
        return None

    while pending:
        index, section_index = pending.pop()
        if section_index in retained[index]:
            continue
        record = records[index]
        section = record['elf'].get_section(section_index)
        checked(section['sh_flags'] & 4, 'Unidentified source data reached closure: ' + section.name)
        retained[index].add(section_index)
        for relocation in record['relocations'][section_index]:
            symbol_index = relocation['r_info_sym']
            symbol = record['symbols'][symbol_index]
            resolved = identity(index, symbol)
            if resolved is not None:
                row, offset = resolved
                alias = symbol.name or f"diagnostic_import_{row['start']:08X}_{offset:X}"
                checked(not any(character.isspace() for character in alias), 'Whitespace in import identity')
                value = row['start'] + offset
                checked(alias not in imports or imports[alias]['address'] == value, 'Conflicting import identity')
                imports[alias] = {'address': value, 'owner_start': row['start'], 'owner_end': row['end'],
                                  'offset': offset, 'kind': 'A' if 'f' in row['Type'] else 'D'}
                rewrite[index][symbol_index] = alias
                continue
            target_index = index
            target = symbol
            if not isinstance(symbol['st_shndx'], int) or not symbol['st_shndx']:
                providers = definitions.get(symbol.name, [])
                checked(len({(i, s['st_shndx']) for i, s in providers}) == 1,
                        'Unresolved or ambiguous import: ' + symbol.name)
                target_index, target = providers[0]
                checked(records[target_index]['compiler_version'] == record['compiler_version'],
                        'Helper uses a different configured compiler')
            target_section = records[target_index]['elf'].get_section(target['st_shndx'])
            if target['st_shndx'] == section_index and target_index == index:
                continue
            checked(target_section['sh_flags'] & 4, 'Unidentified data dependency: ' + target_section.name)
            functions = [item for item in records[target_index]['symbols']
                         if item['st_shndx'] == target['st_shndx'] and item['st_info']['type'] == 'STT_FUNC']
            checked(functions and all(item['st_value'] == 0 for item in functions),
                    'Unmapped helper does not start its own ordinary section')
            pending.append((target_index, target['st_shndx']))
    projected = []
    for index, sections in retained.items():
        original = records[index]
        record = dict(original)
        record['symbols'] = [copy.deepcopy(item) for item in original['symbols']]
        record['relocations'] = copy.deepcopy(original['relocations'])
        table_content = bytearray(original['table'].data())
        for symbol_index, alias in rewrite[index].items():
            new_symbol = copy.deepcopy(original['symbols'][symbol_index])
            new_symbol.name = alias
            new_symbol.entry['st_shndx'] = 'SHN_UNDEF'
            new_symbol.entry['st_value'] = new_symbol.entry['st_size'] = 0
            new_symbol.entry['st_info']['bind'] = 'STB_GLOBAL'
            new_symbol.entry['st_info']['type'] = 'STT_NOTYPE'
            destination_index = len(record['symbols'])
            record['symbols'].append(new_symbol)
            table_content.extend(struct.pack('<IIIBBH', 0, 0, 0, 0x10, 0, 0))
            for section_index in sections:
                for relocation in record['relocations'][section_index]:
                    if relocation['r_info_sym'] == symbol_index:
                        relocation['r_info_sym'] = destination_index
        for symbol_index, symbol in enumerate(record['symbols']):
            key = (index, symbol['st_shndx'])
            if key in root_sections and symbol['st_info']['type'] == 'STT_FUNC':
                address = root_sections[key]['start']
                symbol.name = (f'diagnostic_source_{address:08X}' if symbol.name == root_sections[key]['name']
                               else f'diagnostic_source_alias_{address:08X}_{symbol_index}')
        class SyntheticTable:
            def data(self):
                return bytes(table_content)
        record['table'] = SyntheticTable()
        destination = output / 'projected' / f'source_{index:04d}.o'
        evidence = write_compact_object(record, sections, destination)
        evidence['canonical_snapshot_sha256'] = original['sha256']
        evidence['canonical_source'] = str(original['source'].relative_to(ROOT))
        projected.append(evidence)
        for key, selected_row in selected.items():
            if selected_row['record'] == index:
                selected_row['object'] = destination.name
    save(output / 'projection.json', {'classification': 'Diagnostic-only section projection, ineligible for matching.',
                                      'objects': projected, 'identical_weak_provider_selection': identical_weak_providers,
                                      'data_import_identities': identity_notes, 'imports': imports,
                                      'selected_source_roots': selected})
    return selected, imports, [Path(item['projected_object']) for item in projected]


def run_tool(output, name, arguments, environment):
    command = [str(ROOT / 'data/compilers/wibo'), str(ROOT / 'data/compilers/4.1/791/bin' / (name + '.exe'))] + arguments
    started = time.monotonic()
    result = subprocess.run(command, cwd=output, env=environment, text=True, capture_output=True, timeout=300)
    (output / (name + '.log')).write_text(result.stdout + result.stderr)
    save(output / (name + '_command.json'), {'command': command, 'exit_code': result.returncode,
                                           'seconds': time.monotonic() - started})
    checked(result.returncode == 0, name + ' failed. See ' + str(output / (name + '.log')))


def generate_scaffold(output, rows, selected, imports, regions, bss, environment):
    normal = sorted((row for row in rows if 'i' not in row['Type']), key=lambda row: row['start'])
    required = {item['owner_start'] for item in imports.values()} | {regions[0][0]}
    lower, file_end = regions[0][0], sum(regions[2])
    storage = [{'start': base, 'end': base + length, 'bss': False} for base, length in regions[1:]]
    storage.append({'start': bss[0], 'end': bss[1], 'bss': True})
    for row in normal:
        if 'd' in row['Type'] and row['start'] < regions[1][0]:
            storage.append({'start': row['start'], 'end': row['end'], 'bss': False})
    checked(all(item['start'] % 4 == 0 and item['end'] % 4 == 0 for item in storage),
            'Opaque storage island is not word aligned')
    header = ['/* Diagnostic C++ placeholders. No reconstructed behavior or data. */', 'extern "C" {']
    chunks, providers = {}, {}
    supplied_functions = [row for row in normal if 'f' in row['Type']
                          and (row['start'] in selected or row['start'] in required)]
    for position, row in enumerate(supplied_functions):
        if row['start'] in selected:
            providers[row['start']] = selected[row['start']]
            continue
        batch_name = f'placeholders_{position // 512:04d}'
        lines = chunks.setdefault(batch_name, header.copy())
        name, section = f"diagnostic_placeholder_{row['start']:08X}", f"diagnostic.code.{row['start']:08X}"
        lines.append(f'__attribute__((section("{section}"))) void {name}() {{ return; }}')
        providers[row['start']] = {'provider': name, 'section': section, 'object': batch_name + '.o',
                                   'classification': 'Required ordinary C++ function placeholder'}
    data_lines = chunks.setdefault('opaque_data', header.copy())
    for item in storage:
        item['provider'] = f"diagnostic_storage_{item['start']:08X}"
        item['section'] = f"diagnostic.storage.{item['start']:08X}"
        item['object'] = 'opaque_data.o'
        size = item['end'] - item['start']
        if item['bss']:
            data_lines.extend([f'#pragma arm section zidata="{item["section"]}"',
                               f'unsigned char {item["provider"]}[{size}];', '#pragma arm section zidata'])
        else:
            qualifier = 'extern const ' if item['start'] < regions[2][0] else ''
            data_lines.append(f'{qualifier}unsigned char {item["provider"]}[{size}] __attribute__((section("{item["section"]}"), aligned(1))) = {{0}};')
        for row in normal:
            if 'd' in row['Type'] and item['start'] <= row['start'] < row['end'] <= item['end']:
                providers[row['start']] = {**item, 'storage_start': item['start'], 'storage_end': item['end'],
                                           'provider_offset': row['start'] - item['start'],
                                           'classification': 'Interior of opaque zero/uninitialized data storage'}
    for row in normal:
        providers.setdefault(row['start'], {'classification': 'Unimplemented function, linker zero fill only',
                                            'provider': None, 'original_start': row['start'], 'original_end': row['end']})
    scaffolds, compiler_commands = {}, []
    for batch_name, lines in chunks.items():
        lines.append('}')
        source_path, object_path = output / (batch_name + '.cpp'), output / (batch_name + '.o')
        source_path.write_text('\n'.join(lines) + '\n')
        run_tool(output, 'armcc', ['--cpu=MPCore', '--fpmode=fast', '--apcs=/interwork', '--arm_only',
                                  '--no_exceptions', '--no_rtti', '--enum_is_int', '--signed_chars',
                                  '--gnu', '--split_sections', '--no_debug', '-O3', '-Otime', '-c',
                                  '-o', str(object_path), str(source_path)], environment)
        compiler_commands.append(json.loads((output / 'armcc_command.json').read_text()))
        (output / 'armcc.log').replace(output / (batch_name + '.log'))
        scaffolds[object_path.name] = read_object(object_path)
    save(output / 'placeholder_compilation.json', {'batch_functions': 512, 'commands': compiler_commands})
    for row in supplied_functions:
        if row['start'] in selected:
            continue
        provider = providers[row['start']]
        scaffold = scaffolds[provider['object']]
        symbols = [item for item in scaffold['symbols'] if item.name == provider['provider']
                   and isinstance(item['st_shndx'], int) and item['st_shndx']]
        checked(len(symbols) == 1, 'Missing ordinary placeholder provider')
        section = scaffold['elf'].get_section(symbols[0]['st_shndx'])
        checked(section.name == provider['section'] and symbols[0]['st_value'] == 0
                and 0 < section['sh_size'] <= row['size'], 'Placeholder exceeds its original interval')
        provider['initial_section_bytes'] = section['sh_size']
    for item in storage:
        scaffold = scaffolds[item['object']]
        symbols = [s for s in scaffold['symbols'] if s.name == item['provider']]
        checked(len(symbols) == 1, 'Missing opaque storage provider')
        section = scaffold['elf'].get_section(symbols[0]['st_shndx'])
        checked(section['sh_size'] == item['end'] - item['start'], 'Opaque storage extent changed')
        checked(item['bss'] == (section['sh_type'] == 'SHT_NOBITS'), 'BSS storage is not ordinary NOBITS')
    scatter = [f'FULL_IMAGE 0x{lower:08X} 0x{file_end-lower:X}', '{']
    occupied, keep = [], []
    for row in supplied_functions:
        provider = providers[row['start']]
        provider['region'] = f"ROW_{row['start']:08X}"
        keep.append('--keep=' + provider['provider'])
        scatter.extend([f"  {provider['region']} 0x{row['start']:08X} FIXED 0x{row['size']:X}",
                        '  {', f"    {provider['object']} ({provider['section']})", '  }'])
        length = row['size'] if row['start'] in selected else provider['initial_section_bytes']
        occupied.append((row['start'], row['start'] + length))
    for item in storage:
        item['region'] = f"STORAGE_{item['start']:08X}"
        keep.append('--keep=' + item['provider'])
        if item['bss']:
            continue
        scatter.extend([f"  {item['region']} 0x{item['start']:08X} FIXED 0x{item['end']-item['start']:X}",
                        '  {', f"    {item['object']} ({item['section']})", '  }'])
        occupied.append((item['start'], item['end']))
    cursor, gaps = lower, []
    for start, end in sorted(occupied):
        checked(start >= cursor, 'Overlapping source or data storage')
        if start > cursor:
            gaps.append((cursor, start))
        cursor = end
    if cursor < file_end:
        gaps.append((cursor, file_end))
    for start, end in gaps:
        scatter.extend([f'  UNKNOWN_SPAN_{start:08X} 0x{start:08X} FIXED FILL 0x00000000 0x{end-start:X}', '  {', '  }'])
    scatter.append('}')
    scatter.extend([f'BSS_STORAGE 0x{file_end:08X}', '{'])
    item = next(item for item in storage if item['bss'])
    scatter.extend([f"  {item['region']} 0x{item['start']:08X} UNINIT 0x{item['end']-item['start']:X}",
                    '  {', f"    {item['object']} ({item['section']})", '  }', '}'])
    for row in normal:
        if 'd' in row['Type']:
            item = next(item for item in storage if item['start'] <= row['start'] < row['end'] <= item['end'])
            providers[row['start']]['region'] = item['region']
    root_end = scatter.index('}')
    blocks, current = [], []
    for line in scatter[2:root_end]:
        if re.match(r'^  [A-Za-z_][A-Za-z_0-9]* 0x', line):
            if current:
                blocks.append(current)
            current = [line]
        else:
            current.append(line)
    if current:
        blocks.append(current)
    blocks.sort(key=lambda block: int(block[0].split()[1], 16))
    scatter = scatter[:2] + [line for block in blocks for line in block] + scatter[root_end:]
    (output / 'full_image.scatter').write_text('\n'.join(scatter) + '\n')
    (output / 'mapped_imports.symdefs').write_text('#<SYMDEFS>#\n' + ''.join(
        f"0x{item['address']:08X} {item['kind']} {name}\n" for name,item in sorted(imports.items())))
    save(output / 'providers.json', {'providers': providers, 'linker_zero_spans': gaps, 'opaque_storage': storage,
                                     'bss': bss, 'file_extent': [lower, file_end],
                                     'rows_without_executable_provider': sum(p['provider'] is None for p in providers.values())})
    return providers, keep, (lower, file_end), [output / name for name in scaffolds]


def verify_link(output, rows, selected, imports, providers, extent):
    image = ELFFile(io.BytesIO((output / 'full_image.axf').read_bytes()))
    symbols = list(image.get_section_by_name('.symtab').iter_symbols())
    by_name = collections.defaultdict(list)
    for symbol in symbols:
        by_name[symbol.name].append(symbol)
    failures = []
    payload = bytearray(extent[1] - extent[0])
    allocated = []
    file_sections = []
    known_regions = {provider['region'] for provider in providers.values() if provider.get('region')}
    for section in image.iter_sections():
        if not section['sh_flags'] & 2 or not section['sh_size']:
            continue
        start, end = section['sh_addr'], section['sh_addr'] + section['sh_size']
        allocated.append({'section': section.name, 'start': start, 'end': end, 'type': section['sh_type']})
        if section.name not in known_regions and not section.name.startswith(('UNKNOWN_SPAN_',)):
            failures.append('Unexpected allocated content or residual helper: ' + section.name)
        if section['sh_type'] != 'SHT_NOBITS':
            if not extent[0] <= start < end <= extent[1]:
                failures.append('Allocated file content outside original extent: ' + section.name)
            else:
                file_sections.append((start, end, section.name))
                payload[start-extent[0]:end-extent[0]] = section.data()
    for row in rows:
        if 'i' in row['Type']:
            continue
        provider = providers[row['start']]
        if provider['provider'] is None:
            continue
        provider_start = provider.get('storage_start', row['start'])
        provider_size = provider.get('storage_end', row['end']) - provider_start
        definitions = [s for s in by_name[provider['provider']] if isinstance(s['st_shndx'], int) and s['st_shndx']]
        if len(definitions) != 1 or definitions[0]['st_value'] != provider_start:
            failures.append('Wrong or ambiguous provider address: ' + provider['provider'])
            continue
        section = image.get_section(definitions[0]['st_shndx'])
        if section.name != provider['region'] or section['sh_addr'] != provider_start or section['sh_size'] > provider_size:
            failures.append('Wrong provider placement or size budget: ' + provider['provider'])
        if row['start'] in selected or 'd' in row['Type']:
            if section['sh_size'] != provider_size:
                failures.append('Complete source/data interval extent changed: ' + provider['provider'])
    cursor = extent[0]
    for start, end, name in sorted(file_sections):
        if start != cursor:
            failures.append('ELF load coverage gap or overlap before ' + name)
        cursor = end
    if cursor != extent[1]:
        failures.append('ELF load content does not reach the complete declared file extent')
    # Audit providers in the real linker map, including the supplying input object.
    linker_map = (output / 'full_image.map').read_text()
    globals_text = linker_map.partition('Global Symbols')[2].partition('===')[0]
    for row in rows:
        if 'i' in row['Type']:
            continue
        provider = providers[row['start']]
        if provider['provider'] is None:
            continue
        provider_start = provider.get('storage_start', row['start'])
        provider_size = provider.get('storage_end', row['end']) - provider_start
        pattern = (r'^\s*' + re.escape(provider['provider'])
                   + r'\s+(0x[0-9a-fA-F]+)\s+(?:ARM Code|Data)\s+(\d+)\s+'
                   + r'(?:[^\n]*[/\\])?' + re.escape(provider['object']) + r'\('
                   + re.escape(provider['section']) + r'\)\s*$')
        records = re.findall(pattern, globals_text, re.MULTILINE)
        if len(records) != 1 or int(records[0][0], 16) != provider_start or int(records[0][1]) > provider_size:
            failures.append('Linker map provider or complete interval budget disagrees: ' + provider['provider'])
    for name, item in imports.items():
        owner = providers.get(item['owner_start'])
        if owner is None or owner['provider'] is None or not item['owner_start'] <= item['address'] < item['owner_end']:
            failures.append('Import has no verified ownership interval: ' + name)
        resolved = [s for s in by_name[name] if s['st_shndx'] != 'SHN_UNDEF']
        if len(resolved) != 1 or resolved[0]['st_value'] != item['address']:
            failures.append('Import address not resolved uniquely: ' + name)
    undefined = [s.name for s in symbols if s.name and s['st_shndx'] == 'SHN_UNDEF' and s['st_info']['bind'] == 'STB_GLOBAL']
    if undefined:
        failures.append('Unresolved globals: ' + ', '.join(undefined))
    save(output / 'placement.json', {'placement_verified_before_oracle': not failures,
                                     'failures': failures, 'allocated_sections': allocated})
    checked(not failures, 'Placement/provider/extent validation failed. Oracle was not opened.')
    # This is an ELF-load export, not a reference overlay. Only ELF bytes and zero container padding enter it.
    page_end = (extent[1] + 0xFFF) & ~0xFFF
    payload.extend(bytes(page_end - extent[1]))
    exported = output / 'linked_code.bin'
    exported.write_bytes(payload)
    original = (ROOT / 'data/ver/eu/code.bin').read_bytes()
    checked(digest(original) == 'e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64', 'Oracle hash mismatch')
    checked(len(original) == len(payload), 'Export length differs from original full image')
    compared = []
    for row in rows:
        if 'i' in row['Type'] or row['start'] not in selected:
            continue
        actual = payload[row['start']-extent[0]:row['end']-extent[0]]
        expected = original[row['start']-extent[0]:row['end']-extent[0]]
        compared.append({'address': row['Start'], 'symbol': row['name'], 'complete_bytes': row['size'],
                         'equal': actual == expected, 'generated_sha256': digest(actual), 'retail_sha256': digest(expected)})
    save(output / 'comparison.json', {'classification': 'Linked diagnostic scaffold, no matching eligibility or runtime claim.',
                                      'source_O_intervals': compared, 'source_O_equal': sum(x['equal'] for x in compared),
                                      'source_O_supplied': len(compared), 'original_bytes_in_linker_inputs': 0,
                                      'static_recompiled_behavior_bytes': 0, 'placeholder_behavior_claim': False,
                                      'linked_image_sha256': digest(payload), 'retail_image_sha256': digest(original),
                                      'whole_image_equal': payload == original,
                                      'full_image_different_bytes': sum(a != b for a,b in zip(payload, original)),
                                      'container_zero_padding': [extent[1], page_end]})
    return {'source_O_supplied': len(compared), 'source_O_equal': sum(x['equal'] for x in compared),
            'whole_image_equal': payload == original, 'linked_image_sha256': digest(payload)}


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--checkpoint', help='Committed checkpoint; defaults to HEAD or the frozen snapshot checkpoint')
    parser.add_argument('--output', type=Path, help='Fresh ignored directory; defaults to build/eu/full_image_diagnostic/<timestamp>')
    parser.add_argument('--snapshot', type=Path, help='Use a previously hash-verified frozen snapshot')
    args = parser.parse_args(argv)
    if args.checkpoint is None:
        if args.snapshot:
            args.checkpoint = json.loads((args.snapshot / 'manifest.json').read_text())['checkpoint_attested_by_root']
        else:
            args.checkpoint = subprocess.check_output(['git', 'rev-parse', 'HEAD'], cwd=ROOT, text=True).strip()
    output = (args.output or ROOT / 'build/eu/full_image_diagnostic' / str(time.time_ns())).resolve()
    checked(output.is_relative_to(ROOT / 'build'), 'All output must be under ignored build/')
    checked(not output.exists(), 'Use a fresh output directory for every diagnostic')
    output.mkdir(parents=True)
    (output / 'cmd.exe').write_bytes(b'')
    environment = dict(os.environ)
    environment['TMP'] = str(output / 'temporary')
    Path(environment['TMP']).mkdir()
    environment['ARMCC41INC'] = str(ROOT / 'data/compilers/4.1/791/include')
    environment['ARMCC41LIB'] = str(ROOT / 'data/compilers/4.1/791/lib')
    started = time.monotonic()
    try:
        if args.snapshot:
            rows, records, regions, bss, config, captured = load_snapshot(args.snapshot.resolve())
            checked(captured['checkpoint_attested_by_root'] == args.checkpoint, 'Snapshot checkpoint differs')
            save(output / 'snapshot_reference.json', {'path': str(args.snapshot.resolve()),
                                                     'manifest_sha256': digest((args.snapshot / 'manifest.json').read_bytes())})
        else:
            rows, records, regions, bss, config, captured = snapshot(output, args.checkpoint)
        selected, imports, projected = construct_projection(output, rows, records)
        providers, keep, extent, scaffold_objects = generate_scaffold(output, rows, selected, imports, regions, bss, environment)
        arguments = ['--cpu=MPCore', '--fpu=VFPv2', '--arm_only', '--no_exceptions', '--datacompressor=off',
                     '--inline', '--no_tailreorder', '--no_scanlib', '--largeregions', '--pad=0',
                     '--mangled', '--symbols', '--map', '--show_full_path', '--info=inline',
                     '--no_startup', '--entry=' + providers[regions[0][0]]['provider'],
                     '--list=' + str(output / 'full_image.map'), '--output=' + str(output / 'full_image.axf'),
                     '--scatter=' + str(output / 'full_image.scatter'), str(output / 'mapped_imports.symdefs')]
        arguments.extend(keep)
        arguments.extend(str(path) for path in projected)
        arguments.extend(str(path) for path in scaffold_objects)
        # A via file avoids command-line length limits without changing input ordering.
        (output / 'link.via').write_text('\n'.join('"' + value + '"' if ' ' in value else value for value in arguments) + '\n')
        run_tool(output, 'armlink', ['--via=' + str(output / 'link.via')], environment)
        result = verify_link(output, rows, selected, imports, providers, extent)
        checked(result['source_O_supplied'] == result['source_O_equal'],
                'An accepted source interval changed in the full-image link. See comparison.json.')
        checked(captured['serializer_sha256'] == digest((ROOT / 'tools/pypstem/stepLink.py').read_bytes()), 'Serializer changed during diagnostic')
        save(output / 'result.json', {'success': True, 'seconds': time.monotonic()-started, **result})
        print(json.dumps({'diagnostic_output': str(output), **result}, indent=2))
    except Exception as error:
        save(output / 'result.json', {'success': False, 'seconds': time.monotonic()-started,
                                     'error': str(error), 'original_bytes_in_linker_inputs': 0})
        print(str(error), file=sys.stderr)
        return 1
    return 0


if __name__ == '__main__':
    sys.exit(main())
