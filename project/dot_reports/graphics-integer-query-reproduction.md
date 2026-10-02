# Reproducing graphics integer query evidence

Use the committed source/header on frozen base
`56e9b4b89fe3c5d391c2948f1ce2b2c50706638e`, the owner's authorized local EU
`code.bin` / `exh.bin`, and the existing approved 902/791 compilers and wibo.
No game data is included here. Work in an isolated checkout. Do not run a checker
against a shared map concurrently. The pristine baseline comparison checkout is
expected at `../mario-main56e9` with its original clean build retained.

The scripts below are notes containing the complete test implementation. Extract
them to the ignored evidence directory from the repository root:

```sh
python3 - <<'PYEXTRACT'
from pathlib import Path
import re
text = Path('project/dot_reports/graphics-integer-query-reproduction.md').read_text()
out = Path('build/graphics-integer-query')
out.mkdir(parents=True, exist_ok=True)
blocks = re.findall(r'### ([a-z_]+\.py)\n\n```python\n(.*?)\n```', text, re.S)
assert len(blocks) == 7
for name, source in blocks:
    (out / name).write_text(source + '\n')
print('Extracted', len(blocks), 'scripts')
PYEXTRACT
```

Verify tool and oracle-input identities before building:

```sh
python3 - <<'PYPREFLIGHT'
from pathlib import Path
import hashlib
expected = {'data/ver/eu/map.csv': '0a618a11317104c7cae72a2c4ea48472a6959034cf421e51ca7f343f72026ddc', 'data/config.json': '5d41e617eb5b26374ed60b5383a0940ce0ce7d046344f7f815b7564144822c45', 'tools/check.py': 'e177e0abe130e645e75c5f0dc24abb19dbe83310de55f1f099ec8fc385e07317', 'tools/pypstem/stepBuild.py': 'b7be5977a0a1afc339eb4c9087b447cdc25b1c42daa3fe23af4b765008b0aac0', 'data/compilers/4.0/902/bin/armcc.exe': 'e6aca4ebc280a54065406bcc3a0ae3d9f4d0ac5a4032c3a193adb836c72e70fe', 'data/compilers/4.0/902/bin/armlink.exe': 'f511d2d087fe0cad6f604ff774b4d75476a7d4949c100ea082eac03a368e967d', 'data/compilers/4.0/902/bin/armar.exe': '167e21f8978742de70171cb44fde79f0e39f3521620de20a97eb16de9606a393', 'data/compilers/4.0/902/bin/fromelf.exe': 'aabb1ad390b8244c74580cb9fa4acacb2c838e318b8e13a62bccd97e5e79e930', 'data/compilers/4.1/791/bin/armcc.exe': 'd1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d', 'data/compilers/4.1/791/bin/armlink.exe': 'b9cffb71d7b58f3fd33fa8c4c8af884b75689bb5a9e9014309a08146bba676dc', 'data/compilers/4.1/791/bin/armar.exe': '68088194227d542623663a0a675a5d04edab06ae51117f42e6c5b60e65d01f45', 'data/compilers/4.1/791/bin/fromelf.exe': '01dd61d20003cd6ceea02f1e7880980aa34d954428e24a676187da3c54098f5e', 'data/compilers/wibo': 'aee836280b3d7c80c2031410219c907c9824c11be6b128d0744655de0f22280b'}
expected['data/ver/eu/code.bin'] = 'e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
expected['data/ver/eu/exh.bin'] = '94e61359c80498495dd77bb2df16f0def6fca94fca736dc8c3c929279b44d2c8'
for name, digest in expected.items():
    assert hashlib.sha256(Path(name).read_bytes()).hexdigest() == digest, name
print('Input and tool hashes verified')
PYPREFLIGHT
. ./development_environment.sh
python make.py eu -ca
python build/graphics-integer-query/audit_original.py
python build/graphics-integer-query/check.py
python build/graphics-integer-query/link.py
python build/graphics-integer-query/replay.py
python build/graphics-integer-query/preserve.py
python build/graphics-integer-query/declarations.py
python build/graphics-integer-query/final_audit.py
```

The canonical checker is expected to return status 1 for a 2,056/2,068-byte extent
mismatch. Its wrapper saves this status in `check-result.json`, restores the exact
map bytes even on failure, and finishes normally so diagnostics can continue.
`link.py` is explicitly a separate diagnostic link of the project-built C++
object, not a replacement canonical check. The normal image may use a scaffold
stub because the target remains U.

The replay runs the original and candidate roots and the actual original tail
helper with generated inputs. The arena and output bytes, globals, events and
FPSCR are compared; return paths additionally check SP and callee-saved integer
and VFP registers. No external callee is replaced by a result model. Details of
the 56 diagnostics and excluded input domains are in the main report. The final
suite's two directed-rounding overflow cases are intentionally outside its
66,894 ordinary returning pairs.

`preserve.py` verifies every old physical C++ object and separates scaffold
aliases, CFI records and provenance. Its fixed counts and base paths describe
this frozen proposal, not future main. `declarations.py` scans nearby `mario-*`
source trees; that optional cross-proposal inventory may differ elsewhere.

## Recorded final results

```json
{
  "build": {
    "start": "2026-10-02T08:20:59.200456+00:00",
    "end": "2026-10-02T08:21:19.727526+00:00",
    "seconds": 20.52708280900015,
    "returncode": 0
  },
  "canonical_check": {
    "returncode": 1,
    "stdout": "\u001b[38;5;221mU -> M: The complete compiled section, including its literal pool, has a different size from the original interval.\u001b[0m\u001b[K\n",
    "stderr": "",
    "map_restored": true,
    "map_sha256": "0a618a11317104c7cae72a2c4ea48472a6959034cf421e51ca7f343f72026ddc"
  },
  "replay": {
    "returning_pairs": 66894,
    "compared_return_bytes": 2192517744,
    "events": 72,
    "max_instructions": 81,
    "candidate_bytes": 2056,
    "candidate_sha256": "31f203fe10ba82acfbc8198320364672b37730286ac2c008077dd19f81c667d3",
    "seconds": 14.319865271999788,
    "supported_selectors": [
      2884,
      2885,
      2886,
      2928,
      2929,
      2930,
      2931,
      2932,
      2960,
      2961,
      2962,
      2963,
      2964,
      2965,
      2966,
      2967,
      2968,
      2978,
      3042,
      3056,
      3058,
      3088,
      3089,
      3106,
      3107,
      3379,
      3386,
      3408,
      3410,
      3411,
      3412,
      3413,
      3414,
      3415,
      10752,
      26113,
      26126,
      26127,
      26240,
      26241,
      26242,
      26243,
      26244,
      26245,
      26246,
      26247,
      26248,
      26249,
      26250,
      26251,
      26252,
      26253,
      26254,
      26255,
      26256,
      26257,
      26258,
      26259,
      26260,
      26261,
      26262,
      26263,
      26264,
      26265,
      26266,
      26267,
      26268,
      26269,
      26270,
      26271,
      26497,
      26498,
      26504,
      26625,
      32773,
      32777,
      32823,
      32824,
      32873,
      32968,
      32969,
      32970,
      32971,
      34016,
      34024,
      34068,
      34076,
      34466,
      34467,
      34877,
      34921,
      34964,
      34965,
      35661,
      35725,
      35738,
      35739,
      36006,
      36007,
      36344,
      36345,
      36346
    ]
  },
  "diagnostics": {
    "pairs": 56,
    "returning": 48,
    "memory_faults": 8,
    "divergences": 0,
    "model_rejections": 0,
    "budget_exhaustions": 0
  },
  "preservation": {
    "base": "56e9b4b89fe3c5d391c2948f1ce2b2c50706638e",
    "object_count": 183,
    "input_count": 376,
    "all_baseline_tracked_files": 633,
    "total_objects_with_addition": 185,
    "prior_canonical_cpp_objects": 183,
    "all_equal": true,
    "generated_stub_delta_verified": true,
    "compact_image_equal": true,
    "excluded_difference": "Only repository prefix in STT_FILE absolute source path and in non-allocated .comment, and resulting raw symbol/string table indices; all other symbols, section data, attributes and resolved relocations compare equal"
  }
}
```

## Complete scripts

### audit_original.py

```python
from pathlib import Path
from capstone import *
from capstone.arm import *
import csv,struct,json,hashlib
out=Path('build/graphics-integer-query');raw=Path('data/ver/eu/code.bin').read_bytes();assert hashlib.sha256(raw).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
start,end=0x38f9f0,0x390204;islands=[(0x38fcec,0x38fd14),(0x38fd3c,0x38fd70),(0x38fdb0,0x38fde4),(0x3901d8,end)]
md=Cs(CS_ARCH_ARM,CS_MODE_ARM);md.detail=True
ins={}
for a in range(start,end,4):
 if any(x<=a<y for x,y in islands):continue
 items=list(md.disasm(raw[a-0x100000:a-0x100000+4],a));assert len(items)==1;ins[a]=items[0]
branch_targets=[];literals=[]
for a,i in ins.items():
 if i.mnemonic.startswith('b') and i.mnemonic!='bic' and i.operands[0].type==ARM_OP_IMM:
  dst=i.operands[0].imm;assert dst in ins or dst==0x377dac,(hex(a),hex(dst));branch_targets.append([a,dst,i.mnemonic])
 for o in i.operands:
  if o.type==ARM_OP_MEM and o.mem.base==ARM_REG_PC and o.mem.index==0:
   target=a+8+o.mem.disp;assert any(x<=target<y for x,y in islands),(hex(a),hex(target));literals.append([a,target,struct.unpack_from('<I',raw,target-0x100000)[0]])
rows=[{k.strip():v.strip() for k,v in x.items()} for x in csv.DictReader(open('data/ver/eu/map.csv'))]
callers=[]
for off in range(0,len(raw)-4,4):
 word=struct.unpack_from('<I',raw,off)[0]
 if word&0x0f000000==0x0b000000:
  d=word&0xffffff;d=d-(1<<24) if d&0x800000 else d
  if off+0x100008+d*4==start:
   a=off+0x100000;row=next((x for x in rows if int(x['Start'],16)<=a<int(x['End'],16)),None);callers.append({'address':a,'row':row,'context':[(i.address,i.mnemonic,i.op_str) for i in md.disasm(raw[off-16:off+16],a-16)]})
for a,b in islands[1:3]:
 for p in range(a,b,4):assert struct.unpack_from('<I',raw,p-0x100000)[0] in ins
r={'start':start,'end':end,'whole_bytes':end-start,'root_sha256':hashlib.sha256(raw[start-0x100000:end-0x100000]).hexdigest(),'instruction_count':len(ins),'islands':islands,'literals':literals,'branch_targets':branch_targets,'direct_calls':[(a,i.mnemonic,i.op_str) for a,i in ins.items() if i.mnemonic in ['bl','blx']],'external_tail':[[a,b,m] for a,b,m in branch_targets if b not in ins],'callers':callers,'helper_pool':[(hex(a),hex(struct.unpack_from('<I',raw,a-0x100000)[0])) for a in range(0x377ea4,0x377eb0,4)]}
(out/'original-audit.json').write_text(json.dumps(r,indent=2));print(json.dumps({k:v for k,v in r.items() if k not in ['literals','branch_targets','callers']},indent=2));print('callers',callers)
```

### check.py

```python
from pathlib import Path
import subprocess,sys,hashlib,json
p=Path('data/ver/eu/map.csv'); original=p.read_bytes(); old=b'0x0038F9F0,0x003901D8,0x00390204,          ,U,f,,'; assert original.count(old)==1
try:
 p.write_bytes(original.replace(old,b'0x0038F9F0,0x003901D8,0x00390204,          ,U,f,fn_0038F9F0,'))
 c=subprocess.run([sys.executable,'tools/check.py','fn_0038F9F0','--object','build/eu/obj/lib/CtrSDK/sources/gx_IntegerQuery.o'],capture_output=True,text=True)
 print(c.stdout,c.stderr);Path('build/graphics-integer-query/check-current.log').write_text(c.stdout+c.stderr)
finally:p.write_bytes(original)
assert p.read_bytes()==original
Path('build/graphics-integer-query/check-result.json').write_text(json.dumps({'returncode':c.returncode,'stdout':c.stdout,'stderr':c.stderr,'map_restored':True,'map_sha256':hashlib.sha256(original).hexdigest()},indent=2))
```

### link.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
import csv,subprocess,os,json,hashlib
root=Path.cwd();out=root/'build/graphics-integer-query';out.mkdir(exist_ok=True)
obj=root/'build/eu/obj/lib/CtrSDK/sources/gx_IntegerQuery.o'
rows=[{k.strip():v.strip() for k,v in x.items()} for x in csv.DictReader((root/'data/ver/eu/map.csv').open())]
syms={x['Symbol']:int(x['Start'],16) for x in rows if x['Symbol']}
with obj.open('rb') as f:
 e=ELFFile(f);names=[s.name for s in e.get_section_by_name('.symtab').iter_symbols() if s['st_shndx']=='SHN_UNDEF' and s.name and not s.name.startswith('Lib$$')]
 lines=[]
 for n in names:
  a=int(n.split('_')[1],16) if n.startswith(('fn_','dat_')) else syms[n]
  kind = 'd' if n.startswith('dat_') else 'f'
  assert any(int(row['Start'],16)==a and kind in row['Type'] for row in rows), (n,hex(a))
  lines.append(f'0x{a:08X} {"D" if n.startswith("dat_") else "A"} {n}')
(out/'symbols.sym').write_text('#<SYMDEFS>#\n'+'\n'.join(lines)+'\n')
(out/'candidate.sct').write_text('CANDIDATE_LOAD 0x0038F9F0\n{\n CANDIDATE_CODE 0x0038F9F0\n {\n  gx_IntegerQuery.o (i.fn_0038F9F0, +FIRST)\n  gx_IntegerQuery.o (+RO)\n }\n}\n')
cmd=[str(root/'data/compilers/wibo'),str(root/'data/compilers/4.0/902/bin/armlink.exe'),'--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--inline','--datacompressor=off','--no_debug','--no_scanlib','--mangled','--symbols','--map','--entry=fn_0038F9F0','--keep=fn_0038F9F0',f'--scatter={out}/candidate.sct',f'--output={out}/candidate.axf',f'--list={out}/candidate.map',str(obj),str(out/'symbols.sym')]
p=subprocess.run(cmd,env={**os.environ,'TMP':'/tmp'},stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True);print(p.stdout);assert p.returncode==0
print('Diagnostic root link only; canonical checker ran separately.')
```

### replay.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
import struct,json,random,hashlib,time
OUT=Path('build/graphics-integer-query');BINARY=Path('data/ver/eu/code.bin').read_bytes();BASE=0x38f9f0;END=0x71000000;DB=0x1000000;ST=0x70000000;SP=ST+0x8000;STATE=DB;REGISTRY=DB+0x1000;BUFFER=DB+0x2000;COLOR=DB+0x3000;DEPTH=DB+0x4000;OTHER=DB+0x5000;OUTPUT=DB+0x7000
with (OUT/'candidate.axf').open('rb') as f:
 elf=ELFFile(f);CANDIDATE=next(s.data() for s in elf.iter_sections() if s['sh_addr']==BASE and s['sh_size'])
assert hashlib.sha256(BINARY).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
REGS=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_R12]
def words(*xs):return struct.pack('<'+'I'*len(xs),*[x&0xffffffff for x in xs])
class Engine:
 def __init__(self,candidate):
  self.candidate=candidate;u=self.u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176)
  u.mem_map(0x100000,0x400000);u.mem_write(0x100000,BINARY)
  if candidate:u.mem_write(BASE,CANDIDATE)
  u.mem_map(DB,0x10000);u.mem_map(ST,0x10000);u.mem_map(END,0x1000)
  u.reg_write(UC_ARM_REG_C1_C0_2,15<<20);u.reg_write(UC_ARM_REG_FPEXC,1<<30)
  u.hook_add(UC_HOOK_CODE,self.code);u.hook_add(UC_HOOK_MEM_INVALID,self.invalid)
 def code(self,u,pc,size,user):
  self.instructions+=1
  if pc==END:self.returned=True;u.emu_stop();return
  if pc==0x377dac:self.events.append([pc,u.reg_read(REGS[0]),u.reg_read(REGS[1])])
  if BASE<=pc<BASE+(len(CANDIDATE) if self.candidate else 2068):self.coverage.add(pc)
  elif not 0x377dac<=pc<0x377ea4:raise RuntimeError(f'unexpected code {pc:08x}')
 def invalid(self,u,access,address,size,value,user):self.fault=[access,address,size];return False
 def setup(self,seed=0,unit=0,float_bits=None,flags=None,depth=24,null_color=False,null_depth=False,null_other=False,state_null=False):
  r=random.Random(seed);data=bytearray(r.randbytes(0x8000))
  def w(p,*xs):data[p-DB:p-DB+len(xs)*4]=words(*xs)
  w(STATE+0x58,unit)
  for i,off in enumerate([0x40,0x44,0x4c,0x50,0x5a0]):w(STATE+off,*([float_bits] if float_bits is not None else [struct.unpack('<I',struct.pack('<f',[-255.75,-.75,0,35.5,2147483520.0][i]))[0]]))
  for off in [0x560,0x590]:
   for i in range(4):w(STATE+off+4*i,*([float_bits] if float_bits is not None else [struct.unpack('<I',struct.pack('<f',[0,.1,.5,.99999994][i]))[0]]))
  if flags is not None:
   for off in [0x3c,0x54,0x578,0x57a,0x57b,0x57c,0x57d,0x584,0x585,0x586,0x587,0x588,0x5c3]:data[off]=flags
  w(REGISTRY+8,BUFFER);w(REGISTRY+12,0 if null_other else OTHER);w(BUFFER+12,0 if null_color else COLOR);w(BUFFER+28,0 if null_depth else DEPTH);w(DEPTH+0x38,depth)
  self.initial=bytes(data);self.u.mem_write(DB,self.initial);self.u.mem_write(0x3e3154,words(0 if state_null else STATE));self.u.mem_write(0x3e2e3c,words(REGISTRY));self.saved_globals=bytes(self.u.mem_read(0x3e2e3c,4))+bytes(self.u.mem_read(0x3e3154,4))
 def run(self,query,fpscr=0,output=OUTPUT):
  u=self.u;u.mem_write(DB,self.initial);self.events=[];self.instructions=0;self.coverage=set();self.fault=None;self.returned=False
  for i,r in enumerate(REGS):u.reg_write(r,0xabc00000+i)
  for i in range(8,16):u.reg_write(UC_ARM_REG_D0+i,0x5eed000000000000+i)
  u.reg_write(REGS[0],query);u.reg_write(REGS[1],output);u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,END);u.reg_write(UC_ARM_REG_FPSCR,fpscr)
  try:u.emu_start(BASE,END+4,count=10000)
  except UcError:
   if self.fault is None:raise
  if self.returned:
   assert u.reg_read(UC_ARM_REG_SP)==SP
   assert [u.reg_read(r) for r in REGS[4:12]]==[0xabc00000+i for i in range(4,12)]
   assert [u.reg_read(UC_ARM_REG_D0+i) for i in range(8,16)]==[0x5eed000000000000+i for i in range(8,16)]
  memory=bytes(u.mem_read(DB,0x8000));g=bytes(u.mem_read(0x3e2e3c,4))+bytes(u.mem_read(0x3e3154,4));assert g==self.saved_globals
  return {'memory':memory,'events':self.events,'fpscr':u.reg_read(UC_ARM_REG_FPSCR),'fault':self.fault,'returned':self.returned}
def main():
 start=time.monotonic();engines=[Engine(False),Engine(True)];records=[];coverage=set();maxins=0;comparisons=0;events=0;returning=0;diagnostics=[];supported=[]
 def pair(query,label,fpscr=0,output=OUTPUT,diagnostic=False):
  nonlocal maxins,comparisons,events,returning
  results=[e.run(query,fpscr,output) for e in engines];a,b=results
  coverage.update(engines[0].coverage);maxins=max(maxins,*[e.instructions for e in engines])
  equal=a==b
  if diagnostic:
   diagnostics.append({'label':label,'query':query,'equal':equal,'faults':[a['fault'],b['fault']],'returned':[a['returned'],b['returned']],'fpscr':[a['fpscr'],b['fpscr']],'events_equal':a['events']==b['events'],'memory_equal':a['memory']==b['memory']});return
  if not equal:
   failure={'label':label,'query':query,'fpscr':fpscr,'different':{k:[a[k],b[k]] for k in a if a[k]!=b[k] and k!='memory'},'memory_differences':[(hex(DB+i),x,y) for i,(x,y) in enumerate(zip(a['memory'],b['memory'])) if x!=y][:20]};(OUT/'replay-failure.json').write_text(json.dumps(failure,indent=2));raise AssertionError(failure)
  assert a['returned'] and not a['fault'];returning+=1;comparisons+=len(a['memory'])+8;events+=len(a['events'])
  if a['memory']!=engines[0].initial or a['events']:supported.append(query)
  records.append([label,query,hashlib.sha256(a['memory']).hexdigest(),a['fpscr'],len(a['events'])])
 for e in engines:e.setup()
 # Every 16-bit query, plus high signed/unsigned boundaries, runs the entire root.
 for query in range(65536):pair(query,'census')
 support=sorted(set(supported));print('Census passed',len(support),'supported selectors',flush=True)
 for seed in range(8):
  for e in engines:e.setup(seed=seed+1,unit=seed%3,flags=[0,1,2,255][seed%4],depth=[0,16,24,32][seed%4],null_color=seed==4,null_depth=seed==5,null_other=seed==6)
  for q in support+[0x7fffffff,0x80000000,0xffffffff,0x10000,0xffff0000]:pair(q,f'state-{seed}')
 # Finite convertible float domain. Normalized values are bounded to [-1,1).
 float_queries=[0xb73,0x2a00,0x8038,0xb70,0xc22,0x8005]
 bits=[0,0x80000000,1,0x80000001,0x007fffff,0x00800000,0x3dcccccd,0x3f000000,0x3f7fffff,0xbf000000,0xbf7fffff,0xbf800000]
 for b in bits:
  for e in engines:e.setup(float_bits=b)
  for fpscr in [0,1<<22,2<<22,3<<22,1<<24,1<<25,(1<<24)|(1<<25)]:
   for q in float_queries:pair(q,f'float-{b:08x}',fpscr,diagnostic=(b==0xbf800000 and fpscr==(2<<22) and q in [0xc22,0x8005]))
 # Out-of-domain machine behavior and faults are diagnostics, not C++ portability claims.
 for b in [0x3f800000,0x4f000000,0xcf000001,0x7f800000,0xff800000,0x7fc00001,0x7f800001]:
  for e in engines:e.setup(float_bits=b)
  for q in float_queries:pair(q,f'float-diagnostic-{b:08x}',diagnostic=True)
 for e in engines:e.setup()
 for q in [0xb45,0xb70,0xc22,0xd56,0x8ca6,0]:pair(q,'null-output',output=0,diagnostic=True)
 for e in engines:e.setup(state_null=True)
 for q in [0xb45,0xb70,0xc22,0xd56,0x8ca6,0]:pair(q,'null-state',diagnostic=True)
 r={'returning_pairs':returning,'compared_return_bytes':comparisons,'events':events,'max_instructions':maxins,'candidate_bytes':len(CANDIDATE),'candidate_sha256':hashlib.sha256(CANDIDATE).hexdigest(),'seconds':time.monotonic()-start,'supported_selectors':support,'original_instruction_coverage':sorted(coverage),'diagnostics':diagnostics,'records':records}
 (OUT/'replay-results.json').write_text(json.dumps(r,indent=2));print(json.dumps({k:v for k,v in r.items() if k not in ['records','original_instruction_coverage','diagnostics','supported_selectors']},indent=2));print('diagnostic divergences',sum(not x['equal'] for x in diagnostics),flush=True)
if __name__=='__main__':main()
```

### preserve.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
import json,hashlib,subprocess,collections
root=Path.cwd();base=root.parent/'mario-main56e9';out=root/'build/graphics-integer-query'
def normalized(p):
 e=ELFFile(open(p,'rb'));sections=[];symbols=[]
 def symrec(s):
  n=s['st_shndx'];sec=e.get_section(n).name if isinstance(n,int) else n
  name=s.name.replace(str(root),'<repo>').replace(str(base),'<repo>') if s['st_info']['type']=='STT_FILE' else s.name
  return [name,s['st_info']['type'],s['st_info']['bind'],s['st_other']['visibility'],sec,s['st_value'],s['st_size']]
 for sec in e.iter_sections():
  if isinstance(sec,RelocationSection):
   tab=e.get_section(sec['sh_link']);sections.append([sec.name,[(r['r_offset'],r['r_info_type'],symrec(tab.get_symbol(r['r_info_sym']))) for r in sec.iter_relocations()]])
  elif sec['sh_type'] not in ('SHT_SYMTAB','SHT_STRTAB','SHT_NULL'):
   sections.append([sec.name,sec['sh_type'],sec['sh_flags'],sec['sh_addralign'],sec['sh_entsize'],hashlib.sha256(sec.data().replace(str(root).encode(),b'<repo>').replace(str(base).encode(),b'<repo>') if sec.name=='.comment' else sec.data()).hexdigest()])
 tab=e.get_section_by_name('.symtab')
 for s in tab.iter_symbols():
  symbols.append(symrec(s))
 return {'sections':sections,'symbols':symbols}
records=[];inputs={}
old_objects={p.relative_to(base/'build/eu/obj') for p in (base/'build/eu/obj').rglob('*.o')}
new_objects={p.relative_to(root/'build/eu/obj') for p in (root/'build/eu/obj').rglob('*.o')}
assert new_objects-old_objects=={Path('lib/CtrSDK/sources/gx_IntegerQuery.o')}
assert not old_objects-new_objects
sealed={}
for name in subprocess.check_output(['git','ls-files'],cwd=base,text=True).splitlines():
 p=base/name;q=root/name
 if p.is_file():
  assert q.is_file(),name
  a=hashlib.sha256(p.read_bytes()).hexdigest();b=hashlib.sha256(q.read_bytes()).hexdigest();assert a==b,name;sealed[name]=a
(out/'baseline-tracked-inputs.json').write_text(json.dumps(sealed,indent=2,sort_keys=True))
for p in sorted((root/'build/eu/obj').rglob('*.o')):
 rel=p.relative_to(root/'build/eu/obj')
 if rel.parts[0] not in ('Game','lib') or p.stem=='gx_IntegerQuery':continue
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
assert len(records)==183
# The generated scaffold gains only the six expected address aliases.
stub=Path('build/eu/obj/build/eu/split/stubs.o');a=normalized(root/stub);b=normalized(base/stub)
sa={x[0]:x for x in a['sections'] if x[0] not in ('.debug_frame','.rel.debug_frame')};sb={x[0]:x for x in b['sections'] if x[0] not in ('.debug_frame','.rel.debug_frame')}
expected_functions={'i.'+x for x in ['fn_00377DAC','fn_0038F9F0']}
expected_data={'.sdata_'+x for x in ['dat_003E3154']}
expected=expected_functions|expected_data
assert set(sa)-set(sb)==expected
assert not set(sb)-set(sa)
for n in sb:assert sa[n]==sb[n],n
def debug_frames(path):
 elf=ELFFile(open(path,'rb'));frames={}
 for sec in elf.iter_sections():
  if isinstance(sec,RelocationSection) and sec.name=='.rel.debug_frame':
   tab=elf.get_section(sec['sh_link']);rels=[];owner=None
   for rel in sec.iter_relocations():
    sym=tab.get_symbol(rel['r_info_sym']);name=sym.name
    if name.startswith('i.'):owner=name
    if name.startswith('__ARM_grp_.debug_frame$'):name='<frame-CIE>'
    ndx=sym['st_shndx'];target=elf.get_section(ndx).name if isinstance(ndx,int) else ndx
    rels.append([rel['r_offset'],rel['r_info_type'],name,target,sym['st_value'],sym['st_size']])
   assert owner and owner not in frames
   frame=elf.get_section(sec['sh_info']);frames[owner]=[hashlib.sha256(frame.data()).hexdigest(),rels]
 return frames
fa=debug_frames(root/stub);fb=debug_frames(base/stub)
assert set(fa)-set(fb)==expected_functions
assert not set(fb)-set(fa)
for n in fb:assert fa[n]==fb[n],n+' debug frame'
ca=collections.Counter(json.dumps(x) for x in a['symbols']);cb=collections.Counter(json.dumps(x) for x in b['symbols']);assert not cb-ca
added=[json.loads(x) for x in (ca-cb).elements()]
debug_symbols={x[0] for x in added if x[0].startswith('__ARM_grp_.debug_frame$')}
assert len(debug_symbols)==len(expected_functions)
assert all(x[4] in expected or x[0] in debug_symbols for x in added),added
assert {x[0] for x in added if x[4] not in expected}==debug_symbols
assert {x[0] for x in added if x[1]=='STT_FUNC'}=={x[2:] for x in expected_functions}
oldsource=(base/'build/eu/split/stubs.c').read_text();newsource=(root/'build/eu/split/stubs.c').read_text()
for n in sorted(expected_functions):
 sym=n[2:];address=sym.split('_')[1];kind='STUB' if sym=='fn_0028D1F0' else 'STUB_G'
 addition=f'/* Scaffold alias for unnamed function at 0x{address}. */\n{kind}({sym});\n';assert newsource.count(addition)==1;newsource=newsource.replace(addition,'')
for symbol,size in [('dat_003E3154',4)]:
 address=symbol.split('_')[1];addition=f'/* Zero-filled scaffold data at 0x{address}, not reconstructed data. */\n__weak __attribute__((section(".sdata_{symbol}"), aligned(4))) unsigned char {symbol}[{size}] = {{0}};\n';assert newsource.count(addition)==1;newsource=newsource.replace(addition,'')
assert oldsource==newsource
psa=json.loads((root/stub).with_suffix('.provenance.json').read_text());psb=json.loads((base/stub).with_suffix('.provenance.json').read_text())
assert psa['compiler_sha256']==psb['compiler_sha256']
assert [x.replace(str(root),'<repo>') for x in psa['command']]==[x.replace(str(base),'<repo>') for x in psb['command']]
assert set(psa['inputs'])==set(psb['inputs'])
for key in psa['inputs']:
 assert hashlib.sha256((root/key).read_bytes()).hexdigest()==psa['inputs'][key]
 assert hashlib.sha256((base/key).read_bytes()).hexdigest()==psb['inputs'][key]
 if key!='build/eu/split/stubs.c':assert psa['inputs'][key]==psb['inputs'][key]
stub_delta={'all_prior_sections_symbols_unchanged':True,'old_debug_frame_count':len(fb),'new_debug_frame_count':len(fa),'debug_normalization':'Compare every CFI byte and resolved relocation per function; compiler-generated frame-CIE ordinal names and section-table indices normalize only to that per-function association','added_sections':sorted(expected),'added_symbols':added,'old_generated_source_sha256':psb['inputs']['build/eu/split/stubs.c'],'new_generated_source_sha256':psa['inputs']['build/eu/split/stubs.c']}
(out/'generated-stub-delta.json').write_text(json.dumps(stub_delta,indent=2))
assert (root/'build/eu/code.bin').read_bytes()==(base/'build/eu/code.bin').read_bytes()
result={'base':'56e9b4b89fe3c5d391c2948f1ce2b2c50706638e','object_count':len(records),'input_count':len(inputs),'all_baseline_tracked_files':len(sealed),'total_objects_with_addition':len(new_objects),'prior_canonical_cpp_objects':sum(1 for x in old_objects if x.parts[0] in ('Game','lib')),'all_equal':True,'generated_stub_delta_verified':True,'compact_image_equal':True,'excluded_difference':'Only repository prefix in STT_FILE absolute source path and in non-allocated .comment, and resulting raw symbol/string table indices; all other symbols, section data, attributes and resolved relocations compare equal','objects':records,'inputs':inputs}
(out/'preservation.json').write_text(json.dumps(result,indent=2));print(json.dumps({k:v for k,v in result.items() if k not in ('objects','inputs')},indent=2))
```

### declarations.py

```python
from pathlib import Path
import json,re,hashlib
needle=re.compile(r'dat_003E3154|fn_00377DAC|fn_0038F9F0|struct RenderControl')
r=[];repos=[]
for repo in sorted(Path.cwd().parent.glob('mario-*')):
 if not repo.is_dir():continue
 repos.append(repo.name)
 for prefix in ['lib','Game']:
  for p in (repo/prefix).rglob('*'):
   if p.suffix not in ('.h','.hpp','.cpp','.c'):continue
   text=p.read_text(errors='replace');matches=[[i,line] for i,line in enumerate(text.splitlines(),1) if needle.search(line)]
   if matches:r.append({'repository':repo.name,'file':str(p.relative_to(repo)),'sha256':hashlib.sha256(p.read_bytes()).hexdigest(),'matches':matches})
Path('build/graphics-integer-query/declarations.json').write_text(json.dumps({'repositories':repos,'files':r},indent=2));print('repos',len(repos),'matching files',len(r))
for x in r:
 if x['repository'] in ['mario-main56e9','mario-graphics-integration','mario-texture-binding-state','mario-graphics-integer-query'] or any('fn_00377DAC' in line or 'fn_0038F9F0' in line for _,line in x['matches']):print(x)
```

### final_audit.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
import hashlib,json,subprocess
root=Path.cwd();out=root/'build/graphics-integer-query';obj=root/'build/eu/obj/lib/CtrSDK/sources/gx_IntegerQuery.o'
paths=['lib/CtrSDK/include/retail/GraphicsIntegerQuery.h','lib/CtrSDK/sources/gx_IntegerQuery.cpp','data/ver/eu/map.csv','data/config.json','data/ver/eu/config.json','tools/check.py','tools/pypstem/stepBuild.py','build/eu/obj/lib/CtrSDK/sources/gx_IntegerQuery.o','build/eu/obj/lib/CtrSDK/sources/gx_IntegerQuery.provenance.json','build/eu/code.bin','build/graphics-integer-query/candidate.axf','data/compilers/4.0/902/bin/armcc.exe','data/compilers/4.0/902/bin/armlink.exe','data/compilers/4.0/902/bin/armar.exe','data/compilers/4.0/902/bin/fromelf.exe','data/compilers/4.1/791/bin/armcc.exe','data/compilers/4.1/791/bin/armlink.exe','data/compilers/4.1/791/bin/armar.exe','data/compilers/4.1/791/bin/fromelf.exe','data/compilers/wibo']
with obj.open('rb') as f:
 elf=ELFFile(f);functions=[(s.name,s['st_size']) for s in elf.get_section_by_name('.symtab').iter_symbols() if s['st_info']['type']=='STT_FUNC' and s['st_size']];sections=[(s.name,s['sh_size']) for s in elf.iter_sections() if s.name.startswith('i.')];imports=[s.name for s in elf.get_section_by_name('.symtab').iter_symbols() if s['st_shndx']=='SHN_UNDEF' and s.name and not s.name.startswith('Lib$$')]
r={'base':'56e9b4b89fe3c5d391c2948f1ce2b2c50706638e','source_commit':subprocess.check_output(['git','log','-1','--format=%H','--','lib/CtrSDK/sources/gx_IntegerQuery.cpp'],text=True).strip(),'functions':functions,'sections':sections,'imports':imports,'absent_optional_inputs':[p for p in paths if not (root/p).exists()],'hashes':{p:hashlib.sha256((root/p).read_bytes()).hexdigest() for p in paths if (root/p).exists()}}
(out/'final-audit.json').write_text(json.dumps(r,indent=2));print(json.dumps(r,indent=2))
```

## Optional signature compatibility check

The local command used the actual neighboring completed proposal headers and
host C++ syntax checking. It does not link or adopt either proposal:

```cpp
#include "../../../mario-graphics-integration/lib/CtrSDK/include/retail/GraphicsGlobals.h"
#include "../../../mario-texture-binding-state/lib/CtrSDK/include/retail/GraphicsBindingState.h"
#include <retail/GraphicsIntegerQuery.h>
extern "C" retail_graphics::RenderControl* dat_003E3154;
extern "C" void fn_0038F9F0(unsigned, int*);
extern "C" void fn_00377DAC(unsigned, int*);

```

```sh
c++ -fsyntax-only -I lib/CtrSDK/include build/graphics-integer-query/signature-compatibility.cpp
```

## Retained unsuccessful checkpoints

Ignored evidence includes `form1.o`, `form2.o`, `form3.o`, each canonical checker
result, the form-one replay failure, the complete form-two and form-three replay
results, each physical build log, final clean-build timing, and final replay and
preservation outputs. Form one swapped selector 8B9A/8B9B constants. Form three
changed scalar output control flow and remained a size mismatch. The final source
restores the tested form-two implementation, with no fourth tuning attempt.

The source-generator directory failure happened before any source existed and
was followed by a pristine build. The final manifest's first attempt failed when
an optional version config was absent; the final script explicitly records absent
optional inputs. These failures are setup/reporting history and add no credit.
