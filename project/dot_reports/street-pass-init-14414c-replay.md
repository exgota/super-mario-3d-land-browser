# Executed StreetPassObj reproduction

Run from the committed branch root with approved791/902,wibo and the established Python environment (including Unicorn,Capstone,pyelftools). Supply only the owner's ignored verified EU code.bin/exh.bin and a separately canonical-verified pristine54d checkout for the preservation equivalence step. Scripts never modify source, toolchain configuration, boundaries, Types or binary inputs. The probe temporarily names the existing U root and restores map bytes even on failure.

Extract the Python blocks below to an ignored directory, then execute:

```sh
. ./development_environment.sh
export DEVKITARM=/usr
TMP=/tmp python make.py eu -ca
python build/street-pass-init/probe.py reproduced
python build/street-pass-init/link.py
python build/street-pass-init/replay.py
python build/street-pass-init/preserve.py /path/to/canonical-verified-pristine54d
```

The checker rejection is expected. The replay and preservation scripts assert their complete stated contracts. No script depends on an uncommitted prior fixture or copies compiler objects; BYAML and model state are generated below. A missing pristine baseline requires main's canonical preservation gate, not assuming this equivalence claim applies. The report names the exact required baseline report hash.

Extract with this command:

```python
from pathlib import Path
import re
text=Path('project/dot_reports/street-pass-init-14414c-replay.md').read_text()
out=Path('build/street-pass-init');out.mkdir(parents=True,exist_ok=True)
for name,body in re.findall(r'<!-- script: ([^ ]+) -->\n```python\n(.*?)```',text,re.S):
    (out/name).write_text(body)
```

<!-- script: probe.py -->
```python
from pathlib import Path
import subprocess,os,json,time,hashlib,sys
p=Path('data/ver/eu/map.csv');b=p.read_bytes(); lines=[]
for l in b.decode().splitlines(True):
 f=l.split(',')
 if f[0]=='0x0014414C':assert not f[6].strip();f[6]='fn_0014414C'
 lines.append(','.join(f))
try:
 p.write_text(''.join(lines));t=time.time()
 r=subprocess.run(['python','tools/check.py','fn_0014414C','--object','build/eu/obj/Game/backup/src/MapObj/StreetPassObjInit14414C.o'],capture_output=True,text=True,env=dict(os.environ,TMP='/tmp'))
 result={'source_commit':subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip(),'seconds':time.time()-t,'returncode':r.returncode,'output':r.stdout+r.stderr};print(result);Path('build/street-pass-init/check-'+sys.argv[1]+'.json').write_text(json.dumps(result,indent=2))
finally:p.write_bytes(b)
```

<!-- script: link.py -->
```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
import re,csv,subprocess,os,json
out=Path('build/street-pass-init');obj='build/eu/obj/Game/backup/src/MapObj/StreetPassObjInit14414C.o';rows=[{k.strip():v.strip() for k,v in r.items()} for r in csv.DictReader(open('data/ver/eu/map.csv'))];byname={r['Symbol']:r for r in rows if r['Symbol']};lines=['#<SYMDEFS>#'];imports=[]
with open(obj,'rb') as f:
 e=ELFFile(f);st=e.get_section_by_name('.symtab');root='fn_0014414C'
 for s in st.iter_symbols():
  if s['st_shndx']!='SHN_UNDEF' or not s.name or '$$' in s.name or s['st_info']['bind']=='STB_WEAK':continue
  n=s.name;m=re.fullmatch('(fn|dat)_([0-9A-F]{8})',n)
  if m:r=next(x for x in rows if int(x['Start'],16)==int(m[2],16))
  else:r=byname[n]
  a=int(r['Start'],16);kind='A' if 'f' in r['Type'] else 'D';lines.append(f'0x{a:08X} {kind} {n}');imports.append({'symbol':n,'address':a,'kind':kind,'end':int(r['End'],16)})
 (out/'imports.json').write_text(json.dumps(imports,indent=2))
(out/'imports.sym').write_text('\n'.join(lines)+'\n')
subprocess.run(['data/compilers/wibo','data/compilers/4.1/791/bin/armlink.exe','--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--inline','--datacompressor=off','--no_debug','--no_scanlib','--mangled','--symbols','--map','--entry='+root,'--keep='+root,'--ro_base=0x00500000','--output='+str(out/'candidate.axf'),'--list='+str(out/'candidate.map'),obj,str(out/'imports.sym')],check=True,env=dict(os.environ,TMP='/tmp'))
```

<!-- script: replay.py -->
```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.arm_const import *
from capstone import *
import struct,json,random,time,hashlib,collections,sys
D=Path('build/street-pass-init');ORIG=Path('data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(ORIG).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
ROOT=0x14414c;END=0x144b3c;B=0x800000;SIZE=0x100000;SP=0xa30000;STOP=0x600000
SELF=B;INFO=B+0x400;ITER=B+0x500;SAVE=B+0x600;RES=B+0x800;DATA=B+0x1000;PLAC=B+0x9000;HEAP=B+0x20000;AUX=B+0x60000;PLAYER=B+0x700
VT=B+0x900;PVT=B+0xa00
EP={'init':0x600100,'appear':0x600104,'dead':0x600108,'gravity':0x60010c,'quat':0x600110,'creator':0x600114}
with (D/'candidate.axf').open('rb') as f:
 e=ELFFile(f);CAND=e.header.e_entry;SEGS=[(s['p_vaddr'],s.data()) for s in e.iter_segments() if s['p_type']=='PT_LOAD']
def w(v):return struct.pack('<I',v&0xffffffff)
def rw(u,a):return struct.unpack('<I',u.mem_read(a,4))[0]
def cs(u,a):return bytes(u.mem_read(a,1024)).split(b'\0')[0].decode('ascii')
def vec(v):return struct.pack('<fff',*v)
def byaml(root):
 keys=set();strings=set()
 def scan(v):
  if isinstance(v,dict):
   for k,x in v.items():keys.add(k);scan(x)
  elif isinstance(v,list):
   for x in v:scan(x)
  elif isinstance(v,str):strings.add(v)
 scan(root);keys=sorted(keys);strings=sorted(strings);buf=bytearray(16)
 def alloc(n):
  buf.extend(bytes((-len(buf))%4));p=len(buf);buf.extend(bytes(n));return p
 def table(items):
  p=alloc(4+4*(len(items)+1));buf[p:p+4]=w((len(items)<<8)|0xc2)
  for i,s in enumerate(items):buf[p+4+4*i:p+8+4*i]=w(len(buf)-p);buf.extend(s.encode()+b'\0')
  buf[p+4+4*len(items):p+8+4*len(items)]=w(len(buf)-p);return p
 k=table(keys);s=table(strings)
 def encode(v):
  if isinstance(v,dict):
   p=alloc(4+8*len(v));buf[p:p+4]=w((len(v)<<8)|0xc1)
   for i,(key,val) in enumerate(sorted(v.items())):
    typ,x=encode(val);buf[p+4+8*i:p+12+8*i]=w((typ<<24)|keys.index(key))+w(x)
   return 0xc1,p
  if isinstance(v,list):
   p=alloc(4+((len(v)+3)&~3)+4*len(v));buf[p:p+4]=w((len(v)<<8)|0xc0);base=p+4+((len(v)+3)&~3)
   for i,val in enumerate(v):typ,x=encode(val);buf[p+4+i]=typ;buf[base+4*i:base+4*i+4]=w(x)
   return 0xc0,p
  if isinstance(v,str):return 0xa0,strings.index(v)
  if v is None:return 0xff,0
  return 0xd1,int(v)&0xffffffff
 _,r=encode(root);buf[:16]=struct.pack('<HHIII',0x4259,1,k,s,r);return bytes(buf),r
IMPORTS={x['address'] for x in json.loads((D/'imports.json').read_text()) if x['kind']=='A'}
MODELS={0x2801e0,0x2932b0,0x2933d0,0x32aefc,0x243260,0x290640,0x26902c,0x12f204,0x26c290,0x16743c,0x1b88b8,0x16f8a4,0x26ccd0,0x268eb0,0x327e90,0x27b51c,0x11c224,0x277e5c,0x280474}
coverage=set();reached=set();executed=set();event_counts=collections.Counter();cases=[];digest=hashlib.sha256();totalsteps=0
REGS=list(range(UC_ARM_REG_R4,UC_ARM_REG_R11+1));CAN=[0xabc01000+i*0x1111 for i in range(8)]
DREGS=list(range(UC_ARM_REG_D8,UC_ARM_REG_D15+1));DCAN=[0x1234567811110000+i*0x1111 for i in range(8)]
class Run:
 def __init__(self,which,cfg):
  self.which=which;self.cfg=cfg;self.events=[];self.calls=[];self.heap=HEAP;self.aux=AUX;self.alloccount=0;self.actors={};self.steps=0;self.done=False;self.spinning=False;self.fault=None;self.writes=[];self.hit=set();self.allocs=[]
  u=self.u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.ctl_set_cpu_model(UC_CPU_ARM_1176);u.mem_map(0x100000,0x600000);u.mem_write(0x100000,ORIG)
  for a,data in SEGS:u.mem_write(a,data)
  u.mem_map(B,SIZE);u.mem_map(0xa00000,0x40000);u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000);u.reg_write(UC_ARM_REG_FPSCR,cfg.get('fpscr',0))
  u.mem_write(B,bytes([0xa5])*SIZE);u.mem_write(SELF,w(VT));u.mem_write(SELF+0x60,bytes(0x18));u.mem_write(INFO,w(ITER)+w(0x11111111)+w(0x22222222)+w(0)+w(0x33333333)+w(0xffffffff));u.mem_write(SAVE+0x60,bytes([cfg.get('selected',0)&255]));u.mem_write(PLAYER,vec(cfg.get('player',(5.,1.,3.))))
  for off,ep in [(4,'init'),(16,'appear'),(24,'dead')]:u.mem_write(VT+off,w(EP[ep]))
  u.mem_write(PVT+0x14,w(EP['gravity']));u.mem_write(PVT+0x34,w(EP['quat']))
  db,dr=byaml(cfg.get('table',[[{'Type':'Coin','Reward':1}]*4]));pb,pr=byaml({'GenerateChildren':cfg.get('placements',[])})
  u.mem_write(DATA,db);u.mem_write(PLAC,pb);u.mem_write(ITER,w(PLAC)+w(PLAC+pr))
  for r,v in zip(REGS,CAN):u.reg_write(r,v)
  for r,v in zip(DREGS,DCAN):u.reg_write(r,v)
  for r,v in [(UC_ARM_REG_R0,SELF),(UC_ARM_REG_R1,INFO),(UC_ARM_REG_SP,SP),(UC_ARM_REG_LR,STOP)]:u.reg_write(r,v)
  u.hook_add(UC_HOOK_CODE,self.hook);u.hook_add(UC_HOOK_MEM_WRITE,self.write);u.hook_add(UC_HOOK_MEM_INVALID,self.bad)
 def ret(self,v=0):self.u.reg_write(UC_ARM_REG_R0,v&0xffffffff);self.u.reg_write(UC_ARM_REG_PC,self.u.reg_read(UC_ARM_REG_LR))
 def write(self,u,access,a,n,v,user):
  assert (B<=a and a+n<=B+SIZE) or (0xa00000<=a and a+n<=0xa40000),('write outside comparison',hex(a),n)
  self.writes.append((a,n))
 def bad(self,u,access,a,n,v,user):self.fault=(access,a,n);return False
 def allocate(self,n):
  self.alloccount+=1;self.allocs.append(n)
  if self.alloccount==self.cfg.get('fail_alloc'):return 0
  assert 0<n<0x10000,('model allocation domain',n)
  p=self.heap;self.heap+=((n+31)&~31)+32;assert self.heap<AUX;return p
 def actor(self,p,kind):
  if not p:return 0
  self.actors[p]=kind;q=self.aux;self.aux+=0x200;u=self.u
  u.mem_write(p,w(VT));u.mem_write(p+0x14,w(q));u.mem_write(q,w(PVT)+vec(self.cfg.get('position',(1.,2.,3.))));u.mem_write(q+0x20,vec(self.cfg.get('gravity',(0.,-1.,0.))));u.mem_write(q+0x30,bytes(16))
  if kind=='tower':
   if self.cfg.get('tower_null'):u.mem_write(p+0x64,w(0))
   else:
    z=q+0x80;u.mem_write(p+0x64,w(z));n=self.cfg.get('tower_count',3);u.mem_write(z+0x28,w(n-1))
    for i in range(max(0,n)):u.mem_write(z+4*i,w(q+0x100+i*16))
  if kind=='tenten':
   z=q+0x80;u.mem_write(p+0x60,w(z));n=self.cfg.get('tenten_count',3);u.mem_write(z+0xc,w(n));u.mem_write(z+0x10,w(z+0x20))
   for i in range(max(0,n)):u.mem_write(z+0x20+4*i,w(q+0x100+i*16))
  return p
 def info(self,p):
  u=self.u;i=rw(u,p);return bytes(u.mem_read(i,8)).hex() if i else None
 def event(self,*args):self.events.append(args)
 def hook(self,u,a,n,user):
  self.steps+=1;self.hit.add(a)
  if self.which==0:
   executed.add(a)
   if ROOT<=a<END:coverage.add(a)
  if a==STOP:self.done=True;u.emu_stop();return
  if self.spinning and a==0x280474:u.reg_write(UC_ARM_REG_PC,a);return
  if a in IMPORTS:self.calls.append(a);reached.add(a)
  if a not in MODELS and a not in EP.values():return
  r=[u.reg_read(x) for x in (UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3)]
  if self.cfg.get('spin') and a==0x280474:self.spinning=True;u.reg_write(UC_ARM_REG_PC,a);return
  if a in (0x2932b0,0x2933d0):
   self.event('alloc',r[0],r[1:3] if a==0x2933d0 else []);self.ret(self.allocate(r[0]));return
  if a==0x32aefc:self.ret(SAVE);return
  if a==0x243260:self.event('resource',cs(u,rw(u,r[0]+4)));self.ret(RES);return
  if a==0x290640:self.event('byaml',r[0],cs(u,rw(u,r[1]+4)));self.ret(DATA if not self.cfg.get('null_byaml') else 0);return
  if a==0x26902c:u.mem_write(r[0],w(0)+w(0));self.event('factory');self.ret(r[0]);return
  if a in (0x12f204,0x26c290,0x16743c,0x1b88b8):
   kind={0x12f204:'tower',0x26c290:'tail',0x16743c:'tenten',0x1b88b8:'indicator'}[a]
   self.event('ctor',kind,r[0],r[1] if kind=='indicator' else cs(u,rw(u,r[1]+4)));self.ret(self.actor(r[0],kind));return
  if a==0x268eb0:
   name=cs(u,r[1]);self.event('creatorLookup',name);self.ret(0 if name in self.cfg.get('missing',[]) else EP['creator']);return
  if a==EP['creator']:
   name=cs(u,r[0]);self.event('create',name)
   if self.cfg.get('null_creator'):self.ret(0)
   else:self.ret(self.actor(self.allocate(0x80),'ordinary'))
   return
  if a==0x26ccd0:self.ret(PLAYER);return
  if a==EP['gravity']:self.ret(r[0]+0x20);return
  if a==EP['quat']:u.mem_write(r[0]+0x30,bytes(u.mem_read(r[1],16)));self.event('quat',r[0],bytes(u.mem_read(r[1],16)).hex());self.ret();return
  if a==0x2801e0:self.event('initArchive',r[0],self.info(r[1]),cs(u,rw(u,r[2]+4)),r[3]);self.ret();return
  if a==0x16f8a4:self.event('tailState',r[0]);self.ret();return
  if a==0x327e90:self.ret(self.cfg.get('enabled',1));return
  if a in (0x27b51c,0x11c224,0x277e5c):self.event(hex(a),r[0],self.info(r[1]),r[2] if a!=0x11c224 else None);self.ret();return
  if a==0x280474:
   self.event('callbacks',r[0],bytes(u.mem_read(r[1],16)).hex(),bytes(u.mem_read(r[2],16)).hex());self.ret(self.cfg.get('registered',0));return
  for k,v in EP.items():
   if a==v:self.event(k,r[0],self.info(r[1]) if k=='init' else None);self.ret();return
  raise AssertionError(('unhandled model',hex(a)))
 def run(self):
  try:self.u.emu_start(ROOT if self.which==0 else CAND,STOP,count=150000)
  except UcError as ex:
   if not self.fault:raise
  if self.u.reg_read(UC_ARM_REG_PC)==STOP:self.done=True
  if self.done:
   assert [self.u.reg_read(r) for r in REGS]==CAN
   assert [self.u.reg_read(r) for r in DREGS]==DCAN
   assert self.u.reg_read(UC_ARM_REG_SP)==SP
  return {'memory':bytes(self.u.mem_read(B,SIZE)),'globals':bytes(self.u.mem_read(0x3e0000,0x70000)),'events':self.events,'calls':self.calls,'allocs':self.allocs,'fpscr':self.u.reg_read(UC_ARM_REG_FPSCR)&0x07c0009f,'fault':self.fault,'done':self.done}
def check(label,cfg):
 global totalsteps
 a=Run(0,cfg);x=a.run();b=Run(1,cfg);y=b.run();totalsteps+=a.steps+b.steps
 # Faulting executions compare fault kind/address/size and observable memory/events.
 for key in x:
  if x[key]!=y[key]:
   if key in ('memory','globals'):
    diffs=[(i,xx,yy) for i,(xx,yy) in enumerate(zip(x[key],y[key])) if xx!=yy]
    print('DIFF',label,cfg,key,diffs[:20])
   else:print('DIFF',label,cfg,key,x[key],y[key])
   raise AssertionError((label,key))
 cases.append({'label':label,'done':x['done'],'fault':x['fault'],'steps':[a.steps,b.steps]});digest.update(hashlib.sha256(x['memory']).digest());event_counts.update(z[0] for z in a.events)
 if len(cases)%50==0:print('passed',len(cases),flush=True)

def placement(name,children=()):return {'name':name,'GenerateChildren':[{'name':x} for x in children]}
def one(typ,reward=1,**kw):return dict(table=[[{'Type':typ,'Reward':reward} for _ in range(4)]],placements=[placement('StreetPassObj1')],**kw)
start=time.time()
base=[('empty',{'table':[],'placements':[]}),('null-data',{'null_byaml':True}),('unrecognized',{'placements':[placement('Other')]}),('missing-type',{'table':[[{'Reward':1}]*4],'placements':[placement('StreetPassObj1')]}),('empty-type',one('')),('missing-creator',one('Unknown',missing=['Unknown']))]
for enabled in [0,1,2,255]:
 for registered in [0,1]:
  for typ in ['Coin','KuriboTower','KuriboTailSearch','TentenGenerator']:
   for reward in [0,1,-1]:
    c=one(typ,reward,enabled=enabled,registered=registered)
    if typ=='TentenGenerator':c['placements']=[placement('StreetPassObj1',['Other',typ])]
    base.append((typ,c))
for label,cfg in base:check(label,cfg)
for n in [0,1,2,9,10,11,16]:
 for typ in ['KuriboTower','TentenGenerator']:
  c=one(typ,tower_count=min(n,10),tenten_count=n)
  if typ=='TentenGenerator':c['placements']=[placement('StreetPassObj1',[typ])]
  check('part-count',c)
check('null-tower-parts',one('KuriboTower',tower_null=True))
for i in range(4):
 c=one('Coin');c['placements']=[placement('Other'),placement('StreetPassObj'+str(i+1),['Other','Coin','Unused'])];check('placement-column',c)
for idx in [-128,-1,0,1,2,127]:
 c=one('Coin',selected=idx);c['table']*=2;check('selected',c)
for row in [[],[None],[{'Type':None,'Reward':0}],[{'Type':8,'Reward':'x'}]]:check('malformed-row',{'table':[row],'placements':[placement('StreetPassObj1')]})
for typ in ['Coin','KuriboTower','KuriboTailSearch','TentenGenerator']:
 c=one(typ)
 if typ=='TentenGenerator':c['placements']=[placement('StreetPassObj1',[typ])]
 for n in range(1,10):check('allocation-failure',dict(c,fail_alloc=n))
for spin in [True]:check('nontermination',one('Coin',spin=spin))
for mode in [0,1<<24,1<<25,1<<22,2<<22,3<<22]:
 for value in [0.,-0.,1e-38,1.,-4.,float('inf'),float('nan')]:check('floating',one('KuriboTailSearch',position=(value,2.,-1.),fpscr=mode))
rng=random.Random(14414)
for z in range(100):
 rows=[[{'Type':rng.choice(['Coin','Unknown','KuriboTower','KuriboTailSearch','TentenGenerator','']),'Reward':rng.randrange(-2,3)} for _ in range(4)] for _ in range(rng.randrange(1,4))]
 placements=[placement('StreetPassObj'+str(rng.randrange(1,5)),rng.choice([[],['Coin'],['Other','TentenGenerator']])) for _ in range(rng.randrange(0,7))]
 check('random',{'table':rows,'selected':rng.randrange(len(rows)),'placements':placements,'enabled':rng.randrange(2),'registered':rng.randrange(2),'missing':['Unknown'],'position':tuple(rng.uniform(-100,100) for _ in range(3)),'player':tuple(rng.uniform(-100,100) for _ in range(3))})
result={'cases':len(cases),'normal':sum(x['done'] for x in cases),'faults':sum(x['fault'] is not None for x in cases),'nontermination':sum(not x['done'] and x['fault'] is None for x in cases),'steps':totalsteps,'seconds':time.time()-start,'coverage':len(coverage),'coverage_addresses':sorted(coverage),'imports_reached':sorted(reached),'models':sorted(MODELS),'executed_addresses':sorted(executed),'event_counts':dict(event_counts),'result_sha256':digest.hexdigest(),'cases_detail':cases}
(D/'replay-result.json').write_text(json.dumps(result,indent=2));print({k:v for k,v in result.items() if k not in ('cases_detail','executed_addresses','coverage_addresses')})
```

<!-- script: preserve.py -->
```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
import hashlib,json,subprocess,time,sys
B=Path(sys.argv[1]).resolve();C=Path.cwd();O=Path('build/street-pass-init');O.mkdir(parents=True,exist_ok=True)
REPORT=B/'build/dot-baseline-54d/report.json';EXPECTED='114120a460faf7dcbca722a5afffc48904e81c052a1598788ad627eaccbf9b82'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
assert sha(REPORT)==EXPECTED
for root in (B,C):
 subprocess.run(['git','diff','--exit-code','HEAD','--','Game','lib','data/config.json','data/ver/eu/map.csv'],cwd=root,check=True,capture_output=True)
report=json.loads(REPORT.read_text());assert report['checkpoint']=='54d39734c4edffeeac012157ab71fb11c40d815b' and report['accepted'] and report['clean_build_returncode']==0
checks=report['prior_checks'];assert len(checks)==751 and len({x['symbol'] for x in checks})==731 and all(x['returncode']==0 for x in checks)
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
  return ret,defs

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
  extra.append(p['source']);defs=canonical(C/p['object'])[1];assert [d[0] for d in defs]==['fn_0014414C'],defs
assert extra==['Game/backup/src/MapObj/StreetPassObjInit14414C.cpp']
result={'baseline_checkpoint':report['checkpoint'],'baseline_report_sha256':EXPECTED,'roots':731,'definitions':751,'prior_objects':len(records),'raw_equal_objects':sum(x['raw_equal'] for x in records),'allocated_equal_objects':len(records),'unchanged_inputs':len(inputs),'extra_sources':extra,'seconds':time.monotonic()-start,'map_sha256':sha(C/'data/ver/eu/map.csv'),'records':records,'inputs':inputs,'scope':'Exhaustive unchanged-input and compiler-object equivalence to the separately verified 731-root/751-definition baseline. This is not a new canonical 751-check run.'}
(O/'preservation-equivalence.json').write_text(json.dumps(result,indent=2));print({k:v for k,v in result.items() if k not in ('records','inputs')})
```

