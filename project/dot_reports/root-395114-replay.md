# Reproduce bounded formatter evidence

Use frozen committed source and the authorized EU inputs and compiler. All generated files stay under ignored build/. Run from the repository root. The existing map row is temporarily named, then restored byte-for-byte; no oracle changes.

## Normal project check

```sh
. ./development_environment.sh
export DEVKITARM=/usr
mkdir -p build/root395114
python make.py eu
cp data/ver/eu/map.csv build/root395114/map-original.csv
python - <<'PY'
from pathlib import Path
p=Path('data/ver/eu/map.csv');s=p.read_text()
old='0x00395114,0x00395D38,0x00395D3C,          ,U,f,,'
assert s.count(old)==1
p.write_text(s.replace(old,old[:-1]+'fn_00395114,'))
PY
python tools/check.py fn_00395114 --object build/eu/obj/lib/CtrSDK/sources/retail_BoundedFormat.o
cp build/root395114/map-original.csv data/ver/eu/map.csv
python tools/check.py _ZN2nn2os6detail27ConvertSvcToLibraryPriorityEi --object build/eu/obj/lib/CtrSDK/sources/os_Priority.o
```

The root is expected M because2964!=3112. Restore the saved map even if a command fails. Priority is expected O. Provenance requires committed source.

## Diagnostic execution link

```sh
cat > build/root395114/behavior.sct <<'EOF'
ROOT_LOAD 0x00500000
{
 ROOT_CODE 0x00500000 { retail_BoundedFormat.o (i.fn_00395114, +FIRST) }
}
EOF
cat > build/root395114/imports.sym <<'EOF'
#<SYMDEFS>#
0x0028AB74 A fn_0028AB74
EOF
TMP=/tmp data/compilers/wibo data/compilers/4.0/902/bin/armlink.exe --cpu=MPCore --fpu=VFPv2 --arm_only --no_exceptions --inline --datacompressor=off --no_debug --no_scanlib --mangled --symbols --map --entry=fn_00395114 --keep=fn_00395114 --scatter=build/root395114/behavior.sct --output=build/root395114/behavior.axf --list=build/root395114/behavior.map build/eu/obj/lib/CtrSDK/sources/retail_BoundedFormat.o build/root395114/imports.sym
```

This uses the canonical compiler object and established original callee only. Diagnostic execution is not byte acceptance.

## Whole-root replay

Save this unchanged as build/root395114/validate.py and run with the project environment (plus approved Unicorn). Successful execution writes result.json and coverage.json. Random seed is fixed; wall time varies.

```python
import sys, json, struct, random, time, hashlib
from pathlib import Path
from collections import Counter
from elftools.elf.elffile import ELFFile
from unicorn import Uc, UcError, UC_ARCH_ARM, UC_MODE_ARM, UC_HOOK_CODE, UC_HOOK_MEM_INVALID
from unicorn.arm_const import *
ROOT=0x395114; CAND=0x500000; STOP=0x700000; DATA=0x600000; SIZE=0x20000; SP=0x680000; OUT=DATA+0x100; FMT=DATA+0x8000; STR=DATA+0x9000; COUNT=DATA+0xa000; AP=DATA+0x10000
outdir=Path('build/root395114'); code=Path('data/ver/eu/code.bin').read_bytes()
e=ELFFile(open(outdir/'behavior.axf','rb'));section=e.get_section_by_name('ROOT_CODE');candidate=section.data()
regs=[UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11];saved=[0xabc00000+i for i in range(8)];vregs=[UC_ARM_REG_D8+i for i in range(8)];vsaved=[0x7fe1234500000000+i for i in range(8)]
coverage=set();calls=Counter();machines=[];faults=[None,None]
for j in range(2):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176);u.mem_map(0x100000,0x400000);u.mem_write(0x100000,code);u.mem_map(CAND,0x10000);u.mem_write(CAND,candidate);u.mem_map(DATA,0x100000);u.mem_map(STOP,0x1000)
 def hook(uc,address,size,user,j=j):
  if j==0 and ROOT<=address<0x395d38:coverage.add(address)
  if address==0x28ab74:calls[j]+=1
  if j==1 and address==ROOT:uc.reg_write(UC_ARM_REG_PC,CAND)
 u.hook_add(UC_HOOK_CODE,hook)
 def fault(uc,access,address,size,value,user,j=j):faults[j]=(access,address,size);return False
 u.hook_add(UC_HOOK_MEM_INVALID,fault)
 machines.append(u)

def argdata(values,start):
 out=bytearray()
 for kind,value in values:
  if kind=='q':
   while (start+len(out))%8:out+=b'\xa5'*4
   out+=struct.pack('<Q',value&0xffffffffffffffff)
  else:out+=struct.pack('<I',value&0xffffffff)
 return out

cases=0;groups=Counter();begin=time.monotonic();failures=[]
def check(group,fmt,values=(),capacity=128,alignment=0,strings=b'hello\0',wrapper=False,null=False,alias=None,expected_fault=False,fpscr=0):
 global cases
 f=fmt if isinstance(fmt,bytes) else fmt.encode();b=bytearray([0xa5])*SIZE;b[FMT-DATA:FMT-DATA+len(f)+1]=f+b'\0';b[STR-DATA:STR-DATA+len(strings)]=strings
 ap=AP+alignment;vals=list(values)
 if alias=='string':
  b[OUT-DATA:OUT-DATA+len(strings)]=strings; vals=[(k,OUT if v==STR else v) for k,v in vals]
 if alias=='count_args': vals=[(k,ap+8 if v==COUNT else v) for k,v in vals]
 encoded=argdata(vals,(SP-4) if wrapper else ap);b[ap-DATA:ap-DATA+len(encoded)]=encoded
 results=[]
 for j,u in enumerate(machines):
  u.mem_write(DATA,bytes(b));u.mem_write(SP-0x1000,b'\x7e'*0x2000);faults[j]=None
  for reg,v in zip(regs+vregs,saved+vsaved):u.reg_write(reg,v)
  u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,STOP);u.reg_write(UC_ARM_REG_R0,0 if null else OUT);u.reg_write(UC_ARM_REG_R1,capacity);u.reg_write(UC_ARM_REG_R2,FMT);u.reg_write(UC_ARM_REG_FPSCR,fpscr)
  if wrapper:
   u.reg_write(UC_ARM_REG_R3,struct.unpack('<I',(encoded+b'\0'*4)[:4])[0]);u.mem_write(SP,bytes(encoded[4:]));entry=0x295744
  else:u.reg_write(UC_ARM_REG_R3,ap);entry=ROOT if j==0 else CAND
  error=None
  try:u.emu_start(entry,STOP,count=300000)
  except UcError as exc:error=str(exc)
  returned=u.reg_read(UC_ARM_REG_PC)==STOP
  if returned:
   assert u.reg_read(UC_ARM_REG_SP)==SP,(cases,group,j,'SP')
   assert [u.reg_read(r) for r in regs+vregs]==saved+vsaved,(cases,group,j,'saved')
  if not returned and not error:raise AssertionError((cases,group,j,'did not terminate',fmt))
  results.append((bytes(u.mem_read(DATA,SIZE)),u.reg_read(UC_ARM_REG_R0) if returned else None,returned,faults[j],u.reg_read(UC_ARM_REG_FPSCR)))
 if results[0]!=results[1]:
  fail={'case':cases,'group':group,'format':repr(fmt),'values':values,'capacity':capacity,'alignment':alignment,'wrapper':wrapper,'null':null,'alias':alias,'returns':[x[1:4] for x in results],'diff':[(hex(DATA+i),x,y) for i,(x,y) in enumerate(zip(results[0][0],results[1][0])) if x!=y][:30]}
  (outdir/'failure.json').write_text(json.dumps(fail,indent=2));raise AssertionError(fail)
 if expected_fault!=bool(faults[0]):raise AssertionError((cases,group,'fault expectation',faults[0]))
 cases+=1;groups[group]+=1

for fmt in [b'',b'abc',b'%%',b'x%%y',b'%',b'%q',b'%#x',b'%+d',b'%  +d',b'%5q',b'%5%',b'%.c',b'%f',b'%0.2f',b'%hhn',b'%hhhx',b'\xffA\x80',b'%.',b'%*q']:
 for cap in [0,1,2,4,8,32]:check('syntax',fmt,[('i',3),('i',COUNT),('i',0)],cap)
ints=[0,1,7,8,9,10,15,16,127,128,255,256,32767,32768,65535,65536,0x7fffffff,0x80000000,0xffffffff]
for conv in 'diuoxX':
 for length in ['', 'h','hh','l']:
  for value in ints:
   for flags in ['', ' ', ' +', '0','-','-0']:
    check('integer32','%'+flags+'12'+length+conv,[('i',value)],capacity=[0,1,5,64][value%4])
for conv in 'diuoxX':
 for value in [0,1,0xffffffff,0x100000000,0x7fffffffffffffff,0x8000000000000000,0xffffffffffffffff,1234567890123456789]:
  for align in [0,4]:
   for prec in ['', '.0','.1','.30']:
    check('integer64','%020'+prec+'ll'+conv,[('q',value)],alignment=align)
for conv in 'diuoxX':
 for width in [-20,-1,0,1,20]:
  for precision in [-4,-1,0,1,20]:
   for value in [0,1,0xffffffff]:check('stars','%0*.*'+conv,[('i',width),('i',precision),('i',value)],capacity=17)
for conv in ['p','hp','hhp','lp','llp']:
 for value in [0,1,0xabcdef12,0xffffffffffffffff]:check('pointer','%20'+conv,[(('q' if conv=='llp' else 'i'),value)])
for flags in ['', '0','-', '-0']:
 for width in ['', '1','12']:
  for precision in ['', '.0','.2','.12']:
   for cap in [0,1,3,24]:check('strings','%'+flags+width+precision+'s',[('i',STR)],cap,strings=b'AB\xffCD\0')
for flags in ['', '0','-', '-0']:
 for value in [0,65,127,128,255,256,-1]:
  for width in ['', '1','12']:check('character','%'+flags+width+'c',[('i',value)],capacity=10)
for spec in ['n','hn','hhn','ln','lln']:
 for cap in [0,1,4,20]:
  check('count','hello%'+spec+'tail',[('i',COUNT)],cap)
  if spec!='hhn':check('count_alias','%20d%'+spec+'%u',[('i',123),('i',COUNT),('i',45)],cap,alias='count_args')
for text in [b'',b'A\0',b'abcd\0',b'abcdefghi\0']:
 for fmt in ['%s','%-12s','%12s','%012s','%.3s','x%s']:
  check('string_alias',fmt,[('i',STR)],128,strings=text if text else b'\0',alias='string')
for mode in [0,1<<24,1<<25,1<<22,2<<22,3<<22]:check('fpscr','%lld|%s',[('q',0x8000000000000000),('i',STR)],fpscr=mode)
for cap in [0,1,12,100]:
 for fmt,vals in [('x%d-%llx:%s',[('i',123),('q',0xfedcba9876543210),('i',STR)]),('%lld',[('q',0x8000000000000000)]),('%04d:%02d:%02d %02d:%02d:%02d',[('i',2026),('i',10),('i',2),('i',12),('i',34),('i',56)])]:check('wrapper',fmt,vals,cap,wrapper=True)
check('fault_null_output','x',[],1,null=True,expected_fault=True)
check('zero_capacity_null_output','x',[],0,null=True)
check('fault_null_string','%s',[('i',0)],128,expected_fault=True)
check('zero_precision_null_string','%.0s',[('i',0)])
check('fault_null_count','%n',[('i',0)],expected_fault=True)
check('suppressed_null_count','%hhn',[('i',0)])
randomizer=random.Random(395114)
for iteration in range(1000):
    fmt=[];values=[]
    for field in range(randomizer.randrange(1,7)):
        conv=randomizer.choice(['d','i','u','o','x','X','llx','lld','llu','s','c','n','hn','lln','hhn','%'])
        if conv=='%':fmt.append('%%');continue
        if conv.endswith('n'):
            fmt.append('%'+conv)
            if conv!='hhn':values.append(('i',COUNT+randomizer.randrange(0,4)*8))
            continue
        flags=randomizer.choice(['','0','-',' ',' +'])
        width=randomizer.choice(['','12','*'])
        prec='' if conv=='c' else randomizer.choice(['','.0','.12','.*'])
        fmt.append('%'+flags+width+prec+conv)
        if width=='*':values.append(('i',randomizer.randrange(-24,25)))
        if prec=='.*':values.append(('i',randomizer.randrange(-3,25)))
        values.append(('i',STR) if conv=='s' else ('q' if conv.startswith('ll') else 'i',randomizer.getrandbits(64 if conv.startswith('ll') else 32)))
        fmt.append('|')
    check('mixed_formats',''.join(fmt),values,capacity=randomizer.choice([0,1,7,23,128]),alignment=randomizer.choice([0,4]),wrapper=bool(iteration%2))
result={'returning_fixtures':cases-3,'fault_controls':3,'fixtures':cases,'groups':dict(groups),'seconds':time.monotonic()-begin,'original_instruction_coverage':len(coverage),'uncovered':[hex(a) for a in range(ROOT,0x395d38,4) if a not in coverage],'original_division_calls':dict(calls),'modelled_callees':[],'cpu':'ARM1176','callee_saved_checks':'r4-r11,d8-d15,SP on every return','source_sha256':hashlib.sha256(Path('lib/CtrSDK/sources/retail_BoundedFormat.cpp').read_bytes()).hexdigest(),'object_sha256':hashlib.sha256(Path('build/eu/obj/lib/CtrSDK/sources/retail_BoundedFormat.o').read_bytes()).hexdigest(),'all_equal':True}
(outdir/'result.json').write_text(json.dumps(result,indent=2));(outdir/'coverage.json').write_text(json.dumps(sorted(coverage)));print(json.dumps(result,indent=2))
```

## Pristine canonical baseline gate

The coordinator used the unmodified project acceptance tool in the separate pristine checkout at 45a4305466a7c4cbd87589169384102e88f8d1b5, with this exact empty candidate shape. Run in that checkout, using a fresh output directory. The committed verbatim report records all 726 passing definition checks for 707 prior roots. An accepted empty batch adds no new root, including no formatter acceptance.

```sh
. ./development_environment.sh
export DEVKITARM=/usr
mkdir -p build
cat > build/empty-candidates-45a.json <<'EOF'
{"prior_checkpoint":"45a4305466a7c4cbd87589169384102e88f8d1b5","candidates":[]}
EOF
python tools/acceptance_batch.py build/empty-candidates-45a.json --output build/dot-baseline-45a-reproduction
```

## Preserved-object and input comparison

The independently built pristine45a checkout is the sibling mario-main45a. Its unchanged acceptance_batch.py empty-candidate gate supplies baseline canonical results and must finish before they are cited. Save this as build/root395114/preserve.py and run. Adjust only the baseline checkout path if necessary. It compares175objects/365inputs; it copies no objects.

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
import json,hashlib
root=Path.cwd();base=root.parent/'mario-main45a';out=root/'build/root395114'
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
 if rel.parts[0] not in ('Game','lib') or p.stem=='retail_BoundedFormat':continue
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

## Apply-clean source patch

```sh
git diff 45a4305466a7c4cbd87589169384102e88f8d1b5..HEAD -- lib/CtrSDK/sources/retail_BoundedFormat.cpp > build/root395114/family-source.patch
GIT_INDEX_FILE=/tmp/root395114-applycheck-index git read-tree 45a4305466a7c4cbd87589169384102e88f8d1b5
GIT_INDEX_FILE=/tmp/root395114-applycheck-index git apply --cached --check build/root395114/family-source.patch
```
