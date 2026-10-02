from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
import struct,random,json,hashlib,time
ROOT=Path.cwd(); OUT=ROOT/'build/shader-initializer'; BIN=(ROOT/'data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(BIN).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
V=0x800000;F=0x804000;ALLOC=0x810000;CMD=0x900000;SP=0xa0f000;END=0xb00000
REGS=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_R12]
SNAPS=[('validator',V,0x2000),('flags',F,0x100),('allocation',ALLOC,0x5000),('commands',CMD,0x20000),('globals',0x3e2600,0xc00),('BSS',0x420000,0x1400)]
def w(u,a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
def readw(u,a):return struct.unpack('<I',u.mem_read(a,4))[0]
def machine(candidate):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176)
 for a,n in [(0x100000,0x400000),(0x600000,0x10000),(0x700000,0x1000),(V,0x20000),(CMD,0x20000),(0xa00000,0x10000),(END,0x1000)]:u.mem_map(a,n)
 u.mem_write(0x100000,BIN);entry=0x108690
 if candidate:
  with (OUT/'diagnostic.axf').open('rb') as f:
   e=ELFFile(f);entry=e['e_entry']
   for s in e.iter_sections():
    if s['sh_flags']&2 and s['sh_size'] and s['sh_type']=='SHT_PROGBITS':u.mem_write(s['sh_addr'],s.data())
 u.events=[];u.callcounts={};u.alloc_result=ALLOC;u.fault=None
 def hook(m,a,n,d):
  if a in (0x28a304,0x28a14c,0x28d1f0,0x10a3ac):m.callcounts[hex(a)]=m.callcounts.get(hex(a),0)+1
  if a in (0x700000,0x700004):
   args=[m.reg_read(r) for r in REGS[:4]];m.events.append([hex(a),args])
   if a==0x700000:m.reg_write(UC_ARM_REG_R0,m.alloc_result)
   m.reg_write(UC_ARM_REG_PC,m.reg_read(UC_ARM_REG_LR))
 def fault(m,access,a,n,value,d):m.fault=[access,a,n];return False
 u.hook_add(UC_HOOK_CODE,hook);u.hook_add(UC_HOOK_MEM_INVALID,fault)
 return u,entry
M=[machine(False),machine(True)]
def run(u,entry,arg):
 for i,r in enumerate(REGS):u.reg_write(r,0xa5000000+i)
 u.reg_write(UC_ARM_REG_CPSR,0x10);u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,END);u.reg_write(UC_ARM_REG_R0,arg)
 u.fault=None
 try:u.emu_start(entry,END,count=400000)
 except UcError as e:
  return {'fault':u.fault,'pc':u.reg_read(UC_ARM_REG_PC),'error':str(e)}
 assert u.reg_read(UC_ARM_REG_PC)==END,('instruction cap',hex(u.reg_read(UC_ARM_REG_PC)))
 assert u.reg_read(UC_ARM_REG_SP)==SP
 assert all(u.reg_read(REGS[i])==0xa5000000+i for i in range(4,12))
 return {'returned':True}
def setup(u,seed,kind,capacity):
 u.mem_write(0x100000,BIN);u.mem_write(0x3f4000,bytes(0x10c000))
 for a,n in [(V,0x20000),(CMD,0x20000),(0xa00000,0x10000)]:u.mem_write(a,bytes([0xa6])*n)
 # Actual original producer establishes lookup, three masks, and validator defaults.
 assert run(u,0x1064e4,V)=={'returned':True}
 r=random.Random(seed)
 if kind!='manager':
  for i in range(189):
   w(u,V+0x100c+4*i,r.getrandbits(32));u.mem_write(V+0x15f4+i,bytes([0 if kind=='disabled' else (15 if kind=='all' else r.randrange(256))]))
 w(u,0x3e2e30,CMD);w(u,0x3e2e34,CMD+capacity)
 w(u,0x3e2654,0 if kind=='no_allocator' else 0x700000)
 w(u,0x3e2658,0 if kind=='no_release' else 0x700004)
 u.alloc_result=0 if kind=='failed_allocator' else ALLOC
 u.events=[];u.callcounts={};u.fault=None
 return CMD if kind=='flags_command_alias' else (V+0x100c if kind=='flags_value_alias' else F)
def pair(case):
 seed,kind,capacity=case;outputs=[]
 for u,entry in M:
  arg=setup(u,seed,kind,capacity);reps=[]
  for rep in range(2):
   result=run(u,entry,arg)
   snaps=[bytes(u.mem_read(a,n)) for _,a,n in SNAPS]
   reps.append((result,snaps,list(u.events),dict(u.callcounts),readw(u,0x3e2e30)-CMD))
   if 'fault' in result:break
  outputs.append(reps)
 failures=[]
 if len(outputs[0])!=len(outputs[1]):return {'case':case,'error':'different return/fault counts'}
 for n,(a,b) in enumerate(zip(*outputs)):
  if a[0]!=b[0]:failures.append({'rep':n,'result':[a[0],b[0]]})
  if a[2:]!=b[2:]:failures.append({'rep':n,'events_counts_cursor':[a[2:],b[2:]]})
  for (name,base,size),x,y in zip(SNAPS,a[1],b[1]):
   if x!=y:
    off=next(i for i in range(len(x)) if x[i]!=y[i]);failures.append({'rep':n,'region':name,'address':hex(base+off),'original':x[off//4*4:off//4*4+4].hex(),'source':y[off//4*4:off//4*4+4].hex()})
 return {'case':case,'failures':failures,'returns':sum('returned' in x[0] for x in outputs[0]),'faults':sum('fault' in x[0] for x in outputs[0]),'commands_bytes':[x[4] for x in outputs[0]],'calls':outputs[0][0][3]}
def main():
 cases=[]
 for kind in ['manager','disabled','all','random','no_release','flags_command_alias','flags_value_alias']:
  for seed in range(4):
   for capacity in [0,4,8,12,64,100,512,4096,10880,10900,10920,13000,27000,28500,0x20000]:cases.append((seed,kind,capacity))
 for kind in ['no_allocator','failed_allocator']:
  for capacity in [0,4,64,10880,0x20000]:cases.append((0,kind,capacity))
 t=time.monotonic();results=[]
 for case in cases:
  r=pair(case);results.append(r)
  if r.get('failures') or r.get('error'):print(json.dumps(r));break
 out={'cases_attempted':len(results),'planned':len(cases),'passed':sum(not x.get('failures') and not x.get('error') for x in results),'failed':sum(bool(x.get('failures') or x.get('error')) for x in results),'matching_returns':sum(x.get('returns',0) for x in results if not x.get('failures')),'matching_faults':sum(x.get('faults',0) for x in results if not x.get('failures')),'seconds':time.monotonic()-t,'results':results}
 (OUT/'replay-results.json').write_text(json.dumps(out,indent=2));print(json.dumps({k:v for k,v in out.items() if k!='results'},indent=2))
if __name__=='__main__':main()
