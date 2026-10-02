# Diagnostic reproduction scripts

These scripts read the owner-provided private EU executable and use committed source built by the unchanged project. They do not change compiler objects or oracle code. Run from this worktree with the project environment sourced. `check.py` temporarily names only existing map rows, then restores the exact original map. `link_replay.py` is explicitly a behavior-only link and binds the independently evidenced external matrix; it does not establish canonical closure.

Save each block at the displayed ignored `build/particle-editor-parameters/` path before running it. The preservation and seal scripts use sibling `mario-mainbc5` as the pristine baseline; change only that path when reproducing elsewhere.

## check.py

```python
from pathlib import Path
import re,subprocess,json,hashlib,time
out=Path('build/particle-editor-parameters');p=Path('data/ver/eu/map.csv');old=p.read_bytes()
assert old==Path('../mario-mainbc5/data/ver/eu/map.csv').read_bytes()
src='\n'.join(Path(f).read_text() for f in ['lib/al/src/Effect/retail_ParticleEditorParameters.cpp','lib/al/include/Effect/retail_ParticleEditorParameters.h']);names={int(a,16):prefix+a for prefix,a in re.findall(r'\b(fn_|dat_)([0-9A-F]{8})\b',src)}
lines=[];changed=[]
for l in old.decode().splitlines(True):
 r=l.rstrip('\n').split(',')
 if r and r[0].startswith('0x') and int(r[0],16) in names:
  assert not r[6].strip() or r[6]==names[int(r[0],16)],l
  r[6]=names[int(r[0],16)];changed.append(r[6]);l=','.join(r)+'\n'
 lines.append(l)
begin=time.monotonic()
try:
 p.write_text(''.join(lines))
 command=['python','tools/check.py','fn_002EDFE8','--object','build/eu/obj/lib/al/src/Effect/retail_ParticleEditorParameters.o']
 r=subprocess.run(command,text=True,capture_output=True);(out/'form2-check.txt').write_text(r.stdout+r.stderr);print(r.stdout,r.stderr)
 command=['python','tools/low/checkExactBytes.py','fn_002EDFE8','build/eu/obj/lib/al/src/Effect/retail_ParticleEditorParameters.o','--compiler','4.1/791']
 r=subprocess.run(command,text=True,capture_output=True);(out/'form2-result.json').write_text(r.stdout)
finally:p.write_bytes(old)
assert p.read_bytes()==old
(out/'form2-check-meta.json').write_text(json.dumps({'elapsed_seconds':time.monotonic()-begin,'map_restored_sha256':hashlib.sha256(old).hexdigest(),'renamed_existing_rows':changed},indent=2))
```

## link_replay.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
import csv,re,subprocess,json
out=Path('build/particle-editor-parameters');obj=Path('build/eu/obj/lib/al/src/Effect/retail_ParticleEditorParameters.o');elf=ELFFile(obj.open('rb'))
rows={r[6]:r for r in csv.reader(open('data/ver/eu/map.csv')) if r and r[0].startswith('0x') and r[6]}
imports=[]
for s in elf.get_section_by_name('.symtab').iter_symbols():
 if s['st_shndx']!='SHN_UNDEF' or not s.name or s.name.startswith('Lib$$'):continue
 n=s.name
 if n in rows:a=int(rows[n][0],16);kind='A' if 'f' in rows[n][5] else 'D'
 elif re.fullmatch(r'(fn_|dat_)[0-9A-F]{8}',n):a=int(n[-8:],16);kind='A' if n.startswith('fn_') else 'D'
 else:print('Unresolved unused:',n);continue
 imports.append({'symbol':n,'address':a,'kind':kind,'missing_map':n=='dat_00430A88'})
(out/'imports.sym').write_text('#<SYMDEFS>#\n'+''.join('0x%08X %s %s\n'%(i['address'],i['kind'],i['symbol']) for i in imports))
(out/'behavior.sct').write_text('ROOT_LOAD 0x00500000\n{\n ROOT_CODE 0x00500000 { retail_ParticleEditorParameters.o (i.fn_002EDFE8, +FIRST) }\n REMAINDER +0 { .ANY(+RO,+RW,+ZI) }\n}\n')
command=['data/compilers/wibo','data/compilers/4.1/791/bin/armlink.exe','--scatter',str(out/'behavior.sct'),'--entry','fn_002EDFE8','--no_scanlib','--output',str(out/'behavior.axf'),'--map','--list',str(out/'behavior.map'),str(obj),str(out/'imports.sym')]
r=subprocess.run(command,text=True,capture_output=True);(out/'diagnostic-link.txt').write_text(r.stdout+r.stderr);print(r.stdout,r.stderr);r.check_returncode()
(out/'diagnostic-link.json').write_text(json.dumps({'command':command,'imports':imports,'status':r.returncode,'credit':'behavior diagnostic only; no object editing or canonical metadata change'},indent=2))
```

## replay.py

```python
from pathlib import Path
from collections import Counter
import csv,hashlib,json,struct,time,random
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
OUT=Path('build/particle-editor-parameters'); ROOT=0x2edfe8; END=0x2ee878; CAND=0x500000; STOP=0x700000; DATA=0x800000; SP=0xa08000
CODE=Path('data/ver/eu/code.bin').read_bytes();assert hashlib.sha256(CODE).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
ELF=ELFFile(open(OUT/'behavior.axf','rb'));SECTIONS=[s for s in ELF.iter_sections() if s['sh_flags']&2 and s['sh_size']]
ENTRIES={int(r[0],16) for r in csv.reader(open('data/ver/eu/map.csv')) if r and r[0].startswith('0x') and r[5].strip().startswith('f')}
SR=[UC_ARM_REG_R4+i for i in range(8)];DR=[UC_ARM_REG_D8+i for i in range(8)];SAVED=[0xabcd0000+i for i in range(8)];DSAVED=[0x7fe1234512340000+i for i in range(8)]
def rd(u,a):return struct.unpack('<I',u.mem_read(a,4))[0]
def cs(u,a):
 b=bytearray()
 for i in range(0x10000):
  v=u.mem_read(a+i,1)[0]
  if not v:return bytes(b)
  b.append(v)
 raise RuntimeError('unterminated diagnostic string')
CALLS=[Counter(),Counter()];COVERAGE=[set(),set()];TRACE=[[],[]];EVENTS=[[],[]];FAULTS=[None,None];COUNT=[0,0];MACHINES=[];RECEIVER=[DATA+0x100]*2;INITIALIZING=[True,True]
for j in range(2):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176)
 for a,n in [(0x100000,0x400000),(CAND,0x10000),(STOP,0x1000),(DATA,0x10000),(0xa00000,0x10000)]:u.mem_map(a,n)
 u.mem_write(0x100000,CODE)
 for s in SECTIONS:u.mem_write(s['sh_addr'],s.data())
 def hook(uc,a,size,ignored,j=j):
  if INITIALIZING[j]:return
  COUNT[j]+=1
  if (j==0 and ROOT<=a<END) or (j==1 and CAND<=a<CAND+0x10000):COVERAGE[j].add(a)
  if a in ENTRIES:
   CALLS[j][hex(a)]+=1;TRACE[j].append(a)
   if len(TRACE[j])>30:TRACE[j].pop(0)
  r0=uc.reg_read(UC_ARM_REG_R0)
  if a==0x28e1e4:
   fmt=cs(uc,uc.reg_read(UC_ARM_REG_R1));r2=uc.reg_read(UC_ARM_REG_R2);r3=uc.reg_read(UC_ARM_REG_R3);args=[]
   if b'%5.1f' in fmt:
    args=[struct.pack('<II',r2,r3).hex()]
    if fmt.count(b'%5.1f')==2:args.append(bytes(uc.mem_read(uc.reg_read(UC_ARM_REG_SP),8)).hex())
   elif b'%s' in fmt:args=[cs(uc,r2).hex()]
   elif b'%d' in fmt:args=[r2]
   else:raise AssertionError(fmt)
   EVENTS[j].append(['format',r0-RECEIVER[j],fmt.decode('ascii'),args,rd(uc,r0+8)])
  elif a==0x2499f4:
   r1=uc.reg_read(UC_ARM_REG_R1)
   EVENTS[j].append(['copy',r0-RECEIVER[j],cs(uc,rd(uc,r1+4)).hex(),uc.reg_read(UC_ARM_REG_R2)])
  elif a in (0x21f264,0x21f21c):EVENTS[j].append(['construct',hex(a),r0-RECEIVER[j]])
  elif a==0x292d20:EVENTS[j].append(['clamp',bytes(uc.mem_read(r0,16)).hex()])
  elif a==0x27ccbc:EVENTS[j].append(['normalize',bytes(uc.mem_read(r0,12)).hex()])
  elif a==0x27c18c:EVENTS[j].append(['matrix-copy',r0-RECEIVER[j],uc.reg_read(UC_ARM_REG_R1)])
 u.hook_add(UC_HOOK_CODE,hook)
 def fault(uc,access,a,size,value,ignored,j=j):FAULTS[j]=[access,a,size];return False
 u.hook_add(UC_HOOK_MEM_INVALID,fault)
 u.reg_write(UC_ARM_REG_FPEXC,0x40000000);u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,STOP)
 for initializer in [0x100190,0x3834c8]:
  u.reg_write(UC_ARM_REG_LR,STOP)
  u.emu_start(initializer,STOP,count=100000)
  assert u.reg_read(UC_ARM_REG_PC)==STOP
 INITIALIZING[j]=False;MACHINES.append(u)
INITIAL_GLOBALS=[bytes(u.mem_read(0x3e1000,0x70000)) for u in MACHINES]
assert INITIAL_GLOBALS[0]==INITIAL_GLOBALS[1]
CASES=[];START=time.monotonic()
def run(group,fill=0x55,offset=0x100,owner=0,fpscr=0,repeated=False,pattern=False):
 rng=random.Random(offset+fill);initial=bytes(rng.randrange(256) for _ in range(0x10000)) if pattern else bytes([fill])*0x10000
 outputs=[];counts=[]
 for j,u in enumerate(MACHINES):
  u.mem_write(DATA,initial);u.mem_write(0x3e1000,INITIAL_GLOBALS[j]);u.mem_write(SP-0x4000,bytes([fill])*0x6000);RECEIVER[j]=DATA+offset
  COUNT[j]=0;EVENTS[j]=[];TRACE[j]=[];FAULTS[j]=None
  for invocation in range(2 if repeated else 1):
   for reg,v in zip(SR+DR,SAVED+DSAVED):u.reg_write(reg,v)
   u.reg_write(UC_ARM_REG_FPEXC,0x40000000);u.reg_write(UC_ARM_REG_FPSCR,fpscr);u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,STOP)
   u.reg_write(UC_ARM_REG_R0,RECEIVER[j]);u.reg_write(UC_ARM_REG_R1,owner)
   try:u.emu_start(ROOT if j==0 else CAND,STOP,count=2000000)
   except UcError as exc:
    failure={'case':len(CASES),'machine':j,'error':str(exc),'fault':FAULTS[j],'pc':hex(u.reg_read(UC_ARM_REG_PC)),'trace':[hex(x) for x in TRACE[j]]};(OUT/'replay-failure.json').write_text(json.dumps(failure,indent=2));print(json.dumps(failure,indent=2));raise
   assert u.reg_read(UC_ARM_REG_PC)==STOP,hex(u.reg_read(UC_ARM_REG_PC))
   assert u.reg_read(UC_ARM_REG_R0)==RECEIVER[j]
   assert u.reg_read(UC_ARM_REG_SP)==SP
   assert [u.reg_read(r) for r in SR+DR]==SAVED+DSAVED
  outputs.append((bytes(u.mem_read(DATA,0x10000)),bytes(u.mem_read(0x3e1000,0x70000)),u.reg_read(UC_ARM_REG_FPSCR)&0x0fffffff,EVENTS[j]))
  counts.append(COUNT[j])
 if outputs[0]!=outputs[1]:
  diffs=[[hex(DATA+i),a,b] for i,(a,b) in enumerate(zip(outputs[0][0],outputs[1][0])) if a!=b]
  failure={'case':len(CASES),'group':group,'differences':diffs[:100],'globals_equal':outputs[0][1]==outputs[1][1],'fpscr':[o[2] for o in outputs],'events':[o[3] for o in outputs]};(OUT/'replay-failure.json').write_text(json.dumps(failure,indent=2));print(json.dumps(failure,indent=2));raise AssertionError('replay mismatch')
 CASES.append({'group':group,'fill':fill,'offset':offset,'owner':owner,'fpscr':fpscr,'repeated':repeated,'pattern':pattern,'instructions':counts,'event_count':len(EVENTS[0]),'final_storage_sha256':hashlib.sha256(outputs[0][0]).hexdigest()})
if __name__=='__main__':
 run('base')
 for fill in [0,1,0x7f,0x80,0xff]:run('initial-storage',fill=fill)
 for offset in [0,4,0x104,0xffc,0x4000,0xf000]:run('aligned-location',offset=offset,pattern=True)
 for owner in [0,1,0x800100,0x800104,0x812345,0xffffffff]:run('opaque-owner',owner=owner)
 for fpscr in [0,1<<24,1<<25,1<<22,2<<22,3<<22,(1<<24)|(1<<25)]:run('fpscr',fpscr=fpscr,pattern=True)
 for offset in [0,4,0x100,0xffc]:run('repeat',offset=offset,repeated=True,pattern=True,owner=DATA+offset)
 result={'all_equal':True,'all_returned':True,'case_count':len(CASES),'original_root_invocations':sum(2 if c['repeated'] else 1 for c in CASES),'seconds':time.monotonic()-START,'coverage_counts':[len(x) for x in COVERAGE],'root_instruction_addresses':[hex(x) for x in sorted(COVERAGE[0])],'compiled_instruction_addresses':[hex(x) for x in sorted(COVERAGE[1])],'calls':[dict(x) for x in CALLS],'models':[],'initializer':'unmodified original nninitLocale 00100190 and matrix initializer 003834C8 executed once per machine','compiled_auxiliary':[],'watchdog_instructions':2000000,'cases':CASES}
 (OUT/'replay-result.json').write_text(json.dumps(result,indent=2));print(json.dumps({k:v for k,v in result.items() if k not in ['cases','root_instruction_addresses','compiled_instruction_addresses','calls']},indent=2))
```

## preserve.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
import json,hashlib
root=Path.cwd();base=root.parent/'mario-mainbc5';out=root/'build/particle-editor-parameters'
def normalized(p):
 e=ELFFile(open(p,'rb'));sections=[];symbols=[]
 def symrec(s):
  n=s['st_shndx'];sec=e.get_section(n).name if isinstance(n,int) else n
  return [s.name.replace(str(root),'<repo>').replace(str(base),'<repo>'),s['st_info']['type'],s['st_info']['bind'],s['st_other']['visibility'],sec,s['st_value'],s['st_size']]
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
expected={str(p.relative_to(base/'build/eu/obj')) for p in (base/'build/eu/obj').rglob('*.o') if p.relative_to(base/'build/eu/obj').parts[0] in ('Game','lib')}
assert len(expected)==179,len(expected)
for p in sorted((root/'build/eu/obj').rglob('*.o')):
 rel=p.relative_to(root/'build/eu/obj')
 if rel.parts[0] not in ('Game','lib') or p.stem=='retail_ParticleEditorParameters':continue
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
assert {r['object'] for r in records}==expected
result={'base':'bc5b236802b0300bc6b00f0ac55cf5ae5749c55c','object_count':len(records),'input_count':len(inputs),'all_equal':True,'excluded_difference':'repository prefixes in STT_FILE symbol names and non-allocated .comment, and resulting raw symbol/string table indices; all other symbols, section data, attributes and resolved relocations compare equal','objects':records,'inputs':inputs}
(out/'preservation.json').write_text(json.dumps(result,indent=2));print(json.dumps({k:v for k,v in result.items() if k not in ('objects','inputs')},indent=2))
```

## coverage.py

```python
import json
from pathlib import Path
out=Path('build/particle-editor-parameters');r=json.loads((out/'replay-result.json').read_text())
expected=set(range(0x2edfe8,0x2ee46c,4))|set(range(0x2ee570,0x2ee834,4));observed={int(a,16) for a in r['root_instruction_addresses']}
assert expected==observed
orig=r['calls'][0].copy();orig.pop('0x2edfe8');assert orig==r['calls'][1]
result={'original_instruction_count':len(expected),'all_reachable_instructions_covered':True,'code_ranges':[['002EDFE8','002EE46C'],['002EE570','002EE834']],'original_callee_entry_counts_identical':True,'calls_per_invocation':{a:n/r['original_root_invocations'] for a,n in r['calls'][1].items() if a in ['0x21f264','0x21f21c','0x28e1e4','0x292d20','0x27c18c','0x27ccbc','0x2499f4']},'complete_original_interval_size':2192,'exact_credit':0}
(out/'coverage.json').write_text(json.dumps(result,indent=2));print(result)
```

## seal.py

```python
from pathlib import Path
import hashlib,json,subprocess
root=Path.cwd();base=root.parent/'mario-mainbc5';out=root/'build/particle-editor-parameters'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
records={};gitlinks={}
for record in subprocess.check_output(['git','ls-tree','-r','-z','bc5b236802b0300bc6b00f0ac55cf5ae5749c55c']).split(b'\0'):
 if not record:continue
 info,name=record.split(b'\t');mode,kind,oid=info.decode().split();name=name.decode()
 if kind=='commit':gitlinks[name]=oid;continue
 assert (base/name).is_file(),name
 assert (root/name).is_file(),name
 assert sha(root/name)==sha(base/name),name
 records[name]=sha(root/name)
inputs=['lib/al/include/Effect/retail_ParticleEditorParameters.h','lib/al/src/Effect/retail_ParticleEditorParameters.cpp']
private=['data/ver/eu/code.bin','data/ver/eu/exh.bin','data/compilers/wibo','data/compilers/4.1/791/bin/armcc.exe','data/compilers/4.1/791/bin/armlink.exe','data/compilers/4.1/791/bin/armar.exe']
sealed={'base':'bc5b236802b0300bc6b00f0ac55cf5ae5749c55c','old_tracked_file_count':len(records),'old_tracked_files':records,'gitlinks':gitlinks,'new_source':{p:sha(root/p) for p in inputs},'private_input_tool_hashes':{p:sha(root/p) for p in private},'unchanged_baseline_report_sha256':sha(base/'build/dot-baseline-bc5/report.json'),'all_old_files_equal':True}
(out/'input-seal.json').write_text(json.dumps(sealed,indent=2));print({k:v for k,v in sealed.items() if k in ['base','old_tracked_file_count','new_source','unchanged_baseline_report_sha256','all_old_files_equal']})
```
