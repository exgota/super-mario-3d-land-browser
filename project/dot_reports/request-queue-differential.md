# Request queue bounded behavioral check

This read-only diagnostic compares the unchanged owner executable with the canonical committed-source ARMCC object. It never edits or synthesizes a function object. Run from the repository after its normal build with the existing capstone and pyelftools dependencies. Any unsupported instruction, unmapped read, excessive execution, state divergence or callee-saved register/stack failure aborts. The candidate remains non-exact; this result adds zero exact coverage.

## Recorded result
```json
{
  "pairs_passed": 4568,
  "cases_generated": 4744,
  "max_instructions_per_body": 149,
  "retail_sha256": "42eece83f6ccf438a7184bb66bc72a3d9953dd671a31c3fe43a6fbd260f72e0f",
  "candidate_section_sha256": "7b84c0488071ebfa9f49aa75e0793e29b267effdda48f20d41c62471ae445f7c",
  "object_sha256": "4ed349623a165aa363dd73adb2028027f016a2dc037e45367e319c24bf163220",
  "scope": "Four distinct valid capacity8 queues, types0..3, null/non-null opaque actor values, empty/full queues, membership and duplicate patterns; excludes overflow, invalid modes, malformed/aliased memory, concurrency and hardware faults. Closed integer ARM instruction interpreter; not whole-game or formal proof. No exact-match credit."
}
```

## Reproduction script
```python
from pathlib import Path
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
from capstone.arm import *
from elftools.elf.elffile import ELFFile
import random,hashlib,json
BASE=0x252b1c;MASK=0xffffffff;END=0xf0000000
binary=Path('data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(binary).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
retail=binary[BASE-0x100000:0x252bf8-0x100000]
obj=Path('build/eu/obj/lib/al/src/Execute/alExecuteRequestKeeper.o')
with obj.open('rb') as f:
 elf=ELFFile(f);section=elf.get_section_by_name('i._ZN2al20ExecuteRequestKeeper7requestEPNS_9LiveActorEi');candidate=section.data()
assert len(candidate)==len(retail)==220
cs=Cs(CS_ARCH_ARM,CS_MODE_ARM);cs.detail=True
programs=[{i.address:i for i in cs.disasm(code,BASE)} for code in (retail,candidate)]
assert all(len(p)==55 for p in programs)
def regnum(ins,reg):
 n=ins.reg_name(reg);aliases={'sp':13,'lr':14,'pc':15,'sb':9,'sl':10,'fp':11,'ip':12}
 return aliases[n] if n in aliases else int(n[1:])
def cond(cc,n,z,c,v):
 return {ARM_CC_AL:True,ARM_CC_EQ:z,ARM_CC_NE:not z,ARM_CC_HS:c,ARM_CC_LO:not c,ARM_CC_MI:n,ARM_CC_PL:not n,ARM_CC_VS:v,ARM_CC_VC:not v,ARM_CC_HI:c and not z,ARM_CC_LS:not c or z,ARM_CC_GE:n==v,ARM_CC_LT:n!=v,ARM_CC_GT:not z and n==v,ARM_CC_LE:z or n!=v}[cc]
def run(program,memory,actor,request_type):
 m=dict(memory);r=[0]*16;r[0]=0x1000000;r[1]=actor;r[2]=request_type;r[4]=0xaaaa4444;r[5]=0xbbbb5555;r[13]=0x70001000;r[14]=END;pc=BASE;n=z=c=v=False;steps=0
 def shift(value,o):
  if o.shift.type==0:return value
  assert o.shift.type==ARM_SFT_LSL,(o.shift.type,o.shift.value)
  return (value<<o.shift.value)&MASK
 def value(ins,o):
  if o.type==ARM_OP_IMM:return o.imm&MASK
  assert o.type==ARM_OP_REG
  j=regnum(ins,o.reg);return shift(pc+8 if j==15 else r[j],o)
 def address(ins,o):
  assert o.type==ARM_OP_MEM
  x=r[regnum(ins,o.mem.base)] + o.mem.disp
  if o.mem.index:x+=shift(r[regnum(ins,o.mem.index)],o)*o.mem.scale
  return x&MASK
 while pc!=END:
  steps+=1;assert steps<=5000,('step limit',pc)
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
   x=value(ins,ops[1]);r[regnum(ins,ops[0].reg)]=(x if ident==ARM_INS_MOV else ~x)&MASK
   assert not ins.update_flags
  elif ident in (ARM_INS_ADD,ARM_INS_SUB):
   a,b=value(ins,ops[1]),value(ins,ops[2]);r[regnum(ins,ops[0].reg)]=(a+b if ident==ARM_INS_ADD else a-b)&MASK
   assert not ins.update_flags
  elif ident==ARM_INS_CMP:
   a,b=value(ins,ops[0]),value(ins,ops[1]);result=(a-b)&MASK;n=bool(result&0x80000000);z=result==0;c=a>=b;v=bool(((a^b)&(a^result))&0x80000000)
  elif ident==ARM_INS_LDR:r[regnum(ins,ops[0].reg)]=m[address(ins,ops[1])]
  elif ident==ARM_INS_STR:m[address(ins,ops[1])]=value(ins,ops[0])
  elif ident==ARM_INS_B:nxt=value(ins,ops[0])
  elif ident==ARM_INS_BX:nxt=value(ins,ops[0])
  else:raise AssertionError(('unsupported',ins.mnemonic,ins.op_str))
  pc=nxt
 assert (r[4],r[5],r[13])==(0xaaaa4444,0xbbbb5555,0x70001000)
 return {a:x for a,x in m.items() if a<0x70000000},steps
rng=random.Random(0x252b1c);cases=[]
# All request modes and selected/opposing lengths; two membership patterns.
for typ in range(4):
 for own in range(9):
  for opp in range(9):
   for pattern in range(2):cases.append((typ,own,opp,pattern))
for _ in range(4096):cases.append((rng.randrange(4),rng.randrange(9),rng.randrange(9),rng.randrange(4)))
passed=0;max_steps=0
for number,(typ,own,opp,pattern) in enumerate(cases):
 actor=0 if pattern==3 else 0x60000010;counts=[rng.randrange(9) for _ in range(4)];counts[typ]=own;opposed=typ^1;counts[opposed]=opp
 slots=[[rng.choice([0,0x60000010,0x60000020,0x60000030]) for _ in range(8)] for _ in range(4)]
 if pattern==0:
  slots[typ]=[0x60000020]*8;slots[opposed]=[actor]*8
 elif pattern==1:
  slots[typ]=[actor]*8;slots[opposed]=[0x60000020]*8
 # Stay inside the independently observed capacity contract.
 if own==8 and actor not in slots[typ][:own]:continue
 memory={}
 for q in range(4):
  header=0x2000000+q*0x100;buf=0x3000000+q*0x100
  memory[0x1000000+q*4]=header;memory[header]=8;memory[header+4]=counts[q];memory[header+8]=buf
  for j,x in enumerate(slots[q]):memory[buf+j*4]=x
 results=[run(p,memory,actor,typ) for p in programs]
 assert results[0][0]==results[1][0],('divergence',number,typ,own,opp,pattern)
 # Independent direct queue-model oracle, including scan-after-swap behavior.
 expected=[list(x) for x in slots];newcounts=list(counts);i=0
 while i<newcounts[opposed]:
  if expected[opposed][i]==actor:
   expected[opposed][i]=expected[opposed][newcounts[opposed]-1];newcounts[opposed]-=1
  i+=1
 if actor not in expected[typ][:newcounts[typ]]:
  expected[typ][newcounts[typ]]=actor;newcounts[typ]+=1
 for q in range(4):
  header=0x2000000+q*0x100;buf=0x3000000+q*0x100
  assert results[0][0][header+4]==newcounts[q]
  assert [results[0][0][buf+j*4] for j in range(8)]==expected[q]
 passed+=1;max_steps=max(max_steps,results[0][1],results[1][1])
summary={'pairs_passed':passed,'cases_generated':len(cases),'max_instructions_per_body':max_steps,'retail_sha256':hashlib.sha256(retail).hexdigest(),'candidate_section_sha256':hashlib.sha256(candidate).hexdigest(),'object_sha256':hashlib.sha256(obj.read_bytes()).hexdigest(),'scope':'Four distinct valid capacity8 queues, types0..3, null/non-null opaque actor values, empty/full queues, membership and duplicate patterns; excludes overflow, invalid modes, malformed/aliased memory, concurrency and hardware faults. Closed integer ARM instruction interpreter; not whole-game or formal proof. No exact-match credit.'}
Path('/tmp/request-differential-result.json').write_text(json.dumps(summary,indent=2));print(json.dumps(summary,indent=2))

```
