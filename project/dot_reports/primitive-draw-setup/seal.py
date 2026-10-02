from pathlib import Path
import subprocess,json,hashlib,datetime
from collections import Counter
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
import os
root=Path.cwd();base=Path(os.environ.get('PRIMITIVE_BASELINE', str(root.parent/'mario-main861'))).resolve();out=root/'build/primitive-draw-setup';BASE='86104d96a7f570383bbfefc5fffad998e134496c'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
old={};gitlinks=[]
for line in subprocess.check_output(['git','ls-tree','-r',BASE],text=True).splitlines():
 meta,name=line.split('\t');mode,kind,oid=meta.split()
 if kind=='commit':gitlinks.append({'path':name,'commit':oid});continue
 b=subprocess.check_output(['git','cat-file','blob',oid]);assert (root/name).read_bytes()==b,name;assert (base/name).read_bytes()==b,name
 old[name]=hashlib.sha256(b).hexdigest()
paths=['lib/al/src/Graphics/retail_PrimitiveDrawSetup.cpp','lib/al/include/Observed/PrimitiveDrawSetup.h','tools/check.py','tools/low/checkExactBytes.py','tools/low/buildProvenance.py','data/config.json','data/ver/eu/config.json','data/ver/eu/map.csv','data/ver/eu/code.bin','data/ver/eu/exh.bin','data/compilers/wibo']
for version in ['4.1/791','4.0/902']:
 for name in ['armcc','armlink','armar','fromelf']:paths.append('data/compilers/'+version+'/bin/'+name+'.exe')
files={p:sha(root/p) for p in paths if (root/p).is_file()}
# Stub aliases are generated scaffolding. Compare every original allocated section and definition.
p=root/'build/eu/obj/build/eu/split/stubs.o';q=base/'build/eu/obj/build/eu/split/stubs.o'
if not p.exists():
 p=next((root/'build/eu/obj').rglob('stubs.o'));q=base/p.relative_to(root)
a=ELFFile(p.open('rb'));b=ELFFile(q.open('rb')); sa={s.name:s for s in a.iter_sections() if s['sh_flags']&2};sb={s.name:s for s in b.iter_sections() if s['sh_flags']&2};extra=sorted(sa.keys()-sb.keys())
for n,s in sb.items():
 t=sa[n];assert s.data()==t.data(),n
 for f in ['sh_type','sh_flags','sh_addralign','sh_entsize','sh_size']:assert s[f]==t[f],(n,f)
def sym(e,s):
 idx=s['st_shndx'];return [s.name,e.get_section(idx).name if isinstance(idx,int) else idx,s['st_value'],s['st_size'],str(s['st_info']),str(s['st_other'])]
# Mapping symbols such as $a repeat. Compare complete identities with multiplicity.
def symbol_counter(e):
 return Counter(json.dumps(sym(e,s),sort_keys=True) for s in e.get_section_by_name('.symtab').iter_symbols() if s.name and s['st_info']['type']!='STT_FILE')
aa,bb=symbol_counter(a),symbol_counter(b)
missing=bb-aa
assert not missing,dict(missing)
# Normalized old relocation identities must also survive.
def rels(e):
 records=[]
 for section in e.iter_sections():
  if not isinstance(section,RelocationSection):continue
  tab=e.get_section(section['sh_link']);target=e.get_section(section['sh_info']);relocations=[]
  for x in section.iter_relocations():
   symbol=tab.get_symbol(x['r_info_sym']);record=sym(e,symbol)
   if symbol.name.startswith('__ARM_grp_.debug_frame$'):
    # Numeric debug-group names shift when newly generated placeholders are inserted.
    # Require an actual self-reference to this exact debug frame before normalizing.
    assert symbol['st_shndx']==section['sh_info'] and target.name=='.debug_frame'
    record[0]='<self.debug_frame>'
   relocations.append([x['r_offset'],x['r_info_type'],record])
  records.append([section.name,target.name,hashlib.sha256(target.data()).hexdigest(),relocations])
 return Counter(json.dumps(r,sort_keys=True) for r in records)
ra,rb=rels(a),rels(b)
missing_relocations=rb-ra
assert not missing_relocations,{'missing_record_count':sum(missing_relocations.values()),'first_missing':next(iter(missing_relocations),None)}
stub={'old_allocated_sections':len(sb),'new_allocated_sections':len(sa),'all_old_allocated_sections_symbols_relocations_equal':True,'added_sections':[{'name':n,'size':sa[n]['sh_size'],'sha256':hashlib.sha256(sa[n].data()).hexdigest()} for n in extra],'old_relocation_sections':sum(rb.values()),'new_relocation_sections':sum(ra.values()),'old_symbol_records':sum(bb.values()),'new_symbol_records':sum(aa.values()),'symbol_comparison':'complete name/section/value/size/type/visibility tuples including multiplicity','old_object_sha256':sha(q),'new_object_sha256':sha(p),'debug_normalization':'Only numeric __ARM_grp_.debug_frame$ names are normalized after proving they refer to that exact relocation target section; debug frame bytes and code relocation identities are compared with multiplicity.','credit':'Generated placeholder sections are not reconstructed source and earn no progress.'}
basereport=base/'build/dot-baseline-861/report.json';assert sha(basereport)=='a692f9a29d92c3f8021b62a2254b06fb44776f34cfa2f51849364bde66dee374'
result={'base':BASE,'source_commit':subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip(),'sealed_at':datetime.datetime.now(datetime.timezone.utc).isoformat(),'old_tracked_file_count':len(old),'old_tracked_files':old,'gitlinks':gitlinks,'files':files,'baseline_report_sha256':sha(basereport),'stub_preservation':stub}
(out/'input-seal.json').write_text(json.dumps(result,indent=2));print({'old_files':len(old),'new_stubs':len(extra),'tools_and_inputs':len(files)})
