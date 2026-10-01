# Reproduce the configured builder semantic check

This optional analysis recipe operates only on the owner-provided EU input and the
unchanged project-built object. It does not edit source, map, ranks, the oracle, or
any object. Its original-address diagnostic link is not a canonical checker.
Run the project build first with its configured flags. Save the Python block to a
scratch file, then run it from the repository root using the existing project
Python environment. `--smoke` runs four cases; without it, the recipe runs all 435.
It requires the already available Capstone/Unicorn/pyelftools environment; do not
install or substitute tools merely to run this note.

The observed caller-side ABI contracts and import identities come from the CSV
notes committed alongside the source. The script leaves diagnostic outputs only
under the ignored `build/player_builder_reproduction` directory. All limitations
of the companion report apply: external callees are identically stubbed, and no
match, gameplay, or complete-callee equivalence claim follows.

```python
from pathlib import Path
import json,struct,collections,sys,time,csv,hashlib,subprocess,os
from unicorn import *
from unicorn.arm_const import *
from elftools.elf.elffile import ELFFile
ROOT=Path.cwd()
OUT=ROOT/'build/player_builder_reproduction'
OUT.mkdir(exist_ok=True)
B=(ROOT/'data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(B).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
NAME='_ZN29PlayerActionGraphBuildOutputs5buildEP6Player'
OBJ=ROOT/'build/eu/obj/Game/backup/src/Player/PlayerActionGraphBuilder.o'
PROVENANCE=json.loads(OBJ.with_suffix('.provenance.json').read_text())
assert hashlib.sha256(OBJ.read_bytes()).hexdigest()==PROVENANCE['object_sha256']
assert '-O3' in PROVENANCE['command'] and '-Otime' in PROVENANCE['command']
assert '#pragma O0' not in (ROOT/'Game/backup/src/Player/PlayerActionGraphBuilder.cpp').read_text()
IMPORTS={r['symbol']:(int(r['observed_address'],16),r['kind']) for r in csv.DictReader((ROOT/'project/dot_reports/player-action-builder-source/proposed_imports.csv').open())}
with OBJ.open('rb') as file:
 elf=ELFFile(file);section=elf.get_section_by_name('i.'+NAME);size=section['sh_size']
 symbols=elf.get_section_by_name('.symtab');relocations=elf.get_section_by_name('.reli.'+NAME)
 required={symbols.get_symbol(r['r_info_sym']).name for r in relocations.iter_relocations()}
 assert required==set(IMPORTS)
 assert [s.name for s in elf.iter_sections() if s.name.startswith(('i.','t.'))]==['i.'+NAME]
(OUT/'original_symbols.sym').write_text('#<SYMDEFS>#\n'+''.join('0x%08X %s %s\n'%(a,k,n) for n,(a,k) in sorted(IMPORTS.items())))
(OUT/'candidate.sct').write_text('CANDIDATE_LOAD 0x001A9574\n{\n CANDIDATE_CODE 0x001A9574\n {\n PlayerActionGraphBuilder.o (i.%s, +FIRST)\n }\n}\n'%NAME)
command=[str(ROOT/'data/compilers/wibo'),str(ROOT/'data/compilers/4.1/791/bin/armlink.exe'),'--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--inline','--datacompressor=off','--no_debug','--no_scanlib','--mangled','--symbols','--map','--entry='+NAME,'--keep='+NAME,'--scatter='+str(OUT/'candidate.sct'),'--output='+str(OUT/'candidate.axf'),'--list='+str(OUT/'candidate.map'),str(OBJ),str(OUT/'original_symbols.sym')]
process=subprocess.run(command,env=dict(os.environ,TMP='/tmp'),capture_output=True,text=True)
(OUT/'link.log').write_text(process.stdout+process.stderr)
assert process.returncode==0,process.stdout+process.stderr
with (OUT/'candidate.axf').open('rb') as file:
 elf=ELFFile(file);allocated=[s for s in elf.iter_sections() if s['sh_flags']&2 and s['sh_size']]
 assert len(allocated)==1 and allocated[0]['sh_addr']==0x1a9574 and allocated[0]['sh_size']==size
 D=allocated[0].data()
CONTRACTS={str(int(r['constructor'],16)):[{'core_parameters':int(r['R1_R3_parameters']),'stack_parameters':int(r['stack_word_parameters']),'float_regs':r['VFP_parameters']}] for r in csv.DictReader((ROOT/'project/dot_reports/largest-root-layout/constructor_contracts.csv').open())}
EXTERNS={a for a,k in IMPORTS.values() if k=='A'}
REGS=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3]
class Trace:
 def __init__(self,candidate,fail=None,null=None,seed=0):
  self.u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u=self.u;u.ctl_set_cpu_model(UC_CPU_ARM_1176)
  u.mem_map(0x100000,0x400000);u.mem_write(0x100000,B)
  if candidate:u.mem_write(0x1a9574,D)
  for a,n in [(0x2000000,0x200000),(0x3000000,0x10000),(0x8000000,0x200000),(0x10000000,0x10000)]:u.mem_map(a,n)
  u.mem_write(0x8000000,b'\xa5'*0x10000)
  u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000)
  self.player=0x2000000;self.out=0x20f0000;self.system=0x3000000;self.seed=seed
  for off in range(0,0xf8,4):self.write(self.player+off,0 if off==null else 0x2010000+off*0x100)
  self.write(self.system,self.system+0x1000)
  for off in (0x17c,0x194,0x108,0x1a4):self.write(self.system+0x1000+off,self.system+0x2000+off)
  u.reg_write(UC_ARM_REG_R0,self.out);u.reg_write(UC_ARM_REG_R1,self.player);u.reg_write(UC_ARM_REG_SP,0x10008000);u.reg_write(UC_ARM_REG_LR,0x300f000)
  self.heap=0x8000000;self.fail=fail;self.alloc=[];self.calls=[];self.steps=0;self.fault=None
  u.hook_add(UC_HOOK_CODE,self.hook)
  u.hook_add(UC_HOOK_MEM_INVALID,self.badmemory)
 def write(self,a,v):self.u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def read(self,a):return struct.unpack('<I',self.u.mem_read(a,4))[0]
 def badmemory(self,u,access,address,size,value,_):self.fault=(access,address,size);return False
 def normalize(self,v,target,index):
  if target==0x251fec and index==2:return ('temporary',self.read(v))
  if target==0x251ea0 and index==2 and v:
   raw=bytearray()
   while len(raw)<100:
    c=self.u.mem_read(v+len(raw),1)[0]
    if c==0:break
    raw.append(c)
   return ('string',raw.decode())
  return v
 def hook(self,u,a,size,_):
  self.steps+=1
  if a==0x300f000:u.emu_stop();return
  if a not in EXTERNS and not 0x3002000<=a<0x3002200:return
  rv=[u.reg_read(r) for r in REGS];sp=u.reg_read(UC_ARM_REG_SP);ret=u.reg_read(UC_ARM_REG_LR)
  if str(a) in CONTRACTS:
   co=CONTRACTS[str(a)][0];args=rv[:1+co['core_parameters']]+[self.read(sp+i*4) for i in range(co['stack_parameters'])]
   args=[self.normalize(v,a,i) for i,v in enumerate(args)]
   if co['float_regs']:args.append(('float',u.reg_read(UC_ARM_REG_S0)))
  elif a in (0x2932b0,0x292a78,0x308fc8):args=rv[:1]
  elif a==0x251e28:args=rv[:3]
  elif a in (0x252008,0x251f74):args=rv[:2]
  elif a==0x26e1dc:args=[]
  else:args=rv[:1]
  self.calls.append((a,args))
  result=rv[0];floating=None
  if a in (0x2932b0,0x292a78):
   p=(self.heap+15)&~15;self.heap=p+max(rv[0],16);result=0 if len(self.alloc)==self.fail else p
   self.alloc.append((rv[0],a==0x292a78,result))
  elif a==0x26e1dc:result=self.system
  elif a==0x308fc8:
   p=self.read(self.player+0x7c);result=p+4 if p else 0
  elif 0x3002000<=a<0x3002200:
   if a==0x3002108:floating=0x40400000+(self.seed&7)*0x80000
   else:result={0x17c:0x101,0x194:0x102,0x1a4:0x103}[a-0x3002000]+self.seed
  # Deliberately clobber ABI caller-saved registers after observing arguments.
  if self.seed:
   for i,r in enumerate(REGS[1:]):u.reg_write(r,0xdead0000+i+self.seed)
   u.reg_write(UC_ARM_REG_R12,0xbeef0000+self.seed)
   for i in range(16):u.reg_write(UC_ARM_REG_S0+i,0xbad00000+i+self.seed)
  u.reg_write(UC_ARM_REG_R0,result)
  if floating is not None:u.reg_write(UC_ARM_REG_S0,floating)
  u.reg_write(UC_ARM_REG_PC,ret)
 def run(self):
  try:self.u.emu_start(0x1a9574,0x300f000,count=200000)
  except UcError:
   if self.fault is None:raise
  if self.fault is None:assert self.u.reg_read(UC_ARM_REG_PC)==0x300f000, 'execution did not reach the return sentinel'
  terminal=('fault',self.fault) if self.fault else ('return',self.u.reg_read(UC_ARM_REG_R0))
  heap=[(n,array,p,bytes(self.u.mem_read(p,n)).hex() if p else None) for n,array,p in self.alloc]
  return {'terminal':terminal,'calls':self.calls,'allocations':heap,'outputs':[self.read(self.out+i) for i in range(0,0x24,4)],'steps':self.steps}
def pair(fail=None,null=None,seed=0):
 a=Trace(False,fail,null,seed).run();b=Trace(True,fail,null,seed).run()
 for k in ('terminal','calls','allocations','outputs'):
  if a[k]!=b[k]:
   info={'fail':fail,'null':null,'seed':seed,'field':k,'original':a[k],'candidate':b[k]};(OUT/'replay_failure.json').write_text(json.dumps(info,indent=2));raise AssertionError((fail,null,seed,k))
 return a['terminal'][0]
def main():
 begin=time.monotonic();summary=collections.Counter();cases=[]
 for seed in (0,1,17,255):cases.append((None,None,seed))
 for off in range(0,0xe4,4):cases.append((None,off,17))
 for fail in range(374):cases.append((fail,None,17))
 if '--smoke' in sys.argv:cases=[(None,None,17),(47,None,17),(48,None,17),(None,0x7c,17)]
 for i,(fail,null,seed) in enumerate(cases):
  summary[pair(fail,null,seed)]+=1
  if (i+1)%50==0:print('paired',i+1,'cases',dict(summary),flush=True)
 result={'mode':'smoke' if '--smoke' in sys.argv else 'full','cases':len(cases),'outcomes':dict(summary),'seconds':round(time.monotonic()-begin,3),'scope':'The complete root only, with original external constructor/append behavior stubbed identically. Caller-saved clobber and nonzero heap fill are enabled. The full mode covers null component variants and every root-local allocation failure ordinal; smoke mode uses four selected cases. No gameplay or full-callee equivalence claim.'}
 (OUT/'semantic_results.json').write_text(json.dumps(result,indent=2));print(json.dumps(result,indent=2))
if __name__=='__main__':main()
```
