# Bounded translation wrapper replay

Extract this script and pass repository path plus the untouched canonical candidate.axf from check.py. Uses capstone and pyelftools. See translation-wrapper.md for limits.

```python
from pathlib import Path
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
from capstone.arm import *
from elftools.elf.elffile import ELFFile
import random,hashlib,json,sys
MASK=0xffffffff;END=0xf0000000
assert len(sys.argv)==3, 'usage: replay.py REPO CANDIDATE_AXF'
root=Path(sys.argv[1]);binary=(root/'data/ver/eu/code.bin').read_bytes();assert hashlib.sha256(binary).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
cs=Cs(CS_ARCH_ARM,CS_MODE_ARM);cs.detail=True
def regnum(ins,reg):
 n=ins.reg_name(reg);aliases={'sp':13,'lr':14,'pc':15,'sb':9,'sl':10,'fp':11,'ip':12}
 return aliases[n] if n in aliases else int(n[1:])
def cond(cc,n,z,c,v):
 return {ARM_CC_AL:True,ARM_CC_EQ:z,ARM_CC_NE:not z,ARM_CC_HS:c,ARM_CC_LO:not c,ARM_CC_MI:n,ARM_CC_PL:not n,ARM_CC_VS:v,ARM_CC_VC:not v,ARM_CC_HI:c and not z,ARM_CC_LS:not c or z,ARM_CC_GE:n==v,ARM_CC_LT:n!=v,ARM_CC_GT:not z and n==v,ARM_CC_LE:z or n!=v}[cc]

def run(program,base,memory,valid,success,values,write_on_failure):
 m=dict(memory);r=[0]*16;r[0]=0x1000000;r[1]=0x2000000
 for i in range(4,13):r[i]=0xaaa00000+i
 saved=r[4:12];r[13]=0x70001000;r[14]=END;pc=base;n=z=c=v=False;steps=0;trace=[]
 def shift(value,o):
  if not o.shift.type:return value
  assert o.shift.type==ARM_SFT_LSL
  return value<<o.shift.value&MASK
 def value(ins,o):
  if o.type==ARM_OP_IMM:return o.imm&MASK
  assert o.type==ARM_OP_REG
  j=regnum(ins,o.reg);return shift(pc+8 if j==15 else r[j],o)
 def address(ins,o):
  j=regnum(ins,o.mem.base);x=(pc+8 if j==15 else r[j])+o.mem.disp
  if o.mem.index:x+=shift(r[regnum(ins,o.mem.index)],o)*o.mem.scale
  return x&MASK
 while pc!=END:
  steps+=1;assert steps<10000
  ins=program[pc];nxt=pc+4;ops=ins.operands
  if not cond(ins.cc,n,z,c,v):pc=nxt;continue
  ident=ins.id
  if ident==ARM_INS_PUSH:
   regs=sorted(regnum(ins,o.reg) for o in ops);r[13]-=4*len(regs)
   for j,k in enumerate(regs):m[r[13]+4*j]=r[k]
  elif ident==ARM_INS_POP:
   regs=sorted(regnum(ins,o.reg) for o in ops)
   for j,k in enumerate(regs):r[k]=m[r[13]+4*j]
   r[13]+=4*len(regs)
   if 15 in regs:nxt=r[15]
  elif ident in (ARM_INS_MOV,ARM_INS_MVN):
   x=value(ins,ops[1]);r[regnum(ins,ops[0].reg)]=(x if ident==ARM_INS_MOV else ~x)&MASK;assert not ins.update_flags
  elif ident in (ARM_INS_ADD,ARM_INS_SUB):
   a,b=value(ins,ops[1]),value(ins,ops[2]);r[regnum(ins,ops[0].reg)]=(a+b if ident==ARM_INS_ADD else a-b)&MASK;assert not ins.update_flags
  elif ident==ARM_INS_ORR:
   x=value(ins,ops[1])|value(ins,ops[2]);r[regnum(ins,ops[0].reg)]=x
   if ins.update_flags:n=bool(x&0x80000000);z=x==0
  elif ident==ARM_INS_CMP:
   a,b=value(ins,ops[0]),value(ins,ops[1]);x=(a-b)&MASK;n=bool(x&0x80000000);z=x==0;c=a>=b;v=bool(((a^b)&(a^x))&0x80000000)
  elif ident in (ARM_INS_LDR,ARM_INS_LDRB):r[regnum(ins,ops[0].reg)]=m[address(ins,ops[1])]& (255 if ident==ARM_INS_LDRB else MASK)
  elif ident in (ARM_INS_STR,ARM_INS_STRB):m[address(ins,ops[1])]=value(ins,ops[0])&(255 if ident==ARM_INS_STRB else MASK)
  elif ident==ARM_INS_B:nxt=value(ins,ops[0])
  elif ident==ARM_INS_NOP or ins.mnemonic=="nop":pass
  elif ident in (ARM_INS_BL,ARM_INS_BLX):
   target=value(ins,ops[0]);args=r[:3];r[14]=nxt
   if target==0x276aa0:
    assert args[0]==0x3000000;trace.append(('valid',));ret=int(valid)
   elif target==0x278c30:
    assert args[0]==0x3000000;k=[0x3a2de4,0x3a2dec,0x3a2df4].index(args[2]);trace.append(('get',k))
    if success[k] or write_on_failure:m[args[1]]=values[k]
    ret=int(success[k])
   else:raise AssertionError(hex(target))
   r[0]=ret;r[1]=0xcc000001;r[2]=0xcc000002;r[3]=0xcc000003;r[12]=0xcc00000c
  else:raise AssertionError((ins.mnemonic,ins.op_str))
  pc=nxt
 assert r[4:12]==saved and r[13]==0x70001000
 return {a:x for a,x in m.items() if 0x1000000<=a<0x70000000},r[0],trace,steps

BASE=0x267254;ENDADDR=0x267300
image=Path(sys.argv[2])
with image.open('rb') as f:
 e=ELFFile(f);candidate=next(s.data() for s in e.iter_sections() if s['sh_addr']==BASE and s['sh_size'])
retail=binary[BASE-0x100000:ENDADDR-0x100000];assert len(candidate)==len(retail)==172
codes=[retail,candidate];programs=[{i.address:i for i in cs.disasm(code[:160],BASE)} for code in codes]
rng=random.Random(BASE);passed=0;maxsteps=0
for valid in [False,True]:
 for flags in range(8):
  success=[bool(flags&(1<<k)) for k in range(3)]
  for write_failure in [False,True]:
   for _ in range(64):
    values=[rng.getrandbits(32) for k in range(3)];initial=[rng.getrandbits(32) for k in range(3)];memory={0x2000000:0x3000000,**{0x1000000+4*k:v for k,v in enumerate(initial)}}
    expected=[('valid',)];ok=valid
    if valid:
     for k in range(3):
      expected.append(('get',k))
      if not success[k]:ok=False;break
    runs=[]
    for p,code in zip(programs,codes):
     m=dict(memory)
     for off in range(0,len(code),4):m[BASE+off]=int.from_bytes(code[off:off+4],'little')
     runs.append(run(p,BASE,m,valid,success,values,write_failure))
    assert runs[0][:3]==runs[1][:3]
    assert runs[0][1]==int(ok) and runs[0][2]==expected
    assert [runs[0][0][0x1000000+4*k] for k in range(3)]==(values if ok else initial)
    maxsteps=max(maxsteps,*[r[3] for r in runs]);passed+=1
summary={'pairs_passed':passed,'max_instructions':maxsteps,'retail_sha256':hashlib.sha256(retail).hexdigest(),'candidate_sha256':hashlib.sha256(candidate).hexdigest(),'image':str(image),'scope':'Valid nonaliasing placement/output memory, both validity outcomes, all eight key success masks, 64 arbitrary 32-bit patterns each, failure contracts with/without temp write; deterministic mocked imports, no original BYAML implementation/hardware/fault execution, no exact credit.'}
print(json.dumps(summary,indent=2));Path('/tmp/translation-differential-results.json').write_text(json.dumps(summary,indent=2))
```
