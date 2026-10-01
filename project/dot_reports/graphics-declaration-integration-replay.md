# Reproduce graphics declaration compatibility

Build this branch through the unchanged project command after supplying your own verified EU binary and approved toolchain. No binary data is embedded here. Use ARMCC4.0/902 through the normal make.py module configuration.

For comparisons, build the seven reference checkouts shown below with the same project command. Place them under GRAPHICS_REFERENCE_ROOT with these directory names. Do not replace a reference with a moving branch tip without checking hashes.

- `mario-packed-state`: `750133af7787834367280178a46b09ffd44a9815`
- `mario-integer-state`: `8776aac41b1fc013dbc4ee6c5bd089e2e156a8b3`
- `mario-shader-full`: `6b525d223cf0d8ad04ede74d410fbb3f6667e771`
- `mario-shader-partial`: `7ca30156429b2e6dced907a3a0d81e24e73eda05`
- `mario-float-state-root`: `6acfd5b865f00db4cd3931b090bd8130593fecb6`
- `mario-program-state`: `7d3d944738ad71f117278645c535571839e2ec26`
- `mario-root-3910c0`: `38f3bd0e926e9e084043505202d5d9250d643040`

The reference branches deliberately retain their original differing external declarations. They build independently. The comparison below checks the actual allocated machine output, not compiler debug/file metadata. Run from the integration repository with the project virtualenv active. The scripts write only temporary diagnostic files.

```python
from pathlib import Path
import hashlib,json,os
from elftools.elf.elffile import ELFFile
root=Path(os.environ.get('GRAPHICS_REFERENCE_ROOT','..')).resolve(); integrated_root=Path.cwd()
rows=[('mario-packed-state','FloatState','fn_0020ACAC'),('mario-integer-state','IntegerState','fn_00206474'),('mario-shader-full','shv_FullValidator','__shv_validateShaderValidator'),('mario-shader-partial','shv_PartialValidator','__shv_partialValidateShaderValidator'),('mario-float-state-root','ProgramLink','fn_00245D50'),('mario-program-state','ShaderBinary','fn_002478D8'),('mario-root-3910c0','TextureStateRoot','fn_003910C0')]
def read(p,symbol):
 with p.open('rb') as f:
  e=ELFFile(f);tab=e.get_section_by_name('.symtab');s=tab.get_symbol_by_name(symbol)[0];idx=s['st_shndx'];sec=e.get_section(idx);rel=[]
  for x in e.iter_sections():
   if x['sh_type'] in ('SHT_REL','SHT_RELA') and x['sh_info']==idx:
    st=e.get_section(x['sh_link'])
    for r in x.iter_relocations():rel.append((r['r_offset'],r['r_info_type'],st.get_symbol(r['r_info_sym']).name,r.entry.get('r_addend')))
  allocated=[]
  for si,x in enumerate(e.iter_sections()):
   if not (x['sh_flags']&2 and x['sh_size']):continue
   xr=[]
   for rr in e.iter_sections():
    if rr['sh_type'] in ('SHT_REL','SHT_RELA') and rr['sh_info']==si:
     st=e.get_section(rr['sh_link'])
     for r in rr.iter_relocations():xr.append((r['r_offset'],r['r_info_type'],st.get_symbol(r['r_info_sym']).name,r.entry.get('r_addend')))
   definitions=[(z.name,z['st_value'],z['st_size'],z['st_info']['type'],z['st_info']['bind']) for z in tab.iter_symbols() if z['st_shndx']==si]
   linked_section=e.get_section(x['sh_link']).name if x['sh_link'] else None
   allocated.append((x.name,x['sh_type'],x['sh_flags'],x['sh_addralign'],x['sh_size'],hashlib.sha256(x.data()).hexdigest(),xr,definitions,linked_section))
  return {'object_sha256':hashlib.sha256(p.read_bytes()).hexdigest(),'section':sec.name,'size':sec['sh_size'],'section_sha256':hashlib.sha256(sec.data()).hexdigest(),'relocations':rel,'allocated':allocated}
results=[]
for old,name,sym in rows:
 rel='build/eu/obj/lib/CtrSDK/sources/'+name+'.o';a=read(root/old/rel,sym);b=read(integrated_root/rel,sym)
 r={'symbol':sym,'before_directory':old,'object':rel,'before':a,'after':b,'root_bytes_equal':a['section_sha256']==b['section_sha256'],'relocations_equal':a['relocations']==b['relocations'],'allocated_sections_equal':a['allocated']==b['allocated']};results.append(r);print(sym,b['size'],r['root_bytes_equal'],r['relocations_equal'],r['allocated_sections_equal'])
Path('/tmp/mario-graphics-declaration-comparison.json').write_text(json.dumps(results,indent=2)+'\n')

```

For the header negative/positive check, first write the following source-path inventory to `/tmp/mario-graphics-source-inputs.json`. Each reference checkout must remain pinned to its specified source snapshot; the diagnostic reads its committed HEAD blobs.

```json
[
  [
    "mario-packed-state",
    "b51598a",
    [
      "lib/CtrSDK/sources/FloatState.cpp"
    ]
  ],
  [
    "mario-integer-state",
    "3c89e91",
    [
      "lib/CtrSDK/include/retail/FloatState.h",
      "lib/CtrSDK/include/retail/IntegerState.h",
      "lib/CtrSDK/sources/IntegerState.cpp"
    ]
  ],
  [
    "mario-shader-full",
    "373105d",
    [
      "lib/CtrSDK/include/retail/shv_ValidatorFront.h",
      "lib/CtrSDK/include/shv_ValidatorAccess.h",
      "lib/CtrSDK/include/shv_ValidatorTail.h",
      "lib/CtrSDK/sources/shv_FullValidator.cpp"
    ]
  ],
  [
    "mario-shader-partial",
    "61c9927",
    [
      "lib/CtrSDK/include/retail/shv_PartialValidatorTail.h",
      "lib/CtrSDK/sources/shv_PartialValidator.cpp"
    ]
  ],
  [
    "mario-float-state-root",
    "1c6f030",
    [
      "lib/CtrSDK/include/retail/ProgramLink.h",
      "lib/CtrSDK/sources/ProgramLink.cpp"
    ]
  ],
  [
    "mario-program-state",
    "b7c3900",
    [
      "lib/CtrSDK/include/retail/ShaderBinary.h",
      "lib/CtrSDK/sources/ShaderBinary.cpp"
    ]
  ],
  [
    "mario-root-3910c0",
    "38f3bd0e926e9e084043505202d5d9250d643040",
    [
      "lib/CtrSDK/include/retail/TextureStateRoot.h",
      "lib/CtrSDK/sources/TextureStateRoot.cpp"
    ]
  ]
]
```

Then run this compiler diagnostic. It uses the normal SDK object provenance command, supplies the same RVCT40 include/library environment as project setup, and changes only the diagnostic source/output paths and the before-case header overlay. It does not modify project tools or canonical objects.

```python
from pathlib import Path
import json,subprocess,os
root=Path.cwd(); reference_root=Path(os.environ.get('GRAPHICS_REFERENCE_ROOT','..')).resolve()
source=Path('/tmp/mario-graphics-headers.cpp');source.write_text('''#include <retail/FloatState.h>
#include <retail/IntegerState.h>
#include <retail/TextureStateRoot.h>
#include <retail/ProgramLink.h>
#include <retail/ShaderBinary.h>
#include <shv_ValidatorAccess.h>
#include <retail/shv_ValidatorFront.h>
#include <shv_ValidatorTail.h>
#include <retail/shv_PartialValidatorTail.h>
''')
oldroot=Path('/tmp/mario-graphics-before-headers')
for directory,rev,files in json.loads(Path('/tmp/mario-graphics-source-inputs.json').read_text()):
 for path in files:
  if '/include/' not in path:continue
  p=oldroot/path.split('/include/',1)[1];p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(subprocess.check_output(['git','-C',str(reference_root/directory),'show','HEAD:'+path]))
cmd=json.loads((root/'build/eu/obj/lib/CtrSDK/sources/FloatState.provenance.json').read_text())['command'];cmd[-1]=str(source)
results=[]
for label,before in [('before',True),('after',False)]:
 c=cmd[:];c[c.index('-o')+1]='/tmp/mario-graphics-headers-'+label+'.o';c[c.index('--depend')+1]='/tmp/mario-graphics-headers-'+label+'.d'
 if before:c.insert(2,'-I'+str(oldroot))
 r=subprocess.run(c,cwd=root,env=dict(os.environ,TMP='/tmp',RVCT40INC=str(root/'data/compilers/4.0/902/include'),RVCT40LIB=str(root/'data/compilers/4.0/902/lib')),text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
 Path('/tmp/mario-graphics-headers-'+label+'.log').write_text(r.stdout);results.append({'case':label,'exit':r.returncode,'command':c,'output':r.stdout});print(label,r.returncode,r.stdout[:700])
Path('/tmp/mario-graphics-header-check.json').write_text(json.dumps(results,indent=2))
assert results[0]['exit']!=0 and results[1]['exit']==0

```

Expected outcome: before case fails with incompatible declarations for the shared globals; after case returns zero. Earlier harness invocation mistakes (missing RVCT40INC, or resolving a venv Python symlink to its system interpreter) were corrected; they were not source failures or function attempts.
