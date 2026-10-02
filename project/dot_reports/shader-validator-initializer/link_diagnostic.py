from pathlib import Path
from elftools.elf.elffile import ELFFile
import subprocess,csv,json,hashlib
root=Path.cwd(); out=root/'build/shader-initializer';obj=root/'build/eu/obj/lib/CtrSDK/sources/shv_InitializeValidator.o'
out.mkdir(parents=True, exist_ok=True)
rows=list(csv.reader((root/'data/ver/eu/map.csv').open()));syms={}
for row in rows:
 if len(row)>6:
  try:a=int(row[0],16)
  except:continue
  name=row[6].strip() or ('fn_' if row[5].startswith('f') else 'dat_')+f'{a:08X}'
  syms[name]=(a,'A' if row[5].startswith('f') else 'D')
# Qualified diagnostic only: missing metadata is supplied externally, never to checker.
syms['dat_00420F4C']=(0x420f4c,'D')
with obj.open('rb') as f:
 e=ELFFile(f);und=[s.name for s in e.get_section_by_name('.symtab').iter_symbols() if s['st_shndx']=='SHN_UNDEF' and s.name and not s.name.startswith('Lib$$')]
 print('undefined',und)
lines=['#<SYMDEFS>#']+[f'0x{syms[n][0]:08X} {syms[n][1]} {n}' for n in und]
(out/'diagnostic.sym').write_text('\n'.join(lines)+'\n')
cmd=[str(root/'data/compilers/wibo'),str(root/'data/compilers/4.0/902/bin/armlink.exe'),'--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--no_debug','--no_scanlib','--mangled','--symbols','--map','--ro_base=0x600000','--entry=__shv_initializeShaderValidator','--keep=__shv_initializeShaderValidator',f'--output={out}/diagnostic.axf',f'--list={out}/diagnostic.map',str(obj),str(out/'diagnostic.sym')]
p=subprocess.run(cmd,capture_output=True,text=True);print(p.stdout,p.stderr);assert p.returncode==0
(out/'diagnostic-link.json').write_text(json.dumps({'command':cmd,'output':p.stdout+p.stderr,'exit':p.returncode,'qualification':'Explicit address binding for missing dat_00420F4C metadata. Not canonical linkage.'},indent=2))
