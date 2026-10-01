# JPEG decoder replay appendix

These scripts generate synthetic JPEG/marker fixtures locally and execute the
original and canonical-source roots under ARM1176. They contain no game data.
Read the scope and paired-fault qualification in the main branch report.

From the repository root, save each block under its indicated ignored build
path. Build the committed source using `make.py eu`, then run:

```
PYTHONPATH=. python build/evidence_2397a8/link_replay.py
python build/evidence_2397a8/replay.py
python build/evidence_2397a8/replay_targeted.py
```

The targeted script imports and executes `replay_edges.py` before its additions.
Do not count the standalone edges result again: final returning pairs are
`replay-results.json:passed + targeted-results.json:passed`, or 1344 + 564.
The final targeted failure list deliberately retains the invalid-format paired
fetch fault. The nine explicit successful output proofs increment the returning
count directly and are listed separately from the named group counters.

## build/evidence_2397a8/link_replay.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
import subprocess
from tools.low.checkExactBytes import _resolve_symbol,_read_map
root=Path.cwd();p=Path('build/eu/obj/lib/CtrSDK/sources/jpeg_DecoderRoot.o');rows=_read_map(Path('data/ver/eu/map.csv'));out=['#<SYMDEFS>#']
with p.open('rb') as f:
 e=ELFFile(f)
 for s in e.get_section_by_name('.symtab').iter_symbols():
  if s['st_shndx']=='SHN_UNDEF' and s.name and not s.name.startswith('Lib$$'):
   a,k,row=_resolve_symbol(s,None,rows);out.append(f'0x{a:08X} {k} {s.name}')
Path('build/evidence_2397a8/imports.sym').write_text('\n'.join(out)+'\n')
subprocess.run([str(root/'data/compilers/wibo'),str(root/'data/compilers/4.0/902/bin/armlink.exe'),'--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--no_debug','--no_scanlib','--mangled','--symbols','--map','--ro_base=0x600000','--entry=fn_002397A8','--keep=fn_002397A8','--output=build/evidence_2397a8/replay.axf','--list=build/evidence_2397a8/replay.map',str(p),'build/evidence_2397a8/imports.sym'],check=True)
```

## build/evidence_2397a8/replay.py

```python
from pathlib import Path
import struct,io,random,hashlib,json,sys
from PIL import Image
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
BINARY=Path('data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(BINARY).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
CTX=0x800000;INPUT=0x810000;OUTPUT=0x820000;WRAP=0x940000;RESULT=0x941000;SP=0xa0f000;END=0xb00000
REGS=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_R12]
SNAPS=[('context',CTX,0x4000),('input',INPUT,0x10000),('output',OUTPUT,0x100000),('wrapper',WRAP,0x100),('result',RESULT,0x100)]
COVER=set();CALLEES=set();COUNTS={};FAIL=[];PASS=0
with Path('build/evidence_2397a8/replay.axf').open('rb') as f:
 e=ELFFile(f);CAND=e['e_entry'];SECTIONS=[(s['sh_addr'],s.data()) for s in e.iter_sections() if s['sh_flags']&2 and s['sh_size'] and s['sh_type']=='SHT_PROGBITS']
def init(candidate):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176)
 for a,n in [(0x100000,0x400000),(0x600000,0x10000),(CTX,0x150000),(0xa00000,0x10000),(END,0x1000)]:u.mem_map(a,n)
 u.mem_write(0x100000,BINARY)
 for a,b in SECTIONS:u.mem_write(a,b)
 u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000)
 def hook(u,a,n,d):
  if 0x2397a8<=a<0x23a834 and not candidate:COVER.add(a)
  if a in (0x1fdce8,0x1fe5e0,0x2396f0,0x233598,0x2335e8,0x23361c,0x233a7c,0x28f0a0):CALLEES.add(a)
  if candidate and a==0x2397a8:u.reg_write(UC_ARM_REG_PC,CAND)
 u.hook_add(UC_HOOK_CODE,hook)
 return u
US=[init(False),init(True)]
def w(u,a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
def h(u,a,v):u.mem_write(a,struct.pack('<H',v&0xffff))
def reset(u,data,seed):
 u.mem_write(CTX,bytes(0x150000));u.mem_write(0xa00000,bytes(0x10000));u.mem_write(INPUT,data)
 w(u,WRAP,CTX);w(u,WRAP+4,1)
 for i,r in enumerate(REGS):u.reg_write(r,0xaaa00000+i)
 u.reg_write(UC_ARM_REG_CPSR,0x10);u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,END);u.reg_write(UC_ARM_REG_FPSCR,0)
def run(entry,args,stack,data,seed,setup=None):
 global PASS
 outs=[];errs=[]
 for candidate,u in enumerate(US):
  reset(u,data,seed)
  if setup:setup(u)
  for r,v in zip(REGS,args):u.reg_write(r,v)
  for i,v in enumerate(stack):w(u,SP+4*i,v)
  try:
   u.emu_start(entry,END,count=3000000)
   assert u.reg_read(UC_ARM_REG_PC)==END,'instruction cap at '+hex(u.reg_read(UC_ARM_REG_PC))
   assert u.reg_read(UC_ARM_REG_SP)==SP,'SP not restored'
   assert all(u.reg_read(REGS[i])==0xaaa00000+i for i in range(4,12)),'callee-saved register clobber'
   outs.append(([bytes(u.mem_read(a,n)) for _,a,n in SNAPS],u.reg_read(UC_ARM_REG_R0)))
  except Exception as ex:
   errs.append((candidate,str(ex),hex(u.reg_read(UC_ARM_REG_PC))))
 if errs:
  # A fault is not a successful return; retain both failures explicitly.
  return {'errors':errs}
 diffs=[]
 for (name,a,n),x,y in zip(SNAPS,outs[0][0],outs[1][0]):
  if x!=y:
   i=next(i for i,(p,q) in enumerate(zip(x,y)) if p!=q);diffs.append({'buffer':name,'at':hex(i),'original':x[i:i+16].hex(),'candidate':y[i:i+16].hex()})
 if entry!=0x2397a8 and outs[0][1]!=outs[1][1]:diffs.append({'return':[hex(o[1]) for o in outs]})
 if diffs:return {'diffs':diffs}
 PASS+=1
 return None
def case(name,entry,args,stack,data,setup=None):
 COUNTS[name.split('/')[0]]=COUNTS.get(name.split('/')[0],0)+1
 fail=run(entry,args,stack,data,name,setup)
 if fail:
  FAIL.append({'case':name,**fail});print('FAIL',FAIL[-1],flush=True)
def jpeg(wi,he,sub=0,quality=90,seed=1):
 r=random.Random(seed);im=Image.frombytes('RGB',(wi,he),r.randbytes(wi*he*3));f=io.BytesIO();im.save(f,format='JPEG',quality=quality,subsampling=sub);return f.getvalue()
def direct(data,changes={}):
 def setup(u):
  w(u,CTX,INPUT);w(u,CTX+4,len(data));w(u,CTX+8,0x100000);w(u,CTX+0x14,OUTPUT);w(u,CTX+0x58,INPUT+len(data)-1)
  h(u,CTX+0x1c,1024);h(u,CTX+0x1e,1024)
  for (at,sz),v in changes.items():u.mem_write(CTX+at,int(v).to_bytes(sz,'little',signed=False))
 return setup
def marker(m,p):return bytes([255,m])+struct.pack('>H',len(p)+2)+p
if __name__=='__main__':
 for sub in range(3):
  for wi,he in [(8,8),(16,16),(13,9),(32,24)]:
   for fmt in range(9):
    data=jpeg(wi,he,sub,75+sub*10,wi+he)
    case(f'wrapper_decode/{sub}/{wi}x{he}/{fmt}',0x1fbc94,[WRAP,OUTPUT,0x100000,INPUT],[len(data),wi,he,fmt,0],data)
 for wi,he in [(16,16),(32,32),(64,32)]:
  data=jpeg(wi,he,2)
  for scale in range(1,4):
   for fmt in range(9):case(f'wrapper_scaled/{wi}x{he}/{scale}/{fmt}',0x1fbd54,[WRAP,OUTPUT,0x100000,INPUT],[len(data),wi,he,fmt,0,scale],data)
 for root in [0x218bac,0x218dac,0x21df28]:
  for sub in range(3):
   data=jpeg(13,9,sub)
   args=[WRAP,RESULT,INPUT,len(data)] if root!=0x21df28 else [WRAP,INPUT,len(data),0]
   case(f'wrapper_header/{root:x}/{sub}',root,args,[],data)
 # Direct malformed marker fixtures preserve arbitrary earlier error values.
 for m in [0xc0,0xc1,0xc2,0xc4,0xd8,0xd9,0xda,0xdb,0xdd,0xe0,0xe1,0xe2,0xff,0x00]:
  for n in [0,1,2,3,8,9,10,12,16,17,64,128]:
   for err in [0,0x7d]:
    data=b'\xff\xd8'+marker(m,bytes(n))+b'\xff\xd9'
    case(f'marker_errors/{m:x}/{n}/{err}',0x2397a8,[CTX],[],data,direct(data,{(0x44,1):err}))
 # All truncation boundaries of one standards-generated image.
 data=jpeg(13,9,2)
 for n in range(len(data)+1):
  d=data[:n];case(f'truncation/{n}',0x2397a8,[CTX],[],d,direct(d))
 out={'passed':PASS,'failed':len(FAIL),'groups':COUNTS,'failures':FAIL,'original_instructions_visited':len(COVER),'callees':list(map(hex,sorted(CALLEES)))}
 Path('build/evidence_2397a8/replay-results.json').write_text(json.dumps(out,indent=2));Path('build/evidence_2397a8/coverage.json').write_text(json.dumps(sorted(COVER)))
 print(json.dumps({k:v for k,v in out.items() if k!='failures'},indent=2))
```

## build/evidence_2397a8/replay_edges.py

```python
import replay as R
from replay import *
def segments(data):
 out=[];i=2
 while i<len(data):
  if data[i]!=255:raise ValueError(i)
  m=data[i+1]
  if m==0xda:out.append((m,data[i:]));break
  n=int.from_bytes(data[i+2:i+4],'big');out.append((m,data[i:i+2+n]));i+=2+n
 return out
def transform(data,fn):return b'\xff\xd8'+b''.join(fn(m,s) for m,s in segments(data))
def dcase(name,data,changes={}):case(name,0x2397a8,[CTX],[],data,direct(data,changes))
base=jpeg(32,16,2)
for fmt in range(9):
 for options in [0,1,2,3]:
  for missing in ['all','dc','ac']:
   data=transform(base,lambda m,s:b'' if m==0xc4 and (missing=='all' or (s[4]&16==0)==(missing=='dc')) else s)
   dcase(f'default_huffman/{fmt}/{options}/{missing}',data,{(0x48,1):fmt,(0x64,4):options})
# Sixteen-bit quantization plus the original unusual "nonzero precision nibble" handling.
for selector in [0x10,0x20,0xf0]:
 data=transform(base,lambda m,s: marker(m,bytes([s[4]|selector])+b''.join(bytes([0,v]) for v in s[5:])) if m==0xdb else s)
 dcase(f'quantization16/{selector:x}',data)
for q in [0,1,2,3,15,16,17,18,19,255]:
 for precision in [0,1]:
  vals=bytes((i*13+7)%256 for i in range(64*(precision+1)))
  data=b'\xff\xd8'+marker(0xdb,bytes([q|(0x10 if precision else 0)])+vals)+b'\xff\xd9'
  dcase(f'quantization_edges/{q}/{precision}',data)
# Canonical Huffman code generation, empty tables, illegal selectors and guards.
patterns=[[0]*16,[1]+[0]*15,[2]+[0]*15,[0]*15+[1],[1]*16,[255]+[0]*15,[16]*16,[17]*16,[255]*16,[0]*7+[128]+[0]*8,[0]*7+[255,1]+[0]*7]
for selector in [0,1,0x10,0x11,2,0x20,0xff]:
 for pi,counts in enumerate(patterns):
  values=bytes(i%256 for i in range(sum(counts)))
  data=b'\xff\xd8'+marker(0xc4,bytes([selector]+counts)+values)+b'\xff\xd9'
  for err in [0,29]:dcase(f'huffman_edges/{selector:x}/{pi}/{err}',data,{(0x44,1):err})
# Restart markers generated by the codec, preserving original restart/entropy callees.
for blocks in [1,2,3]:
 for fmt in range(9):
  im=Image.frombytes('RGB',(64,32),random.Random(74).randbytes(64*32*3));f=io.BytesIO();im.save(f,format='JPEG',subsampling=2,restart_marker_blocks=blocks)
  data=f.getvalue();assert b'\xff\xdd' in data
  case(f'restart/{blocks}/{fmt}',0x1fbc94,[WRAP,OUTPUT,0x100000,INPUT],[len(data),64,32,fmt,0],data)
# Original APP parsers execute on ordinary Exif metadata and unknown APP payloads.
for prefix in [b'Exif\0\0II\x2a\0\x08\0\0\0\0\0\0\0\0\0',b'Exif\0\0MM\0\x2a\0\0\0\x08\0\0\0\0\0\0',b'MPF\0'+bytes(36),b'other'+bytes(40)]:
 for app in [0xe1,0xe2]:
  data=base[:2]+marker(app,prefix)+base[2:]
  dcase(f'app_metadata/{app:x}/{prefix[:8].hex()}',data)
  data=base[:2]+marker(app,prefix)*2+base[2:]
  dcase(f'app_repeated/{app:x}/{prefix[:8].hex()}',data)
for request in [0,1]:
 for header in [0,1]:
  for prefix in [b'',marker(0xe0,b'abcd')]:
   data=b'\xff\xd8'+prefix+base
   dcase(f'second_image/{request}/{header}/{len(prefix)}',data,{(0x4c,1):request,(0x4b,1):header,(0x5c,4):0xbeef1234,(0x60,4):0xffff1234})
# Dimension/configuration branches and first-error preservation.
for key,values,size in [(0x1c,[0,1,31,32,1024],2),(0x1e,[0,1,15,16,1024],2),(0x20,[0,1,31,32],2),(0x22,[0,1,15,16],2),(8,[0,1,100,0x100000],4),(0x48,[0,8,9,255],1),(0x4d,[0,1,2,3,4],1),(0x38,[0,1,15,32,100],4),(0x3c,[0,1,15,16,100],4)]:
 for val in values:
  for err in [0,7]:dcase(f'configuration/{key:x}/{val}/{err}',base,{(key,size):val,(0x44,1):err})
# Tail checks independent of successful scan decoding.
for flags in [0,1,0x10000,0x110f1,0xffffffff]:
 for required in [0,1,0x10000,0x20000,0xffffffff]:
  for err in [0,7]:
   data=b'\xff\xd8\xff\xd9';dcase(f'tail/{flags:x}/{required:x}/{err}',data,{(0x5c,4):flags,(0x60,4):required,(0x44,1):err})
# Full wrappers cover the maximum supported scale and retained wrapper parameter alias.
for fmt in range(9):
 data=jpeg(64,32,2)
 case(f'caller_scale4/{fmt}',0x1fbd54,[WRAP,OUTPUT,0x100000,INPUT],[len(data),64,32,fmt,0,4],data,lambda u:w(u,WRAP+8,32))
for fmt in range(9):
 for flags in [1,2,3]:
  data=base
  def setup(u,flags=flags):w(u,WRAP+8,64);w(u,WRAP+12,flags)
  case(f'caller_parameters/{fmt}/{flags}',0x1fbc94,[WRAP,OUTPUT,0x100000,INPUT],[len(data),32,16,fmt,0],data,setup)
out={'passed':R.PASS,'failed':len(R.FAIL),'groups':R.COUNTS,'failures':R.FAIL,'original_instructions_visited':len(R.COVER),'callees':list(map(hex,sorted(R.CALLEES)))}
Path('build/evidence_2397a8/edge-results.json').write_text(json.dumps(out,indent=2));Path('build/evidence_2397a8/edge-coverage.json').write_text(json.dumps(sorted(R.COVER)))
print(json.dumps({k:v for k,v in out.items() if k!='failures'},indent=2))
```

## build/evidence_2397a8/replay_targeted.py

```python
import replay as R
from replay import *
from replay_edges import segments, transform
# The imported edge module executes its deterministic set before these additions.
base=jpeg(32,16,2)
def dcase(name,data,changes={}):case(name,0x2397a8,[CTX],[],data,direct(data,changes))
for head in [b'\0\0',b'\xff\xd9',b'\xff\x00',b'\x00\xd8']:
 for err in [0,7]:
  data=head+bytes(20);dcase(f'invalid_start/{head.hex()}/{err}',data,{(0x44,1):err})
for length in range(2,8):
 for err in [0,7]:
  data=b'\xff\xd8\xff\xdd'+b'\0\4'+bytes(length-2)
  dcase(f'dri_truncated/{length}/{err}',data,{(0x44,1):err})
for err in [0,7]:
 for header in [0,1]:dcase(f'request_second/{header}/{err}',base,{(0x4c,1):1,(0x4b,1):header,(0x44,1):err})
for err in [0,7]:
 for which in ['sof','dqt','dht','all']:
  data=transform(base,lambda m,s:b'' if (m==0xc0 and which in ['sof','all']) or (m==0xdb and which in ['dqt','all']) or (m==0xc4 and which in ['dht','all']) else s)
  dcase(f'missing_markers/{which}/{err}',data,{(0x44,1):err})
for status in [0x100,0x10000,0x1000000,0x101,0xff00]:
 dcase(f'scan_status/{status:x}',base,{(0x44,4):status})
for kind in [0,1,2]:
 sos=base.index(b'\xff\xda');length=int.from_bytes(base[sos+2:sos+4],'big');start=sos+2+length
 for n in [1,2,8,64,256]:
  data=base[:start]+(b'\xff\xd9' if kind==0 else b'\xff\xd0' if kind==1 else b'\xff\xff')*n
  dcase(f'entropy_marker/{kind}/{n}',data)
# Require full wrappers to return a successful byte count with a clear error byte.
proof=[]
for fmt in range(9):
 data=base
 fail=run(0x1fbc94,[WRAP,OUTPUT,0x100000,INPUT],[len(data),32,16,fmt,0],data,fmt)
 assert not fail,fail
 error=US[0].mem_read(CTX+0x44,1)[0];result=US[0].reg_read(UC_ARM_REG_R0)
 assert error==0 and result>0,(fmt,error,result)
 proof.append({'format':fmt,'returned_bytes':result,'output_sha256':hashlib.sha256(US[0].mem_read(OUTPUT,result)).hexdigest()})
out={'passed':R.PASS,'failed':len(R.FAIL),'groups':R.COUNTS,'failures':R.FAIL,'original_instructions_visited':len(R.COVER),'callees':list(map(hex,sorted(R.CALLEES))),'successful_output_proof':proof}
Path('build/evidence_2397a8/targeted-results.json').write_text(json.dumps(out,indent=2));Path('build/evidence_2397a8/targeted-coverage.json').write_text(json.dumps(sorted(R.COVER)))
print(json.dumps({k:v for k,v in out.items() if k!='failures'},indent=2))
```

## Frozen replay-results.json

```json
{
  "passed": 1344,
  "failed": 0,
  "groups": {
    "wrapper_decode": 108,
    "wrapper_scaled": 81,
    "wrapper_header": 9,
    "marker_errors": 336,
    "truncation": 810
  },
  "failures": [],
  "original_instructions_visited": 774,
  "callees": [
    "0x1fdce8",
    "0x1fe5e0",
    "0x233598",
    "0x2335e8",
    "0x23361c",
    "0x233a7c",
    "0x2396f0"
  ]
}
```

## Frozen targeted-results.json

```json
{
  "passed": 564,
  "failed": 1,
  "groups": {
    "default_huffman": 108,
    "quantization16": 3,
    "quantization_edges": 20,
    "huffman_edges": 154,
    "restart": 27,
    "app_metadata": 8,
    "app_repeated": 8,
    "second_image": 8,
    "configuration": 82,
    "tail": 50,
    "caller_scale4": 9,
    "caller_parameters": 27,
    "invalid_start": 8,
    "dri_truncated": 12,
    "request_second": 4,
    "missing_markers": 8,
    "scan_status": 5,
    "entropy_marker": 15
  },
  "failures": [
    {
      "case": "configuration/48/255/0",
      "errors": [
        [
          0,
          "Invalid memory fetch (UC_ERR_FETCH_UNMAPPED)",
          "0xffa6"
        ],
        [
          1,
          "Invalid memory fetch (UC_ERR_FETCH_UNMAPPED)",
          "0xffa6"
        ]
      ]
    }
  ],
  "original_instructions_visited": 912,
  "callees": [
    "0x1fdce8",
    "0x1fe5e0",
    "0x233598",
    "0x2335e8",
    "0x23361c",
    "0x233a7c",
    "0x2396f0",
    "0x28f0a0"
  ],
  "successful_output_proof": [
    {
      "format": 0,
      "returned_bytes": 1024,
      "output_sha256": "baee0e4bf141e5288d7fe3012137ebfdb9e5ad765ab613eae9ec7be6f8886c2c"
    },
    {
      "format": 1,
      "returned_bytes": 1024,
      "output_sha256": "e5d7c4dbe948d69ff7732ac72f5238a33ffd37a2e391d99920d3b275cc193af4"
    },
    {
      "format": 2,
      "returned_bytes": 1024,
      "output_sha256": "4f831aca28db659d00b8a09859d78b4602aabe6b2d0e68089034c3b2c4a21228"
    },
    {
      "format": 3,
      "returned_bytes": 1536,
      "output_sha256": "bc6a2da3bfe8a3f0bed1bfc79424682a5c2389d72f2992a8406179f4a0cc4a15"
    },
    {
      "format": 4,
      "returned_bytes": 1536,
      "output_sha256": "091ef956434a1dcb58f2e09ddcb84f6a36754259fa432743685e9e34a0be7608"
    },
    {
      "format": 5,
      "returned_bytes": 2048,
      "output_sha256": "3f47bef75995c5abc4bec246c173865df3d61eab31a9fffab130af6f7f00ee78"
    },
    {
      "format": 6,
      "returned_bytes": 2048,
      "output_sha256": "5500dd6daaa46ac1ffd7d14761d30969c0d457f1facb1456a5db9b82756fa99b"
    },
    {
      "format": 7,
      "returned_bytes": 1536,
      "output_sha256": "3b53865cbbaffec37d51e0a37e3e060e7b5a7ccb07fe3727714ce91b56739c89"
    },
    {
      "format": 8,
      "returned_bytes": 2048,
      "output_sha256": "6493837c46f4afd782061fd2fa7fc5134974caea6321c1a49e42fa5d808c10fc"
    }
  ]
}
```

