#!/usr/bin/env python3
"""Prove a game compiler with unchanged flags and strict original-address links."""
import argparse
import csv
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
from tools.low.checkExactBytes import check_exact_bytes
from tools.low import cfg
from tools.low.glob import getProjDir, getCompilersDir, needsWibo
from tools.pypstem.defaultFlags import default_flags_comp, default_flags_comp_cxx
from tools.pypstem.manSetup import setup_compiler


def compile_source(source, compiler_version, output_directory):
    setup_compiler(compiler_version)
    root = getProjDir()
    source = root / source
    module = next((data for name, data in cfg.modules.items()
                   if name in ('Game', 'lib/al') and source.is_relative_to(root / name)), None)
    if module is None:
        raise ValueError('Compiler proof inputs must be Game or lib/al source files.')
    flags = []
    for name, data in cfg.modules.items():
        flags.append('-I' + str(root / name / data.get('include_dir', 'include')))
    flags += ['-DVERSION=EU', '-DNON_MATCHING=1']
    flags += ['-D' + name + '=' + str(value) for name, value in cfg.macros.items()]
    flags += default_flags_comp + cfg.flags_compile + default_flags_comp_cxx + cfg.flags_compile_cxx
    flags += ['--no_debug', '--preinclude=' + str(root / cfg.flag_preinclude)]
    flags += module.get('flags', []) + module.get('flags_cxx', [])
    flags += ['-D' + name + '=' + str(value) for name, value in module.get('macros', {}).items()]
    output = output_directory / (source.stem + '.o')
    command = [str(getCompilersDir() / compiler_version / 'bin/armcc.exe')]
    if needsWibo():
        command.insert(0, str(getCompilersDir() / 'wibo'))
    command += flags + ['-c', f'-D__BASE_FILE_NAME__="{source.name}"', '-o', str(output), str(source)]
    source_hash = hashlib.sha256(source.read_bytes()).hexdigest()
    result = subprocess.run(command, cwd=root, env=os.environ.copy(), capture_output=True, text=True)
    (output_directory / (source.stem + '.log')).write_text(result.stdout + result.stderr)
    evidence = {'source': str(source.relative_to(root)),
                'source_sha256': source_hash,
                'source_unchanged': source_hash == hashlib.sha256(source.read_bytes()).hexdigest(),
                'compiler': compiler_version, 'command': command, 'compile_exit': result.returncode}
    return output, evidence


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--primary', default='4.1/791')
    parser.add_argument('--alternative', default='4.1/894')
    parser.add_argument('--functions', type=Path, default=Path('project/compiler_probe_functions.csv'))
    arguments = parser.parse_args()
    functions = list(csv.DictReader(arguments.functions.open()))
    if not functions or len({row['symbol'] for row in functions}) != len(functions):
        parser.error('Provide distinct game functions in the probe manifest.')
    root = getProjDir()
    header_files = sorted(path for directory in (root / 'Game', root / 'lib') for path in directory.rglob('*.h'))
    initial_hashes = {str(path.relative_to(root)): hashlib.sha256(path.read_bytes()).hexdigest() for path in header_files}
    report = {'primary': arguments.primary, 'alternative': arguments.alternative,
              'header_sha256': initial_hashes, 'functions': [], 'required_discriminators': 3}
    output_root = root / 'build/game_compiler_check'
    for row in functions:
        entry = {'symbol': row['symbol'], 'source': row['source'], 'compilers': []}
        for compiler in (arguments.primary, arguments.alternative):
            directory = output_root / compiler / row['symbol']
            directory.mkdir(parents=True, exist_ok=True)
            output, evidence = compile_source(row['source'], compiler, directory)
            if evidence['compile_exit'] == 0:
                evidence['check'] = check_exact_bytes(row['symbol'], output, compiler_version=compiler,
                                                       output_directory=directory / 'exact')
            entry['compilers'].append(evidence)
        primary, alternative = entry['compilers']
        stable = (primary['source_unchanged'] and alternative['source_unchanged']
                  and primary['source_sha256'] == alternative['source_sha256'])
        valid_difference = alternative.get('check', {}).get('reason') in (
            'The complete compiled section, including its literal pool, has a different size from the original interval.',
            'The linked candidate differs from the unchanged original interval.')
        entry['discriminates'] = (stable and primary.get('check', {}).get('exact', False)
                                 and alternative['compile_exit'] == 0 and valid_difference)
        report['functions'].append(entry)
        print(row['symbol'], 'discriminates' if entry['discriminates'] else 'does not discriminate', flush=True)
    final_header_files = sorted(path for directory in (root / 'Game', root / 'lib') for path in directory.rglob('*.h'))
    final_hashes = {str(path.relative_to(root)): hashlib.sha256(path.read_bytes()).hexdigest() for path in final_header_files}
    report['headers_unchanged'] = initial_hashes == final_hashes
    report['discriminators'] = sum(entry['discriminates'] for entry in report['functions'])
    report['passes'] = report['headers_unchanged'] and report['discriminators'] >= 3
    output_root.mkdir(parents=True, exist_ok=True)
    (output_root / 'evidence.json').write_text(json.dumps(report, indent=2) + '\n')
    print(f"{report['discriminators']} / 3 compiler discriminators; M0 {'passes' if report['passes'] else 'not proved'}")
    return 0 if report['passes'] else 1


if __name__ == '__main__':
    raise SystemExit(main())
