# Reproduce the sound track interpreter evidence

This note contains the exact five scripts used for the final 9,616-case replay and equivalence-backed preservation. They are source-text evidence, not replacements for any project tool. Extracted bytes must match the SHA-256 values below and the evidence JSON before execution. No game bytes, objects, compiler executables, or generated image are included here.

## Inputs and scope

Use the final source `lib/al/src/Audio/SoundTrackInterpreter.cpp`, SHA-256 `f428d3e9d6da80da4a44b5f220d429fe40d15123f6dfbc55ceb45f35f1edd69a`, on the frozen `bc5b236802b0300bc6b00f0ac55cf5ae5749c55c` family base. Its two source commits are `1a354ef` and `ee81bda`. The source must be committed before the project build. Keep the original map and project tools unchanged.

Materialize the owner's private EU code/exheader through the existing authorized local setup. `data/ver/eu/code.bin` must hash to `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. Use the existing ARMCC 4.1/791 and 4.0/902 compilers, wibo, and project Python environment with pyelftools, capstone, and Unicorn. Only 791 builds the new audio TU; 902 remains required by the normal SDK module. No 894 compiler or special flags are needed. The replay CPU is Unicorn's ARM1176 model with VFP enabled, as shown in the exact script.

The scripts run from the candidate repository root. Their generated files remain under ignored `build/dot-sound-track/`. The checker wrapper changes only the blank root Symbol spelling temporarily and restores the exact map bytes in a `finally` block. It prints the expected canonical size rejection; this is not an exact-match recipe. The diagnostic link and replay do not edit the original input binary or substitute models for direct original callees.

## Extract and verify the scripts

From the candidate repository root, run this extraction command. It verifies all five payload hashes before writing each exact script to the ignored output directory.

```sh
python3 - <<'PY_EXTRACT'
from pathlib import Path
import hashlib,re
note=Path('project/dot_reports/sound-track-interpreter-reproduction.md').read_text()
blocks=re.findall(r'<!-- BEGIN SCRIPT: ([a-z-]+\.py) SHA256: ([0-9a-f]{64}) -->\n```python\n(.*?)```\n<!-- END SCRIPT -->',note,re.S)
assert len(blocks)==5
out=Path('build/dot-sound-track');out.mkdir(parents=True,exist_ok=True)
for name,expected,body in blocks:
    payload=body.encode('utf-8')
    assert hashlib.sha256(payload).hexdigest()==expected,name
    (out/name).write_bytes(payload)
    print(name,expected)
PY_EXTRACT
```

## Build, canonical check, and whole-root replay

These are the exact command forms used, from the candidate repository root. The final clean build, check, and diagnostic preparation were run before the final snapshot. The scripts assert normal-build provenance and the expected compiler. The replay reads the original private binary directly and compares it with the independently relocated compiler-generated candidate.

```sh
. ./development_environment.sh
export DEVKITARM=/usr
python make.py eu -ca > build/dot-sound-track/final-clean-build.log 2>&1
python build/dot-sound-track/run-check.py final
python build/dot-sound-track/prepare.py final
python build/dot-sound-track/replay.py > build/dot-sound-track/replay.log 2>&1
python build/dot-sound-track/snapshot.py build/dot-sound-track/final-snapshot.json
```

Expected canonical result is return code 1 in `final-check.json`, with a complete-section size rejection: 2236 compiler bytes versus 2208 original bytes. Expected replay is 9,616 completed cases, zero mismatches, all four controls agreeing, 505/508 original executable instruction words fetched, and maximum stack depths 120/120 bytes. The three unfetched words are the statically unreachable switch-default returns described in the main report. The sole modeled target is the external user callback at harness address `00700100`. The replay is not a claim about full audio-device behavior, arbitrary aliasing, portable C++ sanitizers, full script execution, or gameplay.

## Reproduce the preservation comparison

Whole-root replay above is independent of the coordinator's old-root report. The additional preservation comparison requires the coordinator's already verified clean baseline checkout and its original report, whose SHA-256 is `faf671be7c53bc783dd4a09301a1b9eae3f38034f900e357d2357d91f92bed4f`. That report records all 752 roots/772 definitions and is an input to the equivalence proof, not a newly executed checker run in this lane.

The exact tested `preservation.py` intentionally retains these original workspace paths:

- Baseline checkout: `/workspace/scratch/73cdb2c524af/mario-mainbc5`
- Baseline report: `/workspace/scratch/73cdb2c524af/mario-mainbc5/build/dot-baseline-bc5/report.json`
- Candidate checkout: `/workspace/scratch/73cdb2c524af/mario-sound-track`

For a literal replay, use those paths. If reproducing elsewhere, record and review any path relocation separately; do not claim edited scripts retain the tested payload hashes. The source/check/replay stages themselves use the current working directory and need no path edits.

The baseline snapshot was generated by running the same exact `snapshot.py` from the baseline checkout. The final comparison runs from the candidate checkout:

```sh
cd /workspace/scratch/73cdb2c524af/mario-mainbc5
. ./development_environment.sh
python /workspace/scratch/73cdb2c524af/mario-sound-track/build/dot-sound-track/snapshot.py /workspace/scratch/73cdb2c524af/mario-sound-track/build/dot-sound-track/baseline-snapshot.json
cd /workspace/scratch/73cdb2c524af/mario-sound-track
. ./development_environment.sh
python build/dot-sound-track/preservation.py
```

Expected preservation output is 179 old C++ objects, 372 old input hashes, 470 tracked blobs, zero differences, and exactly one added object. The comparison includes the eight old objects with no checked definition. Only workspace prefixes in STT_FILE symbols and compiler command arguments are normalized. The baseline report must already exist and pass its exact hash/content assertions; the scripts do not manufacture it.

## Exact script payloads

### run-check.py

<!-- BEGIN SCRIPT: run-check.py SHA256: 88e21c98c84efdad04595cb1077c43d3372d39a3b0188befb409543232fce03e -->
```python
from pathlib import Path
import subprocess,sys,json,hashlib,time
p=Path('data/ver/eu/map.csv');original=p.read_bytes();old=b'0x003409FC,          ,0x0034129C,          ,U,f,,';assert original.count(old)==1
try:
 p.write_bytes(original.replace(old,old.replace(b',U,f,,',b',U,f,fn_003409FC,')))
 start=time.monotonic();r=subprocess.run([sys.executable,'tools/check.py','fn_003409FC','--object','build/eu/obj/lib/al/src/Audio/SoundTrackInterpreter.o'],capture_output=True,text=True)
 Path('build/dot-sound-track/'+sys.argv[1]+'-check.log').write_text(r.stdout+r.stderr);print(r.stdout+r.stderr)
 Path('build/dot-sound-track/'+sys.argv[1]+'-check.json').write_text(json.dumps({'returncode':r.returncode,'stdout':r.stdout,'stderr':r.stderr,'elapsed_seconds':time.monotonic()-start,'map_sha256':hashlib.sha256(original).hexdigest()},indent=2))
finally:p.write_bytes(original)
assert p.read_bytes()==original
```
<!-- END SCRIPT -->

### prepare.py

<!-- BEGIN SCRIPT: prepare.py SHA256: f72a273f350bb0d625b990cd01437e57974f4650aa8e3c1725b034c7b7664448 -->
```python
from pathlib import Path
import sys,json,subprocess,hashlib
from elftools.elf.elffile import ELFFile
sys.path.insert(0,str(Path.cwd()))
from tools.low.buildProvenance import verify_build_output
from tools.low.checkExactBytes import _read_map
out=Path('build/dot-sound-track');obj=Path('build/eu/obj/lib/al/src/Audio/SoundTrackInterpreter.o')
p=verify_build_output(obj);assert p['compiler']=='4.1/791'
rows=_read_map(Path('data/ver/eu/map.csv'));by={r['Start']:r for r in rows};names={r['Symbol']:r for r in rows if r['Symbol']}
imports=[]
with obj.open('rb') as f:
 e=ELFFile(f);raw=e.get_section_by_name('i.fn_003409FC').data()
 for s in e.get_section_by_name('.symtab').iter_symbols():
  if s['st_shndx']=='SHN_UNDEF' and s.name and not s.name.startswith('Lib$$Request$$'):
   a=int(s.name.split('_')[1],16) if s.name.startswith(('fn_','dat_')) else names[s.name]['Start'];kind='A' if 'f' in by[a]['Type'] else 'D'
   imports.append({'name':s.name,'address':a,'kind':kind})
(out/'replay.sym').write_text('#<SYMDEFS>#\n'+''.join('0x%08X %s %s\n'%(i['address'],i['kind'],i['name']) for i in imports))
(out/'replay.sct').write_text('REPLAY 0x003409FC { CODE 0x003409FC { SoundTrackInterpreter.o (i.fn_003409FC, +FIRST) } }\n')
cmd=['data/compilers/wibo','data/compilers/4.1/791/bin/armlink.exe','--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--inline','--datacompressor=off','--no_debug','--no_scanlib','--entry=fn_003409FC','--keep=fn_003409FC','--scatter='+str(out/'replay.sct'),'--output='+str(out/'candidate.axf'),str(obj),str(out/'replay.sym')]
r=subprocess.run(cmd,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
with (out/'candidate.axf').open('rb') as f:
 e=ELFFile(f);ss=[s for s in e.iter_sections() if s['sh_flags']&2 and s['sh_size']];assert len(ss)==1 and ss[0]['sh_addr']==0x3409fc
 linked=ss[0].data();(out/'candidate.bin').write_bytes(linked)
result={'source_sha256':hashlib.sha256(Path(p['source']).read_bytes()).hexdigest(),'object_sha256':hashlib.sha256(obj.read_bytes()).hexdigest(),'section_bytes':len(raw),'original_bytes':2208,'replay_bytes':len(linked),'replay_sha256':hashlib.sha256(linked).hexdigest(),'imports':imports,'strict_credit':False,'qualification':'Diagnostic link of unchanged compiler object at original address; strict checker rejects whole section size.'}
(out/(sys.argv[1]+'-prepare.json')).write_text(json.dumps(result,indent=2));(out/(sys.argv[1]+'.bin')).write_bytes(linked);print(json.dumps(result,indent=2))
```
<!-- END SCRIPT -->

### replay.py

<!-- BEGIN SCRIPT: replay.py SHA256: 86b148cd8dd8cd8c13c5c58142144dcbbc1178db5d8a0588131d20c2ce008b74 -->
```python
from pathlib import Path
import json,struct,hashlib,random,csv,sys,time
from collections import Counter
from unicorn import *
from unicorn.arm_const import *
ROOT=0x3409fc;END=0x34129c;STOP=0x700000;HEAP=0x600000;TRACK=HEAP;OWNER=HEAP+0x10000;OTHER=HEAP+0x20000;VOICE=HEAP+0x30000;STACK=0x830000
out=Path('build/dot-sound-track');binary=Path('data/ver/eu/code.bin').read_bytes();candidate=Path(sys.argv[1] if len(sys.argv)>1 else out/'candidate.bin').read_bytes()
assert hashlib.sha256(binary).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
word=lambda a:struct.unpack_from('<I',binary,a-0x100000)[0]
GLOBAL=word(0x214950);SEED=word(0x24b5e8)
imports={0x28870c:2,0x214928:2,0x228c94:2,0x228d28:1,0x24b5c4:0,0x2c2810:3,0x2c29e8:1,0x2c2aa0:2,0x2c2be4:1,0x2c2f20:2,0x2c6348:2,0x2c6690:3}
function_entries={int(r[0],16) for r in csv.reader(Path('data/ver/eu/map.csv').open()) if r and r[0].startswith('0x') and 'f' in r[5]}
dataintervals=[(0x340af0,0x340b1c),(0x340b40,0x340b70),(0x340b98,0x340bb0),(0x340e04,0x340e14),(0x3410d0,0x3410f4),(0x34116c,0x341174)]
expected=set(range(ROOT,END,4))-{a for s,e in dataintervals for a in range(s,e,4)}
def run(c,compiled):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176);u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000);u.reg_write(UC_ARM_REG_FPSCR,c.get('fpscr',0))
 u.mem_map(0x100000,0x400000);u.mem_write(0x100000,binary)
 if compiled:u.mem_write(ROOT,candidate)
 u.mem_map(HEAP,0x50000);u.mem_map(STOP,0x1000);u.mem_map(0x800000,0x40000)
 def w(a,*x):u.mem_write(a,struct.pack('<'+'I'*len(x),*[v&0xffffffff for v in x]))
 def h(a,x):u.mem_write(a,struct.pack('<H',x&0xffff))
 def b(a,x):u.mem_write(a,bytes([x&255]))
 def r(a):return int.from_bytes(u.mem_read(a,4),'little')
 def ret():u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 for t in [TRACK,OTHER]:
  w(t+0xc0,OWNER);w(t+0xc4,VOICE if c.get('voices') else 0);b(t+5,0)
  w(t+0x1c,HEAP+0x40000,HEAP+0x40040);b(t+0x24,0x7a)
  b(t+0x40,c.get('depth',1));b(t+0x88,c.get('transpose',0x41))
  for i in range(3):
   b(t+0x28+8*i,c.get('kinds',[0,1,0])[i]);b(t+0x29+8*i,c.get('remaining',2));w(t+0x2c+8*i,HEAP+0x40080+4*i)
  for off in [0x6c,0x72,0x78,0x7e]:
   b(t+off,c.get('rstart',0x10));b(t+off+1,c.get('rtarget',0x71));h(t+off+2,c.get('duration',12));h(t+off+4,c.get('elapsed',5))
  for i in range(16):h(t+0xa0+2*i,c.get('variable',-7))
 for i in range(16):
  w(OWNER+0x88+4*i,{'other':OTHER,'self':TRACK,'null':0}[c.get('target','other')]);h(OWNER+0xc8+2*i,c.get('variable',-7));h(GLOBAL+2*i,c.get('variable',-7))
 w(OWNER+0x80,STOP+0x100 if c.get('callback') else 0,HEAP+0x44000)
 w(SEED,c.get('seed',0x12345678));b(0x3f152d,c.get('debug',1))
 for i in range(2):
  v=VOICE+i*0x200;w(v+0x138,v+0x200 if i==0 else 0);w(v+0x12c,0x11223344,0x55667788,0);b(v+0xc6,1);b(v+0x90,3)
 if c.get('cycle'):w(VOICE+0x338,VOICE)
 if c.get('bad_owner'):w(TRACK+0xc0,0x900000)
 track=0x900000 if c.get('bad_track') else TRACK
 w(STACK,c.get('extra',4));u.reg_write(UC_ARM_REG_R0,HEAP+0x45000);u.reg_write(UC_ARM_REG_R1,track);u.reg_write(UC_ARM_REG_R2,c['op']);u.reg_write(UC_ARM_REG_R3,c.get('arg',3)&0xffffffff);u.reg_write(UC_ARM_REG_SP,STACK);u.reg_write(UC_ARM_REG_LR,STOP)
 saved={x:0xa0000000+x for x in [UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11]+list(range(UC_ARM_REG_D8,UC_ARM_REG_D15+1))}
 for x,y in saved.items():u.reg_write(x,y)
 trace=[];seen=set();counts=Counter();steps=0;minimum=STACK;fault=[];writes=[];models=Counter()
 regs=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3]
 def hook(u,a,n,data):
  nonlocal steps,minimum
  steps+=1;minimum=min(minimum,u.reg_read(UC_ARM_REG_SP))
  if a==STOP:u.emu_stop();return
  if a in expected and not compiled:seen.add(a)
  if a in function_entries and a!=ROOT and not (compiled and ROOT<=a<ROOT+len(candidate)):counts[a]+=1
  if a in imports:trace.append([a,[u.reg_read(x) for x in regs[:imports[a]]]])
  if a==STOP+0x100:
   args=[u.reg_read(x) for x in regs[:3]];desc=args[1];logical=[r(desc),r(desc+4),r(desc+8),bytes(u.mem_read(desc+12,1))[0]]
   trace.append(['callback',[args[0],logical,args[2]]]);models[a]+=1
   h(r(desc),0x1357);h(r(desc+4),0x2468);h(r(desc+8),0x369c);b(desc+12,0xa5);w(args[2],args[0]);ret()
 def onwrite(u,access,a,size,value,data):
  assert any(lo<=a and a+size<=hi for lo,hi in [(HEAP,HEAP+0x50000),(0x3e0000,0x500000),(0x800000,0x840000)]),('unobserved write',hex(a),size)
  if not 0x800000<=a<a+size<=0x840000:writes.append([a,size,value])
 def onfault(u,access,a,size,value,data):fault.append([access,a,size]);return False
 u.hook_add(UC_HOOK_CODE,hook);u.hook_add(UC_HOOK_MEM_WRITE,onwrite);u.hook_add(UC_HOOK_MEM_INVALID,onfault)
 error=None
 try:u.emu_start(ROOT,STOP,count=c.get('limit',10000))
 except UcError as e:error=str(e)
 returned=u.reg_read(UC_ARM_REG_PC)==STOP
 if not error and not returned:error='instruction budget'
 state={'trace':trace,'heap':hashlib.sha256(bytes(u.mem_read(HEAP,0x50000))).hexdigest(),'globals':hashlib.sha256(bytes(u.mem_read(0x3e0000,0x120000))).hexdigest(),'fpscr':u.reg_read(UC_ARM_REG_FPSCR),'returned':returned,'sp':u.reg_read(UC_ARM_REG_SP)==STACK,'saved':all(u.reg_read(x)==y for x,y in saved.items()),'error':error,'fault':fault}
 return state,{'steps':steps,'max_stack':STACK-minimum,'seen':sorted(seen),'original_entries':dict(counts),'modeled_entries':dict(models),'writes':writes,'stop_pc':u.reg_read(UC_ARM_REG_PC)}
cases=[]
for op in range(256):
 for variant in range(3):cases.append({'op':op,'arg':[0,3,255][variant],'extra':[0,4,17][variant],'depth':variant,'kinds':[[0,0,0],[1,1,1],[1,0,1]][variant],'remaining':variant,'voices':variant==2,'target':['other','self','null'][variant]})
for op in range(256):cases.append({'op':0xf000+op,'arg':3,'extra':4})
for op in range(0x80,0x96):
 for arg in [0,15,16,31,32,47,48,255]:
  for val in [-32768,-1,0,1,32767]:
   for extra in ([-16,-1,0,1,16] if op==0x85 else [-32768,-1,0,1,32767]):cases.append({'op':0xf000+op,'arg':arg,'extra':extra,'variable':val})
for op in [0xc0,0xc1,0xc4,0xd7]:
 for start,target in [(0,255),(255,0),(128,127),(127,128)]:
  for duration,elapsed in [(0,0),(12,0),(12,11),(12,12),(-1,0),(12,-1)]:cases.append({'op':op,'arg':128,'extra':-32768,'rstart':start,'rtarget':target,'duration':duration,'elapsed':elapsed})
for op in [0x8a,0xd4,0xfc,0xfd]:
 for depth in range(4):
  for kinds in [[0,0,0],[1,1,1],[1,0,1],[0,1,0]]:
   for remaining in [0,1,2,255]:cases.append({'op':op,'arg':0x12345,'extra':0,'depth':depth,'kinds':kinds,'remaining':remaining})
for op in [0x81,0xe1]:
 for arg in [-32768,-1,0,1,1023,1024,65535,65536,0x7fffffff]:cases.append({'op':op,'arg':arg})
for op in [0xb2,0xc8,0xdd,0x88]:
 for arg in range(5):cases.append({'op':op,'arg':arg,'voices':True})
for arg in [0,15,16,31,32,47,48,255]:
 for debug in [0,1]:cases.append({'op':0xd6,'arg':arg,'debug':debug})
for op in [0xf0e0,0x1e0,0xffe0]:
 for arg in [0,65535]:
  for callback in [0,1]:cases.append({'op':op,'arg':arg,'callback':callback})
for op in [0x10000,0xffffffff,0x101,0xff95,0x180]:cases.append({'op':op,'arg':32,'extra':-1})
for op in [0xb5,0xd8,0xe3,0xca,0xcb]:
 for arg in [-32768,-1,0,1,127,255,32767]:
  for fpscr in [0,0x1000000,0x2000000,0x3000000]:cases.append({'op':op,'arg':arg,'fpscr':fpscr})
# Every possible low-eight-bit register shift count, both signs, with high-bit boundaries.
for extra in [-32768,32767]+list(range(-256,257)):
 for value in [-32768,-127,-1,0,1,127,32767]:
  cases.append({'op':0xf085,'arg':32,'extra':extra,'variable':value})
for seed in [0,1,0x7fffffff,0xffffffff]:
 for extra in [-32768,-32767,-1,0,1,32767]:cases.append({'op':0xf086,'arg':16,'extra':extra,'seed':seed})
# Expected faults / nontermination are compared separately from ordinary returns.
controls=[{'op':0x81,'bad_track':True},{'op':0xb0,'bad_owner':True},{'op':0xf080,'arg':0,'bad_owner':True},{'op':0xdd,'arg':3,'voices':True,'cycle':True,'limit':3000}]
start=time.monotonic();coverage=set();entry_counts=Counter();models=Counter();maxstack=[0,0];mismatches=[];results=[]
for index,c in enumerate(cases):
 a,am=run(c,False);b,bm=run(c,True);coverage.update(am['seen']);entry_counts.update(am['original_entries']);models.update(am['modeled_entries']);maxstack=[max(maxstack[0],am['max_stack']),max(maxstack[1],bm['max_stack'])]
 if a!=b:
  mismatch={'index':index,'case':c,'different':[k for k in a if a[k]!=b[k]],'original':a,'candidate':b};mismatches.append(mismatch);print(json.dumps(mismatch));break
 if not a['returned'] or not a['saved'] or not a['sp']:raise AssertionError((c,a))
 if index%1000==0:print('cases',index,flush=True)
negative=[]
if not mismatches:
 for c in controls:
  a,am=run(c,False);b,bm=run(c,True)
  keys=['error','fault','heap','globals'] if not c.get('cycle') else ['error','fault','heap','globals']
  same=all(a[k]==b[k] for k in keys);negative.append({'case':c,'comparison_keys':keys,'same':same,'original':a,'candidate':b,'original_metadata':am,'candidate_metadata':bm});assert same
result={'source_sha256':hashlib.sha256(Path('lib/al/src/Audio/SoundTrackInterpreter.cpp').read_bytes()).hexdigest(),'candidate_sha256':hashlib.sha256(candidate).hexdigest(),'cases':len(cases),'completed':index+1,'mismatches':mismatches,'executable_instructions':len(expected),'original_fetched_instructions':len(coverage),'uncovered':[hex(a) for a in sorted(expected-coverage)],'original_entries':{hex(k):v for k,v in sorted(entry_counts.items())},'modeled_entries':{hex(k):v for k,v in sorted(models.items())},'max_stack':maxstack,'negative_controls':negative,'elapsed_seconds':time.monotonic()-start,'limits':['Compiled ARM outputs, not portable C++ proof; shift counts cover every low-eight-bit value in both directions and signed-16 extremes; ramp denominator is nonzero whenever elapsed<duration in normal object states; caller-derived widths for arithmetic operands','Owner, track and secondary track are distinct objects; pointer aliasing outside those identities is unproven','Actual original direct callees and internal dependencies execute; external callback is modeled, with verified descriptor ABI and observed state mutation','Void R0 and caller-saved registers are intentionally not compared; no sound hardware or complete sequence playback claim','Instruction coverage is fetched-instruction coverage, including condition-failed ARM instructions']}
(out/'replay.json').write_text(json.dumps(result,indent=2));print(json.dumps({k:v for k,v in result.items() if k not in ['negative_controls','original_entries','mismatches']},indent=2));assert not mismatches
```
<!-- END SCRIPT -->

### snapshot.py

<!-- BEGIN SCRIPT: snapshot.py SHA256: 2d17d888a9502300154011a90cf4e16e7dd4fb11bdc8a88d550c23225d4d8e2a -->
```python
from pathlib import Path
import sys,json,hashlib,io
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
repo=Path.cwd().resolve();sys.path.insert(0,str(repo))
from tools.low.buildProvenance import verify_build_output
records={};all_inputs={}
for p in sorted((repo/'build/eu/obj').rglob('*.o')):
 rel=p.relative_to(repo).as_posix()
 if not rel.startswith(('build/eu/obj/Game/','build/eu/obj/lib/')):continue
 prov=verify_build_output(p)
 original=json.loads(p.with_suffix('.provenance.json').read_text());assert original['language']=='C++'
 all_inputs.update(original['inputs'])
 elf=ELFFile(io.BytesIO(p.read_bytes()));ss=list(elf.iter_sections());table=elf.get_section_by_name('.symtab')
 def sym(x):
  ix=x['st_shndx'];return [x.name,x['st_value'],x['st_size'],dict(x['st_info']),dict(x['st_other']),ss[ix].name if isinstance(ix,int) else ix]
 alloc=[[s.name,s['sh_type'],s['sh_flags'],s['sh_addralign'],s['sh_size'],hashlib.sha256(s.data()).hexdigest()] for s in ss if s['sh_flags']&2]
 symbols=[sym(s) for s in table.iter_symbols()]
 relocs=[]
 for s in ss:
  if isinstance(s,RelocationSection):
   relocs.append([s.name,ss[s['sh_info']].name,[[r['r_offset'],r['r_info_type'],sym(table.get_symbol(r['r_info_sym']))] for r in s.iter_relocations()]])
 attrs=[s.data().hex() for s in ss if s.name=='.ARM.attributes']
 provenance={k:v for k,v in original.items() if k not in ['command','object_sha256']};provenance['command']=[s.replace(str(repo),'<REPO>') for s in original['command']]
 records[rel]={'allocated':alloc,'symbols':symbols,'relocations':relocs,'arm_attributes':attrs,'provenance':provenance}
result={'objects':records,'inputs':all_inputs,'verified_objects':len(records)}
Path(sys.argv[1]).write_text(json.dumps(result,sort_keys=True,indent=2));print(json.dumps({'objects':len(records),'inputs':len(all_inputs)}))
```
<!-- END SCRIPT -->

### preservation.py

<!-- BEGIN SCRIPT: preservation.py SHA256: a76157b84f06322af217fd84a468329adb6a3f57bcc43e867934b4f9ae49e34b -->
```python
import json,hashlib,subprocess
from pathlib import Path
out=Path('build/dot-sound-track');a=json.loads((out/'baseline-snapshot.json').read_text());b=json.loads((out/'final-snapshot.json').read_text());differences=[];normalized_file_paths=0
for p,v in a['objects'].items():
 for left,right in zip(v['symbols'],b['objects'][p]['symbols']):
  if left!=right and left[3]['type']==right[3]['type']=='STT_FILE':
   assert left[1:]==right[1:]
   assert left[0].replace('/workspace/scratch/73cdb2c524af/mario-mainbc5/','<REPO>/')==right[0].replace(str(Path.cwd())+'/','<REPO>/')
   right[0]=left[0];normalized_file_paths+=1
 if b['objects'].get(p)!=v:differences.append({'object':p,'different_fields':[k for k,x in v.items() if b['objects'].get(p,{}).get(k)!=x]})
input_diff=[p for p,v in a['inputs'].items() if b['inputs'].get(p)!=v];added=sorted(set(b['objects'])-set(a['objects']));report=Path('/workspace/scratch/73cdb2c524af/mario-mainbc5/build/dot-baseline-bc5/report.json');digest=hashlib.sha256(report.read_bytes()).hexdigest();assert digest=='faf671be7c53bc783dd4a09301a1b9eae3f38034f900e357d2357d91f92bed4f';r=json.loads(report.read_text());assert r['clean_build_returncode']==0 and len(r['prior_checks'])==772 and all(x['returncode']==0 for x in r['prior_checks'])
tracked=subprocess.check_output(['git','ls-tree','-r','bc5b236','--','Game','lib','tools','data/config.json','data/ver/eu/map.csv'],text=True).splitlines();changed=[];gitlinks=[];blobs=0
for line in tracked:
 meta,p=line.split('\t');mode,kind,digest=meta.split()
 if kind=='commit':
  current=subprocess.check_output(['git','ls-tree','HEAD','--',p],text=True).split()[2];assert current==digest;gitlinks.append(p);continue
 assert kind=='blob';blobs+=1;base=subprocess.check_output(['git','show','bc5b236:'+p]);now=Path(p).read_bytes()
 if base!=now:changed.append(p)
s={'base':'bc5b236802b0300bc6b00f0ac55cf5ae5749c55c','baseline_report_sha256':hashlib.sha256(report.read_bytes()).hexdigest(),'baseline_roots':752,'baseline_actual_definitions':len(r['prior_checks']),'baseline_clean_build_returncode':r['clean_build_returncode'],'baseline_elapsed_seconds':r['elapsed_seconds'],'canonical_objects_compared':len(a['objects']),'normalized_STT_FILE_workspace_prefixes':normalized_file_paths,'input_hashes_compared':len(a['inputs']),'tracked_blob_files_compared':blobs,'unchanged_gitlinks':gitlinks,'object_differences':differences,'input_differences':input_diff,'tracked_differences':changed,'added_objects':added,'second_772_check_run':False};(out/'preservation.json').write_text(json.dumps(s,indent=2));print(json.dumps(s,indent=2));assert not differences and not input_diff and not changed;assert len(added)==1
```
<!-- END SCRIPT -->

