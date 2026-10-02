#!/usr/bin/env python3
"""Compare prior canonical build inputs and every prior ELF section/symbol/relocation.
This establishes equivalence to the separately canonical-checked pristine baseline;
it is not another checker pass. Run only after the final clean project build.
"""
from pathlib import Path
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
import json, hashlib, collections, subprocess, sys
root=Path.cwd();base=(root.parent/'mario-main861').resolve();out=root/'build/item-spawn-dispatcher'
BASE='86104d96a7f570383bbfefc5fffad998e134496c'
assert subprocess.check_output(['git','rev-parse','HEAD'],cwd=base,text=True).strip()==BASE

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def normalized(p):
    with open(p,'rb') as f:
        e=ELFFile(f);sections=[];symbols=[]
        def symrec(s):
            n=s['st_shndx'];sec=e.get_section(n).name if isinstance(n,int) else n
            return [s.name.replace(str(root),'<repo>').replace(str(base),'<repo>'),s['st_info']['type'],s['st_info']['bind'],s['st_other']['visibility'],sec,s['st_value'],s['st_size']]
        for sec in e.iter_sections():
            if isinstance(sec,RelocationSection):
                tab=e.get_section(sec['sh_link']);sections.append([sec.name,[(r['r_offset'],r['r_info_type'],symrec(tab.get_symbol(r['r_info_sym']))) for r in sec.iter_relocations()]])
            elif sec['sh_type'] not in ('SHT_SYMTAB','SHT_STRTAB','SHT_NULL'):
                data=sec.data()
                if sec.name=='.comment':data=data.replace(str(root).encode(),b'<repo>').replace(str(base).encode(),b'<repo>')
                sections.append([sec.name,sec['sh_type'],sec['sh_flags'],sec['sh_addralign'],sec['sh_entsize'],hashlib.sha256(data).hexdigest()])
        for s in e.get_section_by_name('.symtab').iter_symbols():symbols.append(symrec(s))
        return {'sections':sections,'symbols':symbols,'elf_flags':e.header['e_flags']}
expected={str(p.relative_to(base/'build/eu/obj')) for p in (base/'build/eu/obj').rglob('*.o') if p.relative_to(base/'build/eu/obj').parts[0] in ('Game','lib')}
assert len(expected)==179,len(expected)
current={str(p.relative_to(root/'build/eu/obj')) for p in (root/'build/eu/obj').rglob('*.o') if p.relative_to(root/'build/eu/obj').parts[0] in ('Game','lib')}
assert current-expected=={'Game/backup/src/MapObj/ItemSpawnDispatcher2CDEA4.o'}
records=[];inputs={}
for rel in sorted(expected):
    p=root/'build/eu/obj'/rel;q=base/'build/eu/obj'/rel
    pa=json.loads(p.with_suffix('.provenance.json').read_text());pb=json.loads(q.with_suffix('.provenance.json').read_text())
    for prov,obj,repo in ((pa,p,root),(pb,q,base)):
        assert prov['schema']==1 and prov['build_step']=='tools.pypstem.stepBuild' and prov['language']=='C++' and prov['inputs_stable']
        assert prov['object_sha256']==digest(obj)
        assert prov['compiler_sha256']==digest(repo/prov['compiler'])
    for k in ('source','object','version','compiler','compiler_sha256','inputs','inputs_stable','build_step','language'):assert pa[k]==pb[k],(rel,k)
    for name,value in pa['inputs'].items():
        assert digest(root/name)==value and digest(base/name)==value,(rel,name)
        assert subprocess.check_output(['git','show','HEAD:'+name],cwd=root)==(root/name).read_bytes(),name
        inputs[name]=value
    assert [s.replace(str(root),'<repo>') for s in pa['command']]==[s.replace(str(base),'<repo>') for s in pb['command']],rel+' command'
    a=normalized(p);b=normalized(q)
    if a!=b:
        (out/'preserve-failed.json').write_text(json.dumps({'object':rel,'current':a,'base':b},indent=2));raise AssertionError(rel)
    records.append({'object':rel,'current_sha256':digest(p),'baseline_sha256':digest(q),'normalized_elf_sha256':hashlib.sha256(json.dumps(a,sort_keys=True).encode()).hexdigest(),'provenance_sha256':digest(p.with_suffix('.provenance.json'))})
# Scaffold is generated and excluded from canonical source credit. Audit every old
# allocated section and the unwind entry attached to each original scaffold root.
def stubs(repo):
    p=repo/'build/eu/obj/build/eu/split/stubs.o';e=ELFFile(open(p,'rb'));symbols=e.get_section_by_name('.symtab')
    alloc={s.name:[s['sh_type'],s['sh_flags'],s['sh_addralign'],s['sh_entsize'],s.data().hex()] for s in e.iter_sections() if s['sh_flags']&2 and s.name!='.ARM.exidx'}
    unwind={};ex=e.get_section_by_name('.ARM.exidx');rel=e.get_section_by_name('.rel.ARM.exidx')
    if rel:
        for r in rel.iter_relocations():
            offset=r['r_offset'];target=symbols.get_symbol(r['r_info_sym']);name=target.name
            if target['st_info']['type']=='STT_SECTION':name=e.get_section(target['st_shndx']).name
            assert r['r_info_type']==42
            unwind[name]=ex.data()[offset:offset+8].hex()
    return p,alloc,unwind
p,a,au=stubs(root);q,b,bu=stubs(base)
assert set(b)<=set(a)
for key,value in b.items():assert a[key]==value,key
assert set(bu)<=set(au)
for key,value in bu.items():assert au[key]==value,key
aliases=sorted(set(a)-set(b))
# Added source is never assigned O and no old map row changes.
assert (root/'data/ver/eu/map.csv').read_bytes()==(base/'data/ver/eu/map.csv').read_bytes()
baseline_report=base/'build/dot-baseline-861/report.json'
assert digest(baseline_report)=='a692f9a29d92c3f8021b62a2254b06fb44776f34cfa2f51849364bde66dee374'
report={'base':BASE,'baseline_report_sha256':digest(baseline_report),'basis':'pristine baseline canonical 767 roots / 787 definitions plus current exhaustive object/input equivalence; not a fresh checker pass','prior_objects':len(records),'current_cpp_objects':len(current),'inputs':inputs,'objects':records,'all_equal':True,'normalization':'repository prefix in STT_FILE names and nonallocated .comment only; raw symtab/string offsets replaced by resolved symbols','scaffold':{'old_allocated_sections':len(b),'old_unwind_entries':len(bu),'all_old_sections_and_entries_equal':True,'added_sections':aliases,'current_sha256':digest(p),'baseline_sha256':digest(q)},'map_sha256':digest(root/'data/ver/eu/map.csv')}
(out/'preservation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ('inputs','objects')},indent=2))
