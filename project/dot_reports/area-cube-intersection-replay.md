# Cube intersection verification recipes

These Python recipes are notes, not project-tool changes. Copy each fenced block to the stated ignored build path and run it from the frozen family checkout after `. ./development_environment.sh; export DEVKITARM=/usr`. The scripts require the owner’s local EU dump and the already installed ARMCC 791, wibo, Capstone, pyelftools and Unicorn. They never upload the dump.

## build/cube-evidence/check_form.py

```python
import subprocess,json,sys,time,shutil
from pathlib import Path
sys.path.insert(0,str(Path.cwd()))
from tools.low.checkExactBytes import check_exact_bytes
from tools.low.buildProvenance import verify_build_output
form=sys.argv[1]; p=Path('data/ver/eu/map.csv');original=p.read_bytes();s=original.decode();root='fn_0033091C';obj=Path('build/eu/obj/lib/al/src/AreaObj/alAreaShapeCubeIntersection.o');d=Path('build/cube-evidence')/form;d.mkdir(exist_ok=True)
rows=s.splitlines(keepends=True)
for i,line in enumerate(rows):
 for addr,sym in [('0x0033091C',root),('0x00337488','fn_00337488')]:
  if line.startswith(addr+','):
   parts=line.split(',');assert not parts[6].strip();parts[6]=sym;rows[i]=','.join(parts)
try:
 p.write_text(''.join(rows));print(verify_build_output(obj))
 t=time.monotonic();r=subprocess.run([sys.executable,'tools/check.py',root,'--object',str(obj)],capture_output=True,text=True);(d/'check.log').write_text(r.stdout+r.stderr);print(r.stdout+r.stderr)
 result=check_exact_bytes(root,obj,'eu','4.1/791',d/'exact');result['command_returncode']=r.returncode;result['whole_check_seconds']=time.monotonic()-t;(d/'result.json').write_text(json.dumps(result,indent=2));print(result)
 shutil.copy2(obj,d/'source.o');shutil.copy2(obj.with_suffix('.provenance.json'),d/'source.provenance.json')
finally:
 p.write_bytes(original);assert p.read_bytes()==original
```

## build/cube-evidence/link_diagnostic.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
import json,subprocess,os,sys,hashlib
form=sys.argv[1];d=Path('build/cube-evidence')/form;e=json.loads((d/'result.json').read_text())['evidence'];target=d/'diagnostic';target.mkdir(exist_ok=True)
(target/'imports.sym').write_text('#<SYMDEFS>#\n'+''.join(f'0x{x["address"]:08X} {x["kind"]} {x["symbol"]}\n' for x in e['imports']))
(target/'layout.sct').write_text('ROOT 0x0033091C\n{\n CODE 0x0033091C\n {\n  source.o (i.fn_0033091C, +FIRST)\n  *(+RO)\n }\n}\n')
cmd=['data/compilers/wibo','data/compilers/4.1/791/bin/armlink.exe','--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--inline','--datacompressor=off','--no_debug','--no_scanlib','--mangled','--symbols','--map','--entry=fn_0033091C','--keep=fn_0033091C',f'--scatter={target}/layout.sct',f'--output={target}/candidate.axf',f'--list={target}/candidate.map',str(d/'source.o'),str(target/'imports.sym')]
r=subprocess.run(cmd,capture_output=True,text=True,env=dict(os.environ,TMP='/tmp'));(target/'link.log').write_text(r.stdout+r.stderr);print(r.returncode,r.stdout+r.stderr);assert r.returncode==0
with (target/'candidate.axf').open('rb') as f:
 elf=ELFFile(f);sec=elf.get_section_by_name('CODE');b=sec.data();assert sec['sh_addr']==0x33091c
(target/'candidate.bin').write_bytes(b);orig=Path('data/ver/eu/code.bin').read_bytes()[0x23091c:0x2311d8];result={'type':'Diagnostic actual-source link; not an exactness check','size':len(b),'original_size':len(orig),'overlap_different_bytes':sum(a!=b for a,b in zip(b,orig)),'size_delta':len(b)-len(orig),'sha256':hashlib.sha256(b).hexdigest(),'command':cmd};(target/'result.json').write_text(json.dumps(result,indent=2));print(result)
```

## build/cube-evidence/replay.py

```python
import struct,json,hashlib,random,sys,time
from pathlib import Path
from collections import Counter
from unicorn import *
from unicorn.arm_const import *
ROOT=0x33091c;END=0x3311d8;STOP=0x700000;MEM=0x600000;STACK=0x820000
ORIG=Path('data/ver/eu/code.bin').read_bytes();assert hashlib.sha256(ORIG).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
CALLEES={0x216d9c:'local',0x337488:'world',0x27cb48:'add',0x27cb64:'sub',0x27cc64:'scale',0x26d698:'isNearZero',0x1d7978:'inverseTransform',0x27cc48:'divide',0x27cb80:'multiply',0x27cb9c:'transform'}
def f(x):return struct.pack('<f',x)
def vec(xs):return b''.join(f(x) for x in xs)
def w(x):return struct.pack('<I',x&0xffffffff)
def run(candidate,c):
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176);u.mem_map(0x100000,0x400000);u.mem_write(0x100000,ORIG)
 if candidate:u.mem_write(ROOT,candidate)
 u.mem_map(MEM,0x1000);u.mem_map(STOP,0x1000);u.mem_map(0x800000,0x40000)
 u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000);u.reg_write(UC_ARM_REG_FPSCR,c.get('fpscr',0))
 u.mem_write(MEM,bytes([0xa5])*0x1000);u.mem_write(MEM,w(0x3d62c0)+w(0 if c.get('null_matrix') else MEM+0x100)+vec(c.get('scales',[1.,1.,1.]))+bytes([c.get('flag',0)]))
 u.mem_write(MEM+0x100,vec(c.get('matrix',[1.,0.,0.,0.,0.,1.,0.,0.,0.,0.,1.,0.])))
 u.mem_write(MEM+0x200,vec(c['start']));u.mem_write(MEM+0x220,vec(c['end']));out=0 if c.get('out_null') else MEM+c.get('out_offset',0x240)
 if c.get('start_bits'):u.mem_write(MEM+0x200,b''.join(w(x) for x in c['start_bits']))
 if c.get('end_bits'):u.mem_write(MEM+0x220,b''.join(w(x) for x in c['end_bits']))
 for r,v in zip([UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3],[MEM,out,MEM+0x200,MEM+0x220]):u.reg_write(r,v)
 u.reg_write(UC_ARM_REG_SP,STACK);u.reg_write(UC_ARM_REG_LR,STOP)
 saved={r:0xabcd0000+i for i,r in enumerate(list(range(UC_ARM_REG_R4,UC_ARM_REG_R11+1))+list(range(UC_ARM_REG_D8,UC_ARM_REG_D15+1)))}
 for r,v in saved.items():u.reg_write(r,v)
 trace=[];visited=set();steps=0;minsp=STACK;fault=None;reached=set()
 def hook(u,a,size,data):
  nonlocal steps,minsp
  steps+=1;minsp=min(minsp,u.reg_read(UC_ARM_REG_SP))
  if ROOT<=a<END:visited.add(a)
  if a in CALLEES:
   reached.add(a)
   if a in (0x216d9c,0x337488):trace.append([CALLEES[a],bytes(u.mem_read(u.reg_read(UC_ARM_REG_R2),12)).hex()])
 def bad(u,access,a,n,v,data):
  nonlocal fault
  fault=[access,a,n];return False
 u.hook_add(UC_HOOK_CODE,hook);u.hook_add(UC_HOOK_MEM_INVALID,bad)
 error=None
 try:u.emu_start(ROOT,STOP,count=10000)
 except UcError as e:error=str(e)
 if error:result={'fault':fault,'error':error,'trace':trace,'memory':bytes(u.mem_read(MEM,0x1000)).hex()}
 elif u.reg_read(UC_ARM_REG_PC)!=STOP:result={'error':'instruction limit','trace':trace}
 else:result={'return':u.reg_read(UC_ARM_REG_R0),'memory':bytes(u.mem_read(MEM,0x1000)).hex(),'trace':trace,'fpscr_exception_bits':u.reg_read(UC_ARM_REG_FPSCR)&0x9f,'sp_ok':u.reg_read(UC_ARM_REG_SP)==STACK,'callee_saved_ok':all(u.reg_read(r)==v for r,v in saved.items())}
 return result,{'steps':steps,'stack':STACK-minsp,'visited':sorted(visited),'reached':sorted(reached)}
def fixtures():
 cases=[]
 for flag in [0,1,2,255]:
  for axis in range(3):
   for sign in [-1,1]:
    for outside in [False,True]:
     start=[0.,250. if flag==1 else 0.,0.];end=start[:]
     if outside:start[axis]=sign*2000.;end[axis]=0.
     else:end[axis]=sign*2000.
     for alias in [0x240,0x200,0x220,8,20]:cases.append(dict(flag=flag,start=start,end=end,out_offset=alias,category='faces-alias'))
  for start,end in [([0.,0.,0.],[0.,0.,0.]),([-500.,-500.,-500.],[500.,500.,500.]),([600.,600.,600.],[700.,700.,700.]),([-600.,600.,0.],[600.,600.,0.]),([0.,0.,0.],[0.,500.,0.]),([0.,0.,0.],[0.,-500.,0.])]:cases.append(dict(flag=flag,start=start,end=end,category='boundary'))
 rng=random.Random(0x33091c)
 for i in range(1200):
  scales=[rng.choice([.5,1.,2.,-1.,-2.]) for _ in range(3)];angle=rng.randrange(4);co=[1.,0.,-1.,0.][angle];si=[0.,1.,0.,-1.][angle];m=[co,0.,si,rng.uniform(-500,500),0.,1.,0.,rng.uniform(-500,500),-si,0.,co,rng.uniform(-500,500)]
  cases.append(dict(flag=rng.choice([0,1,2,255]),start=[rng.uniform(-4000,4000) for _ in range(3)],end=[rng.uniform(-4000,4000) for _ in range(3)],scales=scales,matrix=m,category='finite-random'))
 specials=[0.,-0.,1e-40,-1e-40,3.4028234663852886e38,-3.4028234663852886e38,float('inf'),float('-inf'),struct.unpack('<f',w(0x7fc00001))[0],struct.unpack('<f',w(0xffc00001))[0]]
 for x in specials:
  for axis in range(3):
   for where in ['start','end','scales']:
    c=dict(start=[0.,0.,0.],end=[2000.,2000.,2000.],scales=[1.,1.,1.],category='fp');c[where][axis]=x
    for fpscr in [0,0x01000000,0x02000000]:cases.append(dict(c,fpscr=fpscr))
 for axis in range(3):
  for scale in [0.,-0.,1e-8,-1e-8]:
   sc=[1.,1.,1.];sc[axis]=scale;cases.append(dict(start=[0.,0.,0.],end=[2000.,2000.,2000.],scales=sc,category='invalid-scale'))
 cases.append(dict(start=[0.,0.,0.],end=[2000.,2000.,2000.],null_matrix=True,category='fault-null-matrix'))
 for offset in [0,4,0xffc]:cases.append(dict(start=[0.,0.,0.],end=[2000.,0.,0.],out_offset=offset,category='fault-or-alias-output'))
 cases.append(dict(start=[0.,0.,0.],end=[2000.,0.,0.],out_null=True,category='fault-null-output'))
 for bits in [0x7f800001,0xff800001,0x7fffffff,0xffffffff]:
  for axis in range(3):
   for target in ['start_bits','end_bits']:
    v=[0,0,0];v[axis]=bits
    for fpscr in [0,0x01000000,0x02000000]:cases.append(dict(start=[0.,0.,0.],end=[2000.,2000.,2000.],**{target:v},fpscr=fpscr,category='raw-fp'))
 for word in range(12):
  for special in [float('nan'),float('inf'),float('-inf'),3.4028234663852886e38]:
   for axis in range(3):
    mat=[1.,0.,0.,0.,0.,1.,0.,0.,0.,0.,1.,0.];mat[word]=special;end=[0.,0.,0.];end[axis]=2000.
    cases.append(dict(start=[0.,0.,0.],end=end,matrix=mat,category='matrix-fp'))
 return cases
if __name__=='__main__':
 form=sys.argv[1];d=Path('build/cube-evidence')/form;candidate=(d/'diagnostic/candidate.bin').read_bytes();cases=fixtures();fail=[];counts=Counter();reached=set();visited=set();steps=[0,0];maxsteps=[0,0];maxstack=[0,0];t=time.monotonic();outcomes=[]
 for i,c in enumerate(cases):
  x,a=run(None,c);y,b=run(candidate,c);visited.update(a['visited']);reached.update(a['reached'])
  for j,z in enumerate([a,b]):steps[j]+=z['steps'];maxsteps[j]=max(maxsteps[j],z['steps']);maxstack[j]=max(maxstack[j],z['stack'])
  if x!=y:
   fail.append({'index':i,'case':c,'fields':[k for k in x.keys()|y.keys() if x.get(k)!=y.get(k)],'original':x,'candidate':y})
  counts[(c['category'],str(x.get('return',x.get('error'))))]+=1;outcomes.append({'index':i,'category':c['category'],'matched':x==y,'return':x.get('return'),'error':x.get('error')})
 result={'cases':len(cases),'passed':len(cases)-len(fail),'failed':len(fail),'failures':fail,'outcomes':outcomes,'counts':{str(k):v for k,v in counts.items()},'original_root_pcs':sorted(visited),'original_callees':sorted(reached),'models':[],'compiled_entries':['fn_0033091C'],'total_steps':steps,'max_steps':maxsteps,'max_stack':maxstack,'seconds':time.monotonic()-t};(d/'replay.json').write_text(json.dumps(result,indent=2));print({k:v for k,v in result.items() if k not in ['failures','outcomes','original_root_pcs']});print('first failures',[(x['index'],x['case'],x['fields']) for x in fail[:5]])
```

## build/cube-evidence/preserve.py

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
import hashlib,json,subprocess,time,sys
B=Path(sys.argv[1]).resolve();C=Path.cwd();O=Path('build/cube-evidence');O.mkdir(parents=True,exist_ok=True)
REPORT=B/'build/dot-baseline-754/report.json';EXPECTED='c208f39fadf49e66c5c98c97b445881ba2e51ade07b5724610cf0309175ec028'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
assert sha(REPORT)==EXPECTED
for root in (B,C):
 subprocess.run(['git','diff','--exit-code','HEAD','--','Game','lib','data/config.json','data/ver/eu/map.csv'],cwd=root,check=True,capture_output=True)
report=json.loads(REPORT.read_text());assert report['checkpoint']=='754f99a30a337756df5aa01c28b4ed6a977fecd5' and report['accepted'] and report['clean_build_returncode']==0
checks=report['prior_checks'];assert len(checks)==753 and len({x['symbol'] for x in checks})==733 and all(x['returncode']==0 for x in checks)
assert (B/'data/ver/eu/map.csv').read_bytes()==(C/'data/ver/eu/map.csv').read_bytes()
def canonical(path):
 with path.open('rb') as f:
  e=ELFFile(f);st=e.get_section_by_name('.symtab');ret=[]
  for i,s in enumerate(e.iter_sections()):
   if not s['sh_flags']&2:continue
   r=[]
   for rel in e.iter_sections():
    if isinstance(rel,RelocationSection) and rel['sh_info']==i:
     for v in rel.iter_relocations():
      sym=st.get_symbol(v['r_info_sym']);ndx=sym['st_shndx'];name=e.get_section(ndx).name if isinstance(ndx,int) else ndx
      r.append((v['r_offset'],v['r_info_type'],sym.name,sym['st_value'],sym['st_size'],sym['st_info']['bind'],sym['st_info']['type'],name))
   ret.append((s.name,s['sh_type'],s['sh_flags'],s['sh_addralign'],s['sh_size'],sha_bytes(s.data()),r))
  defs=sorted((s.name,s['st_value'],s['st_size'],s['st_info']['bind'],s['st_info']['type'],e.get_section(s['st_shndx']).name) for s in st.iter_symbols() if isinstance(s['st_shndx'],int) and s['st_info']['type']=='STT_FUNC')
  all_symbols=[]
  for sym in st.iter_symbols():
   ndx=sym['st_shndx'];section=e.get_section(ndx).name if isinstance(ndx,int) and ndx else ndx
   name=sym.name.replace(str(B),'$ROOT').replace(str(C),'$ROOT')
   all_symbols.append((name,sym['st_value'],sym['st_size'],sym['st_info']['bind'],sym['st_info']['type'],dict(sym['st_other']),section))
  return ret,defs,all_symbols

def sha_bytes(data):return hashlib.sha256(data).hexdigest()
start=time.monotonic();records=[];inputs={};pair_defs={}
paths=sorted(B.glob('build/eu/obj/**/*.provenance.json'))
for prov in paths:
 rel=prov.relative_to(B);p=json.loads(prov.read_text())
 if not p['source'].startswith(('Game/','lib/')):continue
 q=json.loads((C/rel).read_text())
 assert p['schema']==q['schema']==1 and p['inputs_stable'] and q['inputs_stable']
 for key in ('source','object','version','compiler','compiler_sha256','inputs'):assert p[key]==q[key],(str(rel),key)
 for root,r in ((B,p),(C,q)):
  assert sha(root/r['object'])==r['object_sha256'];assert sha(root/r['compiler'])==r['compiler_sha256']
  for name,h in r['inputs'].items():assert sha(root/name)==h;inputs[name]=h
  committed=subprocess.check_output(['git','ls-files','-z','--',*r['inputs']],cwd=root).decode().split('\0');assert set(r['inputs'])<=set(committed)
 assert [x.replace(str(B),'$ROOT') for x in p['command']]==[x.replace(str(C),'$ROOT') for x in q['command']]
 left=canonical(B/p['object']);right=canonical(C/q['object']);assert left==right,p['object']
 pair_defs[p['object']]=left[1]
 records.append({'object':p['object'],'baseline_sha256':p['object_sha256'],'proposal_sha256':q['object_sha256'],'raw_equal':p['object_sha256']==q['object_sha256'],'allocated_equal':True})
for check in checks:assert any(d[0]==check['symbol'] for d in pair_defs[check['object']])
extra=[]
for prov in C.glob('build/eu/obj/**/*.provenance.json'):
 if not (B/prov.relative_to(C)).exists():
  p=json.loads(prov.read_text())
  if not p['source'].startswith(('Game/','lib/')):continue
  extra.append(p['source']);defs=canonical(C/p['object'])[1];assert [d[0] for d in defs]==['fn_0033091C'],defs
assert extra==['lib/al/src/AreaObj/alAreaShapeCubeIntersection.cpp']
result={'baseline_checkpoint':report['checkpoint'],'baseline_report_sha256':EXPECTED,'roots':733,'definitions':753,'prior_objects':len(records),'prior_objects_without_accepted_entries':len(set(x['object'] for x in records)-set(x['object'] for x in checks)),'raw_equal_objects':sum(x['raw_equal'] for x in records),'allocated_equal_objects':len(records),'unchanged_inputs':len(inputs),'extra_sources':extra,'seconds':time.monotonic()-start,'map_sha256':sha(C/'data/ver/eu/map.csv'),'records':records,'inputs':inputs,'scope':'Exhaustive unchanged-input and compiler-object equivalence to the separately verified 733-root/753-definition baseline. This is not a new canonical 753-check run.'}
(O/'preservation-equivalence.json').write_text(json.dumps(result,indent=2));print({k:v for k,v in result.items() if k not in ('records','inputs')})
```

