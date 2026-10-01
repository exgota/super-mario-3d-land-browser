# Bounded CFL root replay

This is a reproducible diagnostic, not the canonical exact checker. It links the
unchanged canonical ARMCC object for ordinary C++ from candidate `d218368` using
observed import addresses, including the unmapped 4244A8 data base. The latter is
explicitly insufficient for exact acceptance. No input object or original bytes
are edited. The retail executable stays private and local.

Save the three Python blocks below into the indicated `/tmp/cfl-root-analysis/`
paths. Run from the repository root after canonical `make.py eu`. Use its venv,
which needs Unicorn, Capstone and pyelftools. The final checker must still be run
independently and currently rejects the missing BSS identity.

The 50 baseline cases cover the supported resolution values, fallback/zero values,
transparent/sparse/dense input patterns, disabled occupancy, absent expressions,
RAM and GPU readback, and final memory-kind conversion. The 24 additional cases
exercise all 16 expressions, partial expression masks, flags 28/30/31, three
memory kinds, shared image caches, and missing category-16 images.

The harness compares the 256 output bytes, texture descriptor fields, complete
mip-payload hashes, and ordered externally visible call events. Stack texture
identity is normalized by initialization order. It verifies return, stack and
callee-saved integer/VFP registers. No scratch-stack byte equality is asserted.
The actual retail projection and untile helpers execute; all remaining external
calls use the explicit deterministic models below. The image/texture models always
succeed except the explicit missing category-16 input. Graphics stubs provide
controlled buffers rather than implementing the GPU. For 1024 textures, the
root's explicit CPU clear may replace that controlled pattern. The test does not
claim rendered-pixel correctness, real allocator/cache coherency, external-global
initialization, complete application execution, or malformed-input behavior.

```sh
. ./development_environment.sh
python /tmp/cfl-root-analysis/link-replay.py
python /tmp/cfl-root-analysis/replay.py
python /tmp/cfl-root-analysis/extended.py
```

## Frozen final results

These results were rerun against the object produced by the final clean canonical
build on 2026-10-01. The only nonempty function symbol is the 7,684-byte root.

```json
{
  "baseline": {
    "pairs": 50,
    "max_instructions": 34129506,
    "candidate_size": 7684,
    "candidate_sha256": "c3ab3e0ca0774805a1edea3df2dcb72dfce426082c226a260961a72f1e9b5fc8",
    "pixel_hashes_compared": true,
    "note": "Bounded root dataflow only; GPU operations and resource/texture/allocator calls are deterministic models, except retail matrix projection and untile helpers. Not rendering or hardware equivalence; not canonical exact credit."
  },
  "additional": {
    "pairs": 24,
    "candidate_sha256": "c3ab3e0ca0774805a1edea3df2dcb72dfce426082c226a260961a72f1e9b5fc8",
    "pixel_hashes_compared": true,
    "max_instructions": 1159036,
    "events": 39564,
    "quad_calls": 4608,
    "null_quad_calls": 288,
    "scope": "All16 expressions and sparse expression sets; every cache-pair selector, alternate source image for expressions12..15, bit28/30/31 settings, three memory kinds, RAM/GPU readback models, and category16 missing-image returns."
  }
}
```

SHA-256 identities of the clean-build inputs/output:

- `lib/CtrSDK/sources/cfl_TextureRoot.cpp`: `d95d2be5784fd429cfb104570ba3dbeff6f53520cc6602b62ea7f739cdb6071d`
- `lib/CtrSDK/include/cfl/cfl_TextureRoot.h`: `abe7a5e5087362ad8df69be3fcdfde0b8608c7178a8a46a5eee57830da67cbbf`
- `build/eu/obj/lib/CtrSDK/sources/cfl_TextureRoot.o`: `f718d8aeb678c8c6e111ca7af192d8ea0bdb4802985185310fd37b1d7e891614`

## Diagnostic link: `/tmp/cfl-root-analysis/link-replay.py`

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
import csv,subprocess,os,json,hashlib
root=Path.cwd();out=root/'build/cfl_bounded_replay';out.mkdir(exist_ok=True)
obj=root/'build/eu/obj/lib/CtrSDK/sources/cfl_TextureRoot.o'
rows=[{k.strip():v.strip() for k,v in x.items()} for x in csv.DictReader((root/'data/ver/eu/map.csv').open())]
syms={x['Symbol']:int(x['Start'],16) for x in rows if x['Symbol']}
with obj.open('rb') as f:
 e=ELFFile(f);names=[s.name for s in e.get_section_by_name('.symtab').iter_symbols() if s['st_shndx']=='SHN_UNDEF' and s.name and not s.name.startswith('Lib$$')]
 lines=[]
 for n in names:
  a=int(n.split('_')[1],16) if n.startswith(('fn_','dat_')) else syms[n]
  lines.append(f'0x{a:08X} {"D" if n.startswith("dat_") else "A"} {n}')
(out/'symbols.sym').write_text('#<SYMDEFS>#\n'+'\n'.join(lines)+'\n')
(out/'candidate.sct').write_text('CANDIDATE_LOAD 0x001119EC\n{\n CANDIDATE_CODE 0x001119EC\n {\n  cfl_TextureRoot.o (i.fn_001119EC, +FIRST)\n  cfl_TextureRoot.o (+RO)\n }\n}\n')
cmd=[str(root/'data/compilers/wibo'),str(root/'data/compilers/4.0/902/bin/armlink.exe'),'--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--inline','--datacompressor=off','--no_debug','--no_scanlib','--mangled','--symbols','--map','--entry=fn_001119EC','--keep=fn_001119EC',f'--scatter={out}/candidate.sct',f'--output={out}/candidate.axf',f'--list={out}/candidate.map',str(obj),str(out/'symbols.sym')]
p=subprocess.run(cmd,env={**os.environ,'TMP':'/tmp'},stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True);print(p.stdout);assert p.returncode==0
print('DEBUG LINK ONLY: includes observed but unmapped BSS 0x004244A8. Not an exact-check result.')
```

## Root comparison: `/tmp/cfl-root-analysis/replay.py`

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import *
import struct,json,random,hashlib,csv,sys
ROOT=Path.cwd(); BINARY=(ROOT/'data/ver/eu/code.bin').read_bytes(); BASE=0x1119ec; END=0x71000000; DB=0x1000000; DS=0x4000000; STACK=0x70000000; SP=STACK+0x18000
assert hashlib.sha256(BINARY).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
with (ROOT/'build/cfl_bounded_replay/candidate.axf').open('rb') as f:
 e=ELFFile(f); CANDIDATE=next(s.data() for s in e.iter_sections() if s['sh_addr']==BASE and s['sh_size'])
R=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_R12]
OCC=DB;INFO=DB+0x1000;MODEL=DB+0x2000;RESOURCE=DB+0x3000;TEXTURES=DB+0x5000;HEAP=DB+0x10000;ALLOC_CB=0x40000000;FREE_CB=0x40000004
U=lambda *v:struct.pack('<'+'I'*len(v),*[x&0xffffffff for x in v])
class Engine:
 def __init__(self,candidate):
  u=self.u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176);u.mem_map(0x100000,0x400000);u.mem_write(0x100000,BINARY)
  if candidate:u.mem_write(BASE,CANDIDATE)
  self.extent=len(CANDIDATE) if candidate else 7600
  u.mem_map(DB,DS);u.mem_map(STACK,0x20000);u.mem_map(END,0x1000);u.mem_map(ALLOC_CB,0x1000)
  u.reg_write(UC_ARM_REG_C1_C0_2,0xf<<20);u.reg_write(UC_ARM_REG_FPEXC,1<<30)
  u.hook_add(UC_HOOK_CODE,self.hook)
 def get(self,a,n=1):return struct.unpack('<'+'I'*n,self.u.mem_read(a,n*4))[0] if n==1 else struct.unpack('<'+'I'*n,self.u.mem_read(a,n*4))
 def put(self,a,*v):self.u.mem_write(a,U(*v))
 def alloc(self,n,align=16):
  p=(self.cursor+align-1)&~(align-1);self.cursor=p+max(n,16);assert self.cursor<DB+DS,(n,hex(p));self.u.mem_write(p,bytes(max(n,16)));return p
 def norm(self,p):
  if STACK<=p<STACK+0x20000:return self.labels.get(p,'stack')
  return p
 def tex(self,p):
  w=list(self.get(p,13));w[0]=self.norm(w[0]);return w
 def inittex(self,p,w,h,levels,field18,field1c,fmt,pixels,kind,borrow):
  self.labels[p]=f'texture{self.texseq}';self.texseq+=1
  own=not(pixels and kind==0x10000 and borrow)
  data=self.alloc(w*h*2*2) if own else pixels
  if pixels and own:
   total=sum((w>>i)*(h>>i)*2 for i in range(levels))
   self.u.mem_write(data,bytes(self.u.mem_read(pixels,total)))
  self.put(p,p,w,h,w,h,levels,field18,field1c,pixels,fmt,data,kind,int(own))
 def ret(self,result=0):
  u=self.u
  for i in [0,1,2,3,12]:u.reg_write(R[i],0xcc000000+i)
  for i in range(16):u.reg_write(UC_ARM_REG_S0+i,0x7fc00000+i)
  u.reg_write(R[0],result&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def hook(self,u,pc,size,_):
  self.steps+=1
  if BASE<=pc<BASE+self.extent:return
  # These two pure helpers execute unchanged retail instructions.
  if 0x11635c<=pc<0x116518 or 0x2841c0<=pc<0x28426c:
   if pc==0x11635c:self.trace.append(('untile',tuple(u.reg_read(r) for r in R[2:4]),self.get(u.reg_read(UC_ARM_REG_SP),2)))
   if pc==0x2841c0:self.trace.append(('projection',tuple(u.reg_read(UC_ARM_REG_S0+i) for i in range(6))))
   return
  a,b,c,d=[u.reg_read(r) for r in R[:4]];sp=u.reg_read(UC_ARM_REG_SP);extra=lambda n:list(self.get(sp,n)) if n>1 else [self.get(sp)]
  ev=None;result=0
  if pc==0x2848f4:
   ev=('allocate',a,b);result=self.alloc(a,b)
  elif pc==0x2847bc:ev=('free',a)
  elif pc==0x283d1c:
   ev=('image',bool(a),b,c);result=0 if (self.null_images and b==16) else 136
   if a:self.put(a,0x00080008,0x01000a01);u.mem_write(a+8,bytes([c&255])*128)
  elif pc==0x283cc4:
   ev=('imageTexture',b,c)
   if b:self.inittex(a,8,8,1,0,0,10,b+8,c,0)
   else:self.put(a,0)
  elif pc==0x283a44:
   tex=self.get(a);self.bound=tex;ev=('framebuffer',self.norm(tex),b,c,d,tuple(extra(2)))
   pix=self.get(tex+0x28);n=b*c*2
   # A deterministic stand-in for GPU render results, including clear/sparse/dense alpha.
   buf=bytearray(n)
   if self.pattern:
    for j in range(n//2):
     if self.pattern==2 or ((j*17+self.renderseq*29+self.seed)%257)==0:buf[2*j]=15
   u.mem_write(pix,bytes(buf));self.renderseq+=1
  elif pc==0x2870a4:ev=('renderSetup',a,b)
  elif pc==0x2873a0:ev=('projectionUpload',bytes(u.mem_read(a,64)).hex())
  elif pc==0x288284:ev=('command',b,c,bytes(u.mem_read(a,b)).hex())
  elif pc==0x284020:ev=('pipeline',a,b,c,d,extra(1)[0])
  elif pc==0x283960:ev=('contextMode',a,b,c)
  elif pc==0x283700:
   ev=('quad',bytes(u.mem_read(a,24)).hex(), self.tex(a+24) if self.get(a+24) else None)
  elif pc==0x2840f4:ev=('identity',a)
  elif pc==0x283fac:ev=('rectangle',a,b,c,tuple(u.reg_read(UC_ARM_REG_S0+i) for i in range(5)))
  elif pc in [0x28c890,0x28ca18,0x284894,0x284868,0x2847e4]:ev=('sync',pc)
  elif pc==0x28e280:ev=('regionStart',a);result=DB if self.gpu else 0x30000000
  elif pc==0x28e240:ev=('regionEnd',a);result=DB+DS-1 if self.gpu else 0x30001000
  elif pc==0x283594:ev=('genTexture',a);self.put(b,101)
  elif pc==0x282d20:ev=('bindTexture',a,b)
  elif pc==0x282b84:ev=('copyTexture',a,b,c,d,tuple(extra(4)))
  elif pc==0x2874d0:ev=('mapTexture',a,b);self.put(c,self.get(self.bound+0x28))
  elif pc==0x282650:ev=('deleteTexture',a,self.get(b))
  elif pc==0x285238:
   x=extra(6);ev=('initTexture',b,c,d,tuple(x[:3]),self.norm(x[3]),x[4],x[5]);self.inittex(a,b,c,d,*x)
  elif pc==0x2881e8:
   ev=('getAllocator',bool(a),bool(b))
   if a:self.put(a,ALLOC_CB)
   if b:self.put(b,FREE_CB)
  elif pc==FREE_CB:ev=('freeTexture',a,b,self.norm(c),d)
  elif pc==0x28820c:
   # This call is semantically the same destructor already inlined elsewhere.
   self.trace.append(('destroyTexture',self.norm(a)))
   if self.get(a):
    if self.get(a+0x30):self.trace.append(('destroyOwned',self.get(a+0x2c),self.norm(self.get(a)),self.get(a+0x28)))
    self.put(a,0,0,0);self.put(a+0x30,0)
  elif pc==0x2823b8:ev=('bindSource',self.norm(a),b,c,d,tuple(extra(2)))
  elif pc==0x282094:
   x=extra(3);ev=('vertices',a,b,c,d,bytes(u.mem_read(x[0],48)).hex(),x[1],x[2])
  elif pc==0x296048:ev=('flush',a,b)
  elif pc==0x288414:u.mem_write(a,bytes(b));ev=('clear',self.norm(a),b)
  elif pc in [0x28ba44,0x28f0a0]:
   contents=bytes(u.mem_read(b,c));u.mem_write(a,contents)
   # C++ assignment may choose inline words or memcpy; compare the externally
   # observed results, not the selected runtime-copy implementation.
  else:raise AssertionError(('unhandled',hex(pc),[hex(x) for x in [a,b,c,d]],self.trace[-4:]))
  if ev:self.trace.append(ev)
  self.ret(result)
 def run(self,case):
  resolution,flags,kind,active,seed,pattern,gpu,null_images=case;self.seed=seed;self.pattern=pattern;self.gpu=gpu;self.null_images=null_images
  u=self.u;u.mem_write(DB,bytes(0x10000));u.mem_write(STACK,bytes(0x20000));self.cursor=HEAP;self.trace=[];self.labels={};self.steps=0;self.texseq=0;self.renderseq=0;self.bound=0
  size={0x80:128,0xe0:128,0x100:256,0x1e0:256,0x200:512,0x400:1024}.get(resolution,64)
  self.put(MODEL+0x6c,RESOURCE);self.put(RESOURCE+0x67c,kind);self.put(RESOURCE+0x68c,flags)
  rng=random.Random(seed)
  values=[0]*72
  for off,maxval in [(0x20,62),(0x24,6),(0x28,8),(0x2c,7),(0x30,8),(0x34,13),(0x38,18),(0x3c,26),(0x40,8),(0x44,8),(0x48,7),(0x4c,12),(0x50,13),(0x54,18),(0x64,26),(0x68,5),(0x6c,8),(0x70,7),(0x74,18),(0x78,6),(0x80,8),(0x84,8),(0x88,18),(0x9c,2),(0xa0,8),(0xa4,18),(0xa8,18)]:values[off//4]=rng.randrange(maxval)
  self.put(INFO,*values)
  for i in range(16):
   target=TEXTURES+i*0x40
   if (active>>i)&1:
    pix=self.alloc(size*size*2*2)
    self.put(target,target,size,size,size,size,1,0,0,0,10,pix,0x10000,1)
    self.put(MODEL+0x70+4*i,target)
  u.mem_write(0x4244a8,bytes(0x100));self.put(0x4244f8,rng.randrange(16));self.put(0x3ef06c,0x12345678)
  u.reg_write(UC_ARM_REG_CPSR,0x10)
  for i,r in enumerate(R):u.reg_write(r,0xaa000000+i)
  for i,v in enumerate([OCC,INFO,MODEL,resolution]):u.reg_write(R[i],v)
  u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,END)
  for i in range(16):u.reg_write(UC_ARM_REG_D0+i,0x123456789abcdef0+i)
  u.emu_start(BASE,END,count=60000000)
  assert u.reg_read(UC_ARM_REG_PC)==END,('incomplete',hex(u.reg_read(UC_ARM_REG_PC)),self.steps)
  assert u.reg_read(UC_ARM_REG_SP)==SP
  assert all(u.reg_read(R[i])==0xaa000000+i for i in range(4,12))
  assert all(u.reg_read(UC_ARM_REG_D0+i)==0x123456789abcdef0+i for i in range(8,16))
  outputs=[]
  for i in range(16):
   if not (active>>i&1):continue
   ptr=TEXTURES+i*0x40;descriptor=self.tex(ptr)
   total=sum((descriptor[3]>>j)*(descriptor[4]>>j)*2 for j in range(descriptor[5]))
   outputs.append((descriptor,hashlib.sha256(bytes(u.mem_read(descriptor[10],total))).hexdigest()))
  return bytes(u.mem_read(OCC,256)).hex(),outputs,self.trace,self.steps

if __name__=='__main__':
 engines=[Engine(False),Engine(True)];cases=[]
 for resolution in [0x40,0x60,0x80,0xe0,0x100,0x1e0,0x200,0x400,0,0x41]:
  for flags,kind,active,pattern,gpu in [(0x10000000,0x10000,0,0,False),(0x10000000,0x10000,1,1,False),(0,0x10000,1,0,False),(0,0x10000,1,1,True),(0xc0000000,0x20000,0xf000,2,False)]:
   if resolution==0:active=0 # no staging allocation is valid only for no active textures
   cases.append((resolution,flags,kind,active,len(cases)+77,pattern,gpu,False))
 maximum=0
 for n,case in enumerate(cases):
  runs=[eng.run(case) for eng in engines]
  if runs[0][:3]!=runs[1][:3]:
   Path('/tmp/cfl-root-analysis/failure.json').write_text(json.dumps({'case':case,'retail':runs[0],'candidate':runs[1]},indent=2))
   for i,(a,b) in enumerate(zip(runs[0][2],runs[1][2])):
    if a!=b:print('EVENT MISMATCH',n,case,i,a,b);break
   print('OUTPUT',runs[0][0]==runs[1][0],runs[0][1]==runs[1][1], 'trace lengths',len(runs[0][2]),len(runs[1][2]));sys.exit(1)
  maximum=max(maximum,runs[0][3],runs[1][3]);print('PASS',n,case,'steps',runs[0][3],runs[1][3],flush=True)
 summary={'pairs':len(cases),'max_instructions':maximum,'candidate_size':len(CANDIDATE),'candidate_sha256':hashlib.sha256(CANDIDATE).hexdigest(),'pixel_hashes_compared':True,'note':'Bounded root dataflow only; GPU operations and resource/texture/allocator calls are deterministic models, except retail matrix projection and untile helpers. Not rendering or hardware equivalence; not canonical exact credit.'}
 Path('/tmp/cfl-root-analysis/replay-results.json').write_text(json.dumps(summary,indent=2));print(json.dumps(summary,indent=2))
```

## Additional cases: `/tmp/cfl-root-analysis/extended.py`

```python
import runpy,json,random,sys
from pathlib import Path
m=runpy.run_path('/tmp/cfl-root-analysis/replay.py',run_name='cfl_replay');engines=[m['Engine'](False),m['Engine'](True)];rng=random.Random(0x1119ec)
cases=[]
for resolution in [0x40,0x60,0x80,0xe0,0x100,0x1e0]:
 for variant in range(4):
  cases.append((resolution,[0,0x10000000,0x40000000,0x80000000][variant],[0x10000,0x20000,0x30000,0x10000][variant],0xffff if variant==0 else [0,0xa55a,0x5aa5,0xffff][variant],rng.randrange(1000,10000),1 if variant%2==0 else 2,bool(variant&1),bool(variant&2)))
maximum=0;events=0;quads=0;nullquads=0
for i,case in enumerate(cases):
 runs=[e.run(case) for e in engines]
 if runs[0][:3]!=runs[1][:3]:
  Path('/tmp/cfl-root-analysis/failure-extended.json').write_text(json.dumps({'case':case,'retail':runs[0],'candidate':runs[1]},indent=2))
  for n,(a,b) in enumerate(zip(runs[0][2],runs[1][2])):
   if a!=b:print('MISMATCH',i,case,n,a,b);break
  print('OUTPUT',runs[0][0]==runs[1][0],runs[0][1]==runs[1][1]);sys.exit(1)
 maximum=max(maximum,*[r[3] for r in runs]);events+=len(runs[0][2]);quads+=sum(e[0]=='quad' for e in runs[0][2]);nullquads+=sum(e[0]=='quad' and e[2] is None for e in runs[0][2]);print('PASS',i,case,runs[0][3],runs[1][3],flush=True)
result={'pairs':len(cases),'candidate_sha256':m['hashlib'].sha256(m['CANDIDATE']).hexdigest(),'pixel_hashes_compared':True,'max_instructions':maximum,'events':events,'quad_calls':quads,'null_quad_calls':nullquads,'scope':'All16 expressions and sparse expression sets; every cache-pair selector, alternate source image for expressions12..15, bit28/30/31 settings, three memory kinds, RAM/GPU readback models, and category16 missing-image returns.'};Path('/tmp/cfl-root-analysis/extended-results.json').write_text(json.dumps(result,indent=2));print(json.dumps(result,indent=2))
```
