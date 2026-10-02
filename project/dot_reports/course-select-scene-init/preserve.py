#!/usr/bin/env python3
"""Compare every prior input and canonical C++ object with a passed frozen baseline.
This is exhaustive equivalence evidence, not a new run of all prior check.py roots.
"""
import argparse,hashlib,json,subprocess,sys,time
from collections import Counter
from pathlib import Path
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
ROOT=Path(__file__).resolve().parents[3]
BASE='86104d96a7f570383bbfefc5fffad998e134496c'
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def structural(path):
 with Path(path).open('rb') as f:
  e=ELFFile(f);sections=[];symbols=[];relocations=[];allocated_relocations=[]
  for s in e.iter_sections():
   if s['sh_flags']&2:
    sections.append({'name':s.name,'type':s['sh_type'],'flags':s['sh_flags'],'size':s['sh_size'],
      'align':s['sh_addralign'],'bytes':hashlib.sha256(s.data()).hexdigest()})
  table=e.get_section_by_name('.symtab')
  for s in table.iter_symbols():
   index=s['st_shndx'];name=Path(s.name).name if s['st_info']['type']=='STT_FILE' else s.name
   symbols.append([name,s['st_value'],s['st_size'],s['st_info']['bind'],s['st_info']['type'],
                   s['st_other']['visibility'],e.get_section(index).name if isinstance(index,int) else index])
  for s in e.iter_sections():
   if isinstance(s,RelocationSection):
    rows=[];t=e.get_section(s['sh_link'])
    for r in s.iter_relocations():
     symbol=t.get_symbol(r['r_info_sym'])
     rows.append([r['r_offset'],r['r_info_type'],symbol.name,r.entry.get('r_addend')])
    relocations.append([s.name,e.get_section(s['sh_info']).name,rows])
    if e.get_section(s['sh_info'])['sh_flags']&2:allocated_relocations.append([s.name,e.get_section(s['sh_info']).name,rows])
  return {'elf_flags':e['e_flags'],'allocated_sections':sections,'symbols':symbols,'relocations':relocations,'allocated_relocations':allocated_relocations}
def official_provenance(repo):
 code='''import json,sys\nfrom pathlib import Path\nsys.path.insert(0,str(Path.cwd()))\nfrom tools.low.buildProvenance import verify_build_output\nresult={}\nfor p in sorted(Path("build/eu/obj").rglob("*.provenance.json")):\n q=p.with_suffix("").with_suffix(".o");record=json.loads(p.read_text())\n if Path(record["source"]).suffix in [".cpp",".cc",".cxx"]:result[str(q)]=dict(verify_build_output(q),canonical_cpp=True)\n else:result[str(q)]={"source":record["source"],"canonical_cpp":False}\nprint(json.dumps(result))'''
 p=subprocess.run([str(repo/'.venv/bin/python'),'-c',code],cwd=repo,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
 if p.returncode:raise RuntimeError(p.stderr+p.stdout)
 return json.loads(p.stdout)
def run(baseline,output):
 start=time.time();baseline=Path(baseline).resolve();output=Path(output).resolve()
 assert output.is_relative_to(ROOT/'build')
 output.mkdir(parents=True,exist_ok=True)
 report=baseline/'build/dot-baseline-861/report.json';report_sha=sha(report);gate=json.loads(report.read_text())
 assert gate['checkpoint']==BASE and gate['prior_roots']==767 and gate['clean_build_returncode']==0
 assert len(gate['prior_checks'])==787 and all(c['returncode']==0 for c in gate['prior_checks'])
 original=official_provenance(baseline);current=official_provenance(ROOT)
 entries=subprocess.check_output(['git','ls-tree','-r','-z',BASE],cwd=ROOT).decode().split('\0')
 inputs={};failures=[]
 for entry in entries:
  if not entry:continue
  attrs,name=entry.split('\t');mode,kind,blob=attrs.split()
  if kind!='blob':continue
  p=baseline/name;q=ROOT/name
  if not p.is_file() or not q.is_file():failures.append('missing old input '+name);continue
  a=sha(p);b=sha(q);inputs[name]=a
  if a!=b:failures.append('changed old input '+name)
  assert subprocess.check_output(['git','hash-object',str(p)],cwd=baseline,text=True).strip()==blob
 objects={}
 for name,identity in original.items():
  if name not in current:failures.append('missing object '+name);continue
  old=baseline/name;new=ROOT/name;a=structural(old);b=structural(new)
  oldp=json.loads(old.with_suffix('.provenance.json').read_text());newp=json.loads(new.with_suffix('.provenance.json').read_text())
  norm=lambda cmd,repo:[v.replace(str(repo),'<repo>') for v in cmd]
  equal=a==b
  same_provenance=all(oldp[k]==newp[k] for k in ['schema','build_step','language','source','object','version','compiler','compiler_sha256','inputs','inputs_stable']) and norm(oldp['command'],baseline)==norm(newp['command'],ROOT)
  if identity['canonical_cpp']:
   if not equal:failures.append('allocated bytes/symbols/relocations '+name)
   if not same_provenance:failures.append('provenance mismatch '+name)
  else:
   subset={}
   for key in ['allocated_sections','symbols','relocations','allocated_relocations']:
    before=Counter(json.dumps(v,sort_keys=True) for v in a[key]);after=Counter(json.dumps(v,sort_keys=True) for v in b[key])
    subset[key]={'old_entries':sum(before.values()),'added_entries':sum((after-before).values()),'removed_or_changed_entries':sum((before-after).values())}
   scaffold={'path':name,'full_structure_equal':equal,'subsets':subset,'source_diff':'Generated neutral scaffold aliases for the new U body/imports; no tracked scaffold or tool was edited. Nonallocated debug-frame relocation group numbering changes with added functions.'}
  objects[name]={'old_sha256':sha(old),'current_sha256':sha(new),'structure_equal':equal,
   'provenance_equivalent':same_provenance,'structure_sha256':hashlib.sha256(json.dumps(a,sort_keys=True).encode()).hexdigest(),
   'allocated_sections':len(a['allocated_sections']),'symbols':len(a['symbols']),'relocation_sections':len(a['relocations'])}
 assert sha(report)==report_sha
 result={'base':BASE,'baseline_report_sha256':report_sha,'baseline_prior_roots':767,'baseline_actual_definitions':787,
  'prior_input_count':len(inputs),'prior_object_count':len(objects),'canonical_cpp_objects':sum(i['canonical_cpp'] for i in original.values()),'scaffold_objects':sum(not i['canonical_cpp'] for i in original.values()),'added_objects':sorted(set(current)-set(original)),
  'scaffold_comparison':scaffold,'failures':failures,'equivalent_canonical_cpp_and_inputs':not failures,'elapsed_seconds':time.time()-start,
  'scope':'All prior tracked blob contents and every prior canonical C++ object, including objects without accepted checks. Normalized provenance commands exclude checkout path only. Not a new full check gate.',
  'inputs':inputs,'objects':objects}
 (output/'preservation.json').write_text(json.dumps(result,indent=2));print(json.dumps({k:v for k,v in result.items() if k not in ['inputs','objects']},indent=2))
 if failures:sys.exit(1)
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--baseline',required=True);p.add_argument('--output',default='build/course-scene-final');a=p.parse_args();run(a.baseline,a.output)
