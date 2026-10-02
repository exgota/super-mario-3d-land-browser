"""Describe candidate extents/imports and nominal type compatibility; no map edits."""
from pathlib import Path
from elftools.elf.elffile import ELFFile
import hashlib,json,re,sys
D=Path('build/transform_state_validation');D.mkdir(parents=True,exist_ok=True)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
obj=Path('build/eu/obj/lib/al/src/Model/alTransformStateFactory.o')
with obj.open('rb') as f:
 e=ELFFile(f);sections=list(e.iter_sections());symbols=e.get_section_by_name('.symtab')
 allocated=[{'section':s.name,'size':s['sh_size'],'flags':s['sh_flags'],'sha256':hashlib.sha256(s.data()).hexdigest()}for s in sections if s['sh_flags']&2]
 relocs=[]
 for s in sections:
  if s['sh_type'] not in ['SHT_REL','SHT_RELA']:continue
  t=sections[s['sh_link']]
  for r in s.iter_relocations():relocs.append({'section':sections[s['sh_info']].name,'offset':r['r_offset'],'type':r['r_info_type'],'symbol':t.get_symbol(r['r_info_sym']).name})
 unresolved=sorted({s.name for s in symbols.iter_symbols()if s['st_shndx']=='SHN_UNDEF' and s.name and not s.name.startswith('Lib$$')})
 functions=[{'name':s.name,'size':s['st_size'],'section':sections[s['st_shndx']].name}for s in symbols.iter_symbols()if s['st_info']['type']=='STT_FUNC' and isinstance(s['st_shndx'],int)]
report={'original_start':'002A2C9C','original_end':'002A34D4','original_complete_bytes':2104,'original_instruction_bytes':2060,'original_instructions':515,'original_pools':[['002A3170','002A3198',40],['002A34D0','002A34D4',4]],'object_sha256':sha(obj),'allocated':allocated,'functions':functions,'undefined_imports':unresolved,'relocations':relocs,'canonical_result':'Source closure rejected: An unresolved source helper is referenced by a non-branch relocation.','canonical_extent_grade':'not reached','missing_canonical_data_identities':['dat_00430C68','dat_00430CF0','dat_00430D20'],'subview':'00430D20 is the three-float scale view at00430CF0+30, not an independent allocation claim'}
(D/'object-audit.json').write_text(json.dumps(report,indent=2))
print(json.dumps({k:report[k]for k in ['original_complete_bytes','original_instructions','allocated','functions','canonical_result']}))
# Optional proposal-local compatibility inspection. It does not assemble proposals.
if len(sys.argv)>2:
 mine=Path('lib/al/include/Observed/TransformStateFactory.h')
 prior=Path(sys.argv[1])/'lib/CtrSDK/include/clean/TransformBlendRoot.h'
 skel=Path(sys.argv[2])/'lib/al/include/Observed/SkeletalAnimationConstruction.h'
 def tokens(path,name):
  s=path.read_text();found=re.findall(r'\bstruct\s+'+name+r'\s*\{.*?\};',s,re.S)
  assert len(found)==1,(path,name)
  return re.findall(r'[A-Za-z_]\w*|0x[0-9a-f]+|\d+|[^\s]',found[0])
 checks=[]
 for other,names in [(prior,['Matrix34','Transform']),(skel,['AllocatorVtable','Allocator'])]:
  for name in names:
   assert tokens(mine,name)==tokens(other,name),(name,'ODR token mismatch')
   checks.append({'type':name,'candidate':str(mine),'candidate_sha256':sha(mine),'reference':str(other),'reference_sha256':sha(other),'definition_tokens_identical':True})
 abi={'type_checks':checks,'shared_C_declarations':{'fn_0028A998':'int(unsigned*)','fn_00291470':'void(transform_blend::Matrix34*,const transform_blend::Matrix34*)','dat_003F389C':'unsigned','dat_00430C68':'transform_blend::Matrix34'},'resolution':'transform_state uses declarations import the same transform_blend nominal types; copied definitions are token-identical, not merely layout-compatible','integration_scope':'No combined source build. Historical inactive Effect packet declares fn291470 using references to another nominal type, and must not be revived unchanged together with this family.'}
 (D/'abi-audit.json').write_text(json.dumps(abi,indent=2));print('Nominal type definitions checked:',len(checks))
