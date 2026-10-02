#!/usr/bin/env python3
"""Replay link only; canonical checker remains the sole matching oracle."""
from pathlib import Path
from elftools.elf.elffile import ELFFile
import re,csv,subprocess,os,json
out=Path('build/item-spawn-dispatcher');out.mkdir(parents=True,exist_ok=True)
obj='build/eu/obj/Game/backup/src/MapObj/ItemSpawnDispatcher2CDEA4.o'
rows=[{k.strip():v.strip() for k,v in r.items()} for r in csv.DictReader(open('data/ver/eu/map.csv'))];byname={r['Symbol']:r for r in rows if r['Symbol']};lines=['#<SYMDEFS>#'];imports=[]
with open(obj,'rb') as f:
    elf=ELFFile(f)
    for s in elf.get_section_by_name('.symtab').iter_symbols():
        if s['st_shndx']!='SHN_UNDEF' or not s.name or '$$' in s.name or s['st_info']['bind']=='STB_WEAK': continue
        n=s.name;m=re.fullmatch('(fn|dat)_([0-9A-F]{8})',n)
        if m:r=next(x for x in rows if int(x['Start'],16)==int(m[2],16))
        else:r=byname[n]
        a=int(r['Start'],16);kind='A' if 'f' in r['Type'] else 'D';lines.append(f'0x{a:08X} {kind} {n}');imports.append({'symbol':n,'address':a,'kind':kind,'end':int(r['End'],16)})
(out/'imports.json').write_text(json.dumps(imports,indent=2)+'\n');(out/'imports.sym').write_text('\n'.join(lines)+'\n')
subprocess.run(['data/compilers/wibo','data/compilers/4.1/791/bin/armlink.exe','--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--inline','--datacompressor=off','--no_debug','--no_scanlib','--mangled','--symbols','--map','--entry=fn_002CDEA4','--keep=fn_002CDEA4','--ro_base=0x00500000','--output='+str(out/'candidate.axf'),'--list='+str(out/'candidate.map'),obj,str(out/'imports.sym')],check=True,env=dict(os.environ,TMP='/tmp'))
