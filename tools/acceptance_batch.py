#!/usr/bin/env python3
"""Build a committed family batch and gate new checks on every prior root."""
import argparse
import csv
import io
import json
from pathlib import Path
import subprocess
import sys
import time

from elftools.elf.elffile import ELFFile

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from tools.acceptance_worker import AcceptanceWorker


def command(arguments, **options):
    return subprocess.run(arguments, check=True, **options)


def map_rows(text):
    return [{key.strip(): value.strip() for key, value in row.items()}
            for row in csv.DictReader(io.StringIO(text))]



def check_candidates(candidates, root, report, save, definitions=None, checker=None, finish_checker=None):
    """Undo every candidate rank change when the complete batch fails."""
    map_path = root / 'data/ver/eu/map.csv'
    prior_map = map_path.read_bytes()
    try:
        for candidate in candidates:
            expected_object = root / 'build/eu/obj' / Path(candidate['source']).with_suffix('.o')
            available = definitions[candidate['symbol']] if definitions is not None else [(True, expected_object)]
            paths = sorted({entry[1] for entry in available})
            strong_paths = {entry[1] for entry in available if entry[0]}
            if not paths or expected_object not in paths or len(strong_paths) > 1:
                report['candidate_checks'].append({'candidate': candidate, 'returncode': 1,
                                                   'seconds': 0, 'definitions': [],
                                                   'error': 'Missing declared or ambiguous canonical definition'})
                save()
                continue
            checks = []
            for object_path in paths:
                started = time.monotonic()
                result = checker(candidate['symbol'], object_path) if checker else subprocess.run([sys.executable, 'tools/check.py', candidate['symbol'], '--object', str(object_path)], capture_output=True, text=True)
                checks.append({'object': str(object_path.relative_to(root)), 'returncode': result.returncode,
                               'seconds': time.monotonic() - started, 'output': result.stdout, 'error': result.stderr})
            report['candidate_checks'].append({'candidate': candidate,
                                               'returncode': 0 if all(check['returncode'] == 0 for check in checks) else 1,
                                               'seconds': sum(check['seconds'] for check in checks),
                                               'definitions': checks})
            save()
        if finish_checker:
            finish_checker()
        report['accepted'] = all(check['returncode'] == 0 for check in report['candidate_checks'])
        if not report['accepted']:
            map_path.write_bytes(prior_map)
            report['candidate_ranks_restored'] = True
        save()
    except BaseException:
        map_path.write_bytes(prior_map)
        report['candidate_ranks_restored'] = True
        report['accepted'] = False
        save()
        raise


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('manifest', type=Path)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--checker-transport', choices=('worker', 'cli'), default='worker')
    arguments = parser.parse_args()
    manifest = json.loads(arguments.manifest.read_text())
    root = Path(__file__).resolve().parents[1]
    if Path.cwd() != root:
        parser.error('Run from the repository root after sourcing development_environment.sh.')
    output = arguments.output.resolve()
    if not output.is_relative_to(root / 'build') or output.exists():
        parser.error('Use a fresh output directory under ignored build/.')
    output.mkdir(parents=True)
    base = manifest['prior_checkpoint']
    command(['git', 'merge-base', '--is-ancestor', base, 'HEAD'])
    dirty = subprocess.check_output(['git', 'diff', '--name-only', 'HEAD'], text=True)
    if dirty.strip():
        parser.error('Commit all intended tracked inputs before the canonical build.')
    previous = map_rows(subprocess.check_output(
        ['git', 'show', base + ':data/ver/eu/map.csv'], text=True))
    previous = [row for row in previous if row['Rank'] == 'O' and 'f' in row['Type']]
    current = map_rows((root / 'data/ver/eu/map.csv').read_text())
    prior_symbols = {row['Symbol'] for row in previous}
    current_accepted = {row['Symbol'] for row in current
                        if row['Rank'] == 'O' and 'f' in row['Type']}
    if current_accepted != prior_symbols:
        parser.error('Prior checkpoint must describe exactly the current accepted roots.')
    current_by_symbol = {row['Symbol']: row for row in current if row['Symbol']}
    ownership = {}
    candidates = manifest['candidates']
    proposed_symbols = set()
    for candidate in candidates:
        source = candidate['source']
        owner = candidate['lane']
        if source in ownership and ownership[source] != owner:
            parser.error('A translation-unit family has multiple owners: ' + source)
        ownership[source] = owner
        symbol = candidate['symbol']
        if symbol in prior_symbols or symbol in proposed_symbols:
            parser.error('Candidate is already accepted or duplicated: ' + symbol)
        proposed_symbols.add(symbol)
        if symbol not in current_by_symbol or current_by_symbol[symbol]['Rank'] != 'M':
            parser.error('Enroll each candidate as M before building: ' + symbol)
    for row in previous:
        actual = current_by_symbol.get(row['Symbol'])
        if actual is None or any(actual[key] != row[key] for key in ('Start', 'Pool', 'End', 'Type', 'SectionName')):
            parser.error('An accepted root identity or extent changed: ' + row['Symbol'])
    started = time.monotonic()
    report = {'checkpoint': subprocess.check_output(['git', 'rev-parse', 'HEAD'], text=True).strip(),
              'prior_checkpoint': base, 'prior_roots': len(previous), 'ownership': ownership,
              'prior_checks': [], 'candidate_checks': [], 'accepted': False}

    def save():
        report['elapsed_seconds'] = time.monotonic() - started
        (output / 'report.json').write_text(json.dumps(report, indent=2) + '\n')

    build_started = time.monotonic()
    with (output / 'clean_build.log').open('w') as stream:
        result = subprocess.run([sys.executable, 'make.py', 'eu', '-ca'], stdout=stream, stderr=subprocess.STDOUT)
    report['clean_build_returncode'] = result.returncode
    report['clean_build_seconds'] = time.monotonic() - build_started
    save()
    if result.returncode:
        return 1
    inventory_started = time.monotonic()
    definitions = {symbol: [] for symbol in prior_symbols | proposed_symbols}
    for object_path in sorted((root / 'build/eu/obj').rglob('*.o')):
        relative = object_path.relative_to(root / 'build/eu/obj')
        if relative.parts[0] not in ('Game', 'lib'):
            continue
        source = relative.with_suffix('.cpp')
        if not (root / source).is_file():
            continue
        with object_path.open('rb') as stream:
            table = ELFFile(stream).get_section_by_name('.symtab')
            if table is None:
                continue
            for definition in table.iter_symbols():
                if definition.name in definitions and isinstance(definition['st_shndx'], int) and definition['st_info']['type'] == 'STT_FUNC':
                    definitions[definition.name].append((definition['st_info']['bind'] == 'STB_GLOBAL', object_path))
    report['definition_inventory_seconds'] = time.monotonic() - inventory_started
    report['checker_transport'] = arguments.checker_transport
    report['worker_jobs'] = []
    worker = AcceptanceWorker(root) if arguments.checker_transport == 'worker' else None
    report['worker_startup'] = worker.ready if worker else None

    def run_check(symbol, object_path):
        if worker:
            result, record = worker.check(symbol, object_path)
            report['worker_jobs'].append(record)
            return result
        return subprocess.run([sys.executable, 'tools/check.py', symbol, '--object', str(object_path)], capture_output=True, text=True)

    def finish_checker():
        nonlocal worker
        if worker:
            worker.close()
            worker = None

    try:
        for index, row in enumerate(previous):
            available = definitions[row['Symbol']]
            strong = [entry for entry in available if entry[0]]
            paths = sorted({entry[1] for entry in available})
            strong_paths = {entry[1] for entry in strong}
            if not paths or len(strong_paths) > 1:
                report['prior_checks'].append({'symbol': row['Symbol'], 'error': 'Missing or ambiguous canonical definition'})
                save()
                return 1
            for object_path in paths:
                job_started = time.monotonic()
                result = run_check(row['Symbol'], object_path)
                report['prior_checks'].append({'symbol': row['Symbol'], 'object': str(object_path.relative_to(root)),
                                               'returncode': result.returncode, 'seconds': time.monotonic() - job_started, 'output': result.stdout, 'error': result.stderr})
            save()
            if (index + 1) % 50 == 0:
                print('Prior roots checked:', index + 1, '/', len(previous), flush=True)
        if any(check.get('returncode', 1) for check in report['prior_checks']):
            print('Preservation rejected. Return the unchanged source proposal to its owner.', flush=True)
            return 1
        # Only the unchanged project checker writes O. Its candidate rank changes
        # remain provisional until the entire batch succeeds; failure restores M.
        check_candidates(candidates, root, report, save, definitions, run_check, finish_checker)
    except BaseException:
        report['accepted'] = False
        save()
        raise
    finally:
        if worker:
            worker.terminate()
    print('Canonical candidates:', sum(check['returncode'] == 0 for check in report['candidate_checks']), '/', len(candidates), flush=True)
    return 0 if report['accepted'] else 1


if __name__ == '__main__':
    sys.exit(main())
