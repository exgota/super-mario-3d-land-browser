# Resource factory 00298C58: frozen NonMatching family

Branch: `dot/resource-construction-298c58`. Frozen base: `45a4305466a7c4cbd87589169384102e88f8d1b5`. Only a new source translation unit and these notes are submitted. No shared header, existing source, configuration, tool, map, ledger or STATE changes. Remote publication belongs to the coordinator.

The complete 2,928-byte retail root is reconstructed as ordinary C++ with original callee imports. It is **not exact**: the final canonical ARMCC902 section is 2,984 bytes, 56 bytes larger. The original interval and eight literal-pool bytes remain unchanged. The project checker reports:

```text
M -> M: The complete compiled section, including its literal pool, has a different size from the original interval.
```

Accepted roots: 0. Accepted complete bytes: 0. Accepted bytes/hour: 0 throughout this lane window. The canonical checker rejects on extent before a linked byte comparison; no byte-similarity improvement is claimed. All local naming/rank changes were temporary and the original map bytes were restored. Main must independently decide whether to adopt this diagnostic identity and run its acceptance gate.

## Source and source-form history

Final source is `lib/CtrSDK/sources/AddressResourceFactory.cpp`, SHA256 `85d0460855fe94644d96a5ced443fd92cca4905d6c076119f0a74313890e9aa9`. Final source commit: `499daf8`; report/packet commit follows it. The final canonical object SHA256 is `8656b1a7f9bd0335369a748560a2ec1e2ebf3818b3bbb465ee0332af4980b73d`. Its only executable section is `i.fn_00298C58`, 2,984 bytes. No separately emitted source helper survives. The raw root section SHA256 is `1e17956edd4968b0746072692ac6a2b4b4b28bf8442f2722c1090647363797e4`.

1. `1ed703c`, Actor/791: 2,620-byte complete section, canonical extent rejection. Source SHA256 `8b7e0e9e562db1a336888614654b35f78cbc73622db1cda97683d0b3266dbd43`
2. `6af0d63`, SDK/902: 2,684 bytes, canonical extent rejection. Flat 28-byte record and independently observed full-width trailing flag. Source SHA256 `2aff6d94a149b08e586d148766f1f956d9317159536f8b2257aae225c0770a2b`
3. `6b7b75c`, SDK/902: 2,984 bytes, canonical extent rejection. Recover explicit append success, stable copy-end pointer, memberwise swap and result-code normalization. Source SHA256 `91b16fbf9716106dc6a5e78f7503278ded0f0b605b982d292bae9046f7588aab`

The final guard/label commit applies the project's `#ifdef NON_MATCHING` convention and neutral names to the default-one/default-zero vectors. Its compiled executable section is byte-identical to form3, so this is not a fourth source hypothesis. Three meaningful unsuccessful forms were used; no cosmetic fourth was attempted. An initial checker invocation used the wrong object path and rejected provenance; the corrected invocation above used the actual project-built object. That path error was not a source form.

Task-clock window: 2026-10-02 00:50:10 UTC through 01:09:07 UTC, 18 minutes 57 seconds including investigation, source, builds, replay and packaging. Observed form1 build/check window 00:53:53..00:54:36, form2 00:55:34..00:56:08, form3 00:57:34..00:57:56; final clean build/check 01:02:38..01:03:22. These are observed task windows, not separately instrumented compile-duration measurements. The initial expanded replay took 48.315 seconds; the final replay below includes a cyclic-parent nontermination control. No earlier acceptance inventory was converted here.

## Recovered roles and remaining identities

This is a resource-object factory. The resource has required flag bits `0x40000001`; counts at +34/+3C and self-relative lists at +38/+40 determine 24/28-byte records. The original constructor at 002997C8 initializes a 0x90-byte object, including vectors at +54/+64 and default-one/default-zero float vectors at +74/+80. Their precise class semantics remain unproven. The factory allocates space for the maximum of resource and caller-requested capacities, invokes virtual initializer slot24, optionally inserts into a parent vector, installs external array storage, copies selected resource records, constructs binding state and emits command state, then assigns paired pointers from signed record indices.

The trailing +18 field of the 28-byte record is a complete word, not a byte. Retail initializes the full word at stack+1C and stores a full word for the nested-reference branch. Final replay compares all four bytes for types 800001/800002/800004 with absent, single and two-level relative references. That correction is grounded in stores and behavior, not just code generation.

The first placement under Actor/791 was only an available configured build hypothesis. The later SDK/902 placement remains provisional: retail has core-register aggregate copies whereas the first 791 form used VFP copies; direct callee 0029E620 emits graphics command words and uses scalar-type constants such as 1400/1401/1402/1406. These observations support investigating graphics-library compilation but do not prove CtrSDK ownership or an exact class name. The configuration, global flags and original data identities were never changed. Existing 902/791 compiler SHA256 values are recorded in the ordinary project provenance files. No removed library, leaked SDK or external reference source was read.

A separate lane is investigating direct callee 002A451C. Its reported particle/attribute-buffer classification is not promoted to an established class identity here. ABI evidence was shared: r0 factory object, r1 resolved resource+30, r2 allocator, r3 outer fifth argument, stack[0] outer sixth argument; result is null or a table with direction +D0, enable bytes +D1 and paired pointers +E4/+E8. Current source remains frozen and independently testable. After freeze, the callee lane confirmed argument4 as the stream allocator and argument5 as the ParticleShape pointer. The machine ABI agrees. Its proposed all-opaque C declaration differs from this TU's structural pointer declaration; reconcile those declarations in an explicitly owned combined source family before a composition gate. This checkpoint does not silently compose them.

## Final whole-root replay

```json
{
  "pairs": 655,
  "agree": 655,
  "returning": 620,
  "seconds": 83.5247712135315,
  "root_instructions": 572,
  "failures": [],
  "original_direct_callee_pairs": 510,
  "original_direct_callee_returns": 475,
  "binding_provider_pairs": 125,
  "initializer_result_pairs": 20,
  "instruction_budget_pairs": 5,
  "fault_pairs": 30
}
```

All 131 fixtures run under ARM1176 emulation at FPSCR 0, 00400000, 00800000, 00C00000 and 01000000. These cover the four rounding modes plus flush-to-zero, not every FPSCR combination or real hardware. The root and its original callees execute from the unmodified owner executable; the candidate comes from the final committed-source project object linked at 00600000 for diagnostic execution. This auxiliary link does not alter canonical checking or establish matching.

The 510 original-direct-callee pairs and their 475 returns remain separate from 125 controlled binding-provider pairs and 20 initializer-result pairs. Constructor, ancestor recursion, resource-copy routines, destructor and other reached direct callees execute their retail code unless a fixture explicitly replaces the initializer result or binding-provider result. Allocator virtual callbacks use deterministic memory or chosen failure; release records ownership. Two hardware endpoints, 0028E280 and 0010B2BC, use explicit no-op/physical-address models. No rendering, scene initialization or gameplay result follows.

Returning pairs compare return value, all modeled external events, retained allocation bytes, source/owner/command-buffer memory, saved registers/SP and FPSCR. Three indeterminate padding bytes following each record's ownership Boolean are masked; every defined record field remains compared, including the full trailing flag. Freed allocations are excluded from retained-memory comparison. Failure controls compare fault type/address/size, preceding external events and FPSCR; they do not claim complete state equivalence at a fault. Five cyclic-parent pairs reach the 500,000-instruction budget without returning; the original recursive callee is retained, and this bounded observation does not prove termination for other inputs.

There are 730 executable retail root instructions, excluding its two literal words. Final replay covers 572/730 (78.356%) and leaves 158 uncovered. Uncovered intervals are 00298F64..00298F80 (7 instructions), 00298F98..00298FAC (5), 002990A0..002991A0 (64), 002992A8..002992CC (9), 002992E4..002992F8 (5), 0029944C..00299558 (67), and 002995A4..002995A8 (1). These are the retired-storage nonempty destructor/free paths, growth/reallocation paths for the two pre-reserved record arrays, and the null-object failure fallback. Original construction starts those arrays empty and reserves at least the resource count, so the tested unmutated resource fixtures do not enter those growth paths. Replay does not validate them independently.

## Clean build and prior-root preservation

The final committed source passed `python make.py eu -ca`: 45 Game, 129 Actor and 2 SDK source units compile, link and export. The diagnostic root's U status means the compact link itself does not exercise the new root. Its canonical object check above remains nonexact.

All 175 pre-existing canonical objects compare equal to the pristine 45a baseline after normalizing repository path text: all section data/attributes, non-file symbols, resolved relocations, compiler hashes, 365 distinct source/header/config inputs and normalized commands agree. Only absolute `STT_FILE` paths, repository prefixes in non-allocated `.comment` and resulting raw symbol/string offsets are excluded. No build objects or archives are shared. The baseline's unchanged empty-candidate acceptance gate passes its clean build, 707 previous roots and all 726 canonical-definition checks. This is equivalence-based preservation evidence for an isolated added TU, not a newly rerun full canonical gate in this worktree. Main still owns that gate.

Original code SHA256 remains `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`; exh SHA256 remains `94e61359c80498495dd77bb2df16f0def6fca94fca736dc8c3c929279b44d2c8`. Inputs remain ignored, with no binary or tool submitted.

## Reproduction

Use this branch's committed source on the exact frozen base and the already authorized ARMCC/wibo/venv inputs. The original map must be restored afterward. For the checker only, name the existing unchanged row temporarily; do not change its boundaries. The checker owns any temporary rank change.

```sh
. ./development_environment.sh
export DEVKITARM=/usr
mkdir -p build/research298c58
python - <<'PY_MAP'
from pathlib import Path
p=Path('data/ver/eu/map.csv')
b=p.read_bytes()
Path('build/research298c58/original-map.csv').write_bytes(b)
s=b.decode().replace('0x00298C58,0x002997C0,0x002997C8,          ,U,f,,',
                    '0x00298C58,0x002997C0,0x002997C8,          ,U,f,fn_00298C58,')
assert s.encode()!=b
p.write_text(s)
PY_MAP
python make.py eu -ca
python tools/check.py fn_00298C58 --object build/eu/obj/lib/CtrSDK/sources/AddressResourceFactory.o
# The expected check exit status is nonzero because the root is not exact.
python - <<'PY_RESTORE'
from pathlib import Path
Path('data/ver/eu/map.csv').write_bytes(Path('build/research298c58/original-map.csv').read_bytes())
PY_RESTORE
```

Save the following three complete scripts at the indicated ignored paths, then run the commands. `preserve.py` expects the independently built pristine baseline at sibling `mario-main45a`; its baseline report is `build/dot-baseline-45a/report.json` with 726 passing prior checks. The scripts do not create an acceptance claim or modify source, configuration, maps or tools.

```sh
PYTHONPATH=. python build/research298c58/link_replay.py
python build/research298c58/replay.py
python build/research298c58/preserve.py
```

### `build/research298c58/link_replay.py`

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
import subprocess
from tools.low.checkExactBytes import _resolve_symbol, _read_map
root=Path.cwd(); obj=Path('build/eu/obj/lib/CtrSDK/sources/AddressResourceFactory.o'); rows=_read_map(Path('data/ver/eu/map.csv')); lines=['#<SYMDEFS>#']
with obj.open('rb') as f:
 e=ELFFile(f)
 for symbol in e.get_section_by_name('.symtab').iter_symbols():
  if symbol['st_shndx']=='SHN_UNDEF' and symbol.name and not symbol.name.startswith('Lib$$'):
   address,kind,row=_resolve_symbol(symbol,None,rows); lines.append(f'0x{address:08X} {kind} {symbol.name}')
Path('build/research298c58/imports.sym').write_text('\n'.join(lines)+'\n')
subprocess.run([str(root/'data/compilers/wibo'),str(root/'data/compilers/4.0/902/bin/armlink.exe'),'--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--no_debug','--no_scanlib','--mangled','--symbols','--map','--ro_base=0x600000','--entry=fn_00298C58','--keep=fn_00298C58','--output=build/research298c58/replay.axf','--list=build/research298c58/replay.map',str(obj),'build/research298c58/imports.sym'],check=True)
```

### `build/research298c58/replay.py`

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
import struct,json,hashlib,time
ROOT=Path.cwd(); OUT=ROOT/'build/research298c58'; CODE=(ROOT/'data/ver/eu/code.bin').read_bytes()
ALLOC=0x7f0000;FREE=0x7f0010;STOP=0x7f1000;RES=0x800000;ALLOCATOR=0x810000;OPT=0x820000;OWNER=0x830000;BINDRES=0x840000;ARGS=0x850000;CMD=0x870000;BIND=0x890000
R=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3]; SAVED=[UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_SP]
SEG=[]
with (OUT/'replay.axf').open('rb') as f:
 e=ELFFile(f)
 for s in e.iter_sections():
  if s['sh_flags']&2:SEG.append((s['sh_addr'],s.data()))

def run(candidate,case,fpscr=0):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176)
 u.mem_map(0x100000,0x400000);u.mem_write(0x100000,CODE);u.mem_map(0x600000,0x10000)
 for a,b in SEG:u.mem_write(a,b)
 u.mem_map(0x700000,0x100000);u.mem_write(0x700000,b'\xa6'*0x100000)
 u.mem_map(0x800000,0x200000);u.mem_map(0x1000000,0x100000);u.mem_write(0x900000,b'\xc3'*0x100000)
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def byte(a,v):u.mem_write(a,bytes([v]))
 put(ALLOCATOR,ALLOCATOR+0x100);put(ALLOCATOR+0x108,ALLOC);put(ALLOCATOR+0x10c,FREE)
 put(RES,case.get('flags',0x40000001));put(RES+0x30,BINDRES-RES-0x30);put(RES+0x44,0x880000-RES-0x44);put(0x880000,0x80000000)
 list24=case.get('list24',[]);list28=case.get('list28',[])
 for n,items in enumerate([list24,list28]):
  field=RES+0x34+n*8;array=RES+0x100+n*0x100;put(field,len(items));put(field+4,array-field-4)
  for i,item in enumerate(items):
   a=RES+0x1000+n*0x2000+i*0x100;put(array+i*4,a-array-i*4)
   put(a,item.get('type',0));byte(a+4,item.get('copy',0));put(a+8,item.get('binding',-1));put(a+0x10,item.get('word10',0))
   if item.get('nested'):
    put(a+0xc,0x40-0xc)
    if item['nested']==2:put(a+0x60,0x10)
 put(OPT+0x10,case.get('capacity24',0));put(OPT+0x14,case.get('capacity28',0))
 put(OPT,case.get('option0',0));put(OPT+4,0);put(OPT+8,0);put(OPT+12,0)
 put(OWNER+0x10,OWNER);put(OWNER+0x14,0 if case.get('owner_no_allocator') else ALLOCATOR)
 put(OWNER+0x18,OWNER+0x100);put(OWNER+0x1c,OWNER+0x100+4*case.get('owner_size',0));put(OWNER+0x20,case.get('owner_mode',1)<<30|case.get('owner_capacity',0))
 for i in range(case.get('owner_size',0)):put(OWNER+0x100+i*4,OWNER+0x500+i*0x100)
 if case.get('parent'):put(OWNER+0xc,OWNER+0x1000);put(OWNER+0x1000+0xc,OWNER+0x1000 if case.get('parent_cycle') else 0)
 # Two command buffers, output packets and an empty original binding resource.
 for field,offset in [(0xc,0),(0x10,0x1000),(0x18,0x2000),(0x1c,0x3000),(0x24,0x4000)]:put(CMD+field,0x1000000+offset)
 put(CMD+0x34,0);put(BINDRES,case.get('binding_count',0));put(BINDRES+4,0);put(ARGS,0)
 byte(BIND+0xd0,case.get('reverse',0))
 for i in range(8):byte(BIND+0xd1+i,1 if case.get('enabled',True) else 0);put(BIND+0xe4+8*i,0x2000000+i*0x100);put(BIND+0xe8+8*i,0x3000000+i*0x100)
 for i,r in enumerate(SAVED[:-1]):u.reg_write(r,0xa4000000+i)
 u.reg_write(UC_ARM_REG_SP,0x7e0000);u.reg_write(UC_ARM_REG_LR,STOP);put(0x7e0000,ARGS);put(0x7e0004,CMD)
 for r,v in zip(R,[OWNER if case.get('owner') else 0,0 if case.get('null_resource') else RES,OPT,ALLOCATOR]):u.reg_write(r,v)
 u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000);u.reg_write(UC_ARM_REG_FPSCR,fpscr)
 events=[];allocs=[];freed=set();cov=set();callees=set();bad=[];steps=0;nalloc=0
 def finish(value=0):
  for r in R+[UC_ARM_REG_R12]:u.reg_write(r,0xa5a5a5a5)
  u.reg_write(UC_ARM_REG_R0,value&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def hook(u,a,z,d):
  nonlocal nalloc,steps
  steps+=1
  if 0x298c58<=a<0x2997c0:cov.add(a)
  if a in (0x2997c8,0x2b3f70,0x2998c0,0x29b57c,0x29afe8,0x2a451c,0x29e620,0x231ef0):callees.add(a)
  args=[u.reg_read(r) for r in R]
  if a==ALLOC:
   nalloc+=1;memory=0x900000+(nalloc-1)*0x10000
   if nalloc==case.get('fail_alloc'):memory=0
   events.append(['alloc',args[:3],memory]);allocs.append((memory,args[1]));finish(memory)
  elif a==FREE:events.append(['free',args[:2]]);freed.add(args[1]);finish()
  elif a==0x2b3f70 and 'init_result' in case:events.append(['init_model',args[:2]]);finish(case['init_result'])
  elif a==0x2a451c and case.get('model_binding'):
   events.append(['binding_model',args+[get(u.reg_read(UC_ARM_REG_SP))]])
   finish(0 if case.get('binding_fail') else BIND)
  elif a==0x28e280:events.append(['hardware_flush',args[0]]);finish()
  elif a==0x10b2bc:events.append(['hardware_address',args[0]]);finish(args[0]+0x10000000)
 def invalid(u,access,address,size,value,data):bad.append([access,address,size]);return False
 u.hook_add(UC_HOOK_CODE,hook);u.hook_add(UC_HOOK_MEM_INVALID,invalid)
 error=None
 try:u.emu_start(0x600000 if candidate else 0x298c58,STOP,count=500000)
 except UcError as e:error=str(e)
 ret=u.reg_read(UC_ARM_REG_R0);terminated=u.reg_read(UC_ARM_REG_PC)==STOP
 ranges=[(0x800000,0x90000),(0x1000000,0x10000)]+[(a,n) for a,n in allocs if a and a not in freed]
 data=[]
 for a,n in ranges:
  b=bytearray(u.mem_read(a,n))
  # Indeterminate C++ struct padding is excluded. Field values remain compared.
  if a==0x900000 and terminated and ret:
   for field,stride in [(0x58,24),(0x68,28)]:
    begin=get(a+field);end=get(a+field+4)
    for entry in range(begin,end,stride):
     off=entry-a
     if 0<=off<len(b)-3:b[off+1:off+4]=b'\0'*3
  data.append(bytes(b))
 result={'return':ret,'terminated':terminated,'error':error,'invalid':bad,'events':events,'memory':[(a,n,hashlib.sha256(b).hexdigest()) for (a,n),b in zip(ranges,data)],'saved':[u.reg_read(r) for r in SAVED],'fpscr':u.reg_read(UC_ARM_REG_FPSCR),'pc':u.reg_read(UC_ARM_REG_PC),'steps':steps,'callees':[hex(a) for a in sorted(callees)]}
 return result,cov,data

def suite():
 cases=[{}, {'fail_alloc':1},{'fail_alloc':2},{'fail_alloc':3},{'fail_alloc':4}, {'init_result':-1}, {'init_result':-2147483648}, {'init_result':1}, {'init_result':2147483647}]
 cases += [{'flags':x} for x in [0,1,0x40000000,0xffffffff]]+[{'null_resource':1}]
 cases += [{'owner':1,'owner_size':size,'owner_capacity':capacity,'owner_mode':mode} for size,capacity in [(0,0),(0,1),(1,1),(2,2),(2,4)] for mode in [0,1,2,3]]
 cases += [{'owner':1,'owner_no_allocator':1}, {'owner':1,'parent':1}, {'owner':1,'parent':1,'parent_cycle':1}]
 cases += [{'capacity24':a,'capacity28':b} for a,b in [(1,1),(5,7),(7,2),(0,4)]]
 # Execute every direct retail callee on both sides for these data variants.
 for n in [0,1,2,5]:
  cases += [{'list24':[{'binding':-1}]*n}, {'list28':[{'binding':-1}]*n}, {'list24':[{}]*n,'list28':[{}]*n}]
 for tag in [0,0x80000,0x100000,0x200000,0x400000,0x800000,0x1000000,0x2000000,0x4000000,0x8000000,0x10000000,0x20000000,0x40000000,0x80000000]:
  cases += [{'list24':[{'type':tag,'copy':1}]},{'list28':[{'type':tag,'copy':1}]}]
 # Controlled binding table isolates all final root-side binding outcomes.
 for reverse in [0,1,255]:
  for enabled in [False,True]:
   for index in [-1,0,3,7]:
    cases += [{'model_binding':1,'reverse':reverse,'enabled':enabled,'list24':[{'binding':index}], 'list28':[{'binding':index}]}]
 for tag in [0x800002,0x800001,0x800004,0x800003]:
  for nested in [0,1,2]:cases += [{'list28':[{'type':tag,'nested':nested}]}]
 cases += [{'list28':[{'type':0x80000,'word10':x}]} for x in [0,1,2,0x101,0xff01]]
 cases += [{'model_binding':1,'binding_fail':1,'list24':[{}],'list28':[{}]}]
 cases += [{'fail_alloc':n,'list24':[{'copy':1}],'list28':[{'copy':1}]} for n in range(1,7)]
 cases += [{'owner':1,'owner_size':2,'owner_capacity':2,'fail_alloc':n} for n in [3,4]]
 return cases
if __name__=='__main__':
 rows=[];coverage=set();t=time.time();failures=[]
 for fpscr in [0,0x400000,0x800000,0xc00000,0x1000000]:
  for i,c in enumerate(suite()):
   a,ca,ma=run(False,c,fpscr);b,cb,mb=run(True,c,fpscr);coverage|=ca
   keys=['return','terminated','error','invalid','events','memory','saved','fpscr'] if a['terminated'] else ['terminated','error','invalid','events','fpscr']
   diff=[k for k in keys if a[k]!=b[k]]
   row={'case':c,'fpscr':fpscr,'diff':diff,'retail':a,'candidate':b};rows.append(row)
   if diff:
    path=OUT/f'failure-{fpscr}-{i}.json';path.write_text(json.dumps(row,indent=2)+'\n');failures.append(str(path))
    print(i,c,'DIFF',diff,'returned',a['terminated'],'pc',hex(a['pc']),'bad',a['invalid'][-2:])
    if 'memory' in diff:print('memory offsets',[[j for j,(x,y) in enumerate(zip(aa,bb)) if x!=y][:30] for aa,bb in zip(ma,mb)])
   elif fpscr==0 and not a['terminated']:print(i,c,'paired stop',hex(a['pc']),a['invalid'][-2:])
 result={'pairs':len(rows),'agree':sum(not r['diff'] for r in rows),'returning':sum(r['retail']['terminated'] for r in rows),'seconds':time.time()-t,'root_instructions':len(coverage),'failures':failures,'rows':rows,'coverage':sorted(coverage)}
 (OUT/'replay-result.json').write_text(json.dumps(result,indent=2)+'\n');print({k:v for k,v in result.items() if k not in ['rows','coverage']})
```

### `build/research298c58/preserve.py`

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
import json,hashlib
root=Path.cwd();base=root.parent/'mario-main45a';out=root/'build/research298c58'
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
 if rel.parts[0] not in ('Game','lib') or p.stem=='AddressResourceFactory':continue
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

