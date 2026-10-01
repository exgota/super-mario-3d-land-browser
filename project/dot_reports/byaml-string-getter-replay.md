# Reproduce the bounded Byaml string getter evidence

Use this notes branch (or exact base `3de056e0dcb33619c32f8b46ef64f74c90d28934` with these two notes), the owner's verified local EU code/exheader, the installed approved ARMCC 4.1/791/wibo toolchain and Python environment with pyelftools and Unicorn 2.1.4. No game data or generated binaries are part of this document. Run from the repository root. The setup refuses changed C++/header/checker/configuration hashes; the unchanged canonical checker enforces committed-source provenance. It checks both complete intervals, retains its generated candidate AXFs and restores the map byte-for-byte if a check ever changes it.

Extract the two complete programs below into the ignored build directory and execute them:

```sh
. ./development_environment.sh
export DEVKITARM=/usr  # Linux binutils location; use the installed location on another host
python - <<'PYTHON'
from pathlib import Path
text=Path('project/dot_reports/byaml-string-getter-replay.md').read_text()
folder=Path('build/dot_byaml_string_getter');folder.mkdir(parents=True,exist_ok=True)
for label in ('canonical','replay'):
    marker='<!-- program: '+label+' -->'
    code=text.split(marker,1)[1].split('```python\n',1)[1].split('\n```',1)[0]
    (folder/(label+'.py')).write_text(code+'\n')
PYTHON
python build/dot_byaml_string_getter/canonical.py
python build/dot_byaml_string_getter/replay.py
```

The first command builds normally before checking canonical output. The expected getter checker exit is 1 with a plain 15-byte mismatch; the accepted lookup exits 0 with all 108 bytes equal. The replay program only reads those linked outputs, and overlays them solely in emulator memory. It requires no map-name diagnostics, function-boundary changes, compiler-option changes, source modifications or custom checker.

## Canonical build/check program

<!-- program: canonical -->
```python
#!/usr/bin/env python3
from pathlib import Path
import hashlib,json,subprocess,shutil
from elftools.elf.elffile import ELFFile
root=Path.cwd();out=root/'build/dot_byaml_string_getter';out.mkdir(parents=True,exist_ok=True)
expected_inputs={
 'lib/al/src/Yaml/alByamlStringTableIter.cpp':'ba5cee2db169501269a2a9a0a54984b76d7bcd710ffe216b82c4ac8952543a91',
 'lib/al/include/Yaml/alByamlStringTableIter.h':'801b05e2dc41f7e4f8cc48059d8caf19a1591e57c3ae92d2308b643588a4527a',
 'tools/check.py':'e177e0abe130e645e75c5f0dc24abb19dbe83310de55f1f099ec8fc385e07317',
 'tools/low/checkExactBytes.py':'aef81a3a21c49bb17b8a19f139d02607899257e04dbe461ece2992cc726fc2e2',
 'data/config.json':'5d41e617eb5b26374ed60b5383a0940ce0ce7d046344f7f815b7564144822c45',
 'data/ver/eu/code.bin':'e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'}
for path,sha in expected_inputs.items():assert hashlib.sha256((root/path).read_bytes()).hexdigest()==sha,path
with (out/'build.log').open('w') as log:subprocess.run(['python','make.py','eu'],stdout=log,stderr=subprocess.STDOUT,check=True)
m=root/'data/ver/eu/map.csv';before=m.read_bytes();obj=root/'build/eu/obj/lib/al/src/Yaml/alByamlStringTableIter.o'
report={'source_head':subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip(),'functions':{}}
try:
 for name,addr,n,expect,delta in [('getString',0x28ca30,24,1,15),('findStringIndex',0x28b518,108,0,0)]:
  sym='_ZNK2al20ByamlStringTableIter'+('9getStringEi' if name=='getString' else '15findStringIndexEPKc')
  old=set((root/'build/exact_checks/eu').glob(f'function_{addr:08X}_*'))
  p=subprocess.run(['python','tools/check.py',sym,'--object',str(obj)],capture_output=True,text=True)
  (out/(name+'_check.log')).write_text(p.stdout+p.stderr);print(name,p.returncode,p.stdout.strip())
  assert p.returncode==expect
  new=set((root/'build/exact_checks/eu').glob(f'function_{addr:08X}_*'))-old;assert len(new)==1
  folder=new.pop();shutil.copytree(folder,out/name,dirs_exist_ok=True)
  with (folder/'candidate.axf').open('rb') as f:
   e=ELFFile(f);sec=e.get_section_by_name('CANDIDATE_CODE');data=sec.data()
   assert sec['sh_addr']==addr and sec['sh_size']==n
  target=(root/'data/ver/eu/code.bin').read_bytes()[addr-0x100000:addr-0x100000+n]
  different=sum(a!=b for a,b in zip(data,target));assert different==delta
  report['functions'][name]={'address':hex(addr),'bytes':len(data),'different_bytes':different,'candidate_sha256':hashlib.sha256(data).hexdigest(),'original_sha256':hashlib.sha256(target).hexdigest(),'checker_exit':p.returncode,'checker_output':p.stdout.strip()}
 assert m.read_bytes()==before
finally:
 if m.read_bytes()!=before:m.write_bytes(before)
report['map_unchanged_sha256']=hashlib.sha256(before).hexdigest();report['object_sha256']=hashlib.sha256(obj.read_bytes()).hexdigest()
(out/'canonical_results.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
```

## Whole-original replay program

<!-- program: replay -->
```python
#!/usr/bin/env python3
"""Bounded whole-ARM BYAML replay; no retail callee replacement."""
from pathlib import Path
import hashlib,json,struct
from collections import Counter
from elftools.elf.elffile import ELFFile
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE,UC_HOOK_MEM_WRITE,UC_HOOK_MEM_READ
from unicorn.arm_const import *
ROOT=Path.cwd();WORK=ROOT/'build/dot_byaml_string_getter'
TARGET=(ROOT/'data/ver/eu/code.bin').read_bytes()
TARGET_SHA='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
assert hashlib.sha256(TARGET).hexdigest()==TARGET_SHA
BASE,DATA,STACK,STOP=0x100000,0x600000,0x700000,0x7ff000
IT,OUT,KEYOUT,QUERY=DATA+0xf000,DATA+0xf020,DATA+0xf040,DATA+0xf100
GET,LOOKUP,STRKEY,PAIRKEY=0x28ca30,0x28b518,0x29101c,0x33753c
REGS=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_R12]
P=lambda n:struct.pack('<I',n&0xffffffff)
CAND={}
for label,addr,n in [('getString',GET,24),('findStringIndex',LOOKUP,108)]:
 with (WORK/label/'candidate.axf').open('rb') as f:
  sec=ELFFile(f).get_section_by_name('CANDIDATE_CODE')
  assert sec['sh_addr']==addr and sec['sh_size']==n
  CAND[addr]=sec.data()
assert CAND[LOOKUP]==TARGET[LOOKUP-BASE:LOOKUP-BASE+108]
assert sum(a!=b for a,b in zip(CAND[GET],TARGET[GET-BASE:GET-BASE+24]))==15

def table(strings,layout):
 """Serialized four-byte header plus u32 relative offsets, valid NUL strings."""
 buf=bytearray(P((len(strings)<<8)|0xc2)+bytes(4*len(strings)))
 buf+=b'\xaa'*(0 if layout==0 else 28)
 indices=list(range(len(strings)))
 if layout==2:indices.reverse()
 offsets=[None]*len(strings)
 for i in indices:
  offsets[i]=len(buf)
  buf+=strings[i]+b'\0'+b'\xcc'*(0 if layout==0 else (i%5))
 for i,offset in enumerate(offsets):struct.pack_into('<I',buf,4+4*i,offset)
 return bytes(buf),offsets

class Runner:
 def __init__(self,compiled):
  self.u=Uc(UC_ARCH_ARM,UC_MODE_ARM);self.u.ctl_set_cpu_model(UC_CPU_ARM_1176)
  self.u.mem_map(BASE,0x400000);self.u.mem_write(BASE,TARGET)
  self.u.mem_map(DATA,0x10000);self.u.mem_map(STACK,0x100000)
  if compiled:
   for address,code in CAND.items():self.u.mem_write(address,code)
  self.u.hook_add(UC_HOOK_CODE,self.code)
  self.u.hook_add(UC_HOOK_MEM_WRITE,self.write)
  self.u.hook_add(UC_HOOK_MEM_READ,self.read)
 def code(self,u,address,size,user):
  assert BASE<=address<BASE+len(TARGET),hex(address)
  self.steps+=1;self.called.add(address)
 def write(self,u,access,address,size,value,user):
  if not STACK<=address<STACK+0x100000:self.writes.append((address,size))
 def read(self,u,access,address,size,value,user):
  if self.entry==GET:self.reads.append((address,size))
 def run(self,entry,memory,args):
  u=self.u;u.mem_write(DATA,memory);u.mem_write(STACK+0xd0000,bytes(0x20000))
  u.reg_write(UC_ARM_REG_CPSR,0x10)
  for i,reg in enumerate(REGS):u.reg_write(reg,0xa1000000+i)
  for i,val in enumerate(args):u.reg_write(REGS[i],val&0xffffffff)
  u.reg_write(UC_ARM_REG_SP,STACK+0xe0000);u.reg_write(UC_ARM_REG_LR,STOP)
  self.steps=0;self.called=set();self.writes=[];self.reads=[];self.entry=entry
  u.emu_start(entry,STOP,count=30000)
  assert u.reg_read(UC_ARM_REG_PC)==STOP,(hex(entry),self.steps)
  assert u.reg_read(UC_ARM_REG_SP)==STACK+0xe0000
  assert all(u.reg_read(REGS[i])==0xa1000000+i for i in range(4,12))
  return (u.reg_read(UC_ARM_REG_R0),bytes(u.mem_read(DATA,0x10000)))

left,right=Runner(False),Runner(True)
counts=Counter();calls=Counter();instructions=0;digest=hashlib.sha256();cases=0

def check(label,entry,memory,args,expect,changes=()):
 global instructions,cases
 original=bytes(memory);x=left.run(entry,original,args);y=right.run(entry,original,args)
 assert x==y,(label,args,x[0],y[0])
 expected=bytearray(original)
 for address,content in changes:expected[address-DATA:address-DATA+len(content)]=content
 assert x==(expect&0xffffffff,bytes(expected)),(label,args,x[0],expect)
 for runner in (left,right):
  assert all(any(a<=address and address+size<=a+len(content) for a,content in changes) for address,size in runner.writes),(label,runner.writes)
  if entry==GET:
   base=struct.unpack_from('<I',original,IT-DATA)[0]
   assert runner.reads==[(IT,4),(base+4+4*args[1],4)],runner.reads
  for address in (GET,LOOKUP,0x28aa60,STRKEY,PAIRKEY,0x28cab4,0x331314):
   if address in runner.called:calls[hex(address)]+=1
  instructions+=runner.steps
 digest.update(P(entry)+P(x[0])+hashlib.sha256(original).digest()+hashlib.sha256(x[1]).digest())
 counts[label]+=1;cases+=1

def memory():
 buf=bytearray(b'\x5a'*0x10000)
 buf[OUT-DATA:OUT-DATA+8]=bytes.fromhex('1122334455667788')
 buf[KEYOUT-DATA:KEYOUT-DATA+4]=P(0x87654321)
 return buf

# Sorted byte strings include empty, long shared prefixes, and bytes >=0x80.
for count in (1,2,3,7,16,31,64,257):
 strings=sorted([b'']+[b'K%04d_'%i+b'x'*(i%65)+bytes([0x80+(i%128)]) for i in range(1,count)])
 for layout,location in ((0,0x80),(1,0x3fc),(2,0x1004)):
  blob,offsets=table(strings,layout);buf=memory()
  buf[location:location+len(blob)]=blob;buf[IT-DATA:IT-DATA+4]=P(DATA+location)
  for index,offset in enumerate(offsets):check('direct_getter',GET,buf,(IT,index),DATA+location+offset)
  # Entire canonical accepted lookup executes original strcmp; each valid string plus absent queries.
  for query in strings+[b'!',b'K0000',b'K9999',b'\xff']:
   test=bytearray(buf);test[QUERY-DATA:QUERY-DATA+len(query)+1]=query+b'\0'
   check('direct_lookup',LOOKUP,test,(IT,QUERY),strings.index(query) if query in strings else -1)
# Repeated offsets/empty strings are accepted by the getter's unchecked access path.
for layout in (0,1,2):
 strings=[b'',b'alpha',b'alpha',b'\x80\xff',b'']
 blob,offsets=table(strings,layout);buf=memory();buf[0x100:0x100+len(blob)]=blob
 for i in (2,4):struct.pack_into('<I',buf,0x104+4*i,offsets[1] if i==2 else offsets[0])
 offsets[2]=offsets[1];offsets[4]=offsets[0];buf[IT-DATA:IT-DATA+4]=P(DATA+0x100)
 for i,offset in enumerate(offsets):check('direct_alias_getter',GET,buf,(IT,i),DATA+0x100+offset)
# Empty table lookup must return -1 without strcmp or getter access.
buf=memory();buf[0x100:0x104]=P(0xc2);buf[IT-DATA:IT-DATA+4]=P(DATA+0x100);buf[QUERY-DATA:QUERY-DATA+2]=b'x\0'
check('empty_lookup',LOOKUP,buf,(IT,QUERY),-1)

# Construct complete BYAML files for two actual original callers and all their callees.
keys=[b'Key%02d'%i for i in range(16)]
strings=[b'',b'a',b'alpha',b'prefix',b'prefix-long',b'z',b'\x80',b'\xff']
for layout in (0,1,2):
 kt,ko=table(keys,layout);st,so=table(strings,layout)
 for typ in (0xa0,0xd0,0xd1,0xd2,0xff,0xc0,0xc1):
  buf=memory();buf[:16]=struct.pack('<HHIII',0x4259,1,0x40,0x2000,0x4000)
  buf[0x40:0x40+len(kt)]=kt;buf[0x2000:0x2000+len(st)]=st
  buf[0x4000:0x4004]=P((len(keys)<<8)|0xc1)
  for i in range(len(keys)):buf[0x4004+i*8:0x400c+i*8]=P((typ<<24)|i)+P(i%len(strings))
  buf[IT-DATA:IT-DATA+8]=P(DATA)+P(DATA+0x4000)
  for i,key in enumerate(keys):
   test=bytearray(buf);test[QUERY-DATA:QUERY-DATA+len(key)+1]=key+b'\0'
   good=typ==0xa0
   changes=((OUT,P(DATA+0x2000+so[i%len(strings)])),) if good else ()
   check('caller_string_by_key',STRKEY,test,(IT,OUT,QUERY),int(good),changes)
   check('caller_data_and_key',PAIRKEY,test,(IT,OUT,KEYOUT,i),1,((OUT,P(i%len(strings))+bytes([typ])),(KEYOUT,P(DATA+0x40+ko[i]))))
  for absent in (b'!',b'Key99',b'\xff'):
   test=bytearray(buf);test[QUERY-DATA:QUERY-DATA+len(absent)+1]=absent+b'\0'
   check('caller_missing_key',STRKEY,test,(IT,OUT,QUERY),0)
  for index in (-1,16,0x7fffffff):check('caller_invalid_pair_index',PAIRKEY,buf,(IT,OUT,KEYOUT,index),0)
 # Invalid root guards avoid any getter access and leave outputs intact.
 for root in (0,DATA+0x4000):
  buf=memory();buf[:16]=struct.pack('<HHIII',0x4259,1,0x40,0x2000,0x4000)
  buf[0x4000:0x4004]=P(0xc0);buf[IT-DATA:IT-DATA+8]=P(DATA)+P(root)
  buf[QUERY-DATA:QUERY-DATA+6]=b'Key00\0'
  check('caller_invalid_root',STRKEY,buf,(IT,OUT,QUERY),0)
  check('caller_invalid_root',PAIRKEY,buf,(IT,OUT,KEYOUT,0),0)

report={'cases':cases,'paired_executions':2*cases,'counts':dict(counts),'instructions':instructions,'actual_entry_visits':dict(calls),'result_sha256':digest.hexdigest(),'retail_sha256':TARGET_SHA,'candidate_sha256':{hex(a):hashlib.sha256(code).hexdigest() for a,code in CAND.items()},'scope':'Constructed serialized BYAML data only. Executes whole retail callers and callees; overlays only the canonical 24-byte getter and byte-exact accepted 108-byte lookup. Compares return values, all 64 KiB input/output memory, allowed output writes, stack pointer, and callee-saved registers. Direct getter also verifies its exact two memory reads. No malformed getter indices, unmapped input safety, actual asset loading, concurrency, performance, or gameplay claim.'}
(WORK/'replay_result.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
```

## Recorded replay result

```json
{
  "cases": 3208,
  "paired_executions": 6416,
  "counts": {
    "direct_getter": 1143,
    "direct_lookup": 1239,
    "direct_alias_getter": 15,
    "empty_lookup": 1,
    "caller_string_by_key": 336,
    "caller_data_and_key": 336,
    "caller_missing_key": 63,
    "caller_invalid_pair_index": 63,
    "caller_invalid_root": 12
  },
  "instructions": 1168984,
  "actual_entry_visits": {
    "0x28ca30": 3084,
    "0x28b518": 3278,
    "0x28aa60": 3276,
    "0x29101c": 810,
    "0x28cab4": 810,
    "0x33753c": 810,
    "0x331314": 798
  },
  "result_sha256": "9ecacc664db2c9d9d40130157c8d1d7f958a1238807232a3c6aa05a6b7f2b154",
  "retail_sha256": "e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64",
  "candidate_sha256": {
    "0x28ca30": "403e01035d63d72e733a43fa345285a1dabf649d3335b68097d02688d15d7ef3",
    "0x28b518": "60c8973d0f7f1441bc4485cd6958d3af4dc52d58ba234280bbf60b08addf11d2"
  },
  "scope": "Constructed serialized BYAML data only. Executes whole retail callers and callees; overlays only the canonical 24-byte getter and byte-exact accepted 108-byte lookup. Compares return values, all 64 KiB input/output memory, allowed output writes, stack pointer, and callee-saved registers. Direct getter also verifies its exact two memory reads. No malformed getter indices, unmapped input safety, actual asset loading, concurrency, performance, or gameplay claim."
}
```
