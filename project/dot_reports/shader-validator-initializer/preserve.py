from pathlib import Path
from elftools.elf.elffile import ELFFile
import json,hashlib,subprocess,sys
ROOT=Path.cwd();BASE=Path(sys.argv[1]).resolve() if len(sys.argv)>1 else ROOT.parent/'mario-main754';OUT=ROOT/'build/shader-initializer'
sys.path.insert(0,str(ROOT))
from tools.low.buildProvenance import verify_build_output
H=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def alloc(path):
 with path.open('rb') as f:
  e=ELFFile(f);secs=list(e.iter_sections());st=e.get_section_by_name('.symtab');res=[]; allsyms=list(st.iter_symbols())
  for i,s in enumerate(secs):
   if not s['sh_flags']&2:continue
   syms=sorted((x.name,x['st_value'],x['st_size'],x['st_info']['type'],x['st_info']['bind'],str(x['st_other'])) for x in allsyms if x['st_shndx']==i)
   rels=[]
   for r in secs:
    if r['sh_type'] not in ('SHT_REL','SHT_RELA') or r['sh_info']!=i:continue
    tab=secs[r['sh_link']]
    for rel in r.iter_relocations():
     sym=tab.get_symbol(rel['r_info_sym']);idx=sym['st_shndx'];target=secs[idx].name if isinstance(idx,int) else idx
     rels.append((rel['r_offset'],rel['r_info_type'],sym.name,target,sym['st_value'],rel.entry.get('r_addend')))
   res.append((s.name,s['sh_type'],s['sh_flags'],s['sh_addralign'],s['sh_size'],HBytes(s.data()) if s['sh_type']!='SHT_NOBITS' else None,syms,rels))
  return res
def HBytes(b):return hashlib.sha256(b).hexdigest()
paths=subprocess.check_output(['git','ls-tree','-r','--name-only','754f99a','--','Game','lib','data','tools','make.py','development_environment.sh'],text=True).splitlines()
unchanged=[]
for rel in paths:
 p=ROOT/rel;b=BASE/rel
 if p.is_file() and b.is_file():
  assert H(p)==H(b),rel;unchanged.append([rel,H(p)])
objects=[]
for p in sorted((BASE/'build/eu/obj').rglob('*.o')):
 rel=p.relative_to(BASE)
 if not (BASE/rel).with_suffix('.provenance.json').exists():continue
 q=ROOT/rel;assert q.exists(),str(q)
 bp=json.loads(p.with_suffix('.provenance.json').read_text())
 if Path(bp['source']).suffix not in ('.cpp','.cc','.cxx'):continue
 a=alloc(p);b=alloc(q);assert a==b,str(rel)
 bp=json.loads(p.with_suffix('.provenance.json').read_text());qp=json.loads(q.with_suffix('.provenance.json').read_text())
 assert bp['inputs']==qp['inputs'],str(rel)
 assert bp['compiler_sha256']==qp['compiler_sha256'],str(rel)
 def norm(v,base):return [x.replace(str(base),'$ROOT') for x in v]
 assert norm(bp['command'],BASE)==norm(qp['command'],ROOT),str(rel)
 prov=verify_build_output(q)
 objects.append({'object':str(rel),'baseline_object_sha256':H(p),'current_object_sha256':H(q),'allocated_signature_sha256':HBytes(json.dumps(a,sort_keys=True).encode()),'allocated_sections':len(a),'provenance':prov,'inputs':qp['inputs']})
report=json.loads((BASE/'build/dot-baseline-754/report.json').read_text());checks=report['prior_checks'];objset={o['object'] for o in objects}
assert all(x['returncode']==0 and x['object'] in objset for x in checks)
out={'baseline':'754f99a30a337756df5aa01c28b4ed6a977fecd5','baseline_report_sha256':H(BASE/'build/dot-baseline-754/report.json'),'prior_roots':733,'actual_definition_checks_in_baseline':len(checks),'all_old_canonical_objects_compared':len(objects),'preserved_inputs':len(unchanged),'method':'All old tracked source/header/build files, provenance input hashes, compiler hashes and normalized commands equal. Every old canonical object allocated section, section bytes, symbol and relocation signature equal. This is preservation equivalence, not a new 753-check run.','inputs':unchanged,'objects':objects}
(OUT/'preservation.json').write_text(json.dumps(out,indent=2));print({k:v for k,v in out.items() if k not in ('inputs','objects')})
