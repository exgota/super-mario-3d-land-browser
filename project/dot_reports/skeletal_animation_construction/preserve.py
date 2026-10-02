from pathlib import Path
from elftools.elf.elffile import ELFFile
import hashlib,json,sys
BASE=Path(sys.argv[1] if len(sys.argv)>1 else '/workspace/scratch/73cdb2c524af/mario-main861').resolve();HERE=Path('.').resolve();D=HERE/'build/skeletal_animation_validation'
EXPECTED='86104d96a7f570383bbfefc5fffad998e134496c'
import subprocess
assert subprocess.check_output(['git','rev-parse','HEAD'],cwd=BASE,text=True).strip()==EXPECTED
baseline=BASE/'build/dot-baseline-861/report.json'
assert hashlib.sha256(baseline.read_bytes()).hexdigest()=='a692f9a29d92c3f8021b62a2254b06fb44776f34cfa2f51849364bde66dee374'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def structural(p):
 with p.open('rb') as f:
  e=ELFFile(f);sections=list(e.iter_sections());alloc=[];symbols=[];relocs=[]
  for i,s in enumerate(sections):
   if s['sh_flags']&2:alloc.append([s.name,s['sh_type'],s['sh_flags'],s['sh_size'],s['sh_addralign'],hashlib.sha256(s.data()).hexdigest()])
   if s['sh_type'] in ('SHT_REL','SHT_RELA'):
    symtab=sections[s['sh_link']]
    relocs.append([s.name,sections[s['sh_info']].name,[[r['r_offset'],r['r_info_type'],symtab.get_symbol(r['r_info_sym']).name,r.entry.get('r_addend')] for r in s.iter_relocations()]])
  for s in e.get_section_by_name('.symtab').iter_symbols():
   idx=s['st_shndx'];section=sections[idx].name if isinstance(idx,int) else idx
   name=s.name.replace(str(BASE),'<ROOT>').replace(str(HERE),'<ROOT>')
   symbols.append([name,s['st_value'],s['st_size'],s['st_info']['bind'],s['st_info']['type'],s['st_other']['visibility'],section])
  return dict(allocated=alloc,symbols=symbols,relocations=relocs)
results=[];inputs={};objects=sorted((BASE/'build/eu/obj').rglob('*.o'))
for old in objects:
 rel=old.relative_to(BASE);new=HERE/rel;assert new.is_file(),str(rel)
 a,b=structural(old),structural(new)
 scaffold=str(rel)=='build/eu/obj/build/eu/split/stubs.o'
 if scaffold:
  ad={x[0]:x for x in a['allocated']};bd={x[0]:x for x in b['allocated']}
  assert len(ad)==len(a['allocated']) and len(bd)==len(b['allocated'])
  assert all(bd.get(k)==v for k,v in ad.items())
  added_scaffolds=sorted(set(bd)-set(ad))
  expected=['i.'+n for n in ['fn_001C440C','fn_0024E47C','fn_001D956C','fn_0024E598','fn_0024E568','fn_0024E49C','fn_00293088','fn_0026AC60','fn_002A63D8']]+['.constdata_'+n for n in ['dat_003D62A0','dat_003D8008','dat_003D82A4','dat_003D8500']]+['.sdata_dat_003E23B8']
  assert added_scaffolds==sorted(expected),(added_scaffolds,sorted(expected))
  def syms(x):return sorted(v for v in x['symbols'] if v[6] in ad or v[6] in ('SHN_UNDEF','SHN_ABS'))
  assert syms(a)==syms(b),'prior scaffold symbols'
  def relocs(x):return sorted(v for v in x['relocations'] if v[1] in ad)
  assert relocs(a)==relocs(b),'prior scaffold allocated relocations'
 else:assert a==b,('object structural mismatch',str(rel))
 op=old.with_suffix('.provenance.json');np=new.with_suffix('.provenance.json')
 assert op.is_file() and np.is_file(),str(rel)
 pa=json.loads(op.read_text());pb=json.loads(np.read_text())
 for p,root in [(pa,BASE),(pb,HERE)]:
  assert p['object_sha256']==sha(root/rel)
  assert p['inputs_stable']
  assert sha(root/p['compiler'])==p['compiler_sha256']
  for path,h in p['inputs'].items():assert sha(root/path)==h,(str(rel),path)
 if scaffold:
  assert pa['inputs'].keys()==pb['inputs'].keys()
  for key in pa['inputs']:
   if key!='build/eu/split/stubs.c':assert pa['inputs'][key]==pb['inputs'][key]
  pa['inputs']['build/eu/split/stubs.c']='<generated scaffold, prior extents preserved>'
  pb['inputs']['build/eu/split/stubs.c']='<generated scaffold, prior extents preserved>'
 else:assert pa['inputs']==pb['inputs'],str(rel)
 for p in [pa,pb]:p['command']=[x.replace(str(BASE),'<ROOT>').replace(str(HERE),'<ROOT>') for x in p['command']];p.pop('object_sha256')
 assert pa==pb,('provenance',str(rel))
 for path,h in pa['inputs'].items():inputs[path]=h
 results.append({'object':str(rel),'baseline_sha256':sha(old),'candidate_sha256':sha(new),'structural_sha256':hashlib.sha256(json.dumps(a,sort_keys=True).encode()).hexdigest(),'allocated_sections':len(a['allocated']),'defined_functions':sum(s[4]=='STT_FUNC' and s[6]!='SHN_UNDEF' for s in a['symbols'])})
newobjects=sorted(str(p.relative_to(HERE))for p in (HERE/'build/eu/obj').rglob('*.o') if not (BASE/p.relative_to(HERE)).exists())
assert newobjects==['build/eu/obj/lib/al/src/Model/alSkeletalAnimationConstruction.o'],newobjects
# Preserve every tracked pre-existing source/header and configured tool, including
# files absent from dependency manifests and objects with no checked definition.
tracked=subprocess.check_output(['git','ls-files','-z'],cwd=BASE).split(b'\0');tracked_hashes={}
for raw in tracked:
 if not raw:continue
 path=raw.decode();old=BASE/path;new=HERE/path
 if old.is_file():assert old.read_bytes()==new.read_bytes(),path;tracked_hashes[path]=sha(old)
report={'kind':'equivalence-backed preservation, not a new 787-definition checker gate','base':EXPECTED,'baseline_report_sha256':sha(baseline),'old_object_count':len(results),'objects':results,'dependency_inputs':inputs,'all_preexisting_tracked_files':tracked_hashes,'new_objects':newobjects,'additional_generated_scaffold_extents':added_scaffolds}
(D/'preservation.json').write_text(json.dumps(report,indent=2));print(json.dumps({k:report[k]for k in ['kind','old_object_count','new_objects']}));print('verified inputs',len(inputs),'tracked files',len(tracked_hashes))
