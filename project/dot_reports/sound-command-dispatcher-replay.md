# Sound command dispatcher: executable verification recipe

This recipe tests committed source36b1183cb81795a1d617573af656ec4658232114 against frozen754f99a30a337756df5aa01c28b4ed6a977fecd5. Locally provide the ignored verified EU dump/exheader, project environment, ARMCC4.1/791 and wibo. Do not upload binary inputs or outputs. Source/object/replay hashes are pinned by the preparation script. No target, oracle, compiler-object or global-flag edit is permitted.

Extract only the named diagnostic scripts into the ignored build directory:

```sh
. ./development_environment.sh
python - <<'PYTHON'
from pathlib import Path
import re
report=Path('project/dot_reports/sound-command-dispatcher-replay.md').read_text()
blocks=re.findall(r'<!-- file: ([^ ]+) -->\n```python\n(.*?)\n```',report,re.S)
assert {name for name,_ in blocks}=={'run-check.py','prepare.py','replay.py','snapshot.py','preservation.py'}
out=Path('build/sound-dispatch');out.mkdir(parents=True,exist_ok=True)
for name,code in blocks:(out/name).write_text(code+'\n')
PYTHON
python make.py eu -ca
python build/sound-dispatch/run-check.py final
python build/sound-dispatch/prepare.py
python build/sound-dispatch/replay.py
```

The expected strict result is rejection, not acceptance. The wrapper invokes unchanged check.py and expects exit1, then restores the entire map byte-for-byte. It changes only the blank symbol on the existing2220-byte root row temporarily. It does not create a BSS row or change a Type, boundary or rank. The preparation script links the original compiler object, without isolating or editing it. Its sole extra-map import is independently verified00430DB0; this diagnostic link has no canonical credit.

Expected final replay:1129 valid returning pairs, four same-observation malformed-alias fault pairs, no unequal pairs, all491 decoded root instructions reached. Three additional negative controls are excluded from returning counts: null target, unmapped next and cyclic budget exhaustion. The full explicit models, meaningful argument arities, original-callee whitelist, compared memory bounds and write guard appear below. The callback side effects are synthetic. Native subset results do not prove modeled callees, concurrency, device audio, hardware cycles or general invalid-pointer behavior.

For preservation, first run snapshot.py from the pristine754 checkout with an absolute filename pointing to this lane's build/sound-dispatch/baseline-snapshot.json. Then run it from this lane's final clean build with build/sound-dispatch/final-snapshot.json, and run preservation.py. The recorded coordinator baseline is local at /workspace/scratch/73cdb2c524af/mario-main754. When reproducing elsewhere, update only that diagnostic checkout location and the verified STT_FILE prefix mapping. The baseline report hash remains pinned. Baseline733/753 was already checked; this comparison is exhaustive object/input equivalence, not another753-check run.

All imported source types and data-address limitations are explained in [the metadata report](sound-command-dispatcher-metadata.md). The full target disassembly and original jump-table words remain in [the packet](../pro_requests/002BECC8.md).

## run-check.py

SHA256 `28aaa556b7bf1aa9bb2794f5ba05fdda0cdf5744697202bd0218dacb83fc0acf`.

<!-- file: run-check.py -->
```python
from pathlib import Path
import subprocess,sys,json
p=Path('data/ver/eu/map.csv');original=p.read_bytes()
old=b'0x002BECC8,0x002BF564,0x002BF574,          ,U,f,,';assert original.count(old)==1
try:
 p.write_bytes(original.replace(old,old.replace(b',U,f,,',b',U,f,fn_002BECC8,')))
 r=subprocess.run([sys.executable,'tools/check.py','fn_002BECC8','--object','build/eu/obj/lib/al/src/Audio/SoundCommandDispatch.o'],capture_output=True,text=True)
 Path('build/sound-dispatch/'+sys.argv[1]+'-check.log').write_text(r.stdout+r.stderr);print(r.stdout+r.stderr)
 Path('build/sound-dispatch/'+sys.argv[1]+'-check.json').write_text(json.dumps({'returncode':r.returncode,'stdout':r.stdout,'stderr':r.stderr},indent=2))
 assert r.returncode==1
finally:p.write_bytes(original)
assert p.read_bytes()==original
```

## prepare.py

SHA256 `2b6af2106cdf8300257c4dfa4295304c5f48527e6294f3f8e521ffa0bb6dcb20`.

<!-- file: prepare.py -->
```python
from pathlib import Path
import sys,json,subprocess,hashlib
from elftools.elf.elffile import ELFFile
sys.path.insert(0,str(Path.cwd()))
from tools.low.buildProvenance import verify_build_output
from tools.low.checkExactBytes import _read_map
out=Path('build/sound-dispatch');obj=Path('build/eu/obj/lib/al/src/Audio/SoundCommandDispatch.o')
p=verify_build_output(obj); assert p['compiler']=='4.1/791'
assert hashlib.sha256(Path(p['source']).read_bytes()).hexdigest()=='ef82ee4a7e9f205ceb26d93feac035f58ff29c28c92b8c6ebd1c077e7468ba66'
assert hashlib.sha256(obj.read_bytes()).hexdigest()=='0acbfd58b5d8dd459857b692e1d2734e9d1d38351bb35e02ce17eebea08f16e1'
rows=_read_map(Path('data/ver/eu/map.csv'));by={r['Start']:r for r in rows}
imports=[]
with obj.open('rb') as f:
 e=ELFFile(f); raw=e.get_section_by_name('i.fn_002BECC8').data()
 for s in e.get_section_by_name('.symtab').iter_symbols():
  if s['st_shndx']=='SHN_UNDEF' and s.name and not s.name.startswith('Lib$$Request$$'):
   assert s.name.startswith(('fn_','dat_')),s.name
   a=int(s.name.split('_')[1],16);kind='A' if s.name.startswith('fn_') else 'D'
   assert a in by or a==0x430db0
   if a in by:assert ('f' in by[a]['Type'])==(kind=='A')
   imports.append({'name':s.name,'address':a,'kind':kind,'canonical_row':a in by})
(out/'replay.sym').write_text('#<SYMDEFS>#\n'+''.join('0x%08X %s %s\n'%(i['address'],i['kind'],i['name']) for i in imports))
(out/'replay.sct').write_text('REPLAY 0x002BECC8 { CODE 0x002BECC8 { SoundCommandDispatch.o (i.fn_002BECC8, +FIRST) } }\n')
cmd=['data/compilers/wibo','data/compilers/4.1/791/bin/armlink.exe','--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--inline','--datacompressor=off','--no_debug','--no_scanlib','--entry=fn_002BECC8','--keep=fn_002BECC8','--scatter='+str(out/'replay.sct'),'--output='+str(out/'candidate.axf'),str(obj),str(out/'replay.sym')]
r=subprocess.run(cmd,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
with (out/'candidate.axf').open('rb') as f:
 elf=ELFFile(f);ss=[s for s in elf.iter_sections() if s['sh_flags']&2 and s['sh_size']];assert len(ss)==1;assert ss[0]['sh_addr']==0x2becc8
 linked=ss[0].data();(out/'candidate.bin').write_bytes(linked)
assert len(raw)==len(linked)==2092
assert hashlib.sha256(linked).hexdigest()=='fb2879536ba82df12ee9ac4c3805bab631caf449ac90e55387649841478867e2'
result={'source_sha':hashlib.sha256(Path(p['source']).read_bytes()).hexdigest(),'object_sha':hashlib.sha256(obj.read_bytes()).hexdigest(),'section_bytes':len(raw),'original_bytes':2220,'replay_bytes':len(linked),'replay_sha':hashlib.sha256(linked).hexdigest(),'imports':imports,'strict_credit':False,'qualification':'Diagnostic link of unchanged compiler object uses independently verified BSS address absent canonical map.'}
(out/'prepare.json').write_text(json.dumps(result,indent=2));print(json.dumps({k:v for k,v in result.items() if k!='imports'}))
```

## replay.py

SHA256 `160ab948ece77959ee88131a010ed94832137a5fcdb3c35b1df1d130bc60b72a`.

<!-- file: replay.py -->
```python
from pathlib import Path
import json,struct,hashlib,random,sys
from collections import Counter
from unicorn import *
from unicorn.arm_const import *
ROOT=0x2becc8; STOP=0x700000; HEAP=0x600000; STACK=0x830000
CMD=HEAP; OBJ=HEAP+0x20000; TRACK=HEAP+0x30000; VT=HEAP+0x40000; FLAG=HEAP+0x41000
GLOBAL=0x430db0;GUARD=0x3f38b8
regs=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3]
# Meaningful argument registers/stack words. Float args reside in s0.
spec={0x2c42fc:(2,0,1),0x2c4308:(2,0,1),0x2c6788:(4,0,0),0x2c68ac:(4,2,0),0x2c6700:(3,0,0),0x2c6684:(3,0,0),0x2c6414:(3,0,0),0x2c6468:(2,0,0),0x2c6348:(2,0,0),0x2c2a84:(3,0,0),0x2c62b4:(3,0,0),0x2c6390:(4,0,0),0x2c635c:(2,0,1),0x2c6330:(2,0,1),0x2c6224:(2,0,1),0x2c6590:(2,0,1),0x2c6378:(2,0,1),0x2c6450:(2,0,1),0x2c6420:(2,0,1),0x2c6438:(2,0,1),0x2c65a8:(3,0,1),0x2c647c:(3,0,0),0x2c6510:(3,0,0),0x2c6638:(2,0,1),0x2c41a4:(4,4,0),0x2c5850:(4,0,0),0x2c5c64:(4,3,0),0x2c44e0:(4,5,0),0x2c4ba0:(4,2,0),0x2c4e28:(2,0,1),0x2c4888:(2,0,1),0x2c5348:(2,0,1),0x22a6e4:(0,0,0),0x2c6be0:(3,0,0),0x22a6d4:(2,0,0),0x22a6cc:(2,0,0),0x28a998:(1,0,0),0x2585c4:(1,0,0),0x2c361c:(3,0,0),0x2c3554:(3,0,0),0x2c35b8:(3,0,0),0x2c34e8:(3,0,0),0x22b410:(0,0,0),0x22a66c:(2,0,0),0x2c6674:(2,0,0),0x2c410c:(2,0,0),0x22b488:(0,0,0),0x22a614:(3,0,0)}
actual={0x2c42fc,0x2c4308,0x2c6700,0x2c6684,0x2c6414,0x2c6468,0x2c6348,0x2c2a84,0x2c62b4,0x2c6390,0x2c635c,0x2c6330,0x2c6224,0x2c6590,0x2c6378,0x2c6450,0x2c6420,0x2c6438,0x2c65a8,0x2c647c,0x2c6510,0x2c6638,0x2c4e28,0x2c4888,0x2c5348,0x28a998,0x2585c4,0x2c6674,0x2c410c}
import csv
function_entries={int(row[0],16) for row in csv.reader(Path('data/ver/eu/map.csv').open()) if row and row[0].startswith('0x') and 'f' in row[5]}
binary=Path('data/ver/eu/code.bin').read_bytes();candidate=Path(sys.argv[1] if len(sys.argv)>1 else 'build/sound-dispatch/candidate.bin').read_bytes()
assert hashlib.sha256(binary).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
assert hashlib.sha256(candidate).hexdigest()=='fb2879536ba82df12ee9ac4c3805bab631caf449ac90e55387649841478867e2'
def run(c,compiled):
 CMD=c.get('command_base',HEAP)
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176);u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000);u.reg_write(UC_ARM_REG_FPSCR,c.get('fpscr',0))
 u.mem_map(0x100000,0x400000);u.mem_write(0x100000,binary)
 if compiled:u.mem_write(ROOT,candidate)
 u.mem_map(HEAP,0x50000);u.mem_map(STOP,0x1000);u.mem_map(0x800000,0x40000)
 def w(a,*x):u.mem_write(a,struct.pack('<'+'I'*len(x),*[v&0xffffffff for v in x]))
 def r(a):return int.from_bytes(u.mem_read(a,4),'little')
 def b(a,x):u.mem_write(a,bytes([x&255]))
 def ret(value=0):u.reg_write(UC_ARM_REG_R0,value&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 w(OBJ,VT);w(TRACK,VT);w(VT,STOP+0x100,STOP+0x100,*[STOP+0x100+4*i for i in range(2,7)])
 for i in range(16):w(OBJ+0x88+4*i,TRACK if c.get('tracks',True) else 0)
 w(OBJ+0xec,1,1);w(OBJ+0xe20,4);b(OBJ+8,1)
 w(GUARD,c.get('guard',1));w(0x3f0980,1)
 ops=c.get('ops',[c.get('op',0)]);targets=[]
 for i,op in enumerate(ops):
  p=CMD+0x200*i;targets.append(p);w(p,CMD+0x200*(i+1) if i+1<len(ops) else 0);b(p+4,op)
  for off in range(0x10,0x1d0,4):w(p+off,c.get('value',0x12345678)^off)
  w(p+0x10,OBJ,3,0x3f400000,0x3e000000)
  if op==2:w(p+0x10,FLAG)
  if op==3:w(p+0x14,FLAG)
  if op in (6,7,9,10,11,17,38):w(p+0x14,c.get('small',0x80ff017f))
  if op==8:
   for off in range(0x14,0x3c,4):w(p+off,c.get('float_bits',0x3e800000)+off)
   w(p+0x28,0x1234)
  if op in (19,20,21):w(p+0x18,c.get('index',3));w(p+0x1c,c.get('short',0x87654321))
  if op==21:w(p+0x14,c.get('track_index',3))
  if op==33:w(p+0x18,c.get('index',1))
  if op==32:w(p+0x18,c.get('index',1));w(p+0x1c,c.get('float_bits',0x3e800000))
  if op in (44,45,46):w(p+0x14,c.get('mask',5))
  if 50<=op<=53:w(p+0x10,c.get('channel',1));w(p+0x14,FLAG);w(p+0x18,23)
  if op in (24,25,26,27,28,29,30,31,35):w(p+0x18,c.get('float_bits',0x3e800000));w(p+0x14,c.get('mask',3))
  if op==14:w(p+0x14,c.get('mode',0));w(p+0x18,c.get('delta',0x7fffffff))
  if op==23:w(p+0x18,c.get('small',0x80ff017f))
  if op==34:w(p+0x18,c.get('small',0x80ff017f))
  if c.get('alias_next') and i==0:w(p+0x10,p)
  if c.get('alias_object_payload') and i==0:w(p+0x10,p+4)
 if c.get('cycle'):w(targets[-1],targets[0])
 if c.get('null_target'):w(CMD+0x10,0)
 if c.get('null_next'):w(CMD,0x900000)
 entry=0 if c.get('null') else CMD
 u.reg_write(UC_ARM_REG_R0,entry);u.reg_write(UC_ARM_REG_SP,STACK);u.reg_write(UC_ARM_REG_LR,STOP)
 saved={x:0xa0000000+x for x in [UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11]+list(range(UC_ARM_REG_D8,UC_ARM_REG_D15+1))}
 for x,y in saved.items():u.reg_write(x,y)
 trace=[];seen=set();allseen=set();steps=0;minimum=STACK;counts=Counter();models=Counter();executed=Counter();writes=[]
 def hook(u,a,n,data):
  nonlocal steps,minimum
  steps+=1;allseen.add(a);minimum=min(minimum,u.reg_read(UC_ARM_REG_SP))
  if a==STOP:u.emu_stop();return
  if a in function_entries and a!=ROOT and (a not in spec or (c.get('native') and a in actual)):executed[a]+=1
  if (ROOT<=a<0x2bece8 or 0x2bedd4<=a<0x2bf110 or 0x2bf114<=a<0x2bf564) and not compiled:seen.add(a)
  if a in spec:
   counts[a]+=1
   nr,ns,nf=spec[a];args=[u.reg_read(x) for x in regs[:nr]]+[r(u.reg_read(UC_ARM_REG_SP)+i*4) for i in range(ns)]
   if nf:args.append(u.reg_read(UC_ARM_REG_S0))
   trace.append([a,args])
   if c.get('native') and a in actual:return
   models[a]+=1
   if a in (0x22a6e4,0x22b410,0x22b488):
    if c.get('mutate_getter'):w(CMD+0x10,FLAG);w(CMD+0x14,0x87654321)
    ret(HEAP+0x42000);return
   if a==0x28a998:
    value=int(r(args[0])==0)
    if value:w(args[0],1)
    ret(value);return
   if a==0x2585c4:
    u.mem_write(args[0],bytes(0x2a0))
    if c.get('mutate_getter'):w(CMD+0x10,2);w(CMD+0x14,HEAP+0x43000)
   elif a==0x2c6348:ret(TRACK if c.get('tracks',True) else 0);return
   else:
    # Deterministic external side effect lets later list operations observe calls.
    w(HEAP+0x44000,(r(HEAP+0x44000)+a+sum(args))&0xffffffff)
   ret(c.get('helper_return',1));return
  if STOP+0x100<=a<STOP+0x200:
   idx=(a-STOP-0x100)//4;args=[u.reg_read(UC_ARM_REG_R0)]
   if idx==6:args.append(u.reg_read(UC_ARM_REG_R1))
   trace.append(['virtual',idx,args]);models[a]+=1
   w(HEAP+0x44004,r(HEAP+0x44004)+idx)
   if c.get('mutate_virtual'):
    w(CMD+0x14,HEAP+0x43000)
    if c.get('change_next'):w(CMD,0)
   ret();return
 def onwrite(u,access,a,size,value,data):
  assert any(lo<=a and a+size<=hi for lo,hi in [(HEAP,HEAP+0x50000),(0x3e1000,0x431058),(0x800000,0x840000)]),('unobserved write',hex(a),size)
  if not 0x800000<=a<a+size<=0x840000:writes.append([a,size,value])
 u.hook_add(UC_HOOK_CODE,hook);u.hook_add(UC_HOOK_MEM_WRITE,onwrite)
 error=None
 try:u.emu_start(ROOT,STOP,count=c.get('limit',100000))
 except UcError as e:error=str(e)
 if not error and u.reg_read(UC_ARM_REG_PC)!=STOP:error='instruction budget'
 state={'trace':trace,'heap':hashlib.sha256(bytes(u.mem_read(HEAP,0x50000))).hexdigest(),'globals':hashlib.sha256(bytes(u.mem_read(0x3e1000,0x51000))).hexdigest(),'fpscr':u.reg_read(UC_ARM_REG_FPSCR),'sp':u.reg_read(UC_ARM_REG_SP)==STACK,'saved':all(u.reg_read(x)==y for x,y in saved.items()),'error':error}
 return state,{'steps':steps,'max_stack':STACK-minimum,'seen':sorted(seen),'allseen':sorted(allseen),'direct_counts':dict(counts),'model_counts':dict(models),'writes':writes,'executed_original_entries':dict(executed),'stop_pc':u.reg_read(UC_ARM_REG_PC)}
cases=[{'null':True}]
for op in range(256):
 for variant in range(3):cases.append({'op':op,'guard':[1,0,2][variant],'small':[0,0x80ff017f,0xff00ff80][variant],'float_bits':[0,0x80000000,0x7fc01234][variant],'tracks':variant!=2,'helper_return':[0,1,0xffffffff][variant]})
for op in (3,4,5,6,7):cases += [{'ops':[op,9],'mutate_virtual':True},{'ops':[op,9],'mutate_virtual':True,'change_next':True}]
for op in (47,48,49,50,51,52,53,54,57,58):cases.append({'op':op,'guard':0,'mutate_getter':True})
for op in (2,8,9,10,11,15,16,17,37,38):cases.append({'op':op,'alias_object_payload':True})
rng=random.Random(0x2becc8)
for i in range(80):cases.append({'ops':[rng.randrange(256) for _ in range(rng.randrange(1,48))],'guard':i%3})
# Independently execute supported original helper paths with real track objects.
for op in [8,14,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,44,45,46,50,51,52,53,55,56]:
 for variant in range(4):cases.append({'op':op,'native':True,'guard':[0,1,2,1][variant],'tracks':variant!=2,'mask':[0,1,0x80000000,0xffff][variant],'mode':variant,'small':[0,127,128,255][variant]})
for op in (50,51,52,53):cases.append({'op':op,'native':True,'guard':0,'command_base':GLOBAL})
for bits in (0x00000001,0x80000001,0x7f800000,0xff800000,0x7f800001,0xffc12345,0x3f800000,0xbf800000):
 for fpscr in (0,0x01000000,0x02000000):
  for op in (8,24,32,44):cases.append({'op':op,'native':True,'float_bits':bits,'fpscr':fpscr})
for op in (19,20,21):
 for value in (0,0x7fff,0x8000,0xffff,0x10000,0xffffffff):cases.append({'op':op,'native':True,'short':value})
for mode in (0,1,2,255):
 for delta in (0,1,0xffffffff,0x7fffffff,0x80000000):cases.append({'op':14,'native':True,'mode':mode,'delta':delta})
results=[];failures=[];coverage=set();all_original=set();counts=Counter();models=Counter();executed=Counter()
for i,c in enumerate(cases):
 a,ma=run(c,False);b,mb=run(c,True);equal=a==b
 results.append({'case':c,'equal':equal,'original_steps':ma['steps'],'compiled_steps':mb['steps'],'error':a['error']})
 if not equal:
  failures.append({'i':i,'case':c,'original':a,'compiled':b});print('FAIL',i,c,[k for k in a if a[k]!=b[k]],flush=True)
 coverage.update(ma['seen']);all_original.update(ma['allseen']);counts.update(ma['direct_counts']);models.update(ma['model_counts']);executed.update(ma['executed_original_entries'])
 assert ma['executed_original_entries']==mb['executed_original_entries']
report={'candidate_sha256':hashlib.sha256(candidate).hexdigest(),'case_count':len(cases),'equal_pairs':len(cases)-len(failures),'failures':failures,'results':results,'original_root_instruction_addresses':sorted(coverage),'original_all_instruction_addresses':sorted(all_original),'direct_counts':dict(counts),'model_counts':dict(models),'native_direct_whitelist':sorted(actual),'executed_original_entries':dict(executed),'valid_returning_pairs':sum(x['equal'] and not x['error'] for x in results),'matching_fault_pairs':sum(x['equal'] and bool(x['error']) for x in results)}
Path('build/sound-dispatch/replay.json').write_text(json.dumps(report,indent=2));print(json.dumps({k:v for k,v in report.items() if k in ['candidate_sha256','case_count','valid_returning_pairs','matching_fault_pairs']}));print('root instruction coverage',len(coverage));assert not failures

controls=[]
for c in [{'op':2,'null_target':True},{'op':9,'null_next':True},{'op':0,'cycle':True,'limit':1000}]:
 a,ma=run(c,False);b,mb=run(c,True);controls.append({'case':c,'original_error':a['error'],'compiled_error':b['error'],'same_error':a['error']==b['error'],'original_pc':ma['stop_pc'],'compiled_pc':mb['stop_pc'],'original_steps':ma['steps'],'compiled_steps':mb['steps'],'counted_as_returning':False})
Path('build/sound-dispatch/negative-controls.json').write_text(json.dumps(controls,indent=2))
```

## snapshot.py

SHA256 `2d17d888a9502300154011a90cf4e16e7dd4fb11bdc8a88d550c23225d4d8e2a`.

<!-- file: snapshot.py -->
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

## preservation.py

SHA256 `78c96468390175c74ececd8ceb0cb84cb66efeef704dfb83e3497d3e56ca6541`.

<!-- file: preservation.py -->
```python
import json,hashlib,subprocess
from pathlib import Path
out=Path('build/sound-dispatch');a=json.loads((out/'baseline-snapshot.json').read_text());b=json.loads((out/'final-snapshot.json').read_text());differences=[];normalized_file_paths=0
for p,v in a['objects'].items():
 for left,right in zip(v['symbols'],b['objects'][p]['symbols']):
  if left!=right and left[3]['type']==right[3]['type']=='STT_FILE':
   assert left[1:]==right[1:]
   assert left[0].replace('/workspace/scratch/73cdb2c524af/mario-main754/','<REPO>/')==right[0].replace(str(Path.cwd())+'/','<REPO>/')
   right[0]=left[0];normalized_file_paths+=1
 if b['objects'].get(p)!=v:differences.append({'object':p,'different_fields':[k for k,x in v.items() if b['objects'].get(p,{}).get(k)!=x]})
input_diff=[p for p,v in a['inputs'].items() if b['inputs'].get(p)!=v];added=sorted(set(b['objects'])-set(a['objects']));report=Path('/workspace/scratch/73cdb2c524af/mario-main754/build/dot-baseline-754/report.json');digest=hashlib.sha256(report.read_bytes()).hexdigest();assert digest=='c208f39fadf49e66c5c98c97b445881ba2e51ade07b5724610cf0309175ec028';r=json.loads(report.read_text());assert r['clean_build_returncode']==0 and len(r['prior_checks'])==753 and all(x['returncode']==0 for x in r['prior_checks'])
tracked=subprocess.check_output(['git','ls-tree','-r','754f99a','--','Game','lib','tools','data/config.json','data/ver/eu/map.csv'],text=True).splitlines();changed=[];gitlinks=[];blobs=0
for line in tracked:
 meta,p=line.split('\t');mode,kind,digest=meta.split()
 if kind=='commit':
  current=subprocess.check_output(['git','ls-tree','HEAD','--',p],text=True).split()[2];assert current==digest;gitlinks.append(p);continue
 assert kind=='blob';blobs+=1;base=subprocess.check_output(['git','show','754f99a:'+p]);now=Path(p).read_bytes()
 if base!=now:changed.append(p)
s={'base':'754f99a30a337756df5aa01c28b4ed6a977fecd5','baseline_report_sha256':hashlib.sha256(report.read_bytes()).hexdigest(),'baseline_roots':733,'baseline_actual_definitions':len(r['prior_checks']),'baseline_clean_build_returncode':r['clean_build_returncode'],'baseline_elapsed_seconds':r['elapsed_seconds'],'canonical_objects_compared':len(a['objects']),'normalized_STT_FILE_workspace_prefixes':normalized_file_paths,'input_hashes_compared':len(a['inputs']),'tracked_blob_files_compared':blobs,'unchanged_gitlinks':gitlinks,'object_differences':differences,'input_differences':input_diff,'tracked_differences':changed,'added_objects':added,'second_753_check_run':False};(out/'preservation.json').write_text(json.dumps(s,indent=2));print(json.dumps(s,indent=2));assert not differences and not input_diff and not changed;assert len(added)==1
```
