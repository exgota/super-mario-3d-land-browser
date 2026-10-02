from pathlib import Path
from elftools.elf.elffile import ELFFile
import csv,re,subprocess,json
out=Path('build/primitive-draw-setup');obj=Path('build/eu/obj/lib/al/src/Graphics/retail_PrimitiveDrawSetup.o');elf=ELFFile(obj.open('rb'))
rows={r[6]:r for r in csv.reader(open('data/ver/eu/map.csv')) if r and r[0].startswith('0x') and r[6]}
imports=[]
for s in elf.get_section_by_name('.symtab').iter_symbols():
 if s['st_shndx']!='SHN_UNDEF' or not s.name or s.name.startswith('Lib$$'):continue
 n=s.name
 if n in rows:a=int(rows[n][0],16);kind='A' if 'f' in rows[n][5] else 'D'
 elif re.fullmatch(r'(fn_|dat_)[0-9A-F]{8}',n):a=int(n[-8:],16);kind='A' if n.startswith('fn_') else 'D'
 else:print('Unresolved unused:',n);continue
 imports.append({'symbol':n,'address':a,'kind':kind,'missing_map':n=='dat_00430A88'})
(out/'imports.sym').write_text('#<SYMDEFS>#\n'+''.join('0x%08X %s %s\n'%(i['address'],i['kind'],i['symbol']) for i in imports))
(out/'behavior.sct').write_text('ROOT_LOAD 0x00500000\n{\n ROOT_CODE 0x00500000 { retail_PrimitiveDrawSetup.o (i.fn_002E0D2C, +FIRST) }\n REMAINDER +0 { .ANY(+RO,+RW,+ZI) }\n}\n')
command=['data/compilers/wibo','data/compilers/4.1/791/bin/armlink.exe','--scatter',str(out/'behavior.sct'),'--entry','fn_002E0D2C','--no_scanlib','--output',str(out/'behavior.axf'),'--map','--list',str(out/'behavior.map'),str(obj),str(out/'imports.sym')]
r=subprocess.run(command,text=True,capture_output=True);(out/'diagnostic-link.txt').write_text(r.stdout+r.stderr);print(r.stdout,r.stderr);r.check_returncode()
(out/'diagnostic-link.json').write_text(json.dumps({'command':command,'imports':imports,'status':r.returncode,'credit':'behavior diagnostic only; no object editing or canonical metadata change'},indent=2))
