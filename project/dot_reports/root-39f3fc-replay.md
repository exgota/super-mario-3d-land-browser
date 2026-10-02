# Reproduce the light-area sort evidence

Use the frozen source, authorized private EU executable/exheader and already approved ARMCC4.1/791/wibo/Unicorn environment. Run from the repository root. All executable artifacts belong under ignored build/. Commit source before project checking. The scripts below are the executed scripts, with no checker/oracle edits. They do not copy vendor header source into the repository.

## Clean project build and canonical checks

```sh
. ./development_environment.sh
export DEVKITARM=/usr
mkdir -p build/root39f3fc
python make.py eu -ca > build/root39f3fc/final-build.log 2>&1
```

Save the following scripts under the indicated ignored paths. Run check.py after the clean build; it restores the map in finally even on failure. It changes only names on three existing rows, retaining both original Type f classifications and every function boundary. The earlier ft-classification check is historical diagnostic evidence only; see root-39f3fc-metadata.md. The names-only rerun was executed on 2026-10-02 at01:30UTC against the same frozen object and reached the same U -> M size failure for both roots.

## build/root39f3fc/check.py

```python
from pathlib import Path
import subprocess,json,hashlib,sys
from elftools.elf.elffile import ELFFile
p=Path('data/ver/eu/map.csv');original=p.read_bytes();text=original.decode();obj=Path('build/eu/obj/lib/al/src/Light/LightAreaIntrosort39F3FC.o');e=ELFFile(obj.open('rb'));tab=e.get_section_by_name('.symtab');names={}
for s in tab.iter_symbols():
 if '__introsort_loop' in s.name and s['st_info']['type']=='STT_FUNC':names[0x39f3fc]=s.name
 if '__adjust_heap' in s.name and s['st_info']['type']=='STT_FUNC':names[0x204950]=s.name
names[0x24e054]='_ZN9dot39f3fc5LightC1ERKS0_'
lines=[]
for line in text.splitlines(True):
 cells=line.rstrip('\n').split(',')
 if cells[0].startswith('0x') and int(cells[0],16) in names:
  a=int(cells[0],16);assert not cells[6];cells[6]=names[a]
  line=','.join(cells)+'\n'
 lines.append(line)
try:
 p.write_text(''.join(lines));
 for addr in (0x39f3fc,0x204950):
  r=subprocess.run([sys.executable,'tools/check.py',names[addr],'--object',str(obj)],text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT);print(hex(addr),r.stdout)
finally:p.write_bytes(original)
Path('build/root39f3fc/names.json').write_text(json.dumps({hex(k):v for k,v in names.items()},indent=2))
```

## build/root39f3fc/link.py

```python
from pathlib import Path
import sys,json,subprocess,os,hashlib
sys.path.insert(0,str(Path.cwd()))
from tools.low.checkExactBytes import _read_map,_isolate_function
from elftools.elf.elffile import ELFFile
out=Path('build/root39f3fc'); names=json.loads((out/'names.json').read_text());root=names['0x39f3fc'];rows=_read_map(Path('data/ver/eu/map.csv'))
for row in rows:
 if hex(row['Start']) in names: row['Symbol']=names[hex(row['Start'])]
section,raw,imports=_isolate_function(Path('build/eu/obj/lib/al/src/Light/LightAreaIntrosort39F3FC.o'),root,rows,out/'behavior.o')
(out/'imports.sym').write_text('#<SYMDEFS>#\n'+''.join(f'0x{x["address"]:08X} {x["kind"]} {x["symbol"]}\n' for x in imports))
(out/'behavior.sct').write_text(f'ROOT_LOAD 0x00500000\n{{\n ROOT_CODE 0x00500000 {{ behavior.o ({section}, +FIRST) }}\n}}\n')
cmd=['data/compilers/wibo','data/compilers/4.1/791/bin/armlink.exe','--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--inline','--datacompressor=off','--no_debug','--no_scanlib','--mangled','--symbols','--map',f'--entry={root}',f'--keep={root}',f'--scatter={out}/behavior.sct',f'--output={out}/behavior.axf',f'--list={out}/behavior.map',str(out/'behavior.o'),str(out/'imports.sym')]
env=os.environ.copy();env['TMP']='/tmp';subprocess.run(cmd,env=env,check=True)
e=ELFFile((out/'behavior.axf').open('rb'));linked=e.get_section_by_name('ROOT_CODE').data()
result={'imports':imports,'raw_section_size':len(raw),'linked_section_size':len(linked),'linked_section_sha256':hashlib.sha256(linked).hexdigest(),'command':cmd}
(out/'link-result.json').write_text(json.dumps(result,indent=2));print(json.dumps(result,indent=2))
```

## build/root39f3fc/link-closure.py

```python
from pathlib import Path
import json,subprocess,os,hashlib
from elftools.elf.elffile import ELFFile
out=Path('build/root39f3fc');names=json.loads((out/'names.json').read_text());root=names['0x39f3fc'];heap=names['0x204950'];obj=Path('build/eu/obj/lib/al/src/Light/LightAreaIntrosort39F3FC.o');e=ELFFile(obj.open('rb'));tab=e.get_section_by_name('.symtab');sym=next(s for s in tab.iter_symbols() if s.name==root);sec=e.get_section(sym['st_shndx']).name
(out/'closure-imports.sym').write_text('#<SYMDEFS>#\n0x0024E054 A '+names['0x24e054']+'\n')
(out/'closure.sct').write_text(f'ROOT_LOAD 0x00500000\n{{\n ROOT_CODE 0x00500000 {{ LightAreaIntrosort39F3FC.o ({sec}, +FIRST) LightAreaIntrosort39F3FC.o (+RO) }}\n}}\n')
cmd=['data/compilers/wibo','data/compilers/4.1/791/bin/armlink.exe','--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--inline','--datacompressor=off','--no_debug','--no_scanlib','--mangled','--symbols','--map',f'--entry={root}',f'--keep={root}',f'--scatter={out}/closure.sct',f'--output={out}/closure.axf',f'--list={out}/closure.map',str(obj),str(out/'closure-imports.sym')]
env=os.environ.copy();env['TMP']='/tmp';subprocess.run(cmd,env=env,check=True)
e=ELFFile((out/'closure.axf').open('rb'));section=e.get_section_by_name('ROOT_CODE');symbols={'root_entry':e['e_entry']};assert symbols['root_entry']==0x500000
result={'linked_size':len(section.data()),'linked_sha256':hashlib.sha256(section.data()).hexdigest(),'symbols':symbols,'command':cmd}
(out/'closure-link.json').write_text(json.dumps(result,indent=2));print(json.dumps(result,indent=2))
```

## build/root39f3fc/validate.py

```python
from pathlib import Path
import json,random,struct,hashlib,time
from collections import Counter
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
ROOT=0x39f3fc; END=0x39fe34; CAND=0x500000; DATA=0x600000; SIZE=0x40000; ARRAY=DATA+0x100; STR=DATA+0x20000; SP=0x680000; STOP=0x700000; STRIDE=0x128
out=Path('build/root39f3fc');code=Path('data/ver/eu/code.bin').read_bytes();e=ELFFile((out/'behavior.axf').open('rb'));candidate=e.get_section_by_name('ROOT_CODE').data()
savedregs=[UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11]+[UC_ARM_REG_D8+i for i in range(8)]
savedvals=[0xabc10000+i for i in range(8)]+[0x7fe1234500000000+i for i in range(8)]
coverage=set();calls=[Counter(),Counter()];trace=[[],[]];faults=[None,None];machines=[];current=[None]
for j in range(2):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176);u.reg_write(UC_ARM_REG_FPEXC,0x40000000);u.mem_map(0x100000,0x400000);u.mem_write(0x100000,code);u.mem_map(CAND,0x10000);u.mem_write(CAND,candidate);u.mem_map(DATA,0x100000);u.mem_map(STOP,0x1000)
 def hook(uc,a,size,user,j=j):
  if j==0 and ROOT<=a<END:coverage.add(a)
  if a in (0x204950,0x24e054,0x24dafc,0x2d5918,0x28aa60):calls[j][hex(a)]+=1
  if a==0x2d5918:
   def tag(r):
    p=uc.reg_read(r)
    try:return bytes(uc.mem_read(p,8)).hex()
    except UcError:return 'unmapped:'+hex(p)
   trace[j].append((tag(UC_ARM_REG_R0),tag(UC_ARM_REG_R1)))
 u.hook_add(UC_HOOK_CODE,hook)
 def invalid(uc,access,a,size,value,user,j=j):faults[j]=(access,a,size);return False
 u.hook_add(UC_HOOK_MEM_INVALID,invalid);machines.append(u)

seed=random.Random(0x39f3fc);total=0;returned_count=0;fault_count=0;budget_count=0;groups=Counter();semantic_assertions=0;begin=time.monotonic();cases=[]
def fields(record):return record[:8]+b''.join(record[x:x+0x5d] for x in (8,0x68,0xc8))
def check(group,n,order='shuffle',depth=None,mode=0,pad=0xa5,duplicate=False,null_name=False,bad_range=False,budget=1000000,expect="return"):
 global total,returned_count,fault_count,budget_count,semantic_assertions
 depth=n if depth is None else depth
 ids=list(range(n))
 if order=='reverse':ids.reverse()
 elif order=='shuffle':seed.shuffle(ids)
 elif order=='zigzag':ids=ids[::2]+ids[1::2][::-1]
 elif order=='rotate':ids=ids[n//3:]+ids[:n//3]
 b=bytearray([pad])*SIZE
 for i,k in enumerate(ids):
  name=(('same' if duplicate else f'Light{k:06d}')+'\0').encode();p=STR+k*16;b[p-DATA:p-DATA+len(name)]=name
  base=ARRAY-DATA+i*STRIDE
  struct.pack_into('<II',b,base,0 if null_name else p,k)
  for light in (8,0x68,0xc8):
   for word in range(23):
    bits=seed.getrandbits(32)
    if word<8:bits=[0,0x80000000,0x7f800000,0xff800000,0x7fc12345,0x7f812345,1,0x80000001][word]
    struct.pack_into('<I',b,base+light+4*word,bits)
   b[base+light+0x5c]=k%2
 input_records=[bytes(b[ARRAY-DATA+i*STRIDE:ARRAY-DATA+(i+1)*STRIDE]) for i in range(n)]
 original_fields=sorted(fields(r) for r in input_records);results=[]
 for j,u in enumerate(machines):
  u.mem_write(DATA,bytes(b));u.mem_write(SP-0x20000,b'\x7e'*0x21000);trace[j].clear();faults[j]=None
  for reg,val in zip(savedregs,savedvals):u.reg_write(reg,val)
  u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,STOP);u.reg_write(UC_ARM_REG_R0,0 if bad_range else ARRAY);u.reg_write(UC_ARM_REG_R1,n*STRIDE if bad_range else ARRAY+n*STRIDE);u.reg_write(UC_ARM_REG_R2,depth&0xffffffff);u.reg_write(UC_ARM_REG_R3,0x2d5918);u.reg_write(UC_ARM_REG_FPSCR,mode)
  error=None
  try:u.emu_start(ROOT if j==0 else CAND,STOP,count=budget)
  except UcError as ex:error=str(ex)
  returned=u.reg_read(UC_ARM_REG_PC)==STOP
  status='return' if returned else ('fault' if error else 'budget')
  if returned:
   assert u.reg_read(UC_ARM_REG_SP)==SP,(total,group,j,'SP')
   assert [u.reg_read(r) for r in savedregs]==savedvals,(total,group,j,'saved')
  mem=bytes(u.mem_read(DATA,SIZE));results.append({'memory':mem,'status':status,'fault':faults[j],'fpscr':u.reg_read(UC_ARM_REG_FPSCR),'trace':trace[j].copy()})
 a,z=results
 if a['status']!=z['status'] or a['fault']!=z['fault'] or a['fpscr']!=z['fpscr'] or (a['status']!='budget' and (a['memory']!=z['memory'] or a['trace']!=z['trace'])):
  failure={'case':total,'group':group,'n':n,'order':order,'depth':depth,'mode':mode,'statuses':[a['status'],z['status']],'faults':[a['fault'],z['fault']],'fpscr':[a['fpscr'],z['fpscr']],'trace_lengths':[len(a['trace']),len(z['trace'])],'memory_diff':[(hex(DATA+i),x,y) for i,(x,y) in enumerate(zip(a['memory'],z['memory'])) if x!=y][:30]}
  (out/'failure.json').write_text(json.dumps(failure,indent=2));raise AssertionError(failure)
 if expect:assert a['status']==expect,(group,a['status'])
 if a['status']=='return':
  returned_count+=1
  final_records=[a['memory'][ARRAY-DATA+i*STRIDE:ARRAY-DATA+(i+1)*STRIDE] for i in range(n)]
  assert sorted(fields(r) for r in final_records)==original_fields,(total,'record preservation')
  semantic_assertions+=1
  for i,r in enumerate(final_records):
   for off in (8,0x68,0xc8):assert r[off+0x5d:off+0x60]==input_records[i][off+0x5d:off+0x60];semantic_assertions+=1
  if n<=16:assert a['memory']==bytes(b);semantic_assertions+=1
  if depth==0 and n>16 and not duplicate:
   assert [struct.unpack_from('<I',r,4)[0] for r in final_records]==list(range(n-1,-1,-1)),(total,'heap sorted')
   semantic_assertions+=1
 elif a['status']=='fault':fault_count+=1
 else:budget_count+=1
 groups[group]+=1;cases.append({'group':group,'n':n,'order':order,'depth':depth,'mode':mode,'status':a['status'],'comparator_calls':len(a['trace']),'fault':a['fault'],'fpscr':a['fpscr'],'memory_sha256':hashlib.sha256(a['memory']).hexdigest(),'comparator_trace_sha256':hashlib.sha256(json.dumps(a['trace']).encode()).hexdigest()});total+=1

for n in list(range(19))+[31,32,33,63,64,65,127,128,129,257]:
 for order in ('forward','reverse','shuffle','zigzag','rotate'):
  for depth in ((0,n) if n>16 else (n,)):check('unique_names',n,order,depth)
for n in (17,18,33,65,129):
 for depth in (1,2,3,-1,-2,-0x80000000):check('depth_budget',n,'shuffle',depth)
for mode in (0,1<<22,2<<22,3<<22,1<<24,1<<25,(1<<24)|(1<<25)|0x9f):
 for depth in (0,65):check('fpscr_payload_bits',65,'shuffle',depth,mode)
for pad in (0,0x55,0xff):
 for n in (16,17,33):check('padding',n,'shuffle',n,pad=pad)
for n in (17,33):check('duplicate_heap',n,depth=0,duplicate=True,expect='return')
for n in (17,33):check('duplicate_partition_fault',n,duplicate=True,expect='fault')
check('null_name_fault',17,null_name=True,expect='fault')
check('null_range_fault',17,bad_range=True,expect='fault')
check('null_empty_range',0,bad_range=True,expect='return')
result={'all_compared_equal':True,'fixtures':total,'returning_pairs':returned_count,'fault_pairs':fault_count,'budget_pairs':budget_count,'groups':dict(groups),'semantic_assertions':semantic_assertions,'seconds':time.monotonic()-begin,'original_root_instruction_addresses':len(coverage),'original_total_code_addresses':(END-ROOT)//4,'uncovered':[hex(a) for a in range(ROOT,END,4) if a not in coverage],'original_callee_calls':dict(calls[0]),'candidate_callee_calls':dict(calls[1]),'modeled_endpoints':[],'cpu':'ARM1176','source_sha256':hashlib.sha256(Path('lib/al/src/Light/LightAreaIntrosort39F3FC.cpp').read_bytes()).hexdigest(),'object_sha256':hashlib.sha256(Path('build/eu/obj/lib/al/src/Light/LightAreaIntrosort39F3FC.o').read_bytes()).hexdigest(),'linked_sha256':hashlib.sha256(candidate).hexdigest()}
(out/'result.json').write_text(json.dumps(result,indent=2));(out/'cases.json').write_text(json.dumps(cases,indent=2));(out/'coverage.json').write_text(json.dumps(sorted(coverage)));print(json.dumps(result,indent=2))
```

## build/root39f3fc/preserve.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
import json,hashlib
root=Path.cwd();base=root.parent/'mario-main45a';out=root/'build/root39f3fc'
def normalized(p):
 e=ELFFile(open(p,'rb'));sections=[];symbols=[]
 def symrec(s):
  n=s['st_shndx'];sec=e.get_section(n).name if isinstance(n,int) else n
  return [s.name,s['st_info']['type'],s['st_info']['bind'],s['st_other']['visibility'],sec,s['st_value'],s['st_size']]
 for sec in e.iter_sections():
  if isinstance(sec,RelocationSection):
   tab=e.get_section(sec['sh_link']);sections.append([sec.name,[(r['r_offset'],r['r_info_type'],symrec(tab.get_symbol(r['r_info_sym']))) for r in sec.iter_relocations()]])
  elif sec['sh_type'] not in ('SHT_SYMTAB','SHT_STRTAB','SHT_NULL'):
   sections.append([sec.name,sec['sh_type'],sec['sh_flags'],sec['sh_addralign'],sec['sh_entsize'],hashlib.sha256(sec.data().replace(str(root).encode(),b'<repo>').replace(str(base).encode(),b'<repo>') if sec.name=='.comment' else sec.data()).hexdigest()])
 tab=e.get_section_by_name('.symtab')
 for s in tab.iter_symbols():
  if s['st_info']['type']!='STT_FILE':symbols.append(symrec(s))
 return {'sections':sections,'symbols':symbols}
records=[];inputs={}
for p in sorted((root/'build/eu/obj').rglob('*.o')):
 rel=p.relative_to(root/'build/eu/obj')
 if rel.parts[0] not in ('Game','lib') or p.stem=='LightAreaIntrosort39F3FC':continue
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
result={'base':'45a4305466a7c4cbd87589169384102e88f8d1b5','object_count':len(records),'input_count':len(inputs),'all_equal':True,'excluded_difference':'STT_FILE absolute source path, repository prefix in non-allocated .comment, and resulting raw symbol/string table indices; all other symbols, section data, attributes and resolved relocations compare equal','objects':records,'inputs':inputs}
(out/'preservation.json').write_text(json.dumps(result,indent=2));print(json.dumps({k:v for k,v in result.items() if k not in ('objects','inputs')},indent=2))
```

## Run both independently linked execution suites

```sh
python build/root39f3fc/check.py > build/root39f3fc/final-check.log 2>&1
python build/root39f3fc/link.py
python build/root39f3fc/link-closure.py
python build/root39f3fc/validate.py
python - <<'PYVARIANT'
from pathlib import Path
p=Path('build/root39f3fc/validate.py');s=p.read_text()
s=s.replace("out/'behavior.axf'","out/'closure.axf'").replace("out/'result.json'","out/'closure-result.json'").replace("out/'cases.json'","out/'closure-cases.json'").replace("out/'coverage.json'","out/'closure-coverage.json'").replace("out/'failure.json'","out/'closure-failure.json'")
Path('build/root39f3fc/validate-closure.py').write_text(s)
PYVARIANT
python build/root39f3fc/validate-closure.py
python - <<'PYCOMPARE'
from pathlib import Path
import json
p=Path('build/root39f3fc')
assert json.loads((p/'cases.json').read_text())==json.loads((p/'closure-cases.json').read_text())
PYCOMPARE
python build/root39f3fc/preserve.py
```

The helper's relocated diagnostic placement 0x00500A4C is only linker-owned execution storage, not an asserted retail map boundary. The root-only link uses the unchanged project's isolation routine to retain compiler bytes/relocations and bind known original imports. Canonical checking always consumes the original canonical object, never these diagnostic isolated objects.

## Pristine baseline canonical gate

Run in an independent checkout of45a4305466a7c4cbd87589169384102e88f8d1b5 with no shared build artifacts. The preserved-object script expects it as sibling mario-main45a; change only this checkout path if necessary.

```sh
. ./development_environment.sh
export DEVKITARM=/usr
mkdir -p build
cat > build/empty-candidates-45a.json <<'EOFBATCH'
{"prior_checkpoint":"45a4305466a7c4cbd87589169384102e88f8d1b5","candidates":[]}
EOFBATCH
python tools/acceptance_batch.py build/empty-candidates-45a.json --output build/dot-baseline-45a-reproduction
```

Expected: a successful clean build, accepted true,707 prior roots and726 O -> O prior definition checks. An empty candidate batch gives no new-function credit.

## Exact executed summaries

### result.json

```json
{
  "all_compared_equal": true,
  "fixtures": 265,
  "returning_pairs": 261,
  "fault_pairs": 4,
  "budget_pairs": 0,
  "groups": {
    "unique_names": 205,
    "depth_budget": 30,
    "fpscr_payload_bits": 14,
    "padding": 9,
    "duplicate_heap": 2,
    "duplicate_partition_fault": 2,
    "null_name_fault": 1,
    "null_range_fault": 1,
    "null_empty_range": 1
  },
  "semantic_assertions": 39567,
  "seconds": 23.744910604000324,
  "original_root_instruction_addresses": 508,
  "original_total_code_addresses": 654,
  "uncovered": "146 consecutive addresses 0x0039F544..0x0039F788 inclusive, four-byte stride",
  "original_callee_calls": {
    "0x24e054": 84066,
    "0x204950": 9024,
    "0x2d5918": 71158,
    "0x28aa60": 71157,
    "0x24dafc": 28956
  },
  "candidate_callee_calls": {
    "0x24e054": 84066,
    "0x204950": 9024,
    "0x2d5918": 71158,
    "0x28aa60": 71157,
    "0x24dafc": 28956
  },
  "modeled_endpoints": [],
  "cpu": "ARM1176",
  "source_sha256": "e8d26f39de21af91e72f5387e6533dc3b2e918a0fce2a1630a451336a4e82ec4",
  "object_sha256": "8e329d332fe5b1a054157219edde52ae9fb3ab90f10170ac823655bfee41f5d8",
  "linked_sha256": "5e6a2bb25c0a93356581e342393540a62a55040db9a835f9a598dd3c624403c7"
}
```

### closure-result.json

```json
{
  "all_compared_equal": true,
  "fixtures": 265,
  "returning_pairs": 261,
  "fault_pairs": 4,
  "budget_pairs": 0,
  "groups": {
    "unique_names": 205,
    "depth_budget": 30,
    "fpscr_payload_bits": 14,
    "padding": 9,
    "duplicate_heap": 2,
    "duplicate_partition_fault": 2,
    "null_name_fault": 1,
    "null_range_fault": 1,
    "null_empty_range": 1
  },
  "semantic_assertions": 39567,
  "seconds": 22.052446583002165,
  "original_root_instruction_addresses": 508,
  "original_total_code_addresses": 654,
  "uncovered": "146 consecutive addresses 0x0039F544..0x0039F788 inclusive, four-byte stride",
  "original_callee_calls": {
    "0x24e054": 84066,
    "0x204950": 9024,
    "0x2d5918": 71158,
    "0x28aa60": 71157,
    "0x24dafc": 28956
  },
  "candidate_callee_calls": {
    "0x24e054": 84066,
    "0x2d5918": 71158,
    "0x28aa60": 71157
  },
  "modeled_endpoints": [],
  "cpu": "ARM1176",
  "source_sha256": "e8d26f39de21af91e72f5387e6533dc3b2e918a0fce2a1630a451336a4e82ec4",
  "object_sha256": "8e329d332fe5b1a054157219edde52ae9fb3ab90f10170ac823655bfee41f5d8",
  "linked_sha256": "91a393c03295fde9edf9b7468dbd84197972ad93e473b3683a5ac6b1c48301a1"
}
```

### link-result.json

```json
{
  "imports": [
    {
      "symbol": "_ZN9dot39f3fc5LightC1ERKS0_",
      "address": 2416724,
      "kind": "A",
      "map_symbol": "_ZN9dot39f3fc5LightC1ERKS0_",
      "map_start": 2416724,
      "map_end": 2416824,
      "defined_in_input": false
    },
    {
      "symbol": "_ZSt13__adjust_heapIPN9dot39f3fc4AreaEiS1_PFbRKS1_S4_EEvT_T0_S8_T1_T2_",
      "address": 2115920,
      "kind": "A",
      "map_symbol": "_ZSt13__adjust_heapIPN9dot39f3fc4AreaEiS1_PFbRKS1_S4_EEvT_T0_S8_T1_T2_",
      "map_start": 2115920,
      "map_end": 2117160,
      "defined_in_input": true
    }
  ],
  "raw_section_size": 2636,
  "linked_section_size": 2636,
  "linked_section_sha256": "5e6a2bb25c0a93356581e342393540a62a55040db9a835f9a598dd3c624403c7",
  "command": [
    "data/compilers/wibo",
    "data/compilers/4.1/791/bin/armlink.exe",
    "--cpu=MPCore",
    "--fpu=VFPv2",
    "--arm_only",
    "--no_exceptions",
    "--inline",
    "--datacompressor=off",
    "--no_debug",
    "--no_scanlib",
    "--mangled",
    "--symbols",
    "--map",
    "--entry=_ZSt16__introsort_loopIPN9dot39f3fc4AreaEiPFbRKS1_S4_EEvT_S7_T0_T1_",
    "--keep=_ZSt16__introsort_loopIPN9dot39f3fc4AreaEiPFbRKS1_S4_EEvT_S7_T0_T1_",
    "--scatter=build/root39f3fc/behavior.sct",
    "--output=build/root39f3fc/behavior.axf",
    "--list=build/root39f3fc/behavior.map",
    "build/root39f3fc/behavior.o",
    "build/root39f3fc/imports.sym"
  ]
}
```

### closure-link.json

```json
{
  "linked_size": 4444,
  "linked_sha256": "91a393c03295fde9edf9b7468dbd84197972ad93e473b3683a5ac6b1c48301a1",
  "symbols": {
    "root_entry": 5242880
  },
  "command": [
    "data/compilers/wibo",
    "data/compilers/4.1/791/bin/armlink.exe",
    "--cpu=MPCore",
    "--fpu=VFPv2",
    "--arm_only",
    "--no_exceptions",
    "--inline",
    "--datacompressor=off",
    "--no_debug",
    "--no_scanlib",
    "--mangled",
    "--symbols",
    "--map",
    "--entry=_ZSt16__introsort_loopIPN9dot39f3fc4AreaEiPFbRKS1_S4_EEvT_S7_T0_T1_",
    "--keep=_ZSt16__introsort_loopIPN9dot39f3fc4AreaEiPFbRKS1_S4_EEvT_S7_T0_T1_",
    "--scatter=build/root39f3fc/closure.sct",
    "--output=build/root39f3fc/closure.axf",
    "--list=build/root39f3fc/closure.map",
    "build/eu/obj/lib/al/src/Light/LightAreaIntrosort39F3FC.o",
    "build/root39f3fc/closure-imports.sym"
  ]
}
```

### hashes.json

```json
{
  "lib/al/src/Light/LightAreaIntrosort39F3FC.cpp": "e8d26f39de21af91e72f5387e6533dc3b2e918a0fce2a1630a451336a4e82ec4",
  "data/ver/eu/map.csv": "d517e7aa3eefdbb859e781d7d974f8daf4fbcb33b964edb7d0a5ef6a11db8bd9",
  "data/ver/eu/code.bin": "e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64",
  "data/compilers/4.1/791/bin/armcc.exe": "d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d",
  "data/compilers/4.1/791/include/algorithm": "4350c398a5c12cfb57d401c152747c1357ffbdc7783a57cf8ee23da6df372eaa",
  "data/compilers/4.1/791/include/algorithm.cc": "d2f6a1ba8d45c1c9593bb748982b208d757f236e7584d89b3c97d6791d95ac23",
  "build/eu/obj/lib/al/src/Light/LightAreaIntrosort39F3FC.o": "8e329d332fe5b1a054157219edde52ae9fb3ab90f10170ac823655bfee41f5d8",
  "build/root39f3fc/behavior.axf": "2c88a34c16552c1b455ece0b21c081721d58aaccc5b02920ec59865dff10a4e9",
  "build/root39f3fc/closure.axf": "1fbd6aed57cba851030cce739d1b10341add7e8c751277175e20480a72b00534",
  "build/root39f3fc/preservation.json": "561ef0d7e356b511eb631ea2cdc63ca25f9302b332fa90eb6d882a93df52503c"
}
```

