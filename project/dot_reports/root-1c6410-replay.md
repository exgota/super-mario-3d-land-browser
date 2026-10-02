# Executed reproduction: lighting-data director 0x001C6410

Run from the repository root at source/header commit `3685cdd11de554510af171d84fb575152c6b35a4`, based on main `3de056e0dcb33619c32f8b46ef64f74c90d28934`. This document contains every script used for the final measurement, evidence checks, behavioral replay and accepted-root audit. All outputs stay under ignored `build/root-1c6410`. The private EU executable, exheader, approved ARMCC 791/902 and wibo and Python dependencies must already be available locally. They are not part of this document or branch.

No map interval, checker, compiler flag, source object or target byte is changed. The canonical check temporarily assigns names to nine existing unnamed function rows and restores the complete original map in a `finally` block. The diagnostic link below isolates genuine committed-source ARMCC code and allows its known smaller size solely for behavioral replay. It does not replace the strict check or assert exact credit.

Create the following files exactly as shown, then run the final command block. These scripts were executed on the dot cloud computer. The final clean build and canonical object SHA are unchanged by the last two naming/comment-only commits.

## Diagnostic names

Save as `build/root-1c6410/diagnostic-names.json`:

```json
{
  "0x1c6410": "_ZN9dot1c64108DirectorC1Ev",
  "0x1c5060": "_ZN9dot1c64104AreaC1EPKcS2_",
  "0x1c50a4": "_ZN9dot1c64104AreaC1Ev",
  "0x1d4bc4": "_ZN9dot1c64103MapC1EPKcS2_",
  "0x1d4be0": "_ZN9dot1c64103MapC1Ev",
  "0x39e160": "_ZN9dot1c64105EntryC1Ev",
  "0x24da80": "_ZN9dot1c64105LightC1ERKNS_5ColorES3_S3_S3_S3_RKN4sead7Vector3IfEEb",
  "0x24e054": "_ZN9dot1c64105LightC1ERKS0_",
  "0x27b350": "_ZN9dot1c64109AllocatorC1EPN4sead4HeapE"
}
```

## Canonical check with restoration

Save as `build/root-1c6410/check.py`:

```python
import json, pathlib, subprocess, sys, hashlib
p=pathlib.Path('data/ver/eu/map.csv'); original=p.read_bytes()
names={int(k,16):v for k,v in json.loads(pathlib.Path('build/root-1c6410/diagnostic-names.json').read_text()).items()}
try:
 lines=original.decode().splitlines(keepends=True)
 for i,line in enumerate(lines):
  fields=line.rstrip('\n').split(',')
  if fields[0].startswith('0x') and int(fields[0],16) in names:
   assert not fields[6];fields[6]=names[int(fields[0],16)];lines[i]=','.join(fields)+'\n'
 p.write_text(''.join(lines))
 result=subprocess.run([sys.executable,'tools/check.py','_ZN9dot1c64108DirectorC1Ev','--object','build/eu/obj/lib/al/src/Light/LightDataDirector1C6410.o'],capture_output=True,text=True)
 pathlib.Path('build/root-1c6410/check-'+sys.argv[1]+'.log').write_text(result.stdout+result.stderr)
 print(result.stdout+result.stderr)
finally:
 p.write_bytes(original)
 assert p.read_bytes()==original

```

## Diagnostic original-address link

Save as `build/root-1c6410/prepare.py`:

```python
import hashlib,json,subprocess,sys
from pathlib import Path
sys.path.insert(0,str(Path.cwd()))
from tools.low.checkExactBytes import _read_map,_isolate_function
from tools.low.buildProvenance import verify_build_output
from elftools.elf.elffile import ELFFile
out=Path('build/root-1c6410'); obj=Path('build/eu/obj/lib/al/src/Light/LightDataDirector1C6410.o'); names={int(k,16):v for k,v in json.loads((out/'diagnostic-names.json').read_text()).items()}
provenance=verify_build_output(obj);rows=_read_map(Path('data/ver/eu/map.csv'))
for r in rows:
 if r['Start'] in names:
  assert not r['Symbol'];r['Symbol']=names[r['Start']]
symbol=names[0x1c6410]
section,compiled,imports=_isolate_function(obj,symbol,rows,out/'candidate.o')
(out/'imports.json').write_text(json.dumps(imports,indent=2))
(out/'symbols.sym').write_text('#<SYMDEFS>#\n'+''.join('0x%08X %s %s\n'%(i['address'],i['kind'],i['symbol']) for i in imports))
(out/'candidate.sct').write_text('REPLAY 0x001C6410 { CODE 0x001C6410 { candidate.o ('+section+', +FIRST) } }\n')
command=['data/compilers/wibo','data/compilers/4.1/791/bin/armlink.exe','--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--inline','--datacompressor=off','--no_debug','--no_scanlib','--entry='+symbol,'--keep='+symbol,'--scatter='+str(out/'candidate.sct'),'--output='+str(out/'replay.axf'),str(out/'candidate.o'),str(out/'symbols.sym')]
r=subprocess.run(command,capture_output=True,text=True);(out/'link.log').write_text(r.stdout+r.stderr);assert r.returncode==0,r.stdout+r.stderr
with (out/'replay.axf').open('rb') as f:s=ELFFile(f).get_section_by_name('CODE');data=s.data();assert s['sh_addr']==0x1c6410
b=Path('data/ver/eu/code.bin').read_bytes();orig=b[0xc6410:0xc70fc]
summary={'size':len(data),'original_size':len(orig),'different_bytes_at_equal_offsets':sum(a!=b for a,b in zip(data,orig)),'object_section_size':len(compiled),'linked_sha256':hashlib.sha256(data).hexdigest(),'object_sha256':hashlib.sha256(obj.read_bytes()).hexdigest(),'imports':len(imports),'canonical':'full-size mismatch; zero exact credit','diagnostic_extra_identity':'none; existing-row names only'}
(out/'prepare.json').write_text(json.dumps(summary,indent=2));print(json.dumps(summary,indent=2))
from capstone import *
c=Cs(CS_ARCH_ARM,CS_MODE_ARM);c.skipdata=True
(out/'candidate.txt').write_text('\n'.join(f'{i.address:08x}: {i.mnemonic:8s} {i.op_str}' for i in c.disasm(data,0x1c6410)))

```

## Independent caller and path-prefix evidence

Save as `build/root-1c6410/evidence.py`:

```python
import hashlib,json,struct,csv
from pathlib import Path
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
b=Path('data/ver/eu/code.bin').read_bytes();assert hashlib.sha256(b).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
rows=[{k.strip():v.strip() for k,v in r.items()} for r in csv.DictReader(open('data/ver/eu/map.csv'))]
r=next(r for r in rows if int(r['Start'],16)==0x1c6410)
assert int(r['Pool'],16)==0x1c6808 and int(r['End'],16)==0x1c70fc and r['Rank'].strip()=='U'
assert hashlib.sha256(b[0xc6410:0xc70fc]).hexdigest()=='eea62229b770fbe1503a97f3f5d27e3c91182d1b0a4df0efd918f3be2df82d5a'
def word(a):return struct.unpack_from('<I',b,a-0x100000)[0]
def decode(lo,hi):
 c=Cs(CS_ARCH_ARM,CS_MODE_ARM)
 return [{'address':hex(i.address),'instruction':i.mnemonic+' '+i.op_str} for i in c.disasm(b[lo-0x100000:hi-0x100000],lo)]
assert word(0x3d7ac4)==0x39e0e4
assert word(0x243b00)==word(0x28cb6c)==0x3d7abc
assert word(0x243a9c)==0xe58d0000 and word(0x28cb50)==0xe5840000
assert word(0x1c0c0c)==0xe3a00d12 # allocation size 0x480
report={'root':r,'caller_allocation':decode(0x1c0c0c,0x1c0c24),'independent_path_initializer':decode(0x243a7c,0x243ab8),'independent_variadic_path_constructor':decode(0x28cb38,0x28cb70),'storage_initializer':decode(0x28aea8,0x28aee4),'termination_slot':decode(0x39e0e4,0x39e0fc),'existing_table_row_words':[hex(word(a)) for a in range(0x3d7abc,0x3d7ad0,4)],'statement':'Existing map row is 20 bytes. Only its observed address point and dispatch slot are established; complete table allocation and C++ owner are unclaimed.'}
Path('build/root-1c6410/evidence.json').write_text(json.dumps(report,indent=2));print(json.dumps(report,indent=2))

```

## Whole-root replay and independent semantic assertions

Save as `build/root-1c6410/replay.py`:

```python
import hashlib,json,struct,random,sys
from pathlib import Path
from unicorn import *
from unicorn.arm_const import *
from elftools.elf.elffile import ELFFile
BASE=0x100000;ROOT=0x1c6410;OBJ=0x500000;MEM=0x500000;STOP=0x900000;STACK=0xa10000
REGS=[UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3]
SAVED=list(range(UC_ARM_REG_R4,UC_ARM_REG_R11+1));VFP=list(range(UC_ARM_REG_D8,UC_ARM_REG_D15+1))
def pack(*v):return struct.pack('<'+'I'*len(v),*[x&0xffffffff for x in v])
def byaml(obj):
 keys=sorted(set(k for d in [obj]+[v for v in obj.values() if isinstance(v,dict)] for k in d))
 strings=sorted(v for v in obj.values() if isinstance(v,str))
 data=bytearray(b'YB\x01\0'+bytes(12))
 def align():data.extend(bytes(-len(data)%4))
 def table(vals):
  align();base=len(data);data.extend(pack(0xc2|(len(vals)<<8)));idx=len(data);data.extend(bytes(4*(len(vals)+1)))
  for i,s in enumerate(vals):struct.pack_into('<I',data,idx+i*4,len(data)-base);data.extend(s.encode()+b'\0')
  struct.pack_into('<I',data,idx+len(vals)*4,len(data)-base);align();return base
 k=table(keys);v=table(strings)
 def node(obj):
  nested={key:node(value) for key,value in obj.items() if isinstance(value,dict)};align();base=len(data);data.extend(pack(0xc1|(len(obj)<<8)))
  for key,value in sorted(obj.items()):
   if isinstance(value,dict):typ=0xc1;value=nested[key]
   elif isinstance(value,str):typ=0xa0;value=strings.index(value)
   elif isinstance(value,bool):typ=0xd0;value=int(value)
   elif isinstance(value,float):typ=0xd2;value=struct.unpack('<I',struct.pack('<f',value))[0]
   else:typ=0xd1
   data.extend(pack(keys.index(key)|(typ<<24),value))
  return base
 r=node(obj);struct.pack_into('<III',data,4,k,v,r);return bytes(data)

def run(binary,candidate,case):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176);u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000)
 u.mem_map(BASE,0x400000);u.mem_write(BASE,binary)
 if candidate is not None:u.mem_write(ROOT,candidate)
 u.mem_map(MEM,0x300000);u.mem_map(STOP,0x1000);u.mem_map(0xa00000,0x20000)
 u.mem_write(MEM,bytes([case.get('sentinel',0x95)])*0x300000)
 u.mem_write(0xa00000,bytes([0x73])*0x20000)
 def w(a,*v):u.mem_write(a,pack(*v))
 def word(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def cstr(a):return bytes(u.mem_read(a,512)).split(b'\0')[0].decode('shift_jis',errors='replace')
 # Four controlled default light records. All original copy constructors run.
 for j,a in enumerate([0x42ac28,0x42ac88,0x42ace8,0x42ad48]):
  u.mem_write(a,struct.pack('<23f',*[(i+j+1)/37 for i in range(23)])+bytes([j&1,0x19,0x27,0x35]))
 w(0x3e23b8,0x510000)
 paths=[[],[]];blobs=[[],[]]
 for kind,items in enumerate(case.get('items',[[],[]])):
  for i,item in enumerate(items):
   path=item.get('path',f'file{i}.byml');paths[kind].append(path)
   blob=byaml({'Name':item.get('name',f'item{i:03d}'),'Interpolate Frame':item.get('interp',10),**item.get('values',{})})
   addr=0x600000+kind*0x60000+i*0x1000
   if item.get('bad_magic'):blob=b'ZZ'+blob[2:]
   if item.get('bad_version'):blob=blob[:2]+b'\x02\0'+blob[4:]
   u.mem_write(addr,blob);blobs[kind].append(0 if item.get('null_file') else addr)
 for kind in [0,1]:
  w(0x520000+kind*0x100,0x521000+kind*0x100)
  w(0x521000+kind*0x100,0x522000)
 w(0x522000+0x18,STOP+4)
 trace=[];pcs=set();callees=set();allocations=[];last=[ROOT];steps=[0];calls={}
 def ret(v=None):
  if v is not None:u.reg_write(UC_ARM_REG_R0,v)
  u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
 def hook(uc,pc,size,_):
  last[0]=pc;steps[0]+=1
  if ROOT<=pc<0x1c70fc:pcs.add(pc)
  if pc==STOP:uc.emu_stop();return
  args=[uc.reg_read(r) for r in REGS]
  if pc in [0x1c5060,0x1c50a4,0x1d4bc4,0x1d4be0,0x39e160,0x24da80,0x24e054,0x24dafc,0x1c4f64,0x1d4b5c,0x24e0b8,0x39f3fc,0x39e5bc,0x3a0908,0x39fe3c,0x39ea0c,0x2d5918,0x2d5934,0x2905f0,0x39e0e4]:callees.add(pc);calls[hex(pc)]=calls.get(hex(pc),0)+1
  if pc in (0x2932b0,0x2933d0):
   n=args[0];index=len(allocations)+1;addr=0 if index==case.get('fail',-1) else 0x700000+index*0x20000
   assert n<0x20000,(n,case)
   trace.append(['new' if pc==0x2932b0 else 'array',n,addr]);allocations.append((addr,n))
   ret(addr);return
  if pc==0x293088:trace.append(['currentHeap',args[0]]);ret(0x510100);return
  if pc in (0x29c228,0x29a4dc):
   k=pc==0x29a4dc
   arg=args[0];trace.append(['create',int(k),bytes(uc.mem_read(arg,2)).hex(),word(arg+4),word(arg+8),word(arg+12),word(args[1]+4)])
   addr=0 if case.get('creator_fail')==int(k) else 0x530000+int(k)*0x1000
   if addr:w(addr,0x61+int(k),0x73+int(k),0x540000+int(k)*0x1000)
   ret(addr);return
  if pc in (0x2907e0,0x243260):
   path=cstr(word(args[0]+4));k=int(path.endswith('Map'));trace.append(['exists' if pc==0x2907e0 else 'resource',path])
   if pc==0x2907e0:ret(case.get('exists',[True,True])[k])
   else:ret(0 if case.get('null_resource',[False,False])[k] else 0x520000+k*0x100)
   return
  if pc==0x2512f0:
   k=(args[0]-0x520000)//0x100;trace.append(['count',k,cstr(word(args[1]+4))]);ret(case.get('counts',[len(paths[0]),len(paths[1])])[k]);return
  if pc==0x251204:
   k=(args[0]-0x520000)//0x100;i=args[3];trace.append(['path',k,i,cstr(word(args[2]+4))]);text=paths[k][i].encode();cap=word(args[1]+8);data=word(args[1]+4);u.mem_write(data,text[:cap-1]+b'\0');ret();return
  if pc==STOP+4:
   k=(args[0]-0x521000)//0x100;path=cstr(word(args[1]+4));i=paths[k].index(path);trace.append(['file',k,path]);ret(blobs[k][i]);return
 u.hook_add(UC_HOOK_CODE,hook)
 for j,r in enumerate(SAVED):u.reg_write(r,0xa1100000+j*0x1111)
 for j,r in enumerate(VFP):u.reg_write(r,0x1122334455660000+j)
 u.reg_write(UC_ARM_REG_R0,OBJ);u.reg_write(UC_ARM_REG_SP,STACK);u.reg_write(UC_ARM_REG_LR,STOP)
 fault=None
 try:u.emu_start(ROOT,STOP+0x10,count=1000000)
 except UcError as e:fault=str(e)
 if fault:return {'fault':fault,'trace':trace,'pc':hex(last[0])},pcs,callees,calls
 assert last[0]==STOP,('budget',hex(last[0]),case)
 assert u.reg_read(UC_ARM_REG_SP)==STACK
 assert [u.reg_read(r) for r in SAVED]==[0xa1100000+j*0x1111 for j in range(8)]
 assert [u.reg_read(r) for r in VFP]==[0x1122334455660000+j for j in range(8)]
 # Independent semantic checks supplement original-versus-candidate equality.
 expected_checks=0
 for kind in [0,1]:
  offset=8+kind*16;array=word(OBJ+offset);capacity=word(OBJ+offset+4);count=word(OBJ+offset+12)
  items=case.get('items',[[],[]])[kind]
  active=case.get('exists',[True,True])[kind] and not case.get('null_resource',[False,False])[kind]
  directory_count=case.get('counts',[len(case.get('items',[[],[]])[0]),len(case.get('items',[[],[]])[1])])[kind]
  selected=[it for i,it in enumerate(items) if i<directory_count and '.byml' in it.get('path',f'file{i}.byml')[:127]] if active and capacity else []
  assert count==len(selected),('independent count',kind,count,selected,case)
  expected_names=['NULL' if any(it.get(x) for x in ['bad_magic','bad_version','null_file']) else it.get('name',f'item{i:03d}') for i,it in enumerate(selected)]
  if expected_names:expected_names=sorted(expected_names[:-1],reverse=True)+expected_names[-1:]
  width=0x128 if kind==0 else 0x64
  got_names=[cstr(word(array+i*width)) for i in range(count)]
  assert got_names==expected_names,('independent sort range',kind,got_names,expected_names,case)
  expected_checks+=1+count
  byname={it.get('name'):it for it in selected}
  for i,name in enumerate(got_names):
   it=byname.get(name)
   if not it:continue
   rec=array+i*width
   if kind==0:
    assert word(rec+4)==it.get('interp',10)&0xffffffff,('interpolation',case);expected_checks+=1
   for j,key in enumerate(['Player Light','Obj Light','MapObj Light'] if kind==0 else ['Light Param']):
    values=it.get('values',{}).get(key)
    if not values:continue
    light=rec+(8+j*0x60 if kind==0 else 4)
    names=[f'{col}.{ch}' for col in ['Ambient','Diffuse','Specular0','Specular1','ConstantColor5'] for ch in 'rgba']+['Direction.x','Direction.y','Direction.z']
    for field,key in enumerate(names):
     if key in values:
      assert bytes(u.mem_read(light+field*4,4))==struct.pack('<f',values[key]),('light field',key,case);expected_checks+=1
    if 'IsCameraFollow' in values:
     assert bytes(u.mem_read(light+0x5c,1))==bytes([values['IsCameraFollow']]);expected_checks+=1
 for index,off in enumerate([0x474,0x478]):
  ptr=word(OBJ+off)
  if not ptr:continue
  expected=[0.0]*8+[.5,.5,.5,0]*2+[0.0]*4+[0,0,1] if index==0 else [.23,.22,.178,1,.255,.254,.213,1]+[.5,.5,.5,1]*2+[.2,.2,.155,1]+[-1,-1,-.7]
  assert bytes(u.mem_read(ptr,92))==struct.pack('<23f',*expected),('fallback',index,case)
  assert bytes(u.mem_read(ptr+92,1))==bytes([index]);expected_checks+=24
 snapshots=[]
 # Pointer targets are canonicalized by their string contents, never raw local-pool addresses.
 for addr,n in allocations:
  if not addr:continue
  snap=bytearray(u.mem_read(addr,n));offsets=[]
  if n in [0x128,0x64]:offsets=[0]
  elif n>8 and struct.unpack_from('<I',snap)[0] in [0x128,0x64]:
   width,count=struct.unpack_from('<II',snap);offsets=[8+i*width for i in range(count)]
  strings=[]
  for off in offsets:
   ptr=struct.unpack_from('<I',snap,off)[0]
   strings.append((off,cstr(ptr) if ptr else None));struct.pack_into('<I',snap,off,0)
  snapshots.append((addr,n,snap.hex(),strings))
 return {'return':u.reg_read(UC_ARM_REG_R0),'object':bytes(u.mem_read(OBJ,0x480)).hex(),'allocations':snapshots,'trace':trace,'fault':None,'independent_checks':expected_checks},pcs,callees,calls

def cases():
 b={'items':[[],[]]};out=[b]
 for ex in [(0,0),(1,0),(0,1),(1,1)]:
  for null in [(0,0),(1,0),(0,1),(1,1)]:out.append(dict(b,exists=ex,null_resource=null))
 for kind in [0,1,2]:
  for n in [1,2,3,16,17,18,31,40]:
   for order in ['forward','reverse','random']:
    ids=list(range(n))
    if order=='reverse':ids.reverse()
    if order=='random':random.Random(927+n).shuffle(ids)
    items=[{'name':f'key{j:03d}','interp':j-20} for j in ids]
    out.append({'items':[items if kind in [0,2] else [],items if kind in [1,2] else []]})
 colors=['Ambient','Diffuse','Specular0','Specular1','ConstantColor5']
 params={f'{name}.{channel}':(i*4+j-8)/10.0 for i,name in enumerate(colors) for j,channel in enumerate('rgba')}
 params.update({'Direction.x':1.25,'Direction.y':-2.5,'Direction.z':3.75,'IsCameraFollow':True})
 for kind in [0,1]:
  for n in [1,3,18,33]:
   items=[]
   for i in range(n):
    values={'Player Light':params,'Obj Light':dict(params,**{'Ambient.r':-.25}), 'MapObj Light':dict(params,IsCameraFollow=False)} if kind==0 else {'Light Param':params}
    items.append({'name':f'light{n-i:03d}','interp':i-5,'values':values})
   pair=[[],[]];pair[kind]=items;out.append({'items':pair})
 for kind in [0,1]:
  items=[{'name':f'key{i:03d}','path':p} for i,p in enumerate(['ignore.txt','valid.byml','contains.byml.extra','UPPER.BYML','.byml','suffixbyml','again.byml'])]
  pair=[[],[]];pair[kind]=items;out.append({'items':pair})
 for bad in ['bad_magic','bad_version','null_file']:
  for kind in [0,1]:
   pair=[[],[]];pair[kind]=[{'name':'broken',bad:True}];out.append({'items':pair,'malformed':bad})
 for n in [-3,0]:out.append({'items':[[],[]],'counts':[n,n]})
 for kind in [0,1,2]:
  items=[{'name':f'key{i:03d}'} for i in [5,4,3,2,1,0]]
  pair=[items if kind in [0,2] else [],items if kind in [1,2] else []]
  for fail in range(1,7):out.append({'items':pair,'fail':fail})
 for fail in [0,1]:out.append(dict(b,creator_fail=fail))
 for sentinel in [0,255,53]:out.append(dict(b,sentinel=sentinel))
 return out
if __name__=='__main__':
 binary=Path('data/ver/eu/code.bin').read_bytes();assert hashlib.sha256(binary).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
 with Path('build/root-1c6410/replay.axf').open('rb') as f:candidate=ELFFile(f).get_section_by_name('CODE').data()
 allcases=cases();coverage=set();callees=set();countcalls={};faults=0;semantic_checks=0
 for i,case in enumerate(allcases):
  a,pc,cal,calls=run(binary,None,case);b,_,_,_=run(binary,candidate,case)
  for k in a:
   if k=='pc':continue
   if a[k]!=b[k]:
    print('FAIL',i,case,k)
    if k=='object':print([(hex(j//2),a[k][j:j+8],b[k][j:j+8]) for j in range(0,len(a[k]),8) if a[k][j:j+8]!=b[k][j:j+8]][:30])
    else:print(str(a[k])[:3000],str(b[k])[:3000])
    Path('build/root-1c6410/failure.json').write_text(json.dumps({'case':case,'original':a,'candidate':b},indent=2));raise SystemExit(1)
  coverage.update(pc);callees.update(cal);faults+=a['fault'] is not None;semantic_checks+=a.get('independent_checks',0)
  for k,v in calls.items():countcalls[k]=countcalls.get(k,0)+v
 summary={'pairs':len(allcases),'returning_pairs':len(allcases)-faults,'fault_pairs':faults,'independent_checks_per_implementation':semantic_checks,'original_pcs':list(map(hex,sorted(coverage))),'actual_callees':list(map(hex,sorted(callees))),'call_counts':countcalls,'candidate_sha256':hashlib.sha256(candidate).hexdigest()}
 Path('build/root-1c6410/replay.json').write_text(json.dumps(summary,indent=2));print({k:v for k,v in summary.items() if k!='original_pcs'})

```

## Every accepted canonical definition

Save as `build/root-1c6410/preserve.py`:

```python
import csv,json,hashlib,sys,time
from pathlib import Path
sys.path.insert(0,str(Path.cwd()))
from elftools.elf.elffile import ELFFile
from tools.low.buildProvenance import verify_build_output
from tools.low.checkExactBytes import check_exact_bytes
start=time.time();rows=list(csv.DictReader(open('data/ver/eu/map.csv')));want={r['Symbol'].strip():r for r in rows if r['Rank'].strip()=='O' and 'f' in r['Type']};found={x:[] for x in want}
for p in Path('build/eu/obj').rglob('*.o'):
 if '/build/eu/split/' in str(p):continue
 with p.open('rb') as f:
  e=ELFFile(f);sym=e.get_section_by_name('.symtab')
  if not sym:continue
  for s in sym.iter_symbols():
   if s.name in found and isinstance(s['st_shndx'],int) and s['st_info']['type']=='STT_FUNC':found[s.name].append(p)
results=[];verified={}
for symbol,paths in found.items():
 assert paths,(symbol,'no object')
 for p in paths:
  if p not in verified:verified[p]=verify_build_output(p)
  r=check_exact_bytes(symbol,p,'eu',verified[p]['compiler'])
  results.append({'symbol':symbol,'object':str(p),'exact':r['exact'],'reason':r['reason']})
  assert r['exact'],results[-1]
summary={'roots':len(want),'definitions':len(results),'objects':len(verified),'seconds':time.time()-start,'results':results}
Path('build/root-1c6410/preserve.json').write_text(json.dumps(summary,indent=2));print({k:v for k,v in summary.items() if k!='results'})

```

## Frozen-result assertions

Save as `build/root-1c6410/freeze.py`:

```python
import json,hashlib
from pathlib import Path
out=Path('build/root-1c6410')
expected={'lib/al/include/Light/LightDataDirector1C6410.h':'1ed185056458f38dbbc7920dfdbf469751d2f9c6adcb175e453067662ac9feb0','lib/al/src/Light/LightDataDirector1C6410.cpp':'0c23f9a3461bab7637562bb01a2e48d82c54b25677ed79c0185aa0d647d2edb9','build/eu/obj/lib/al/src/Light/LightDataDirector1C6410.o':'f794fcbddc1d69a5871417e7d1da9e55876d3dedc6687ce0260702ae7f7175a3','data/ver/eu/map.csv':'514c3ec6f2343e8a40fc73996df78fc95b2e33f4897fbdda6bb0311df6cf40fa','data/ver/eu/code.bin':'e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'}
for p,sha in expected.items():assert hashlib.sha256(Path(p).read_bytes()).hexdigest()==sha,p
replay=json.loads((out/'replay.json').read_text());preserve=json.loads((out/'preserve.json').read_text());prep=json.loads((out/'prepare.json').read_text())
assert [replay[k] for k in ['pairs','returning_pairs','fault_pairs','independent_checks_per_implementation']]==[130,129,1,14253]
assert len(replay['original_pcs'])==762 and len(replay['actual_callees'])==20
assert [preserve[k] for k in ['roots','definitions','objects']]==[663,682,165]
assert prep['size']==3256 and prep['original_size']==3308 and prep['imports']==34
assert replay['candidate_sha256']==prep['linked_sha256']=='8324d345af6a372e71ac5af4c6894dbf606ba8f55a75b1595eb2509e6035e456'
files=[Path(p) for p in expected]+list(out.glob('*.py'))+[out/p for p in ['diagnostic-names.json','prepare.json','replay.json','evidence.json','preserve.json','imports.json','check-final-clean.log','clean-build.log']]
manifest={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files}
(out/'frozen.json').write_text(json.dumps(manifest,indent=2));print('Verified',len(manifest),'frozen files; canonical zero exact credit')

```

## Executed commands and expected result

Create `build/root-1c6410` before saving the files above. In this workspace the existing private inputs and virtual environment are ignored symlinks to the approved local `mario-dot` checkout; use equivalent local inputs in another checkout.

```sh
. ./development_environment.sh
export DEVKITARM=/usr
python make.py eu -ca > build/root-1c6410/clean-build.log 2>&1
python build/root-1c6410/check.py final-clean
python build/root-1c6410/prepare.py
python build/root-1c6410/evidence.py > build/root-1c6410/evidence.log
python build/root-1c6410/replay.py > build/root-1c6410/replay.log 2>&1
python build/root-1c6410/preserve.py > build/root-1c6410/preserve-final.log 2>&1
python build/root-1c6410/freeze.py
```

The strict checker reports the complete-section size mismatch. Diagnostic preparation reports 3,256 versus 3,308 bytes and 34 established imports. Replay reports 130 pairs, 129 returning pairs, one matching fault and 14,253 independent semantic checks per implementation. It visits 762 original instruction addresses and executes 20 tracked real original callee entries. The accepted-root audit passes 663 roots / 682 definitions across 165 source objects; the measured final clean-build audit took 124.405 seconds, excluding the build. Reproduction timing may vary. The final freeze command verifies all hashes and result counts.

The replay deliberately preserves the original last-record exclusion and descending sort order. The independent semantic assertions first rejected an incorrect ascending-order expectation; the final descending expectation above is the executed one. An earlier harness key spelling omitted Direction fields; the final dot-separated Direction.x/y/z keys above exercise and independently verify those original parser writes. Neither harness correction changed the retained compiled source.

Modeled functions are named directly in the hook. They cover allocation/current heap, the two graphics creators and archive/directory/file boundaries. Original constructors, BYAML decoding, nested light parsing, copy/assignment, color clamp, string termination, comparators and partition/insertion routines execute from the verified private executable. The six damaged/null BYAML cases and single null-first-creator fault are separate observations, not successful initialization evidence. A fault pair compares fault class and ordered calls only. This diagnostic is not production rendering or a complete startup test.
