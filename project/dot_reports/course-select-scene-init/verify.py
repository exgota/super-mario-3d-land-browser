#!/usr/bin/env python3
"""Reproduce the strict check and separate diagnostic link. Never grants credit."""
import argparse, hashlib, json, os, subprocess, sys, time
from pathlib import Path
ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT))
from elftools.elf.elffile import ELFFile
from tools.low.checkExactBytes import _read_map, _resolve_symbol, check_exact_bytes
from tools.low.buildProvenance import verify_build_output
SYMBOL = 'fn_0017CB1C'
OBJECT = ROOT / 'build/eu/obj/Game/backup/src/Scene/CourseSelectSceneInit.o'

def digest(path): return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def run(output, clean=False):
    os.chdir(ROOT)
    output = Path(output).resolve()
    assert output.is_relative_to(ROOT/'build')
    output.mkdir(parents=True, exist_ok=True)
    started = time.time()
    env = dict(os.environ, DEVKITARM='/usr', TMP='/tmp')
    command = [str(ROOT/'.venv/bin/python'), 'make.py', 'eu'] + (['-ca'] if clean else [])
    with (output/'build.log').open('w') as stream:
        build = subprocess.run(command, env=env, stdout=stream, stderr=subprocess.STDOUT)
    if build.returncode: raise RuntimeError('Project build failed; see build.log')
    provenance = verify_build_output(OBJECT)
    mp = ROOT/'data/ver/eu/map.csv'
    saved = mp.read_bytes()
    before = digest(mp)
    needle = '0x0017CB1C,0x0017D124,0x0017D398,          ,U,f,,'
    changed = saved.decode().replace(needle, needle[:-1] + SYMBOL + ',')
    assert changed.encode() != saved, 'Expected frozen blank U row'
    try:
        mp.write_text(changed)
        check = subprocess.run([str(ROOT/'.venv/bin/python'), 'tools/check.py', SYMBOL,
                                '--object', str(OBJECT)], env=env,
                               stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        (output/'check.txt').write_text(check.stdout)
        result = check_exact_bytes(SYMBOL, OBJECT, output_directory=output/'strict')
        (output/'strict.json').write_text(json.dumps(result, indent=2))
    finally:
        mp.write_bytes(saved)
    assert digest(mp) == before

    # Diagnostics use the untouched full compiler object, not the compact U stub.
    # Only existing map identities are imported. No original machine-code bytes
    # enter this link. This link deliberately makes no size/equality claim.
    rows = _read_map(mp)
    with OBJECT.open('rb') as stream:
        elf = ELFFile(stream)
        symbols = elf.get_section_by_name('.symtab')
        imports=[]
        for symbol in symbols.iter_symbols():
            if symbol['st_shndx'] != 'SHN_UNDEF' or not symbol.name or symbol.name.startswith('Lib$$'):
                continue
            address, kind, row = _resolve_symbol(symbol, None, rows)
            imports.append({'symbol':symbol.name, 'address':address, 'kind':kind,
                            'row_end':row['End'], 'row_type':row['Type']})
        compiled_size=elf.get_section_by_name('i.'+SYMBOL)['sh_size']
    (output/'imports.json').write_text(json.dumps(imports, indent=2))
    (output/'imports.sym').write_text('#<SYMDEFS>#\n' + ''.join(
        f"0x{i['address']:08X} {i['kind']} {i['symbol']}\n" for i in imports))
    (output/'diagnostic.sct').write_text('DIAGNOSTIC 0x0017CB1C {\n BODY 0x0017CB1C {\n '
        + OBJECT.name + ' (i.'+SYMBOL+', +FIRST)\n }\n}\n')
    command=[str(ROOT/'data/compilers/wibo'),str(ROOT/'data/compilers/4.1/791/bin/armlink.exe'),
        '--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--inline',
        '--datacompressor=off','--no_debug','--no_scanlib','--mangled','--symbols','--map',
        '--entry='+SYMBOL,'--keep='+SYMBOL,'--scatter='+str(output/'diagnostic.sct'),
        '--output='+str(output/'diagnostic.axf'),'--list='+str(output/'diagnostic.map'),
        str(OBJECT),str(output/'imports.sym')]
    link=subprocess.run(command, env=env, stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True)
    (output/'diagnostic-link.log').write_text(link.stdout)
    if link.returncode: raise RuntimeError('Diagnostic link failed')
    result={'source_sha256':digest(ROOT/'Game/backup/src/Scene/CourseSelectSceneInit.cpp'),
            'object_sha256':digest(OBJECT),'provenance':provenance,'compiled_size':compiled_size,
            'original_size':2172,'build_returncode':build.returncode,'check_returncode':check.returncode,
            'check_output':check.stdout,'strict_result':result,'diagnostic_command':command,
            'diagnostic_sha256':digest(output/'diagnostic.axf'),'map_restored_sha256':before,
            'elapsed_seconds':time.time()-started}
    (output/'verification.json').write_text(json.dumps(result,indent=2))
    print(json.dumps({k:result[k] for k in ['compiled_size','original_size','check_output','elapsed_seconds']},indent=2))
    return result

if __name__=='__main__':
    ap=argparse.ArgumentParser();ap.add_argument('--output',default='build/course-scene-final');ap.add_argument('--clean',action='store_true');a=ap.parse_args();run(a.output,a.clean)
