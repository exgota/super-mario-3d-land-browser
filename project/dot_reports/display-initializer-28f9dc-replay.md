# Reproduce the display initializer proposal

Use the committed source checkpoint 168991f or a later reports-only descendant on dot/display-initializer-28f9dc. Run from its repository root. The owner's private code/exheader and the approved compiler/venv are required; they are not included in this branch. No tools or compiler configuration changes are required.

The neighboring pristine worktree `../mario-main45a` must be at 45a4305466a7c4cbd87589169384102e88f8d1b5 with its canonical build completed. The coordinator's recorded baseline used `tools/acceptance_batch.py` with `{"prior_checkpoint":"45a4305466a7c4cbd87589169384102e88f8d1b5","candidates":[]}` and a fresh output directory under build/. Its clean build and all 707 prior-root/726 actual-definition checks passed. The preservation recipe below reads that worktree without modifying it.

These commands recreate the measured clean build and canonical rejection:

```sh
mkdir -p build/display_init
. ./development_environment.sh
export DEVKITARM=/usr
python make.py eu -ca
```

Save each following block at its stated ignored path, then execute the final command block. The canonical probe names only the existing target row temporarily and restores its exact bytes, even on failure. It deliberately asserts the current rejection; success would require a new reviewed checkpoint. Diagnostic linking does not edit or replace the canonical object.

## Canonical committed-source check

Save as `build/display_init/canonical.py`.

```python
from pathlib import Path
import subprocess,hashlib,json
root=Path.cwd();out=root/'build/display_init'
source=root/'lib/CtrSDK/sources/DisplayInitializer.cpp'
header=root/'lib/CtrSDK/include/retail/DisplayInitializer.h'
assert hashlib.sha256(source.read_bytes()).hexdigest()=='974b508210515cace41dc88455bf2f2cabfe981935228c2cd19ffebbac5987b4'
assert hashlib.sha256(header.read_bytes()).hexdigest()=='2fd0f0080f0ba716b60124b60c6d250b39354aea62b86f8639d62bd5e31a847f'
assert hashlib.sha256((root/'data/ver/eu/code.bin').read_bytes()).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
p=root/'data/ver/eu/map.csv';raw=p.read_bytes()
old=b'0x0028F9DC,0x002903CC,0x002905F0,          ,U,f,,'
new=b'0x0028F9DC,0x002903CC,0x002905F0,          ,U,f,fn_0028F9DC,'
assert raw.count(old)==1
p.write_bytes(raw.replace(old,new))
try:
 r=subprocess.run(['.venv/bin/python','tools/check.py','fn_0028F9DC','--object','build/eu/obj/lib/CtrSDK/sources/DisplayInitializer.o'],capture_output=True,text=True)
 (out/'final-check.log').write_text(r.stdout+r.stderr)
 print('Canonical checker exit:',r.returncode);print(r.stdout,r.stderr)
 assert r.returncode==1
 assert 'An unresolved source helper is referenced by a non-branch relocation.' in r.stdout+r.stderr
finally:p.write_bytes(raw)
assert p.read_bytes()==raw
```

## Diagnostic import links

Save as `build/display_init/link.py`.

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
import csv,subprocess,os,json
root=Path.cwd(); out=root/'build/display_init';obj=root/'build/eu/obj/lib/CtrSDK/sources/DisplayInitializer.o'
rows=[{k.strip():v.strip() for k,v in r.items()} for r in csv.DictReader((root/'data/ver/eu/map.csv').open())]
syms={r['Symbol'] or ('fn_' if 'f' in r['Type'] else 'dat_')+r['Start'][2:].upper():int(r['Start'],16) for r in rows}
syms.update(dat_0041CFA0=0x41cfa0,fn_0028AEF0=0x28aef0)
with obj.open('rb') as f:
 e=ELFFile(f);names=[s.name for s in e.get_section_by_name('.symtab').iter_symbols() if s['st_shndx']=='SHN_UNDEF' and s.name and not s.name.startswith('Lib$$')]
(out/'imports.sym').write_text('#<SYMDEFS>#\n'+''.join('0x%08X A %s\n'%(syms[n],n) for n in names))
for label,addr in [('behavior',0x500000),('paired',0x28f9dc)]:
 cmd=[str(root/'data/compilers/wibo'),str(root/'data/compilers/4.0/902/bin/armlink.exe'),'--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--inline','--datacompressor=off','--no_debug','--no_scanlib','--mangled','--symbols','--map','--entry=fn_0028F9DC','--keep=fn_0028F9DC','--ro_base=0x%08X'%addr,'--output='+str(out/(label+'.axf')),'--list='+str(out/(label+'.map')),str(obj),str(out/'imports.sym')]
 r=subprocess.run(cmd,capture_output=True,text=True,env=dict(os.environ,TMP='/tmp'));print(label,r.returncode,r.stdout,r.stderr);assert r.returncode==0
with (out/'paired.axf').open('rb') as f:
 e=ELFFile(f);s=next(s for s in e.iter_sections() if s['sh_addr']==0x28f9dc and s['sh_flags']&2);b=s.data();ret=(root/'data/ver/eu/code.bin').read_bytes()[0x18f9dc:0x1905f0]
 result={'candidate_bytes':len(b),'target_bytes':len(ret),'positional_mismatch':sum(x!=y for x,y in zip(b,ret))+abs(len(b)-len(ret))};print(result);(out/'diagnostic-diff.json').write_text(json.dumps(result,indent=2)+'\n')
```

## Exhaustive existing-object preservation

Save as `build/display_init/preserve.py`.

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
import json,hashlib
root=Path.cwd();base=root.parent/'mario-main45a';out=root/'build/display_init'
def normalized(p):
 e=ELFFile(open(p,'rb'));sections=[];symbols=[]
 def symrec(s):
  n=s['st_shndx'];sec=e.get_section(n).name if isinstance(n,int) else n
  return [s.name,s['st_info']['type'],s['st_info']['bind'],s['st_other']['visibility'],sec,s['st_value'],s['st_size']]
 for sec in e.iter_sections():
  if isinstance(sec,RelocationSection):
   tab=e.get_section(sec['sh_link']);sections.append([sec.name,[(r['r_offset'],r['r_info_type'],symrec(tab.get_symbol(r['r_info_sym']))) for r in sec.iter_relocations()]])
  elif sec['sh_type'] not in ('SHT_SYMTAB','SHT_STRTAB','SHT_NULL'):
   sections.append([sec.name,sec['sh_type'],sec['sh_flags'],sec['sh_addralign'],sec['sh_entsize'],hashlib.sha256(sec.data().replace(str(root).encode(),b'<repo>').replace(str(base).encode(),b'<repo>') if sec.name=='.comment' else sec.data()).hexdigest()])
 tab=e.get_section_by_name('.symtab')
 for s in tab.iter_symbols():
  if s['st_info']['type']!='STT_FILE':symbols.append(symrec(s))
 return {'sections':sections,'symbols':symbols}
records=[];inputs={}
for p in sorted((root/'build/eu/obj').rglob('*.o')):
 rel=p.relative_to(root/'build/eu/obj')
 if rel.parts[0] not in ('Game','lib') or p.stem=='DisplayInitializer':continue
 q=base/'build/eu/obj'/rel;pa=json.loads(p.with_suffix('.provenance.json').read_text());pb=json.loads(q.with_suffix('.provenance.json').read_text())
 assert pa['inputs']==pb['inputs'],str(rel)+' inputs'
 for key,value in pa['inputs'].items():
  assert hashlib.sha256((root/key).read_bytes()).hexdigest()==value
  assert hashlib.sha256((base/key).read_bytes()).hexdigest()==value
  inputs[key]=value
 assert pa['compiler_sha256']==pb['compiler_sha256'],str(rel)+' compiler'
 assert [x.replace(str(root),'<repo>') for x in pa['command']]==[x.replace(str(base),'<repo>') for x in pb['command']],str(rel)+' command'
 a=normalized(p);b=normalized(q)
 if a!=b:
  (out/'preserve-failed.json').write_text(json.dumps({'object':str(rel),'current':a,'baseline':b},indent=2));raise AssertionError(str(rel)+' ELF')
 records.append({'object':str(rel),'current_object_sha256':hashlib.sha256(p.read_bytes()).hexdigest(),'base_object_sha256':hashlib.sha256(q.read_bytes()).hexdigest(),'normalized_elf_sha256':hashlib.sha256(json.dumps(a,sort_keys=True).encode()).hexdigest(),'compiler_sha256':pa['compiler_sha256']})
result={'base':'45a4305466a7c4cbd87589169384102e88f8d1b5','object_count':len(records),'input_count':len(inputs),'all_equal':True,'excluded_difference':'STT_FILE absolute source path, repository prefix in non-allocated .comment, and resulting raw symbol/string table indices; all other symbols, section data, attributes and resolved relocations compare equal','objects':records,'inputs':inputs}
(out/'preservation.json').write_text(json.dumps(result,indent=2));print(json.dumps({k:v for k,v in result.items() if k not in ('objects','inputs')},indent=2))
```

## Whole-root bounded replay

Save as `build/display_init/replay.py`.

```python
from pathlib import Path
import struct,json,hashlib,time
from unicorn import *
from unicorn.arm_const import *
from elftools.elf.elffile import ELFFile
ROOT=Path.cwd(); OUT=ROOT/'build/display_init';CODE=(ROOT/'data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(CODE).hexdigest() == 'e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
CTX=0x41cfa0;ALLOC=0x7f0000;FREE=0x7f0010;STOP=0x7f1000
R=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3]
MANAGERS={0x106b74:'state',0x106444:'fb',0x107168:'vb',0x1064e4:'shader',0x106fb4:'texture',0x1071d8:'validator'}
DIRECT={0x107a24:'lowinit',0x107b10:'first',0x107c0c:'speculative',0x107b24:'callback',0x28ca14:'write',0x107adc:'masked',0x28c890:'split',0x28e240:'physical',0x28a814:'lock',0x28c640:'submit',0x28a7b4:'unlock',0x107acc:'yield'}
ENDPOINTS=set(MANAGERS)|set(DIRECT)|{ALLOC,FREE,0x28ca18,0x28f6f0,0x28c500}
SEG=[]
with (OUT/'behavior.axf').open('rb') as f:
 e=ELFFile(f)
 for s in e.iter_sections():
  if s['sh_flags']&2:SEG.append((s['sh_addr'],s.data()))

def run(candidate,case,fpscr):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176)
 u.mem_map(0x100000,0x400000);u.mem_write(0x100000,CODE)
 u.mem_map(0x500000,0x10000)
 for a,b in SEG:u.mem_write(a,b)
 u.mem_map(0x700000,0x100000);u.mem_map(0x800000,0x400000)
 u.mem_write(0x800000,b'\xA6'*0x400000)
 def read(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def write(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def byte(a,v):u.mem_write(a,bytes([v]))
 # All synthetic state and allocations are deterministic for the paired runs.
 u.mem_write(CTX,bytes([0xC3])*0x180);byte(CTX+0x10,case.get('initialized',0))
 write(0x3e2e38,case.get('old_error',0))
 for i,r in enumerate([UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11]):u.reg_write(r,0xa4000000+i)
 u.reg_write(UC_ARM_REG_SP,0x7e0000);u.reg_write(UC_ARM_REG_LR,STOP)
 u.reg_write(UC_ARM_REG_R0,0 if case.get('null_allocate') else ALLOC);u.reg_write(UC_ARM_REG_R1,0 if case.get('null_release') else FREE)
 u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000)
 u.reg_write(UC_ARM_REG_FPSCR,fpscr)
 events=[]; allocations=[];coverage=set();counts={}; nalloc=0;yield_count=0;invalid=[];steps=0
 def finish(value=0):
  for r in R+[UC_ARM_REG_R12]:u.reg_write(r,0xa5a5a5a5)
  u.reg_write(UC_ARM_REG_R0,value&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def hook(u,a,z,d):
  nonlocal nalloc,yield_count,steps
  steps+=1
  if (0x28f9dc<=a<0x2905f0) or (0x500000<=a<0x510000):coverage.add(a)
  if a not in ENDPOINTS: return
  args=[u.reg_read(r) for r in R]
  if a==ALLOC:
   nalloc+=1
   memory=0x800000+(nalloc-1)*0x20000+(case.get('alignment',0) if args[3]==0x8010 else 0)
   if nalloc==case.get('fail_alloc'):memory=0
   if nalloc==1 and case.get('preexisting_buffer'):
    # A callback side-effect fixture installs a valid pre-existing list after
    # the root clears Context. Original bind/search/delete still execute.
    prior=0xb00000; oldbuffer=0xb20000
    u.mem_write(prior,bytes(0x3c))
    for off,value in [(0,1),(4,oldbuffer),(8,0x10000),(12,0x20),(24,oldbuffer+0x10000),(28,16),(32,2),(44,0x300)]: write(prior+off,value)
    write(CTX+0x20,prior)
   events.append(['alloc',args,memory]);allocations.append([memory,args[3]])
   finish(memory);return
  if a==FREE:events.append(['free',args]);finish();return
  if a in MANAGERS:
   events.append([MANAGERS[a],args[0]])
   if case.get('fail_manager')==MANAGERS[a]:finish(0x80000001);return
   if case.get('model_managers',False):
    # Opaque but deterministic endpoint model; original managers are the default.
    write(args[0],a);finish();return
  if a==0x28ca18:
   count=counts.get('error',0);counts['error']=count+1
   if count==1:write(0x3e2e38,case.get('error',0))
   events.append(['error',read(0x3e2e38)])
  if a==0x28f6f0:events.append(['bind',args[0]])
  if a==0x28c500:events.append(['delete',args[0],[read(args[1]+i*4) for i in range(args[0])]])
  if a in DIRECT:
   name=DIRECT[a]
   if name=='first':events.append([name]);finish(case.get('first',1));return
   if name=='speculative':events.append([name]);finish(case.get('speculative',7));return
   if name=='callback':events.append([name,args[:2]])
   elif name=='write':events.append([name,args[0],read(args[1]),args[2]])
   elif name=='masked':events.append([name,args[0],read(args[1]),read(args[2]),args[3]])
   elif name=='physical':events.append([name,args[0]]);finish(0x1f5fffff);return
   elif name=='split':
    events.append([name]);p=read(CTX+0x9c)
    if p:
     write(p+0x20,case.get('record_count',0));write(p+0x28,case.get('processed',0));write(p+0x2c,case.get('mode',0x300))
     # Existing fields/records let bounded tests vary packed bits and count.
    if case.get('prebusy'):byte(CTX+0x11,1)
    if case.get('clear_allocator'):write(0x3e2654,0)
    if case.get('clear_release'):write(0x3e2658,0)
   elif name=='submit':
    events.append([name]);
    if case.get('delay',0)==0:byte(CTX+0x11,0)
   elif name=='yield':
    yield_count+=1;events.append([name]);
    if not case.get('nontermination') and yield_count>=case.get('delay',1):byte(CTX+0x11,0)
    if yield_count==10 and case.get('nontermination'):u.emu_stop();return
   elif name=='lock':
    events.append([name]);counts['lock']=counts.get('lock',0)+1
    if case.get('busy_on_final_lock') and counts['lock']==2:
     byte(CTX+0x11,1);p=read(CTX+0xa0);write(p+0x24,1)
   else:events.append([name])
   finish();return
 def mem_invalid(u,access,address,size,value,data):invalid.append([access,address,size]);return False
 u.hook_add(UC_HOOK_CODE,hook);u.hook_add(UC_HOOK_MEM_INVALID,mem_invalid)
 fault=None
 try:u.emu_start(0x500000 if candidate else 0x28f9dc,STOP,count=300000)
 except UcError as e:fault=str(e)
 domains=[bytes(u.mem_read(a,n)) for a,n in [(0x3e0000,0x30000),(0x410000,0x20000),(0x800000,0x400000)]]
 result={'return':u.reg_read(UC_ARM_REG_R0),'terminated':u.reg_read(UC_ARM_REG_PC)==STOP,'fault':fault,'invalid':invalid,'events':events,'memory':[hashlib.sha256(x).hexdigest() for x in domains], 'saved':[u.reg_read(r) for r in [UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_SP]],'fpscr':u.reg_read(UC_ARM_REG_FPSCR), 'pc':u.reg_read(UC_ARM_REG_PC)}
 return result,coverage,domains,steps

def suite():
 cases=[{}, {'preexisting_buffer':1}, {'null_allocate':1},{'null_release':1},{'null_allocate':1,'null_release':1},{'initialized':1},{'initialized':255}]
 cases += [{'fail_alloc':n} for n in range(1,11)]
 cases += [{'fail_manager':x} for x in MANAGERS.values()]
 cases += [{'error':e} for e in [1,0x502,0xffffffff]]
 cases += [{'first':0},{'first':2},{'old_error':0x502},{'speculative':0xffffffff}]
 cases += [{'alignment':n} for n in range(16)]
 cases += [{'delay':n} for n in [1,2,5]]
 cases += [{'record_count':n,'processed':p} for n in [0,1,14] for p in [0,15,0x7fffffff]]
 cases += [{'first':0,'record_count':n,'processed':p} for n in [0,1,14] for p in [0,15]]
 cases += [{'mode':0,'delay':2},{'prebusy':1,'delay':2},{'clear_allocator':1},{'clear_release':1},{'busy_on_final_lock':1},{'nontermination':1,'delay':1}]
 return cases
if __name__=='__main__':
 start=time.time();results=[];coverage=set();differences=[]
 for mode in [0,0x400000,0x800000,0xc00000,0x1000000]:
  for index,case in enumerate(suite()):
   a,ca,ma,sa=run(False,case,mode);b,cb,mb,sb=run(True,case,mode);coverage|=ca
   # Fault addresses/events/state remain comparable; caller-saved registers at faults
   # are not specified, and instruction positions differ for nontermination.
   comparable=['return','terminated','fault','invalid','events','memory','saved','fpscr'] if a['terminated'] else ['terminated','fault','invalid','events','memory','fpscr']
   diff=[k for k in comparable if a[k]!=b[k]]
   row={'case':case,'fpscr':mode,'equal':not diff,'diff':diff,'retail_steps':sa,'candidate_steps':sb,'returned':a['terminated'],'fault':a['fault']}
   results.append(row)
   if diff:
    path=OUT/('failure-%d-%d.json'%(mode,index));path.write_text(json.dumps({'case':case,'retail':a,'candidate':b},indent=2)+'\n');differences.append(str(path))
    if 'memory' in diff:
     print('memory first offsets',[[i for i,(x,y) in enumerate(zip(aa,bb)) if x!=y][:20] for aa,bb in zip(ma,mb)])
    print('DIFF',mode,index,case,diff,path)
    if len(differences)>=3:break
  if len(differences)>=3:break
 result={'pairs':len(results),'agree':sum(r['equal'] for r in results),'returning':sum(r['returned'] for r in results),'seconds':time.time()-start,'results':results,'failures':differences,'root_coverage':len(coverage),'coverage_addresses':[hex(a) for a in sorted(coverage)]}
 (OUT/'verified-replay-result.json').write_text(json.dumps(result,indent=2)+'\n');print({k:v for k,v in result.items() if k not in ['results','coverage_addresses']})

 if result['agree'] != result['pairs']: raise SystemExit(1)
```

```sh
.venv/bin/python build/display_init/canonical.py
.venv/bin/python build/display_init/link.py
.venv/bin/python build/display_init/preserve.py
.venv/bin/python build/display_init/replay.py
git diff 45a4305466a7c4cbd87589169384102e88f8d1b5 HEAD -- lib/CtrSDK/include/retail/DisplayInitializer.h lib/CtrSDK/sources/DisplayInitializer.cpp > build/display_init/family.patch
git -C ../mario-main45a apply --check ../mario-display-init/build/display_init/family.patch
```

The final command assumes this proposal directory is `mario-display-init`; adjust only the patch path when reproducing elsewhere. The replay report contains each scenario, FPSCR state, termination/fault classification, instruction counts and the union of root addresses executed. Memory/callback/scheduler domain limits and the separate extended allocator fixture are explained in the main report.
