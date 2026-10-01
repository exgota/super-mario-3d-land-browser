# Root 0x00230794: reproducible ARM replay appendix

This appendix rebuilds committed ordinary C++ and compares it with the owner's verified executable. It does not contain game bytes. The exact claim remains **NonMatching**: the project checker rejects the 3,252-byte candidate against the unchanged 3,748-byte interval. The shorter diagnostic link below is deliberately separate from that check.

Use source commit `2e16a8b` (or this report's descendant with those source files unchanged), ARMCC 4.1/791, the repository's existing toolchain and original map, and the locally held verified EU code.bin/exh.bin. Run from the repository root. The Python replay requires Unicorn 2.1.4 and pyelftools. No tools, configuration, map boundaries, ranks, or binary inputs are modified by the committed proposal. The temporary row-name change below is restored with `finally`.

Save the first Python block as `build/dot_230794/prepare.py`, and the second as `build/dot_230794/replay.py`. First create that ignored directory. Then run:

```sh
. ./development_environment.sh
export DEVKITARM=/usr
mkdir -p build/dot_230794
python make.py eu -ca
python build/dot_230794/prepare.py
python build/dot_230794/replay.py --scope modeled --output build/dot_230794/modeled_final.json
python build/dot_230794/replay.py --scope original --output build/dot_230794/original_final.json
python build/dot_230794/replay.py --scope caller --output build/dot_230794/caller_final.json
```

## Canonical verification and diagnostic link

The unchanged project `check.py --object` verifies committed-source provenance first and reports `U -> M` for the size mismatch. `_isolate_function` below is the project's existing source-object linker helper; it takes the canonical ARMCC output and resolves only imports with established map rows. No object edits, guessed helper address, retail instruction input, or source-closure workaround is used. All ordinary inline C++ helpers have already disappeared in the canonical object.

```python
import hashlib, json, subprocess, sys
from pathlib import Path
sys.path.insert(0,str(Path.cwd()))
from tools.low.checkExactBytes import _read_map, _isolate_function
from elftools.elf.elffile import ELFFile
out=Path('build/dot_230794');out.mkdir(parents=True,exist_ok=True)
expected={
 'data/ver/eu/code.bin':'e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64',
 'data/compilers/4.1/791/bin/armcc.exe':'d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d',
 'lib/al/src/ResourceTagBuilder230794.cpp':'2cb80a07e91871cfad94b541e8a1629d79a366aacb40cb69e2a5d76faeaeeec3',
 'lib/al/include/Resource/ResourceTagBuilder230794.h':'3804fe6b022a1e6a9a6fb918be91f73683f69c1a506c8940db641242fa791e99',
}
for name,digest in expected.items():assert hashlib.sha256(Path(name).read_bytes()).hexdigest()==digest,name
obj=Path('build/eu/obj/lib/al/src/ResourceTagBuilder230794.o')
p=Path('data/ver/eu/map.csv');original=p.read_bytes();line=next(x for x in original.splitlines() if x.startswith(b'0x00230794,'))
assert line==b'0x00230794,0x0023162C,0x00231638,          ,U,f,,'
try:
 p.write_bytes(original.replace(line,line.replace(b',U,f,,',b',U,f,fn_00230794,')))
 result=subprocess.run([sys.executable,'tools/check.py','fn_00230794','--object',str(obj)],capture_output=True,text=True)
 text=result.stdout+result.stderr;print(text);(out/'strict_check.txt').write_text(text)
 assert result.returncode==1
 assert 'U -> M' in text and 'different size' in text
finally:p.write_bytes(original)
assert p.read_bytes()==original
section,compiled,imports=_isolate_function(obj,'fn_00230794',_read_map(p),out/'candidate.o')
assert section=='i.fn_00230794' and len(compiled)==3252
(out/'imports.json').write_text(json.dumps(imports,indent=2))
(out/'original_symbols.sym').write_text('#<SYMDEFS>#\n'+''.join('0x%08X %s %s\n'%(x['address'],x['kind'],x['symbol']) for x in imports))
(out/'candidate.sct').write_text('REPLAY 0x00230794 { CODE 0x00230794 { candidate.o (i.fn_00230794, +FIRST) } }\n')
command=['data/compilers/wibo','data/compilers/4.1/791/bin/armlink.exe','--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--inline','--datacompressor=off','--no_debug','--no_scanlib','--entry=fn_00230794','--keep=fn_00230794','--scatter='+str(out/'candidate.sct'),'--output='+str(out/'replay.axf'),str(out/'candidate.o'),str(out/'original_symbols.sym')]
result=subprocess.run(command,capture_output=True,text=True);(out/'link.log').write_text(result.stdout+result.stderr);assert result.returncode==0,result.stdout+result.stderr
with (out/'replay.axf').open('rb') as stream:
 section=ELFFile(stream).get_section_by_name('CODE');assert section['sh_addr']==0x230794 and section['sh_size']==3252
 assert hashlib.sha256(section.data()).hexdigest()=='de98429c09626b3b5521d3cf87f1163459e2ae913a33ec1c1aeb34fe82ba4a1e'
print('Canonical provenance checked, diagnostic link reproduced; map restored.')
```

## Replay driver

Each pair loads the complete original executable into separate ARM1176 Unicorn instances. The candidate instance overlays only the canonical linked section at the original root address. Input fixtures are constructed independently of either implementation. The model scope intercepts all direct size/constructor/traversal boundaries; size models apply a declared deterministic size increment, not an assertion about retail allocator behavior. The original scope executes every reached original estimator and its nested original callees, plus the original common traversal on empty child lists. The caller scope enters the original `0x0029BC48` wrapper and executes original `0x00338920` child traversal, with constructor/type-query boundaries modeled.

Synthetic callback address `0x00600004` is only an emulator hook for the fixture's virtual table, never a source import or claimed retail helper. The comparison normalizes temporary option addresses to their fields and omits the two padding bytes. It compares final fixture memory, ordered boundary traces, return, stack restoration, and preserved core/VFP registers. The instruction budget is a failing bound, not a success criterion. Faults and ABI failures make the script exit nonzero. Cases are deduplicated within each scope.

```python
import argparse, hashlib, json, struct
from pathlib import Path
from unicorn import Uc, UcError, UC_ARCH_ARM, UC_MODE_ARM, UC_HOOK_CODE
from unicorn.arm_const import *
from elftools.elf.elffile import ELFFile
ROOT=0x230794; BASE=0x100000; STOP=0x600000; TYPECALL=0x600004
MEM=0x500000; BUILDER=MEM; PRIMARY=MEM+0x100; SECONDARY=MEM+0x140
RESOURCE=MEM+0x200; SKELETON=MEM+0x600; DICT=MEM+0x800; OBJECT=MEM+0x1000; VT=MEM+0x1100; TYPES=MEM+0x1200
STACK=0x710000
ESTVAL={0x231a98:16,0x2af598:28,0x29a968:16,0x29c4ac:16,0x299df4:16,0x2a1644:16,0x2a89d4:16,0x2b1064:16}
ESTREF={0x29d100:32,0x29d798:32,0x2a1cc4:16}
CREATE={0x2b4a90:16,0x2a0f0c:16,0x2afba8:28,0x29d854:32,0x2a201c:16,0x29c8dc:16,0x299f0c:16,0x29aa80:16,0x2a175c:16,0x2a8b0c:16,0x2b13e8:16}
TAGS=[0x40000000,0x40000002,0x40000006,0x4000000a,0x40000012,0x40000042,0x40000092,0x400000a2,0x40000112,0x40000122,0x40000222,0x40000422,0x800000,0,0xffffffff,0x40000043]
REGS=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3]
def pack(*v):return struct.pack('<'+'I'*len(v),*[x&0xffffffff for x in v])
def opts(data):return [data[0],data[1]]+list(struct.unpack('<'+'I'*((len(data)-4)//4),data[4:]))
def run(binary,candidate,case,scope):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176);u.reg_write(UC_ARM_REG_C1_C0_2,0x00f00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000);u.mem_map(BASE,0x300000);u.mem_write(BASE,binary)
 if candidate is not None:u.mem_write(ROOT,candidate)
 u.mem_map(MEM,0x20000);u.mem_map(STOP,0x1000);u.mem_map(0x700000,0x20000)
 def w(a,*v):u.mem_write(a,pack(*v))
 def word(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 cfg=case['cfg'];tag=case['tag'];est=case['estimate'];dictionary=MEM+0x80 if case.get('negative',False) else DICT;p=PRIMARY if case['primary'] else 0;s=(p or PRIMARY) if case['alias'] else SECONDARY;s=s if case['secondary'] else 0
 w(BUILDER,RESOURCE,cfg[0],*cfg[1:7],cfg[7]);u.mem_write(BUILDER+0x24,bytes([cfg[8]]))
 w(PRIMARY,case['size'],case['alignment'],case['maximum']);w(SECONDARY,case['size']+3,case['alignment'],case['maximum'])
 w(RESOURCE,tag);w(RESOURCE+0x20,case.get('children',0),0,case['items'],dictionary-(RESOURCE+0x2c))
 w(RESOURCE+0xe0,SKELETON-(RESOURCE+0xe0) if tag==0x40000092 else 0)
 w(SKELETON+0x18,case['joints'])
 for i in range(case['items']):
  item=MEM+(0x400 if case.get('negative',False) else 0x2000)+i*0x80;w(dictionary+0x28+i*16,item-(dictionary+0x28+i*16));w(item,0,1 if i==case['selected'] else 0,0,1,2,0,3)
 # Other original estimator resource pointers are all valid empty records.
 for off in [0x30,0x34,0x38,0x3c,0xc8,0xe4]:w(RESOURCE+off,MEM+0x4000-(RESOURCE+off))
 w(OBJECT,VT);w(VT,0,0,TYPECALL)
 if case.get('children',0):
  childarray=MEM+0x5800;w(RESOURCE+0x24,childarray-(RESOURCE+0x24))
  for i in range(case['children']):
   child=MEM+0x6000+i*0x200;w(childarray+i*4,child-(childarray+i*4));w(child,case.get('child_tag',0x40000000))
 if cfg[4]==MEM+0x5000:w(MEM+0x5164,MEM+0x5200,MEM+0x520c)
 type_mode=case['result'];head={0:0,1:0x3f0730,2:TYPES,3:TYPES,4:TYPES}[type_mode]
 w(TYPES,0 if type_mode==2 else TYPES+4);w(TYPES+4,0x3f0730 if type_mode==3 else TYPES+8);w(TYPES+8,0)
 args=[BUILDER,p,s,0x501800,0 if case['null'] else RESOURCE,0x501840,0x501880,case['recurse'],est]
 for r,a in zip(REGS,args):u.reg_write(r,a)
 entry=ROOT
 if scope=='caller':
  entry=0x29bc48;u.reg_write(UC_ARM_REG_R0,BUILDER);u.reg_write(UC_ARM_REG_R1,args[5]);u.reg_write(UC_ARM_REG_R2,args[6])
 u.reg_write(UC_ARM_REG_SP,STACK);u.mem_write(STACK,pack(*args[4:]));u.reg_write(UC_ARM_REG_LR,STOP)
 saved={r:0xa0000000+r for r in [UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11]+list(range(UC_ARM_REG_D8,UC_ARM_REG_D15+1))}
 for r,v in saved.items():u.reg_write(r,v)
 trace=[];visited=set();instructions=0
 def ret(value=0):
  for r in REGS+[UC_ARM_REG_R12]:u.reg_write(r,0xcc000000+r)
  u.reg_write(UC_ARM_REG_R0,value);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def hook(u,a,z,unused):
  nonlocal instructions
  instructions+=1
  if ROOT<=a<0x23162c:visited.add(a)
  if a==STOP:u.emu_stop();return
  r=[u.reg_read(x) for x in REGS];sp=u.reg_read(UC_ARM_REG_SP)
  if a==TYPECALL:trace.append(['type',r[0]]);ret(head);return
  if a in CREATE:
   n=CREATE[a];rec=['construct',a,r[0],r[1],opts(bytes(u.mem_read(r[2],n))),r[3]]
   if a==0x29d854:rec.append(word(sp))
   trace.append(rec);ret(OBJECT if type_mode else 0);return
  if a==0x29fffc:
   trace.append(['construct_model',a,opts(bytes(u.mem_read(r[0],32))),r[1],r[2],r[3]]);ret(OBJECT if type_mode else 0);return
  if a==0x2a75f4:
   trace.append(['construct_special',a,r[0],r[2]]);w(r[1],0x12345678);ret(OBJECT if type_mode else 0);return
  if a==0x338920:
   trace.append(['children',*r,*struct.unpack('<4I',u.mem_read(sp,16))])
   if scope=='caller' or (scope=='original' and case.get('children',0)==0):return
   ret();return
  if a in ESTVAL or a in ESTREF or a in (0x2319f4,0x2a6e54):
   if a in ESTVAL:
    n=ESTVAL[a];data=pack(r[2],r[3])+bytes(u.mem_read(sp,n-8));rec=['size',a,r[0],r[1],opts(data)]
   elif a in ESTREF:rec=['size',a,r[0],r[1],opts(bytes(u.mem_read(r[2],ESTREF[a])))]
   elif a==0x2319f4:rec=['size',a,r[0],r[1],r[2]]
   else:rec=['size',a,r[0],r[1]]
   trace.append(rec)
   if scope=='original':return
   w(r[0],(word(r[0])+(a&0xfc)+4)&0xffffffff);ret();return
 u.hook_add(UC_HOOK_CODE,hook)
 error=None
 try:u.emu_start(entry,0,count=200000)
 except UcError as e:error=str(e)
 if u.reg_read(UC_ARM_REG_PC)!=STOP and error is None:error='instruction budget'
 badregs=[r for r,v in saved.items() if u.reg_read(r)!=v]
 result={'return':u.reg_read(UC_ARM_REG_R0),'memory':bytes(u.mem_read(MEM,0x12000)).hex(),'trace':trace,'error':error,'sp':u.reg_read(UC_ARM_REG_SP),'badregs':badregs}
 return result,visited,instructions

def main():
 ap=argparse.ArgumentParser();ap.add_argument('--scope',choices=['modeled','original','caller'],default='modeled');ap.add_argument('--limit',type=int,default=0);ap.add_argument('--output',default='build/dot_230794/replay_result.json');args=ap.parse_args()
 binary=Path('data/ver/eu/code.bin').read_bytes();assert hashlib.sha256(binary).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
 e=ELFFile(open('build/dot_230794/replay.axf','rb'));candidate=e.get_section_by_name('CODE').data()
 cases=[]
 for tag in TAGS:
  for k in range(32):
   cases.append(dict(tag=tag,cfg=[[0,0,0,0,0,0,0,0,0],[1,8,4,2,3,4,5,6,1],[255,1,3,4,5,6,7,8,255],[1,0,2,0,1,2,0,3,1]][k%4],estimate=(k//4)%2,primary=(k%4)!=0,secondary=(k%4)!=1,alias=(k%4)==2,recurse=(k//8)%2,null=k==31,result=(k//2)%5,size=[0,3,31,0xffffff00][k%4],alignment=[1,4,16,64][k%4],maximum=[0,8,32,128][k%4],items=(k//2)%4,joints=k%4,selected=(k//4)%4))
 for tag in (0x40000000,0x40000092):
  base=next(c for c in cases if c['tag']==tag)
  cases.extend([dict(base,estimate=1,primary=True,secondary=True,items=i,joints=i,negative=True) for i in (0,2)])
 if args.scope=='original':
  cases=[dict(c,estimate=1,result=0,items=0) for c in cases if c['tag']!=0x40000092]+[dict(c,estimate=1,result=0) for c in cases if c['tag']==0x40000092]
 if args.scope=='original':
  for c in cases:
   c['cfg']=list(c['cfg']);c['cfg'][4]=MEM+0x5000 if c['cfg'][0]==255 else 0
 if args.scope=='caller':
  cases=[dict(c,estimate=0,primary=False,secondary=False,recurse=1,null=False,children=k%3,child_tag=TAGS[(k//3)%13]) for k,c in enumerate(cases)]
 cases=list({json.dumps(c,sort_keys=True):c for c in cases}.values())
 if args.limit:cases=cases[:args.limit]
 passed=0;faults=[];mismatches=[];coverage=set();executed=set();totalsteps=0
 for i,c in enumerate(cases):
  a,v,n=run(binary,None,c,args.scope);b,_,m=run(binary,candidate,c,args.scope);coverage|=v;totalsteps+=n+m;executed|={x[1] for x in a['trace'] if x[0]=='size'}
  if a!=b:
   mismatches.append({'case':c,'original':a,'candidate':b});print('MISMATCH',i,hex(c['tag']),[x for x in a if a[x]!=b[x]],flush=True)
   if len(mismatches)>=6:break
  elif a['error'] or a['sp']!=STACK or a['badregs']:
   faults.append({'case':c,'error':a['error'] or 'ABI postcondition failed'})
  else:passed+=1
 result=dict(scope=args.scope,cases=len(cases),processed=i+1,passed=passed,faults=faults,mismatches=mismatches,root_instructions_covered=len(coverage),root_coverage=sorted(coverage),instructions=totalsteps,estimate_callees=sorted(executed),candidate_sha256=hashlib.sha256(candidate).hexdigest())
 Path(args.output).write_text(json.dumps(result,indent=2));print(json.dumps({k:v for k,v in result.items() if k not in ('faults','mismatches','root_coverage')},indent=2));print('faults',len(faults),'mismatches',len(mismatches))
 if faults or mismatches or i+1!=len(cases):raise SystemExit(1)
if __name__=='__main__':main()
```

## Frozen outcomes and limits

| Scope | Distinct fixtures / passing pairs | Original root addresses visited | Total instructions in both sides |
| --- | ---: | ---: | ---: |
| Modeled boundaries | 516 / 516 | 934 | 79,697 |
| Original size routines | 276 / 276 | 781 | 93,274 |
| Original caller and create traversal | 516 / 516 | 430 | 151,270 |

All final sets have zero mismatches, zero faults, and no ABI preservation failures. Their combined 934-address set covers every instruction address in `0x00230794..0x0023162C`; the three pool words are excluded. Visiting an address is not proof that all predicates at that address were true in some case.

The same candidate section SHA-256 appears in every run: `de98429c09626b3b5521d3cf87f1163459e2ae913a33ec1c1aeb34fe82ba4a1e`. The canonical object hash is `4aba120afe9fe2b27d91836ca922b7178f39ad3cdf382e6c716c5019a76bc382`.

The original estimator scope reaches `0x002319F4`, `0x00231A98`, `0x00299DF4`, `0x0029A968`, `0x0029C4AC`, `0x0029D100`, `0x0029D798`, `0x002A1644`, `0x002A1CC4`, `0x002A6E54`, `0x002A89D4`, `0x002AF598`, and `0x002B1064`. Extended resource lists are empty. The model-shaped resource also exercises its auxiliary first-match loop, zero/nonzero list counts, and zero/nonzero skeletal counts. Configurations cover byte flags 0/1/255, size wraparound, primary/secondary identity, signed negative resource-relative dictionary offsets, missing/unknown tags, null input and result, unrelated/direct/inherited runtime types, optional object fields, and recurse on/off. The original caller scope uses zero/one/two child resources.

Constructor internals and virtual type lookup are models, not reconstructed or verified implementations. Nonempty estimate recursion, cycles, invalid/overlapping records, arbitrary callback mutation, concurrent changes, real asset datasets, allocation failure inside original constructors, and gameplay are excluded. The original nonempty estimate traversal passes its current resource back into the root at `0x003389C0`; the harness does not replace that behavior with an invented terminating implementation.

Form 1 failed because tag `0x40000112` incorrectly copied builder+0x1C into options+0x0C. Form 2 repaired the measured error, and form 3 retained the same canonical object while clarifying opaque field names and signed offsets. An early original-estimator fixture faulted because it supplied a small integer as the optional object pointer; the final fixture domain supplies zero or valid mapped storage. Neither earlier failed run contributes to the passing counts above.
