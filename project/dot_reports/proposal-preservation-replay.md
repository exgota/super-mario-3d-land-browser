# Reproduce combined proposal preservation

The source manifest and every observation are in [evidence.json](proposal-preservation/evidence.json). This is a notes-only branch. Begin in its checkout, with the five source branches fetched normally and the owner's ignored EU inputs, approved toolchain and project venv available. No game input is downloaded or distributed by these scripts. The immutable published commit for every required blob is in the manifest; if a blob is missing, fetch that named dot branch through the repository's authorized normal workflow.

The first script creates a new local dot worktree, verifies every source blob and the entire tested tree, and commits those inputs. It does not modify the starting checkout or main. Save and run it as a temporary script from this notes checkout:

```python
from pathlib import Path
import hashlib, json, os, subprocess, tempfile
repository = Path.cwd()
evidence = json.loads(Path('project/dot_reports/proposal-preservation/evidence.json').read_text())
checkout = Path(tempfile.mkdtemp(prefix='mario-proposal-audit-'))
def git(*args):
    return subprocess.check_output(['git', '-C', str(repository), *args])
# Required blobs must already be available from the five fetched dot branches.
for item in evidence['input_manifest']:
    raw = git('cat-file', 'blob', item['blob'])
    assert hashlib.sha1(b'blob '+str(len(raw)).encode()+b'\0'+raw).hexdigest()==item['blob']
branch = 'dot/proposal-audit-' + checkout.name.rsplit('-', 1)[-1]
subprocess.run(['git','-C',str(repository),'worktree','add','-b',branch,str(checkout),evidence['base']],check=True)
for item in evidence['input_manifest']:
    path=checkout/item['path']; path.parent.mkdir(parents=True,exist_ok=True)
    path.write_bytes(git('cat-file','blob',item['blob']))
subprocess.run(['git','-C',str(checkout),'add',*[x['path'] for x in evidence['input_manifest']]],check=True)
tree=subprocess.check_output(['git','-C',str(checkout),'write-tree']).decode().strip()
assert tree==evidence['tested_tree'],(tree,evidence['tested_tree'])
subprocess.run(['git','-C',str(checkout),'commit','-m','Reproduce pinned proposal preservation inputs'],check=True)
for relative in ['.venv','data/compilers','data/ver/eu/code.bin','data/ver/eu/exh.bin']:
    target=repository/relative; link=checkout/relative
    assert target.exists(),target
    if not link.exists():
        link.parent.mkdir(parents=True,exist_ok=True)
        link.symlink_to(target.resolve(),target_is_directory=target.is_dir())
print('PROPOSAL_CHECKOUT='+str(checkout))
```

Use its printed path for PROPOSAL_CHECKOUT. Save the next Python block as a temporary audit script, then run:

```sh
export PROPOSAL_CHECKOUT=/absolute/path/printed/by/setup
cd "$PROPOSAL_CHECKOUT"
. ./development_environment.sh
export DEVKITARM=/usr # cloud Linux binutils location; use the configured installation on macOS
python make.py eu -ca
python /absolute/path/to/saved-audit-script.py
```

The audit below is the executed driver with worktree/output paths parameterized. On another platform, adjust only the DEVKITARM path in its environment to the existing project binutils installation. It inventories all663 pinned accepted roots and the Tree proposal, tests every emitted definition through the ordinary CLI, records every result and restores the original map in a finally block. No expected byte is embedded or changed.

```python
import csv,hashlib,json,os,re,subprocess,time
from pathlib import Path
from elftools.elf.elffile import ELFFile
root=Path(os.environ['PROPOSAL_CHECKOUT'])
map_path=root/'data/ver/eu/map.csv';before=map_path.read_bytes()
rows=[{k.strip():v.strip() for k,v in r.items()} for r in csv.DictReader(before.decode().splitlines())]
accepted=[r for r in rows if r['Rank']=='O' and r['Type'].startswith('f')]
assert len(accepted)==663,len(accepted)
found={r['Symbol']:[] for r in accepted}
new='_ZN4sead8TreeNode14pushFrontChildEPS0_';found[new]=[]
for p in sorted((root/'build/eu/obj').rglob('*.o')):
 if 'split' in p.parts:continue
 with p.open('rb') as f:
  elf=ELFFile(f);syms=elf.get_section_by_name('.symtab')
  if syms:
   for s in syms.iter_symbols():
    if s.name in found and isinstance(s['st_shndx'],int) and s['st_info']['type']=='STT_FUNC':found[s.name].append(str(p.relative_to(root)))
missing={n:v for n,v in found.items() if not v}
(root/'build/proposal-preservation-inventory.json').write_text(json.dumps(found,indent=2))
print('Roots',len(found),'definitions',sum(len(v) for v in found.values()),'missing',missing,flush=True)
assert not missing,missing
jobs=[(n,p) for n,ps in found.items() for p in sorted(set(ps))]
clean=lambda s:re.sub(r'\x1b\[[0-9;]*[A-Za-z]','',s).strip()
result={'base':'3de056e0dcb33619c32f8b46ef64f74c90d28934','prior_roots':len(accepted),'prior_bytes':sum(int(r['End'],16)-int(r['Start'],16) for r in accepted),'results':[]}
t=time.monotonic()
try:
 for i,(n,p) in enumerate(jobs):
  run=subprocess.run([str(root/'.venv/bin/python'),'tools/check.py',n,'--object',p],cwd=root,env=dict(os.environ,DEVKITARM='/usr',TMP='/tmp'),capture_output=True,text=True)
  out=clean(run.stdout+run.stderr);passed=run.returncode==0 and 'complete source-generated function interval matches byte for byte' in out
  result['results'].append(dict(symbol=n,object=p,sha256=hashlib.sha256((root/p).read_bytes()).hexdigest(),exit=run.returncode,exact=passed,output=out))
  result['elapsed_seconds']=time.monotonic()-t
  (root/'build/proposal-preservation-results.json').write_text(json.dumps(result,indent=2))
  if not passed:print('FAIL',n,p,out,flush=True)
  elif (i+1)%50==0:print('Checked',i+1,'/',len(jobs),flush=True)
finally:map_path.write_bytes(before)
assert map_path.read_bytes()==before
print('Exact',sum(r['exact'] for r in result['results']),'/',len(jobs),'seconds',time.monotonic()-t,flush=True)
assert all(r['exact'] for r in result['results'])
```

## Allocated-section comparison

This independent comparison uses the individually tested reference objects. Build their pinned source branches normally in sibling directories named as in the script, or change only the directory names below. Set PROPOSAL_REFERENCE_ROOT to their parent directory and PROPOSAL_CHECKOUT to the combined worktree. The published immutable commits are the evidence manifest entries; each reference object/source hash is also frozen there. Do not use later altered source as a reference. This comparison is not an alternative canonical checker and grants no exact credit.

```python
from pathlib import Path
import hashlib,json,os
from elftools.elf.elffile import ELFFile
root=Path(os.environ['PROPOSAL_REFERENCE_ROOT'])
combined=Path(os.environ['PROPOSAL_CHECKOUT'])
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

rows=[('mario-tree-front', 'lib/al/src/Util/seadTreeNode.o', '_ZN4sead8TreeNode14pushFrontChildEPS0_'), ('mario-note-generator', 'Game/backup/src/MapObj/NoteObjGenerator.o', '_ZN16NoteObjGenerator4initERKN2al13ActorInitInfoE'), ('mario-executor-kit', 'lib/al/src/Execute/alExecuteTableHolderUpdateInit.o', '_ZN2al24ExecuteTableHolderUpdate4initEPKNS_12ExecuteOrderEi'), ('mario-executor-kit', 'lib/al/src/LiveActor/alLiveActorKit.o', '_ZN2al12LiveActorKit7endInitEv'), ('mario-collider-invalidation', 'lib/al/src/Collision/alCollider.o', '_ZN2al8Collider12onInvalidateEv'), ('mario-bomb-hei-control', 'Game/backup/src/Enemy/BombHei.o', '_ZN7BombHei7controlEv')]
results=[]
for old,rel,sym in rows:
 rel='build/eu/obj/'+rel;a=read(root/old/rel,sym);b=read(combined/rel,sym)
 r={'symbol':sym,'before_directory':old,'object':rel,'before':a,'after':b,'root_bytes_equal':a['section_sha256']==b['section_sha256'],'relocations_equal':a['relocations']==b['relocations'],'allocated_sections_equal':a['allocated']==b['allocated']};results.append(r);print(sym,b['size'],r['root_bytes_equal'],r['relocations_equal'],r['allocated_sections_equal'])
(combined/'build/proposal-preservation-sections.json').write_text(json.dumps(results,indent=2))

assert all(r['allocated_sections_equal'] for r in results)
```

The reported audit and comparisons were executed before packaging. The setup verifies the complete tested tree before building; path parameterization leaves the audit/comparison logic unchanged. The ten source blobs remain byte-for-byte those already published on the individual branches. All three Pro-held proposals are absent. Retain the individual partial reports' data-closure, model and runtime limits when interpreting preservation.
