# Integer argument reader bounded ARM replay

Run from the repository root after the canonical checks in integer-arguments.md. Requires pyelftools and Unicorn 2.1.4. Reads only local owner-supplied game data; no game bytes are included here.

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import *
import struct,random,hashlib,json,csv
ROOT=Path.cwd();binary=(ROOT/'data/ver/eu/code.bin').read_bytes();assert hashlib.sha256(binary).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
TARGETS=[(0x2794f8,0,False),(0x27d1dc,1,False),(0x27d180,2,False),(0x27aff8,3,False),(0x2693d8,4,False),(0x2671d4,5,False),(0x266148,6,False),(0x2730f8,7,False),(0x1bd080,8,False),(0x2670d8,2,True)]
DB=0x1000000;OUT=DB;INFO=DB+0x100;ITER=DB+0x200;BLOB=DB+0x1000;END=0x71000000;SP=0x70008000
REGS=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_R12]
entries={}
for r in csv.reader((ROOT/'data/ver/eu/map.csv').open()):
 try:a=int(r[0],16)
 except:continue
 if r[5].startswith('f'):entries[a]=r[6] or f'fn_{a:08X}'
class Engine:
 def __init__(self,base,code,direct):
  self.base=base;self.direct=direct;self.u=u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176);u.mem_map(0x100000,0x400000);u.mem_write(0x100000,binary);u.mem_write(base,code);u.mem_map(DB,0x10000);u.mem_map(0x70000000,0x10000);u.mem_map(END,4096);u.reg_write(UC_ARM_REG_C1_C0_2,0xf<<20);u.reg_write(UC_ARM_REG_FPEXC,1<<30);u.hook_add(UC_HOOK_CODE,self.hook)
 def hook(self,u,pc,size,_):
  self.steps+=1;assert 0x100000<=pc<0x100000+len(binary),hex(pc)
  if pc in entries and pc!=self.base:self.trace.append(entries[pc])
 def run(self,data):
  u=self.u;u.mem_write(DB,bytes(data));u.mem_write(0x70000000,bytes(0x10000));u.reg_write(UC_ARM_REG_CPSR,0x10)
  for i,r in enumerate(REGS):u.reg_write(r,0xaa000000+i)
  u.reg_write(UC_ARM_REG_R0,OUT);u.reg_write(UC_ARM_REG_R1,ITER if self.direct else INFO);u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,END);self.steps=0;self.trace=[]
  u.emu_start(self.base,END,count=20000);assert u.reg_read(UC_ARM_REG_PC)==END,('did not return',self.steps)
  assert u.reg_read(UC_ARM_REG_SP)==SP and all(u.reg_read(REGS[i])==0xaa000000+i for i in range(4,12))
  return bytes(u.mem_read(DB,0x10000)),u.reg_read(UC_ARM_REG_R0),self.trace,self.steps

def data_case(arg,scenario,value,initial):
 d=bytearray(0x10000)
 def put(a,v):struct.pack_into('<I',d,a-DB,v&0xffffffff)
 put(OUT,initial);put(INFO,ITER);put(ITER,0 if scenario=='invalid_header' else BLOB);put(ITER+4,0 if scenario=='null_root' else BLOB+0x200)
 # Little-endian BYAML header and a sorted string-key table.
 struct.pack_into('<HHIII',d,BLOB-DB,0x5942,1,0x20,0,0x200)
 keys=[f'Arg{i}' for i in range(9)]
 if scenario=='missing_key':keys.remove(f'Arg{arg}')
 if scenario=='empty_keys':keys=[]
 table=BLOB+0x20;put(table,0xc2|(len(keys)<<8));cursor=4+4*(len(keys)+1)
 for i,key in enumerate(keys):
  put(table+4+4*i,cursor);encoded=key.encode()+b'\0';d[table-DB+cursor:table-DB+cursor+len(encoded)]=encoded;cursor+=len(encoded)
 put(table+4+4*len(keys),cursor)
 pairs=[]
 for i,key in enumerate(keys):
  if scenario=='missing_pair' and key==f'Arg{arg}':continue
  typ=0xd1
  if key==f'Arg{arg}' and scenario.startswith('type_'):typ=int(scenario[5:],16)
  pairs.append((i,typ,value if key==f'Arg{arg}' else (i+1)*17))
 if scenario=='empty_root':pairs=[]
 put(BLOB+0x200,(0xc0 if scenario=='array_root' else 0xc1)|(len(pairs)<<8))
 for j,(i,t,v) in enumerate(pairs):put(BLOB+0x204+8*j,i|(t<<24));put(BLOB+0x208+8*j,v)
 success=scenario=='integer' and value!=0xffffffff
 return d,success

rng=random.Random(0x2794f8);values=[0,1,0xffffffff,0xfffffffe,0x80000000,0x7fffffff]+[rng.getrandbits(32) for _ in range(26)]
scenarios=['integer','invalid_header','null_root','missing_key','empty_keys','missing_pair','empty_root','array_root','type_d0','type_d2','type_a0','type_ff'];summaries=[];all_helpers=set()
for base,arg,direct in TARGETS:
 image=max((ROOT/'build/exact_checks/eu').glob(f'function_{base:08X}_*/candidate.axf'),key=lambda p:p.stat().st_mtime)
 with image.open('rb') as f:e=ELFFile(f);candidate=next(s.data() for s in e.iter_sections() if s['sh_addr']==base and s['sh_size'])
 original=binary[base-0x100000:base-0x100000+92];assert len(candidate)==92
 engines=[Engine(base,original,direct),Engine(base,candidate,direct)];count=0;maximum=0
 for scenario in scenarios:
  for value in values:
   initial=rng.getrandbits(32);data,success=data_case(arg,scenario,value,initial);results=[e.run(data) for e in engines]
   assert results[0][:3]==results[1][:3],('divergence',hex(base),scenario,hex(value))
   expected=bytearray(data)
   if success:struct.pack_into('<I',expected,0,value)
   assert results[0][0]==bytes(expected) and results[0][1]==int(success),('model disagreement',hex(base),scenario,hex(value),results[0][1],success,results[0][0][:4].hex())
   count+=1;maximum=max(maximum,*[r[3] for r in results]);all_helpers.update(results[0][2])
 summaries.append({'address':hex(base),'arg':arg,'direct':direct,'pairs':count,'max_instructions':maximum,'original_sha256':hashlib.sha256(original).hexdigest(),'candidate_sha256':hashlib.sha256(candidate).hexdigest(),'differing_bytes':sum(a!=b for a,b in zip(original,candidate))})
result={'total_pairs':sum(s['pairs'] for s in summaries),'per_function':summaries,'actual_retail_helper_entries':sorted(all_helpers),'scope':'Unicorn2.1.4 ARM1176, real retail Byaml validity/key lookup/hash search/string comparison/integer conversion execute. No imported helper is mocked. Synthetic bounded little-endian BYAML objects cover valid integers/sentinel, missing keys/pairs, empty structures, null/invalid views, nonhash roots and wrong data types. Full non-stack memory, boolean result, helper trace and preserved registers compare with independent key/type/sentinel model. Excludes malformed offsets, aliasing/reentrancy/concurrency/faults and physical hardware. No exact credit.'}
print(json.dumps(result,indent=2));(ROOT/'build/integer-argument-replay-results.json').write_text(json.dumps(result,indent=2))
```
