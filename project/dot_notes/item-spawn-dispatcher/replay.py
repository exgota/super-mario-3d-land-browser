#!/usr/bin/env python3
"""Whole-root ARM replay. Original direct callees run unchanged in both sides.
Only the explicitly listed engine environment and terminal virtual calls are modeled.
This is bounded functional evidence, never a matching oracle.
"""
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import Uc, UcError, UC_ARCH_ARM, UC_MODE_ARM, UC_HOOK_CODE, UC_HOOK_MEM_INVALID
from unicorn.arm_const import *
import struct, json, hashlib, time, random, collections, sys
D=Path('build/item-spawn-dispatcher');ORIG=Path('data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(ORIG).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
ROOT=0x2cdea4;END=0x2ce700;B=0x800000;SIZE=0x40000;SP=0x930000;STOP=0x600000
SOURCE=B;POSITION=B+0x100;ORIENTATION=B+0x120;HOLDER=B+0x200;SCENE=B+0x300;SCENES=B+0x400;SYSTEM=B+0x500;PLAYER=B+0x600;PS1=B+0x700;PS2=B+0x800
POSE=B+0xa00;PVT=B+0xb00;AVT=B+0xc00;IVT=B+0xd00;POOLS=B+0x1000;ITEMS=B+0x20000
EP={'gravity':STOP+4,'pose_quat':STOP+8,'calc_mtx':STOP+12,'actor_appear':STOP+16,'make_appeared':STOP+20,'item_appear':STOP+24,'item_alternate':STOP+28,'base_mtx':STOP+32}
# holder field, pending active count, cycle field. All independently read from the original helpers.
POOLSPECS={0x225e70:(0x48,0x18,0xa34),0x225f68:(0x30,0x10,0xa2c),0x226060:(0x2c,0x10,0xa2c),0x226158:(0x1c,0x14,0xa30),0x22624c:(0x18,0x1c,0xa38),0x226344:(0x3c,0x1c,0xa38),0x22643c:(0x28,0x1c,0xa38),0x226534:(0x34,0x1c,0xa38),0x22662c:(0x38,0x1c,0xa38),0x226724:(0xc,0x1c,0xa38),0x22681c:(0x24,0x14,0xa30),0x226910:(0x14,0x14,0xa30),0x227490:(0x20,0x1c,0xa38),0x227588:(0x10,0x34,0xa50),0x2276d8:(8,0x30,0xa4c),0x22804c:(4,0x5c,0xa78)}
MODELS={0x28e688:'game-system provider',0x272a0c:'random-vector provider',0x27f11c:'scene-area predicate',0x271110:'coin sound/effect routing',0x280610:'nerve transition',0x31a9cc:'coin launch endpoint',0x227f8c:'coin launch endpoint'}
with (D/'candidate.axf').open('rb') as f:
    elf=ELFFile(f);CAND=elf.header.e_entry;SEGS=[(s['p_vaddr'],s.data()) for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
IMPORTS={i['address']:i['symbol'] for i in json.loads((D/'imports.json').read_text())}
def w(x):return struct.pack('<I',x&0xffffffff)
def vec(v):return struct.pack('<'+'f'*len(v),*v)
def rw(u,a):return struct.unpack('<I',u.mem_read(a,4))[0]
def hexmem(u,a,n):return bytes(u.mem_read(a,n)).hex()
def run(candidate, case):
    u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0x100000,0x400000);u.mem_write(0x100000,ORIG)
    u.mem_map(0x500000,0x10000);u.mem_map(STOP,0x1000);u.mem_map(B,SIZE);u.mem_map(0x900000,0x40000)
    for a,data in SEGS:u.mem_write(a,data)
    u.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);u.reg_write(UC_ARM_REG_FPEXC,0x40000000)
    def put(a,x):u.mem_write(a,w(x))
    def putvec(a,v):u.mem_write(a,vec(v))
    put(SYSTEM+0x50,SCENE);put(SYSTEM+0x5c,PLAYER);put(SCENE+4,SCENES);put(SCENES+40,0 if case.get('null_holder') else HOLDER)
    put(PLAYER+0x74,PS1);put(PS1+0x40,PS2);put(PS2,case.get('power',0))
    put(SOURCE+0x14,0 if case.get('null_pose') else POSE);put(POSE,PVT);putvec(POSE+4,case.get('origin',(1,2,3)));putvec(POSE+0x30,case.get('gravity',(0,-1,0)))
    put(PVT+0x14,EP['gravity']);put(PVT+0x34,EP['pose_quat']);put(PVT+0x40,EP['calc_mtx'])
    put(AVT+0x3c,EP['base_mtx']);put(AVT+0xc,EP['actor_appear']);put(AVT+0x10,EP['make_appeared']);put(IVT,EP['item_appear']);put(IVT+8,EP['item_alternate'])
    putvec(POSITION,case.get('position',(13,-7,29)));putvec(ORIENTATION,case.get('orientation',(0,0,0,1)))
    u.mem_write(HOLDER+0x50,bytes(case.get('flags',(1,0,0))))
    for index,(addr,(field,countoff,cycleoff)) in enumerate(POOLSPECS.items()):
        pool=POOLS+index*0x1000;item=ITEMS+index*0x1000;put(HOLDER+field,pool)
        put(pool,2);put(pool+4,2);put(pool+8,pool+0xc00)
        put(pool+0xc00,item);put(pool+0xc04,item+0x200)
        put(pool+countoff,0 if case.get('missing_record') else 2);put(pool+countoff+4,2);put(pool+countoff+8,pool+0xc20)
        put(pool+0xc20,pool+0xc40);put(pool+0xc24,pool+0xc60)
        put(pool+0xc40,SOURCE+0x80);put(pool+0xc60,SOURCE);put(pool+0xc48,3);put(pool+0xc68,case.get('remaining',20));put(pool+cycleoff,case.get('cycle',0))
        for n,flag in enumerate(case.get('available',(1,1))):
            obj=item+n*0x200;put(obj,AVT);put(obj+0x14,obj+0x100);put(obj+0x100,PVT);u.mem_write(obj+0x54,bytes([flag]));
            put(obj+(0x64 if addr in (0x226158,0x22624c,0x226910,0x227588) else 0x60),IVT)
    if case.get('alias_position'):position=POSE+4
    else:position=POSITION
    if case.get('null_position'):position=0
    orientation=0 if case.get('null_orientation') else ORIENTATION
    for reg,value in zip((UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3),(case['kind'],SOURCE,position,orientation)):u.reg_write(reg,value)
    u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,STOP)
    entry=CAND if candidate else ROOT
    model_fault=[];trace=[];imports=collections.Counter();original=collections.Counter();models=collections.Counter();fault=[];steps=0;stopped=False;randomcalls=0
    def readmem(a,n):
        try:return bytes(u.mem_read(a,n))
        except UcError:
            model_fault.append(['read',a,n]);raise
    def hx(a,n):return readmem(a,n).hex()
    def regs():return [u.reg_read(x) for x in (UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3)]
    def ret(value=None):
        if value is not None:u.reg_write(UC_ARM_REG_R0,value)
        u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
    def code(uc,a,size,user):
        nonlocal steps,stopped,randomcalls
        steps+=1
        if a==STOP:stopped=True;u.emu_stop();return
        if a in IMPORTS:imports[a]+=1
        if 0x100000<=a<0x500000 and not(ROOT<=a<END):original[a]+=1
        r0,r1,r2,r3=regs()
        if a in EP.values():
            name=next(k for k,v in EP.items() if v==a);models[a]+=1
            if name=='gravity':ret(r0+0x30)
            elif name=='base_mtx':ret(0)
            elif name=='pose_quat':trace.append([name,r0,hx(r1,16)]);u.mem_write(r0+0x40,readmem(r1,16));ret()
            elif name=='calc_mtx':u.mem_write(r1,vec((1,0,0,0,0,1,0,0,0,0,1,0)));ret()
            elif name in ('item_appear','item_alternate'):
                trace.append([name,r0,hx(r1,12),hx(r2,16)]);ret()
            else:trace.append([name,r0]);ret()
        elif a in MODELS:
            models[a]+=1
            if a==0x28e688:ret(SYSTEM)
            elif a==0x272a0c:
                trace.append(['random',u.reg_read(UC_ARM_REG_S0)])
                n=randomcalls;randomcalls+=1
                putvec(r0,((n%3-1)*0.125,(n%5-2)*0.125,(n%7-3)*0.125));ret()
            elif a==0x27f11c:ret(case.get('area',0))
            elif a==0x271110:trace.append(['coin_effect']);ret()
            elif a==0x280610:trace.append(['nerve',r0,r1]);ret()
            else:trace.append(['launch',a,r0,hx(r1,12),hx(r2,12),r3]);ret()
    def invalid(uc,access,a,size,value,user):fault.append([access,a,size]);return False
    u.hook_add(UC_HOOK_CODE,code);u.hook_add(UC_HOOK_MEM_INVALID,invalid)
    error=None
    try:u.emu_start(entry,STOP+0x100,count=300000)
    except (UcError,ValueError) as ex:error=str(ex)
    memory=bytes(u.mem_read(B,SIZE))
    return {'stopped':stopped,'error':error,'fault':fault,'model_fault':model_fault,'trace':trace,'memory_sha256':hashlib.sha256(memory).hexdigest(),'imports':dict(imports),'models':dict(models),'original_instructions':len(original),'steps':steps}

def fixtures():
    result=[]
    for kind in list(range(30))+[30,31,0xffffffff,0x80000000]:
        for flags in [(0,0,0),(1,0,0),(1,1,0),(1,0,1),(1,1,1)]:
            for power in (0,1,2):result.append(dict(kind=kind,flags=flags,power=power))
    for kind in range(30):
        for tweak in [dict(available=(0,1)),dict(available=(0,0)),dict(available=(255,0)),dict(missing_record=True),dict(remaining=0),dict(cycle=1),dict(alias_position=True),dict(null_pose=True),dict(null_holder=True),dict(null_position=True),dict(null_orientation=True)]:
            result.append(dict(kind=kind,flags=(1,1,1),**tweak))
    rng=random.Random(0x2cdea4)
    for i in range(120):
        result.append(dict(kind=rng.choice([3,5,6,7,8,9,10,11]),flags=(1,1,1),area=i%2,
            gravity=tuple(rng.uniform(-2,2) for _ in range(3)),position=tuple(rng.uniform(-100,100) for _ in range(3)),
            orientation=tuple(rng.uniform(-1,1) for _ in range(4))))
    return result
start=time.time();cases=fixtures()
if len(sys.argv)>1:cases=cases[:int(sys.argv[1])]
results=[];covered=collections.Counter();modeled=collections.Counter();divergences=[]
for index,c in enumerate(cases):
    original=run(False,c);candidate=run(True,c)
    keys=['stopped','error','fault','model_fault','trace','memory_sha256','imports','models']
    unequal=[k for k in keys if original[k]!=candidate[k]]
    covered.update(original['imports']);modeled.update(original['models'])
    result={'index':index,'fixture':c,'equal':not unequal,'differences':unequal,'original':original,'candidate':candidate}
    results.append(result)
    if unequal:divergences.append(index)
report={'cases':len(results),'equal':len(results)-len(divergences),'divergences':divergences,'seconds':time.time()-start,'complete_returns':sum(x['original']['stopped'] for x in results),'fault_cases':sum(bool(x['original']['fault']) for x in results),'model_rejection_cases':sum(bool(x['original']['model_fault']) for x in results),'covered_imports':{hex(k):v for k,v in covered.items()},'uncovered_imports':{hex(k):v for k,v in IMPORTS.items() if k not in covered},'models':{hex(k):v for k,v in modeled.items()},'model_descriptions':{**{hex(k):v for k,v in MODELS.items()},**{hex(v):k for k,v in EP.items()}},'results':results}
(D/'replay-results.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k!='results'},indent=2))
if divergences: print(json.dumps(results[divergences[0]],indent=2));sys.exit(1)
