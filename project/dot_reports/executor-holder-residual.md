# Executor holder residual, bounded dot requeue

Frozen base: `8991bec8cb27ff1a2dac0003f059d270a0b17493`. Branch: `dot/executor-holder-residual`. Intended C++ checkpoint: `2e60145`. Original packet and Pro response are unchanged. This pass began 2026-10-02T01:31:54Z after the coordinator fetched main and released the graded Pro reservations.

## Result and scope

The unchanged project checker reports **8 differing bytes in the complete 908-byte interval**, reduced from the faithfully reproduced Pro form 3's 22. The entire 88-byte literal pool equals the original. This is a guarded NonMatching proposal, not an exact match. The pass stops after four unsuccessful meaningful new forms. There is no fifth tuning form, main adoption, new exact root or accepted byte credit.

Only `lib/al/src/Execute/alExecuteTableHolderUpdateInit.cpp` is added. Existing holder methods, its paired accepted createExecutorListTable at 001E36EC, all headers, LiveActor/kit sources, shared sead code, Course family, flags, tools, classifications, boundaries, and acceptance bookkeeping remain unchanged. All constructors retain neutral address-derived imports at existing rows. No speculative public constructor name or alias map is needed.

## Reproduction and bounded forms

Each intended C++ source was committed before the ordinary `make.py eu` build and unchanged `tools/check.py --object` call. All builds used the configured ARMCC 4.1/791, without per-file or global flag changes. Reproduction and final verification used clean `make.py eu -ca`; the intermediate builds were incremental project builds. The historical 791/894 Pro equivalence is retained in the original response; this new pass used 791 only.

| Source checkpoint | Substantive hypothesis | Complete bytes | Canonical outcome |
| --- | --- | ---: | --- |
|2486f10|Exact supplied Pro form 3, self-contained neutral imports|908|22 differing bytes; full pool equal|
|bf1deda, new 1|Separate result-producing inline scopes for combined/Execute allocation and construction|900|Strict extent rejection; not compared as exact|
|7039167, new 2|Extract unchanged goto-based actor count into an inline returning helper|908|10 differing bytes; full pool equal|
|f9814b3, new 3|Conditional constructor selection retains allocator's null pointer value on failure|908|10 differing bytes; full pool equal|
|58b30ac, new 4|Conditional constructor selection has explicit null-pointer result|908|8 differing bytes; full pool equal|
|2e60145, final|Guard the unchanged best fourth form with NON_MATCHING|908|8 differing bytes; full pool equal|

Form 1 shortened the section by 8 bytes and was rejected. Form 2 corrects all twelve order/count r8↔r9 instructions while retaining the already-correct count-scope r6/r7 allocation. Form 3 changes both omitted conditional assignments into `moveq r6,r0`, but reverses the final append buffer/index register roles. Form 4 gives both target `moveq r6,#0` instructions and keeps that final append reversal. These are allocation/result lifetime tests; no flags, dummy stores, register/volatile directives, padding or byte edits were used.

## Residual diagnosis

The final remaining six instructions are:

| Address | Original | Generated |
| --- | --- | --- |
|001E398C|ldr r1,[r4,#18]|ldr r0,[r4,#18]|
|001E3990|add r1,r1,#1|add r0,r0,#1|
|001E3994|str r1,[r4,#18]|str r0,[r4,#18]|
|001E3AE0|ldr r0,[r4,#10]|ldr r0,[r4,#8]|
|001E3AE4|ldr r1,[r4,#8]|ldr r1,[r4,#10]|
|001E3AEC|str r6,[r0,r1,lsl#2]|str r6,[r1,r0,lsl#2]|

Offsets in this table are hexadecimal. The first actor pair preserves exactly the original early `mov r6,r0`, but the generated counter update reuses dead r0. The final append has equivalent buffer/index roles in exchanged registers. The first-count loop, all constructor calls and null paths, layout/functor shared assignment, branches, return at 001E3B08 and pool at 001E3B0C are otherwise byte-identical. On ordinary readable holder storage, the actor counter changes the same field, and both append forms compute the same destination and value. The swapped scratch registers are overwritten or caller-clobbered before they become observable through the declared interface. The final pair also reverses two loads from distinct ordinary fields with no intervening call or store. This explains the bounded execution agreement; it does not cover concurrently changing, volatile or unreadable holder memory. These observations justify a frozen compiler-allocation diagnosis, not more cosmetic source cycling. Further work requires a genuinely new independently grounded lifetime or collection contract after other work and a fresh requeue.

## Preservation and closure

The final clean build succeeds, links and exports in **53.565 seconds**. The serialized unchanged canonical checker preserves **717/717 current accepted roots and all 736/736 actual canonical definitions**. Every accepted definition from every provenance-recorded project object is checked, including duplicate definitions of accepted roots. There are no missing accepted roots. This is a current 8991bec gate, not the historical 707/726 or 709/728 baseline. The full map is restored byte for byte. An earlier gate enumeration accidentally included 717 generated C stub definitions alongside the 736 actual C++ definitions; it was interrupted after 100 checks, with the map independently verified unchanged. That incomplete run is retained as `build/executor-residual/preserve-enumeration-interrupted.log` and supplies no preservation credit. The complete gate explicitly selects Game/lib C++ provenance inputs and rechecks every actual definition.

The candidate object has one nonempty executable section, the 908-byte root. The inline actor-count helper disappears; no new executable helper body remains. Imported closure consists of the two existing allocation services, original string comparison, the existing kind/count helpers, and the six original constructors. All imports resolve through unchanged map rows. The original callees execute during the bounded replay below; exact matching of those imported callees is not claimed by this source proposal.

## Bounded whole-root execution

The final project-checker-linked candidate is substituted only at the unchanged root address in a separate ARM1176 Unicorn machine. Its counterpart executes the original root. Both load the hash-verified private original executable, so original string comparison, kind/count providers, all six constructors and their reached base constructors run directly. Only array allocation at 00292A78 and scalar allocation at 002932B0 are controlled services. Hooks record request sizes/results, clobber caller-saved registers, provide deterministic storage or selected failures, and implement the two declared alias controls. The candidate is never assembled or hand-edited.

Across 183 cases at each of four FPSCR states, **692 returning original/candidate pairs and 40 separate fault pairs** agree. Comparisons include the full 68-byte holder, every allocated region, allocation requests/results, mutation storage, fault type/address/width and partial state. SP and callee-saved general registers agree on normal returns. All four FPSCR states remain unchanged, including fault runs: 0, 00400000, 01000000, 03C0009F. This is bounded flag-state preservation, not an exhaustive FPSCR or floating-point exception proof.

A separate Python collection model makes **1384 returning-run checks** across original and generated executions. It derives capacities by kind, ordered common/category insertion, unknown-kind null insertion, object name/capacity/empty-buffer fields and functor first-entry state from the input records and allocation events. It is not a transcription of ARM instructions. Vtable contents are covered by original/candidate memory comparison, not independently recovered by this model.

Valid ordinary inputs use a zero-initialized holder, readable 16-byte records, terminated strings, nonnegative counts up to 20 and capacities 0..8. Cases include every known kind, unknown/case-mismatched/empty strings, mixed order, sixty deterministic random lists, six scalar failure positions, buffer replacement during allocation and a count-shrink control. Negative counts -2/-1 and ten array-allocation failure positions are additional bounded ABI/fault controls, not evidence that arbitrary invalid C++ inputs are supported. Array-fault cases are not normal-return successes. No claim covers real allocator internals, arbitrary corrupt/overlapping state, reinitializing a populated holder, arbitrary callbacks, complete game initialization, rendering or gameplay.

## Frozen identities and timing

- Final source SHA256: `499492976d8a29574a4b6bb16c2372b823e32b38faba6cafbf328f58de80d0bb`
- Apply-clean source-only patch SHA256: `dbb15eb1bddb81ca2d7c2ca0f76cf0a36d8acd3c3db3494c8d4d57c62d4751cf`
- Source patch applies without changes to the named base via `git apply --check`; it contains one added C++ file
- The final guarded source produces the same complete linked bytes as form 4
- Canonical object SHA256: `8e8a0cd86475d3a45ee731336e3573868170f39b20da2036475dbd25aeb2ac98`
- Canonical linked section SHA256: `9db256676a81da51e28e3a67810cb010bd7cc76c1580ec01abd48c720b25cd7a`
- Restored map SHA256: `2dde00f86cc0bc6f174f3a281853a4faacea1f16b6b628d602bea687b4cdb805`
- Original executable SHA256: `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`
- Canonical preservation window: 2026-10-02T01:42:24.818785+00:00 to 2026-10-02T01:52:00.023624+00:00, 575.205 seconds
- Replay window: 21.365 seconds, ending 2026-10-02T01:40:25.914689+00:00

Preparation, four new forms, clean final build and preservation belong to this single lane's elapsed window. Final report packaging is subsequent and is not hidden inside checker timings. This report was generated at 2026-10-02T01:52:21.392836+00:00. New accepted complete bytes are 0, so observed accepted throughput is **0 bytes/hour**. The 908-byte source is only partial inventory. No predicted intake rate is reported as achieved throughput.

## Reproduction recipes

Use the approved toolchain/private inputs already described by the project. Run from this isolated branch's repository root. Source this repository's development environment first and select the installed ARM binutils location. The scripts below are complete executed verification recipes stored in notes, not modifications of project tools.

```sh
. ./development_environment.sh
export DEVKITARM=/usr
python - <<'PYEXTRACT'
from pathlib import Path
import re
report=Path('project/dot_reports/executor-holder-residual.md').read_text()
for name,code in re.findall(r'<!-- replay-file: ([^>]+) -->\n```python\n(.*?)\n```',report,re.S):
    path=Path(name)
    assert path.parts[:2]==('build','executor-residual')
    path.parent.mkdir(parents=True,exist_ok=True)
    path.write_text(code+'\n')
PYEXTRACT
python make.py eu -ca
python build/executor-residual/measure.py final
python build/executor-residual/replay.py
python build/executor-residual/preserve.py
git diff --exit-code -- data/ver/eu/map.csv
```

The frozen form checkpoints above reproduce the source search: check out each in its own dot worktree, materialize approved private inputs, run the project build, then the same measure script with a distinct form label. `measure.py` records canonical checks and compiler-output hashes. Only when the strict checker rejects extent does its separately labeled diagnostic link use the actual generated extent; this cannot grant exact credit. The final candidate uses the strict checker’s unchanged908-byte original-address link directly. Original and generated disassembly/logs/binaries remain ignored and private under `build/executor-residual/`.

<!-- replay-file: build/executor-residual/measure.py -->
```python
from pathlib import Path
import sys,json,subprocess,hashlib,shutil,datetime
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
R=Path.cwd();sys.path.insert(0,str(R))
from tools.low.checkExactBytes import check_exact_bytes, _read_map, _isolate_function
from tools.low.buildProvenance import verify_build_output
label=sys.argv[1];out=R/'build/executor-residual'/label;out.mkdir(exist_ok=True)
obj=R/'build/eu/obj/lib/al/src/Execute/alExecuteTableHolderUpdateInit.o';sym='_ZN2al24ExecuteTableHolderUpdate4initEPKNS_12ExecuteOrderEi';src=R/'lib/al/src/Execute/alExecuteTableHolderUpdateInit.cpp'
p=R/'data/ver/eu/map.csv';baseline=p.read_bytes()
try:
 r=subprocess.run([sys.executable,'tools/check.py',sym,'--object',str(obj)],capture_output=True,text=True)
 (out/'canonical-check.txt').write_text(r.stdout+r.stderr)
finally:p.write_bytes(baseline)
provenance=verify_build_output(obj)
j=check_exact_bytes(sym,obj,output_directory=out/'canonical')
j.update(source_sha256=hashlib.sha256(src.read_bytes()).hexdigest(),commit=subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip(),timestamp=datetime.datetime.now(datetime.timezone.utc).isoformat(),provenance=provenance)
shutil.copy2(src,out/'source.cpp');shutil.copy2(obj,out/'source.o')
(out/'result.json').write_text(json.dumps(j,indent=2)+'\n')
print(r.stdout+r.stderr);print(json.dumps({k:v for k,v in j.items() if k not in ['evidence','provenance']},indent=2));print(j['evidence'].get('different_bytes'),j['evidence'].get('compiled_section_size'))
# Actual-extent diagnostic link is separate from the strict canonical result.
if not (out/'canonical/candidate.axf').exists():
    import os
    directory=out/'diagnostic'; directory.mkdir(exist_ok=True)
    section,raw,imports=_isolate_function(obj,sym,_read_map(p),directory/'candidate.o')
    (directory/'symbols.sym').write_text('#<SYMDEFS>#\n'+''.join(f'0x{i["address"]:08X} {i["kind"]} {i["symbol"]}\n' for i in imports))
    (directory/'candidate.sct').write_text(f'LOAD 0x1E37D8\n{{\n CODE 0x1E37D8 0x{len(raw):X}\n {{\n candidate.o ({section}, +FIRST)\n }}\n}}\n')
    cmd=[str(R/'data/compilers/wibo'),str(R/'data/compilers/4.1/791/bin/armlink.exe'),'--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--inline','--datacompressor=off','--no_debug','--no_scanlib','--mangled','--symbols','--map',f'--entry={sym}',f'--keep={sym}',f'--scatter={directory}/candidate.sct',f'--output={directory}/candidate.axf',f'--list={directory}/candidate.map',str(directory/'candidate.o'),str(directory/'symbols.sym')]
    z=subprocess.run(cmd,capture_output=True,text=True,env=dict(os.environ,TMP='/tmp'));(directory/'link.log').write_text(z.stdout+z.stderr);assert z.returncode==0
    (directory/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
    with (directory/'candidate.axf').open('rb') as f:
        e=ELFFile(f);b=e.get_section_by_name('CODE').data()
    (out/'candidate.bin').write_bytes(b)
    orig=(R/'data/ver/eu/code.bin').read_bytes()[0xe37d8:0xe3b64]
    print('Diagnostic complete differences',sum(x!=y for x,y in zip(orig,b))+abs(len(orig)-len(b)))
    cs=Cs(CS_ARCH_ARM,CS_MODE_ARM)
    (out/'generated.txt').write_text('\n'.join(f'{i.address:08X}: {i.mnemonic} {i.op_str}' for i in cs.disasm(b,0x1e37d8))+'\n')
if (out/'canonical/candidate.axf').exists():
 with (out/'canonical/candidate.axf').open('rb') as f:
  e=ELFFile(f);b=e.get_section_by_name('CANDIDATE_CODE').data()
 (out/'candidate.bin').write_bytes(b)
 start=0x1e37d8;original=(R/'data/ver/eu/code.bin').read_bytes()[start-0x100000:0x1e3b64-0x100000]
 cs=Cs(CS_ARCH_ARM,CS_MODE_ARM)
 one=list(cs.disasm(original[:820],start));two=list(cs.disasm(b[:820],start));lines=[]
 for x,y in zip(one,two):
  lines.append(f'{"=" if x.bytes==y.bytes else "!"} {x.address:08X}: {x.mnemonic:8} {x.op_str:30} | {y.mnemonic:8} {y.op_str}')
 (out/'residual.txt').write_text('\n'.join(lines)+'\n');print('\n'.join(l for l in lines if l.startswith('!')))
 print('pool equal',b[820:]==original[820:])
```

<!-- replay-file: build/executor-residual/replay.py -->
```python
"""Bounded whole-root ARM execution, with explicit external hooks; zero exact credit."""
from pathlib import Path
import hashlib,json,random,struct
from unicorn import Uc,UcError,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE,UC_HOOK_MEM_INVALID
from unicorn.arm_const import *
ROOT=Path.cwd(); OUT=ROOT/'build/executor-residual/final'
CODE=(ROOT/'data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(CODE).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
BASE=0x1000000;HEAP=BASE+0x100000;STACK=0x800000;STOP=0x700ff0;CALLBACK=0x700100
REGS=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_R12]
HELPERS=[0x242cc4,0x242ccc,0x292308,0x1e43d4,0x1e43bc,0x1e85a4,0x2415dc,0x243b94,0x1e3bc8,0x1e7618,0x1dc890]
KHELPERS=[0x1cc9b0,0x1bfd70,0x1cca1c,0x1d44e0,0x1bd91c,0x1c2bc0]
def word(v):return struct.pack('<I',v&0xffffffff)
def setup(label,candidate):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176)
 u.mem_map(0x100000,0x400000);u.mem_write(0x100000,CODE)
 if candidate:u.mem_write(0x1e37d8 if label=='holder' else 0x274990,(OUT/'candidate.bin').read_bytes())
 u.mem_map(BASE,0x200000);u.mem_map(STACK,0x10000);u.mem_map(0x700000,0x1000)
 for i,r in enumerate(REGS):u.reg_write(r,0xB0000000+i*0x1111)
 u.reg_write(UC_ARM_REG_SP,STACK+0xff00);u.reg_write(UC_ARM_REG_LR,STOP)
 u.reg_write(UC_ARM_REG_FPSCR,CURRENT_FPSCR)
 return u
def read(u,a):return struct.unpack('<I',u.mem_read(a,4))[0]
def put(u,a,v):u.mem_write(a,word(v))
def string(u,a):
 b=bytearray()
 for n in range(512):
  x=bytes(u.mem_read(a+n,1))
  if x==b'\0':return bytes(b)
  b+=x
 raise AssertionError('unterminated string')
def ret(u,value=0):
 for i,r in enumerate([UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R12]):u.reg_write(r,0xD1234000+i)
 u.reg_write(UC_ARM_REG_R0,value);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
def finish(u,start):
 faults=[]
 def invalid(u,access,address,size,value,data):faults.append((access,address,size));return False
 u.hook_add(UC_HOOK_MEM_INVALID,invalid)
 error=None
 try:u.emu_start(start,STOP,count=1000000)
 except UcError as exc:error=exc.errno
 if not error:assert u.reg_read(UC_ARM_REG_PC)==STOP,'instruction bound'
 if not error:
  assert u.reg_read(UC_ARM_REG_SP)==STACK+0xff00
  for i in range(4,12):assert u.reg_read(REGS[i])==0xB0000000+i*0x1111
 return error,faults
KINDS=['ActorMovement','ActorCalcAnim','ActorMovementCalcAnim','LayoutUpdate','Execute','Functor','Unknown']
def holder(case,candidate):
 u=setup('holder',candidate); count=case.get('count',len(case['entries']))
 for i,(kind,cap) in enumerate(case['entries']):
  name=BASE+0x2000+i*0x80;typ=name+0x30
  u.mem_write(name,f'actor_{i}'.encode()+b'\0');u.mem_write(typ,kind.encode()+b'\0')
  u.mem_write(BASE+0x1000+i*16,word(name)+word(typ)+word(cap)+word(0xABCD0000+i))
 next_alloc=HEAP;allocs=[];seen={};scalar=0;array=0;changed=False
 def hook(u,address,size,data):
  nonlocal next_alloc,scalar,array,changed
  if address in HELPERS:seen[address]=seen.get(address,0)+1
  if address not in (0x292a78,0x2932b0):return
  is_scalar=address==0x2932b0
  if is_scalar:scalar+=1
  else:array+=1
  amount=u.reg_read(UC_ARM_REG_R0)
  fail=(is_scalar and scalar==case.get('fail_scalar')) or (not is_scalar and array==case.get('fail_array')) or amount>0x10000
  result=0
  if not fail:
   result=next_alloc;extent=max(16,(amount+15)&~15);next_alloc+=extent
   u.mem_write(result,b'\xA5'*extent)
  allocs.append(('scalar' if is_scalar else 'array',amount,result))
  if is_scalar and not changed and case.get('mutation'):
   changed=True
   if case['mutation']=='shrink':put(u,BASE+0xC,1)
   elif case['mutation']=='actor_buffer':
    source=read(u,BASE+0x1c);target=BASE+0x9000
    u.mem_write(target,bytes(u.mem_read(source,read(u,BASE+0x14)*4)));put(u,BASE+0x1c,target)
  ret(u,result)
 u.hook_add(UC_HOOK_CODE,hook)
 u.reg_write(UC_ARM_REG_R0,BASE);u.reg_write(UC_ARM_REG_R1,BASE+0x1000);u.reg_write(UC_ARM_REG_R2,count&0xffffffff)
 error,faults=finish(u,0x1e37d8)
 assert u.reg_read(UC_ARM_REG_FPSCR)==CURRENT_FPSCR
 if error is None: validate_model(u,case,allocs)
 return dict(error=error,faults=faults,allocations=allocs,holder=bytes(u.mem_read(BASE,0x44)).hex(),heap=bytes(u.mem_read(HEAP,next_alloc-HEAP)).hex(),mutation=bytes(u.mem_read(BASE+0x9000,128)).hex()),seen

MODEL_CHECKS=0
CURRENT_FPSCR=0

def validate_model(u,case,allocations):
 # Independent collection contract over ordered records and constructed storage.
 global MODEL_CHECKS
 records=case['entries']; count=case.get('count',len(records))
 assert read(u,BASE+0xC)==(1 if case.get('mutation')=='shrink' else count)&0xffffffff
 assert read(u,BASE)==0 and read(u,BASE+4)==0
 groups=[KINDS[:3],['LayoutUpdate'],['Execute'],['Functor']]
 capacity_offsets=[0x14,0x20,0x2c,0x38]
 if count<=0: visited=[]
 elif case.get('mutation')=='shrink': visited=records[:1]
 else: visited=records[:count]
 assert read(u,BASE+8)==len(visited)
 if case.get('mutation')=='shrink':
  assert read(u,BASE+0xC)==1
 all_buffer=read(u,BASE+0x10)
 values=[read(u,all_buffer+i*4) for i in range(len(visited))]
 scalar_values=[allocation[2] for allocation in allocations if allocation[0]=='scalar']
 recognized=[i for i,(kind,cap) in enumerate(visited) if kind in KINDS[:6]]
 assert [values[i] for i in recognized]==scalar_values
 for i,(kind,cap) in enumerate(visited):
  pointer=values[i]
  if kind not in KINDS[:6]: assert pointer==0
  elif pointer:
   assert read(u,pointer+4)==BASE+0x2000+i*0x80
   if kind=='Functor': assert read(u,pointer+8)==0
   else:
    assert read(u,pointer+8)==cap and read(u,pointer+12)==0
    buffer=read(u,pointer+16)
    assert bytes(u.mem_read(buffer,cap*4))==bytes(cap*4)
 for kinds,offset in zip(groups,capacity_offsets):
  wanted=[values[i] for i,(kind,cap) in enumerate(visited) if kind in kinds]
  assert read(u,BASE+offset)==sum(kind in kinds for kind,cap in records[:max(0,count)])
  assert read(u,BASE+offset+4)==len(wanted)
  buffer=read(u,BASE+offset+8)
  assert [read(u,buffer+i*4) for i in range(len(wanted))]==wanted
 MODEL_CHECKS+=1
cases=[dict(entries=[],count=n) for n in [-2,-1,0]]
for kind in KINDS+['actorMovement','']:
 for cap in [0,1,2,3,7]:cases.append(dict(entries=[(kind,cap)]))
for one in KINDS:
 for two in KINDS:cases.append(dict(entries=[(one,1),(two,3)]))
for cap in [0,1,3,5]:
 for order in [KINDS,list(reversed(KINDS))]:cases.append(dict(entries=[(k,cap) for k in order]))
rng=random.Random(0x1e37d8)
for _ in range(60):cases.append(dict(entries=[(rng.choice(KINDS),rng.randrange(9)) for i in range(rng.randrange(1,21))]))
for failure in range(1,7):cases.append(dict(entries=[(k,3) for k in KINDS],fail_scalar=failure))
for failure in range(1,11):cases.append(dict(entries=[(k,3) for k in KINDS],fail_array=failure))
for mutation in ['actor_buffer','shrink']:cases.append(dict(entries=[(k,3) for k in KINDS],mutation=mutation))

import time,datetime
began=time.monotonic()
summary={'returning_pairs':0,'fault_pairs':0,'case_count_per_fpscr':len(cases),'helper_entries':{}}
for CURRENT_FPSCR in [0,0x00400000,0x01000000,0x03c0009f]:
 for n,c in enumerate(cases):
  original,seen=holder(c,False);generated,_=holder(c,True)
  assert original==generated,('holder',CURRENT_FPSCR,n,c,{k:(original[k],generated[k]) for k in original if original[k]!=generated[k]})
  summary['fault_pairs' if original['error'] else 'returning_pairs']+=1
  for a,count in seen.items():summary['helper_entries'][f'{a:08X}']=summary['helper_entries'].get(f'{a:08X}',0)+count
summary.update(model_checks=MODEL_CHECKS,seconds=time.monotonic()-began,finished_utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),fpscr_states=[0,0x00400000,0x01000000,0x03c0009f],candidate_sha256=hashlib.sha256((OUT/'candidate.bin').read_bytes()).hexdigest(),executable_sha256=hashlib.sha256(CODE).hexdigest())
(OUT/'replay-summary.json').write_text(json.dumps(summary,indent=2)+'\n')
print(json.dumps(summary,indent=2))
```

<!-- replay-file: build/executor-residual/preserve.py -->
```python
from pathlib import Path
import sys,json,subprocess,hashlib,time,datetime
from elftools.elf.elffile import ELFFile
R=Path.cwd();sys.path.insert(0,str(R))
from tools.low.checkExactBytes import _read_map
OUT=R/'build/executor-residual/final';OUT.mkdir(exist_ok=True)
p=R/'data/ver/eu/map.csv';baseline=p.read_bytes();rows=_read_map(p)
accepted={(r['Symbol'] or f"fn_{r['Start']:08X}"):r for r in rows if r['Rank']=='O' and 'f' in r['Type']}
assert len(accepted)==717,len(accepted)
records=[]
for obj in sorted((R/'build/eu/obj').rglob('*.o')):
 if not obj.with_suffix('.provenance.json').exists():continue
 provenance=json.loads(obj.with_suffix('.provenance.json').read_text())
 if Path(provenance['source']).suffix not in ('.cpp','.cc','.cxx'):continue
 if not provenance['source'].startswith(('Game/','lib/')):continue
 with obj.open('rb') as stream:
  elf=ELFFile(stream);table=elf.get_section_by_name('.symtab')
  if table is None:continue
  for sym in table.iter_symbols():
   if sym.name in accepted and isinstance(sym['st_shndx'],int) and sym['st_shndx'] and sym['st_info']['type']=='STT_FUNC':
    records.append((sym.name,str(obj.relative_to(R))))
found={s for s,o in records};assert found==set(accepted),set(accepted)-found
print('accepted roots',len(accepted),'actual definitions',len(records),flush=True)
checks=[];began=time.monotonic();start=datetime.datetime.now(datetime.timezone.utc).isoformat()
try:
 for i,(sym,obj) in enumerate(records):
  c=subprocess.run([sys.executable,'tools/check.py',sym,'--object',obj],capture_output=True,text=True)
  checks.append(dict(symbol=sym,object=obj,returncode=c.returncode,output=c.stdout+c.stderr))
  if c.returncode:print('FAIL',sym,obj,c.stdout+c.stderr,flush=True)
  if i%100==99:print('checked',i+1,flush=True)
finally:p.write_bytes(baseline)
summary=dict(base='8991bec8cb27ff1a2dac0003f059d270a0b17493',commit=subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip(),accepted_roots=len(accepted),actual_definitions=len(records),passed=sum(c['returncode']==0 for c in checks),checks=checks,map_sha256=hashlib.sha256(baseline).hexdigest(),map_restored=p.read_bytes()==baseline,started_utc=start,finished_utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),seconds=time.monotonic()-began)
(OUT/'preservation.json').write_text(json.dumps(summary,indent=2)+'\n')
print(json.dumps({k:v for k,v in summary.items() if k!='checks'},indent=2),flush=True)
assert all(c['returncode']==0 for c in checks)
```
