#!/usr/bin/env python3
"""Prepare a scratch family proposal from complete files, without granting credit.

Run from the repository root after sourcing development_environment.sh:
    python tools/prepare_family_handoff.py specification.json --output build/proposal

The specification supplies base_commit, lane, ownership (path to lane),
ownership_record_commit, final_files (path to {file, sha256}), units, targets,
compilers, evidence, form_history and timing. Each unit identifies source,
provenance and baseline_contracts files with their hashes. Targets are addresses
and source paths; identity and size come from the frozen map. Optional new_names
name only blank whole rows and require a hashed human evidence reference. A blank
target requires an explicitly supplied fn_<address> neutral name. Its strict
diagnostic remains pending until independently reviewed metadata is committed;
the preparer never applies the separate import_names.patch. Optional
additional_frozen_headers maps header paths to {sha256}; these read-only inputs
must match frozen Git and current bytes before and after preparation. Supplied
headers are copied, but only compiler-reported dependencies enter the effective
closure. Units may
explicitly explain removed unaccepted definitions. No ownership or ABI review
is inferred. replay_only suppresses enrollment inventory for historical replays.

One invocation is one meaningful form. Earlier forms remain immutable artifacts
identified by form_history, with their hash indices verified and copied. Physical
compiler failures, source mismatches and preparation refusals remain separate.
The unchanged project build/check and its full preservation gate remain required.
"""

import argparse
import contextlib
import csv
import datetime
import difflib
import hashlib
import io
import json
import os
from pathlib import Path
import re
import shlex
import subprocess
import sys
import time

from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection


class PreparationError(ValueError):
    pass


def digest(data):
    return hashlib.sha256(data).hexdigest()


def utc():
    return datetime.datetime.now(datetime.timezone.utc).isoformat()


def write_json(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, indent=2) + '\n')


def require(condition, message):
    if not condition:
        raise PreparationError(message)


def relative_path(name):
    path = Path(name)
    require(not path.is_absolute() and '..' not in path.parts and str(path) == name,
            'Use a normalized repository-relative path: ' + name)
    return path


def supplied_file(root, entry):
    path = (root / relative_path(entry['file'])).resolve()
    require(path.is_relative_to(root) and path.is_file(), 'Missing repository evidence: ' + entry['file'])
    data = path.read_bytes()
    require(digest(data) == entry['sha256'], 'Input drift: ' + entry['file'])
    return path, data


def git_file(root, base, name):
    result = subprocess.run(['git', 'show', base + ':' + name], cwd=root, capture_output=True)
    require(result.returncode == 0, 'Frozen base has no file: ' + name)
    return result.stdout


def rows(text):
    return [{key.strip(): value.strip() for key, value in row.items()}
            for row in csv.DictReader(io.StringIO(text))]


def function_contracts(path):
    elf = ELFFile(io.BytesIO(path.read_bytes()))
    table = elf.get_section_by_name('.symtab')
    require(table is not None, 'Compiler output has no symbol table.')
    result = {}
    for symbol in table.iter_symbols():
        if symbol['st_info']['type'] != 'STT_FUNC' or not isinstance(symbol['st_shndx'], int):
            continue
        section = elf.get_section(symbol['st_shndx'])
        relocations = []
        for relocation_section in elf.iter_sections():
            if not isinstance(relocation_section, RelocationSection) or relocation_section['sh_info'] != symbol['st_shndx']:
                continue
            symbols = elf.get_section(relocation_section['sh_link'])
            for relocation in relocation_section.iter_relocations():
                relocations.append({'offset': relocation['r_offset'], 'type': relocation['r_info_type'],
                                    'symbol': symbols.get_symbol(relocation['r_info_sym']).name})
        result[symbol.name] = {'section': section.name, 'size': section['sh_size'], 'offset': symbol['st_value'],
                               'binding': symbol['st_info']['bind'], 'sha256': digest(section.data()),
                               'relocations': relocations}
    return result


def patch(old, new, name):
    return b''.join(difflib.diff_bytes(difflib.unified_diff, old.splitlines(True), new.splitlines(True),
                                     fromfile=('a/' + name if old else '/dev/null').encode(),
                                     tofile=('b/' + name).encode()))


def dependency_names(path, snapshot, root):
    dependencies = set()
    external = {}
    for line in path.read_text().splitlines():
        fields = shlex.split(line)
        require(len(fields) == 2, 'Unsupported compiler dependency record.')
        dependency = Path(fields[1]).resolve()
        if dependency.is_relative_to(snapshot):
            dependencies.add(str(dependency.relative_to(snapshot)))
        elif dependency.is_relative_to(root / 'data/compilers'):
            external[str(dependency.relative_to(root))] = digest(dependency.read_bytes())
        else:
            raise PreparationError('Dependency escaped frozen snapshot: ' + str(dependency))
    return dependencies, external


def verify_history(root, output, number, entry):
    index_path, data = supplied_file(root, entry['hash_index'])
    index = json.loads(data)
    files = index.get('files', index.get('sha256', index))
    require(isinstance(files, dict), 'Unsupported prior-form hash index.')
    artifact_root = root / relative_path(entry.get('artifact_root', str(index_path.parent.relative_to(root))))
    for name, expected in files.items():
        path = artifact_root / relative_path(name)
        require(path.is_file() and digest(path.read_bytes()) == expected, 'Prior-form artifact drift: ' + name)
    destination = output / 'form_history' / str(number) / index_path.name
    destination.parent.mkdir(parents=True, exist_ok=True)
    destination.write_bytes(data)
    return {'form': entry['form'], 'meaningful': bool(entry.get('meaningful', True)),
            'hash_index': entry['hash_index'], 'copied_hash_index': str(destination.relative_to(output)),
            'artifact_root': str(artifact_root.relative_to(root)), 'verified_files': len(files),
            'physical_compile_failures': entry.get('physical_compile_failures', 0)}


def prepare(root, specification, output, report):
    sys.path.insert(0, str(root))
    from tools.low.checkExactBytes import check_exact_bytes
    from tools.pypstem.manSetup import setup_compiler

    @contextlib.contextmanager
    def phase(name):
        started = time.monotonic()
        record = {'phase': name, 'start_utc': utc()}
        try:
            yield
        finally:
            record.update(end_utc=utc(), seconds=time.monotonic() - started)
            report['phases'].append(record)

    base = subprocess.check_output(['git', 'rev-parse', '--verify', specification['base_commit'] + '^{commit}'], cwd=root, text=True).strip()
    ownership_commit = subprocess.check_output(['git', 'rev-parse', '--verify', specification['ownership_record_commit'] + '^{commit}'], cwd=root, text=True).strip()
    report.update(base_commit=base, ownership_record_commit=ownership_commit, lane=specification['lane'], ownership_review='Supplied human evidence only; no approval inferred.',
                  identity_review='Human review required; no public name or ABI approval inferred.')
    ownership = specification['ownership']
    final_files = {}
    baseline_files = {}
    additional_headers = {}
    evidence_hashes = {}
    with phase('validate_and_snapshot_inputs'):
        for name, entry in specification['final_files'].items():
            path = relative_path(name)
            require(ownership.get(name) == specification['lane'], 'Forbidden owned path: ' + name)
            require(path.parts[0] in ('Game', 'lib') and path.suffix in ('.cpp', '.cc', '.cxx', '.h', '.hpp'),
                    'Only complete owned C++ source/header files may be proposed: ' + name)
            _, data = supplied_file(root, entry)
            require(data, 'An empty final file is not a complete source proposal: ' + name)
            final_files[name] = data
            baseline_files[name] = git_file(root, base, name)
            destination = output / 'final_files' / name
            destination.parent.mkdir(parents=True, exist_ok=True)
            destination.write_bytes(data)
        evidence_records = []
        for entry in specification.get('evidence', []):
            path, data = supplied_file(root, entry)
            destination = output / 'human_evidence' / str(len(evidence_records)) / path.name
            destination.parent.mkdir(parents=True, exist_ok=True)
            destination.write_bytes(data)
            evidence_hashes[entry['file']] = entry['sha256']
            evidence_records.append({'original': entry, 'copied_path': str(destination.relative_to(output))})
        report['human_evidence'] = evidence_records
        for name, entry in specification.get('additional_frozen_headers', {}).items():
            path = relative_path(name)
            require(path.parts[0] in ('Game', 'lib') and path.suffix in ('.h', '.hpp'),
                    'An additional frozen input must be a repository header: ' + name)
            require(name not in final_files, 'Additional frozen header overlaps an owned final file: ' + name)
            data = git_file(root, base, name)
            require(digest(data) == entry['sha256'], 'Additional header differs from frozen Git: ' + name)
            require((root / name).is_file() and digest((root / name).read_bytes()) == entry['sha256'],
                    'Current additional header drift: ' + name)
            additional_headers[name] = data
            destination = output / 'additional_frozen_headers' / name
            destination.parent.mkdir(parents=True, exist_ok=True)
            destination.write_bytes(data)
        report['additional_frozen_header_hashes'] = {name: digest(data) for name, data in additional_headers.items()}
        map_data = git_file(root, base, 'data/ver/eu/map.csv')
        base_rows = rows(map_data.decode())
        by_address = {int(row['Start'], 16): row for row in base_rows}
        current_map_data = (root / 'data/ver/eu/map.csv').read_bytes()
        current = {int(row['Start'], 16): row for row in rows(current_map_data.decode())}
        names = {int(entry['address'], 0): entry for entry in specification.get('new_names', [])}
        require(len(names) == len(specification.get('new_names', [])), 'Duplicate name proposal.')
        require(names.keys() <= by_address.keys(), 'New name has no existing whole row.')
        for address, entry in names.items():
            row = by_address[address]
            require(not row['Symbol'] and entry.get('evidence') in evidence_hashes,
                    'Known names cannot be renamed; new names need frozen human evidence.')
            require(re.fullmatch(r'[A-Za-z_][A-Za-z_0-9]*', entry['symbol']), 'Invalid proposed symbol.')
            actual = current.get(address)
            require(actual is not None and actual['Symbol'] in ('', entry['symbol']) and
                    all(actual[key] == row[key] for key in ('Start', 'Pool', 'End', 'Type', 'SectionName')),
                    'Current proposed identity or extent drift.')
            require(not any(other['Symbol'] == entry['symbol'] and int(other['Start'], 16) != address
                            for other in base_rows + list(current.values())), 'Proposed symbol is already used by another row.')
        targets = []
        for entry in specification['targets']:
            address = int(entry['address'], 0)
            require(address in by_address, 'No frozen whole target row.')
            row = by_address[address]
            require('f' in row['Type'] and row['Rank'] != 'O', 'Target must be an unaccepted frozen function.')
            name_proposal = names.get(address) if not row['Symbol'] else None
            require(row['Symbol'] or (name_proposal is not None and name_proposal['symbol'] == f'fn_{address:08X}'),
                    'A blank target needs an explicit evidenced whole-row neutral fn_<address> name.')
            symbol = row['Symbol'] or name_proposal['symbol']
            require(entry.get('symbol', symbol) == symbol, 'Target symbol differs from its frozen identity or explicit proposal.')
            require(entry['source'] in final_files and Path(entry['source']).suffix in ('.cpp', '.cc', '.cxx'), 'Target lacks a complete source file.')
            actual = current.get(address)
            require(actual is not None and actual['Symbol'] in (row['Symbol'], symbol) and
                    all(actual[key] == row[key] for key in ('Start', 'Pool', 'End', 'Type', 'SectionName')),
                    'Current target identity or extent drift.')
            require(specification.get('replay_only', False) or actual['Rank'] != 'O', 'Current target is already accepted; use replay_only.')
            targets.append({'address': row['Start'], 'symbol': symbol, 'source': entry['source'],
                            'complete_bytes': int(row['End'], 16) - address, 'whole_map_row': row,
                            'metadata_name_pending': not actual['Symbol'],
                            'name_proposal': name_proposal,
                            'carried_ready_bytes': entry.get('carried_ready_bytes', 0)})
            require(0 <= targets[-1]['carried_ready_bytes'] <= targets[-1]['complete_bytes'], 'Invalid carried bytes for target.')
        require(len({item['symbol'] for item in targets}) == len(targets), 'Duplicate target.')
        report['targets'] = targets
        report['form_history'] = [verify_history(root, output, number, entry)
                                  for number, entry in enumerate(specification.get('form_history', []))]
        report['current_form'] = specification['form']
        report['strict_diagnostics_pending_metadata'] = any(target['metadata_name_pending'] for target in targets)

    all_compiler_results = []
    report['compiler_results'] = all_compiler_results
    inputs_used = set()
    current_dependency_hashes = {}
    unit_sources = {unit['source'] for unit in specification['units']}
    require(unit_sources == {item['source'] for item in targets}, 'Every target source needs exactly one compiler unit.')
    require(len(unit_sources) == len(specification['units']), 'Duplicate compiler unit.')
    for unit in specification['units']:
        source = unit['source']
        _, provenance_data = supplied_file(root, unit['provenance'])
        provenance = json.loads(provenance_data)
        _, baseline_data = supplied_file(root, unit['baseline_contracts'])
        baseline = json.loads(baseline_data)
        evidence_directory = output / 'unit_evidence' / Path(source).with_suffix('')
        evidence_directory.mkdir(parents=True, exist_ok=True)
        (evidence_directory / 'project_command.json').write_bytes(provenance_data)
        (evidence_directory / 'baseline_contracts.json').write_bytes(baseline_data)
        require(provenance.get('schema') == 1 and provenance.get('build_step') == 'tools.pypstem.stepBuild'
                and provenance.get('language') == 'C++' and provenance['source'] == source
                and provenance.get('inputs_stable') and source in provenance['inputs']
                and 'data/config.json' in provenance['inputs'], 'Not an actual project compiler command record.')
        compiler = root / provenance['compiler']
        require(compiler.is_file() and digest(compiler.read_bytes()) == provenance['compiler_sha256'], 'Recorded compiler drift.')
        configuration = json.loads(git_file(root, base, 'data/config.json'))
        module = next((value for name, value in configuration['modules'].items()
                       if Path(source).is_relative_to(Path(name) / value.get('source_dir', '.'))), None)
        require(module is not None, 'Source is outside frozen configured modules.')
        configured_compiler = module.get('compiler', configuration['compiler'])
        require(specification['compilers'] == [configured_compiler, '4.1/894'] and configured_compiler != '4.1/894',
                'Use the configured primary compiler followed by the requested alternate4.1/894.')
        require(provenance['compiler'] == 'data/compilers/' + configured_compiler + '/bin/armcc.exe', 'Configured compiler record mismatch.')
        command_template = provenance['command']
        prefix = [str(compiler)] if sys.platform == 'win32' else [str(root / 'data/compilers/wibo'), str(compiler)]
        require(command_template[:len(prefix)] == prefix and '-c' in command_template
                and '-E' not in command_template and '-S' not in command_template
                and command_template[-1] == str(root / source), 'Unsupported direct project compiler command.')
        unit_targets = [target for target in targets if target['source'] == source]
        target_names = {target['symbol'] for target in unit_targets}
        accepted_names = {row['Symbol'] for row in base_rows if row['Rank'] == 'O' and 'f' in row['Type']}
        allowed_removals = unit.get('allowed_removed_unaccepted_definitions', {})
        require(not (accepted_names & allowed_removals.keys()) and all(allowed_removals.values()), 'An accepted definition may not be removed.')
        for compiler_version in specification['compilers']:
            require(re.fullmatch(r'[0-9]+\.[0-9]+/[0-9]+', compiler_version), 'Invalid compiler version.')
            setup_compiler(compiler_version)
            directory = output / 'diagnostics' / Path(source).stem / compiler_version
            snapshot = directory / 'input_snapshot'
            directory.mkdir(parents=True)
            frozen_inputs = {}
            with phase('snapshot:' + source + ':' + compiler_version):
                for name, expected in provenance['inputs'].items():
                    relative_path(name)
                    data = git_file(root, base, name)
                    require(digest(data) == expected, 'Recorded effective dependency differs from frozen base: ' + name)
                    if name not in final_files:
                        require((root / name).is_file() and digest((root / name).read_bytes()) == expected,
                                'Current effective dependency drift: ' + name)
                    data = final_files.get(name, data)
                    frozen_inputs[name] = digest(data)
                    destination = snapshot / name
                    destination.parent.mkdir(parents=True, exist_ok=True)
                    destination.write_bytes(data)
                for name, data in additional_headers.items():
                    require((root / name).is_file() and digest((root / name).read_bytes()) == digest(data),
                            'Current additional header drift before compilation: ' + name)
                    frozen_inputs[name] = digest(data)
                    destination = snapshot / name
                    destination.parent.mkdir(parents=True, exist_ok=True)
                    destination.write_bytes(data)
                # Complete explicitly owned files are available for new includes;
                # only actual dependency records enter the effective closure.
                for name, data in final_files.items():
                    destination = snapshot / name
                    destination.parent.mkdir(parents=True, exist_ok=True)
                    destination.write_bytes(data)
            command = list(command_template)
            command[len(prefix) - 1] = str(root / 'data/compilers' / compiler_version / 'bin/armcc.exe')
            for index, argument in enumerate(command):
                if argument.startswith('-I') and Path(argument[2:]).is_relative_to(root):
                    destination = snapshot / Path(argument[2:]).relative_to(root)
                    destination.mkdir(parents=True, exist_ok=True)
                    command[index] = '-I' + str(destination)
                elif argument.startswith('--preinclude='):
                    original = Path(argument.split('=', 1)[1])
                    require(original.is_relative_to(root), 'Preinclude is outside repository.')
                    command[index] = '--preinclude=' + str(snapshot / original.relative_to(root))
            object_path = directory / Path(source).with_suffix('.o').name
            dependency_path = directory / Path(source).with_suffix('.d').name
            command[command.index('-o') + 1] = str(object_path)
            command[command.index('--depend') + 1] = str(dependency_path)
            command[-1] = str(snapshot / source)
            compiler_inputs = {str(Path(argument).relative_to(root)): digest(Path(argument).read_bytes()) for argument in command[:len(prefix)]}
            record = {'source': source, 'compiler': compiler_version, 'compiler_input_hashes': compiler_inputs,
                      'command': command, 'start_utc': utc(),
                      'final_source_sha256': digest(final_files[source]), 'snapshot_input_hashes_before': frozen_inputs}
            all_compiler_results.append(record)
            write_json(directory / 'before_compile.json', record)
            with phase('compile:' + source + ':' + compiler_version):
                result = subprocess.run(command, cwd=root, env=dict(os.environ, TMP='/tmp'), capture_output=True, text=True, timeout=60)
            (directory / 'compile.log').write_text(result.stdout + result.stderr)
            record['compile_exit'] = result.returncode
            if result.returncode:
                record.update(physical_compile_failure=True, end_utc=utc())
                write_json(directory / 'result.json', record)
                continue
            object_hash = digest(object_path.read_bytes())
            record['object_sha256'] = object_hash
            require(all(digest((root / name).read_bytes()) == expected for name, expected in compiler_inputs.items()), 'Compiler input drift.')
            with phase('dependency_and_definition_validation:' + source + ':' + compiler_version):
                dependencies, external = dependency_names(dependency_path, snapshot, root)
                dependencies.add('data/config.json')
                require(dependencies <= frozen_inputs.keys() | final_files.keys(), 'An effective dependency was not frozen before compilation.')
                effective = {name: digest((snapshot / name).read_bytes()) for name in sorted(dependencies)}
                require(all(effective[name] == frozen_inputs.get(name, digest(final_files.get(name, b''))) for name in dependencies), 'Snapshot input drift during compile.')
                inputs_used.update(dependencies)
                current_dependency_hashes.update({name: expected for name, expected in effective.items() if name not in final_files})
                actual = function_contracts(object_path)
                removed = set(baseline) - actual.keys()
                added = actual.keys() - baseline.keys()
                changed = [name for name in baseline.keys() & actual.keys() if name not in target_names and baseline[name] != actual[name]]
                record.update(effective_dependency_hashes=effective, external_dependencies=external, definition_contracts=actual,
                              additional_frozen_headers_used=sorted(dependencies & additional_headers.keys()),
                              additional_frozen_headers_unused=sorted(additional_headers.keys() - dependencies),
                              removed_definitions=sorted(removed), added_definitions=sorted(added), changed_non_target_definitions=changed,
                              preserved_accepted_definitions=sorted(accepted_names & actual.keys()))
                require(not changed, 'Non-target definition changed: ' + ', '.join(changed))
                require(removed <= allowed_removals.keys() and not (removed & accepted_names), 'Unexplained or accepted definition removal.')
                require(added <= target_names, 'New undeclared helper definition.')
                require(all(name in actual and actual[name] == baseline[name] for name in accepted_names & baseline.keys()), 'Accepted family definition changed.')
            record['checks'] = []
            for target in unit_targets:
                if target['metadata_name_pending']:
                    definition = actual.get(target['symbol'])
                    extent_valid = definition is not None and definition['offset'] == 0 and definition['size'] == target['complete_bytes']
                    check = {'exact': False, 'strict_diagnostic_performed': False, 'pending_metadata': True,
                             'compiled_extent_valid': extent_valid,
                             'reason': 'Strict diagnostic awaits independently reviewed target metadata; no equality grade was performed.',
                             'evidence': {'symbol': target['symbol'], 'object_sha256': object_hash,
                                          'original_start': int(target['address'], 16),
                                          'original_size': target['complete_bytes'], 'definition_contract': definition}}
                else:
                    with phase('complete_interval_check:' + target['symbol'] + ':' + compiler_version):
                        check = check_exact_bytes(target['symbol'], object_path, provenance['version'], compiler_version,
                                                  output_directory=directory / ('function_' + target['address'][2:]))
                record['checks'].append({'target': target, 'result': check})
            require(digest(object_path.read_bytes()) == object_hash, 'Compiler object changed during checking.')
            require(all(digest((snapshot / name).read_bytes()) == expected for name, expected in effective.items()), 'Effective snapshot drift after checks.')
            record.update(end_utc=utc(), object_unchanged=True)
            write_json(directory / 'result.json', record)
    report['effective_dependency_paths'] = sorted(inputs_used)
    require(all((root / name).is_file() and digest((root / name).read_bytes()) == expected
                for name, expected in current_dependency_hashes.items()), 'Current effective dependency drift after checks.')
    report['effective_current_dependency_hashes_unchanged'] = current_dependency_hashes
    report['additional_frozen_header_hashes_after'] = {name: digest((root / name).read_bytes()) for name in additional_headers}
    require(report['additional_frozen_header_hashes_after'] == report['additional_frozen_header_hashes'],
            'Current additional header drift after checks.')
    require(all((root / name).is_file() and digest((root / name).read_bytes()) == expected
                for name, expected in evidence_hashes.items()), 'Frozen human evidence drift after checks.')
    report['human_evidence_hashes_after'] = evidence_hashes
    report['paired_exact'] = bool(all_compiler_results) and all(record.get('compile_exit') == 0 and
        len(record.get('checks', [])) > 0 and all(item['result']['exact'] for item in record['checks']) for record in all_compiler_results)

    with phase('generate_patches_queue_and_report'):
        source_patch = b''.join(patch(baseline_files[name], data, name) for name, data in final_files.items())
        (output / 'source.patch').write_bytes(source_patch)
        named_lines = []
        for line in map_data.decode().splitlines(True):
            fields = line.rstrip('\n').split(',')
            address = int(fields[0], 16) if fields[0].startswith('0x') else None
            if address in names:
                entry = names[address]
                fields[6] = entry['symbol']
                line = ','.join(fields) + '\n'
            named_lines.append(line)
        named = ''.join(named_lines).encode()
        current_named_lines = []
        for line in current_map_data.decode().splitlines(True):
            fields = line.rstrip('\n').split(',')
            address = int(fields[0], 16) if fields[0].startswith('0x') else None
            if address in names and not fields[6].strip():
                fields[6] = names[address]['symbol']
                line = ','.join(fields) + '\n'
            current_named_lines.append(line)
        enrolled_lines = []
        target_addresses = {int(target['address'], 16) for target in targets}
        for line in named.decode().splitlines(True):
            fields = line.rstrip('\n').split(',')
            address = int(fields[0], 16) if fields[0].startswith('0x') else None
            if address in target_addresses:
                fields[4] = 'M'
                line = ','.join(fields) + '\n'
            enrolled_lines.append(line)
        (output / 'import_names.patch').write_bytes(patch(current_map_data, ''.join(current_named_lines).encode(), 'data/ver/eu/map.csv'))
        (output / 'enrollment.patch').write_bytes(patch(named, ''.join(enrolled_lines).encode(), 'data/ver/eu/map.csv'))
        report['source_hashes'] = {name: {'baseline_source_sha256': digest(baseline_files[name]), 'final_source_sha256': digest(data)} for name, data in final_files.items()}
        report['new_names'] = [{'proposal': entry, 'whole_map_row': by_address[address],
                                'evidence_sha256': evidence_hashes[entry['evidence']],
                                'already_present_in_current_metadata': current[address]['Symbol'] == entry['symbol']}
                               for address, entry in names.items()]
        report['timing_basis'] = specification.get('timing', {})
        started = specification.get('timing', {}).get('start_utc', report['start_utc'])
        previous_minutes = (datetime.datetime.now(datetime.timezone.utc) - datetime.datetime.fromisoformat(started)).total_seconds() / 60
        meaningful_forms = 1 + sum(entry['meaningful'] for entry in report['form_history'])
        required_names = sorted({item['symbol'] for record in all_compiler_results for check in record.get('checks', [])
                                 for item in check['result'].get('evidence', {}).get('imports', [])})
        report['required_names'] = required_names
        queue = []
        if report['paired_exact'] and not specification.get('replay_only', False):
            for target in targets:
                queue.append({'lane': specification['lane'], 'address': target['address'], 'symbol': target['symbol'], 'source': target['source'],
                              'attempts': meaningful_forms + 1, 'meaningful_source_forms': meaningful_forms, 'previous_minutes': previous_minutes,
                              'complete_bytes': target['complete_bytes'], 'carried_ready_bytes': target['carried_ready_bytes'],
                              **report['source_hashes'][target['source']], 'package': str(output.relative_to(root)),
                              'required_names': required_names,
                              'source_patch': 'source.patch', 'import_names_patch': 'import_names.patch', 'enrollment_patch': 'enrollment.patch',
                              'paired_diagnostics': 'report.json', 'timing_basis': report['timing_basis']})
        write_json(output / 'canonical_acceptance_queue.json', queue)
        report['canonical_queue_rows'] = len(queue)
        report['canonical_acceptance_pending'] = True
        report['new_exact_credit'] = 0
        pending_ready = report['strict_diagnostics_pending_metadata'] and bool(all_compiler_results) and all(
            record.get('compile_exit') == 0 and record.get('checks') and all(
                item['result']['exact'] or (item['result'].get('pending_metadata') and item['result']['compiled_extent_valid'])
                for item in record['checks']) for record in all_compiler_results)
        report['status'] = 'paired_exact_proposal' if report['paired_exact'] else 'metadata_pending' if pending_ready else 'unsuccessful_form'
        (output / 'review.md').write_text('# Family handoff\n\n' + f"Base `{base}`. Complete final files and hashes are in report.json. "
            f"Paired exact: {report['paired_exact']}. Queue rows: {len(queue)}. New exact credit: 0.\n\n"
            'Source ownership, ABI, layout and data identity remain supplied human evidence. '
            'Blank-target metadata remains a separate unapplied proposal; its pending extent/contracts are not an equality grade. '
            'Root must commit the complete source, run the unchanged project build/checker and preserve every previous root and canonical definition.\n')


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('specification', type=Path)
    parser.add_argument('--output', type=Path, required=True)
    arguments = parser.parse_args()
    root = Path.cwd().resolve()
    require((root / 'project/BRIEF.md').is_file() and (root / '.git').exists(), 'Run from the repository root.')
    output = arguments.output.resolve()
    require(output.is_relative_to(root / 'build') and not output.exists(), 'Use a fresh ignored build output directory.')
    require(subprocess.run(['git', 'check-ignore', '--quiet', str(output)], cwd=root).returncode == 0, 'Output is not ignored by Git.')
    output.mkdir(parents=True)
    report = {'start_utc': utc(), 'phases': [], 'status': 'preparing', 'new_exact_credit': 0}
    started = time.monotonic()
    protected_paths = ['data/ver/eu/map.csv', 'data/config.json', 'data/ver/eu/code.bin', 'data/ver/eu/exh.bin',
                       'tools/check.py', 'tools/low/checkExactBytes.py', 'tools/low/buildProvenance.py', 'tools/acceptance_batch.py']
    protected = {name: digest((root / name).read_bytes()) for name in protected_paths}
    try:
        data = arguments.specification.read_bytes()
        (output / 'specification.json').write_bytes(data)
        prepare(root, json.loads(data), output, report)
        require(all(digest((root / name).read_bytes()) == expected for name, expected in protected.items()),
                'Protected oracle/build input drift during preparation.')
    except (PreparationError, KeyError, OSError, ValueError, subprocess.SubprocessError) as error:
        report.update(status='preparation_refused', error=str(error), canonical_queue_rows=0)
        write_json(output / 'canonical_acceptance_queue.json', [])
    finally:
        report.update(end_utc=utc(), elapsed_seconds=time.monotonic() - started)
        report['protected_input_hashes_before'] = protected
        report['protected_input_hashes_after'] = {name: digest((root / name).read_bytes()) for name in protected}
        report['protected_inputs_unchanged'] = report['protected_input_hashes_after'] == protected
        report['preparer_sha256'] = digest(Path(__file__).read_bytes())
        write_json(output / 'report.json', report)
        index = {str(path.relative_to(output)): digest(path.read_bytes()) for path in sorted(output.rglob('*')) if path.is_file() and path != output / 'sha256_manifest.json'}
        write_json(output / 'sha256_manifest.json', {'file_count': len(index), 'files': index})
    print(json.dumps({'status': report['status'], 'paired_exact': report.get('paired_exact', False),
                      'queue_rows': report.get('canonical_queue_rows', 0), 'seconds': report['elapsed_seconds'],
                      'error': report.get('error')}, indent=2))
    return 0 if report['status'] == 'paired_exact_proposal' else 1


if __name__ == '__main__':
    sys.exit(main())
