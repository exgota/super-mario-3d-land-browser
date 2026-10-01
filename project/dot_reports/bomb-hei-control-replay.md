# BombHei control replay

Run this from a checkout containing the proposed source/header. The original EU `code.bin` and `exh.bin` must already be in the ignored `data/ver/eu/` directory. Use the repository's installed ARMCC/wibo toolchain, Python dependencies, ARM binutils, and Unicorn 2.1.4. No source, object, tool, boundary, compiler flag, or binary patch is part of this recipe.

The source/header must be committed before building. The commands below deliberately expect a nonmatching checker result. They temporarily name the existing anonymous root row, then restore the complete original map in a finally block even if checking fails. The map's rank is never committed.

## Canonical build and full-interval check

```sh
. ./development_environment.sh
# Linux override for this dot environment; macOS uses the repository's default.
export DEVKITARM=/usr
python make.py eu -ca
mkdir -p build/bomb_hei_replay
python - <<'PY_CHECK'
from pathlib import Path
import hashlib, json, subprocess, sys

source=Path('Game/backup/src/Enemy/BombHei.cpp')
header=Path('Game/backup/include/Enemy/BombHei.h')
expected={
    source:'916533c6d9bfd7046f3eb417d5d795555c41b58837df7c5ac639641ac4935ff3',
    header:'1f1185d8a879d64b18e265154dd24c469aa7659b6f2a4e5cec12a4245b4d7fc1',
    Path('data/ver/eu/code.bin'):'e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
}
for path,digest in expected.items():
    assert hashlib.sha256(path.read_bytes()).hexdigest()==digest,path
for path in [source,header]:
    assert path.read_bytes()==subprocess.check_output(['git','show','HEAD:'+str(path)])

map_path=Path('data/ver/eu/map.csv')
original=map_path.read_bytes()
rows=original.decode().splitlines(True)
found=0
for i,row in enumerate(rows):
    if row.startswith('0x0030E678,'):
        columns=row.split(',')
        assert columns[2].strip()=='0x0030E938'
        assert columns[1].strip()=='0x0030E8D8'
        assert columns[6].strip() in ['', '_ZN7BombHei7controlEv']
        columns[6]='_ZN7BombHei7controlEv'
        rows[i]=','.join(columns)
        found+=1
assert found==1
try:
    map_path.write_text(''.join(rows))
    command=[sys.executable,'tools/check.py','_ZN7BombHei7controlEv',
             '--object','build/eu/obj/Game/backup/src/Enemy/BombHei.o']
    checked=subprocess.run(command,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True)
    Path('build/bomb_hei_replay/check.txt').write_text(checked.stdout)
    print(checked.stdout,end='')
    assert checked.returncode==1,checked.returncode
    assert 'The linked candidate differs from the unchanged original interval.' in checked.stdout
    diagnostic=subprocess.run([sys.executable,'tools/low/checkExactBytes.py',
        '_ZN7BombHei7controlEv','build/eu/obj/Game/backup/src/Enemy/BombHei.o'],
        stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True)
    Path('build/bomb_hei_replay/exact.json').write_text(diagnostic.stdout)
    assert diagnostic.returncode==1,diagnostic.stdout
    result=json.loads(diagnostic.stdout)
    assert result['exact'] is False and not result.get('rejected',False)
    evidence=result['evidence']
    assert evidence['compiled_section_size']==704
    assert evidence['different_bytes']==49
    assert evidence['object_sha256']=='05d4c6e8b98ec37b133005f8c249daecf2414b49e6e0e1392eec4fe964b1060c'
    assert evidence['linked_section_sha256']=='1adfba5f10cc022d50346ff99a4d5acfe8844795110d0d854f56985b2ed634d1'
    print('704 complete bytes; 49 differing bytes; exact=False')
finally:
    map_path.write_bytes(original)
assert map_path.read_bytes()==original
print('Map restored byte-for-byte')
PY_CHECK
```

The checker validates committed-source provenance and constructs the candidate at the original address using only existing import rows. This is a canonical nonmatch, not a diagnostic exactness claim.

## Execute bounded original-instruction replay

This script consumes the unchanged checker-linked candidate from the preceding step. The original/candidate data are loaded into separate emulator memories for execution; repository game data and canonical objects stay unchanged. Arithmetic-slice cases execute real original/candidate instructions with no external calls. Whole-root cases execute all root instructions but replace all nine external targets with the explicitly defined models. These scopes must remain separate when reporting results.

```sh
cat > build/bomb_hei_replay/verify_math.py <<'PY_REPLAY'
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM, UC_HOOK_CODE
from unicorn.arm_const import *
import json, random, struct

root = Path.cwd()
result = json.loads(Path('build/bomb_hei_replay/exact.json').read_text())
with open(Path(result['evidence']['output_directory']) / 'candidate.axf','rb') as stream:
    candidate = ELFFile(stream).get_section_by_name('CANDIDATE_CODE').data()
retail = (root/'data/ver/eu/code.bin').read_bytes()
def machine(replacement=None):
    u=Uc(UC_ARCH_ARM, UC_MODE_ARM)
    u.mem_map(0x100000,0x400000)
    u.mem_write(0x100000,retail)
    if replacement is not None: u.mem_write(0x30e678,replacement)
    u.mem_map(0x600000,0x10000)
    u.reg_write(UC_ARM_REG_C1_C0_2,0x00f00000)
    u.reg_write(UC_ARM_REG_FPEXC,0x40000000)
    return u
machines=[machine(),machine(candidate)]
saved_regs = [UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11]
saved_vfp = [globals()['UC_ARM_REG_S'+str(i)] for i in range(16,32)]
def seed_saved(u):
    for i,reg in enumerate(saved_regs+saved_vfp):u.reg_write(reg,0x12340000+i)
def snapshot_saved(u):
    return [u.reg_read(reg) for reg in saved_regs+saved_vfp+[UC_ARM_REG_SP]]

def run_rotation(u,mem,offset,fpscr):
    seed_saved(u)
    u.mem_write(0x600000,mem)
    u.reg_write(UC_ARM_REG_SP,0x600040)
    u.reg_write(UC_ARM_REG_R0,0x600040+offset)
    u.reg_write(UC_ARM_REG_FPSCR,fpscr)
    before=snapshot_saved(u)
    u.emu_start(0x30e6a8,0x30e73c,count=100)
    assert u.reg_read(UC_ARM_REG_PC)==0x30e73c
    assert snapshot_saved(u)==before
    after=bytes(u.mem_read(0x600000,len(mem)))
    out=0x40+offset
    assert after[:out]==mem[:out] and after[out+12:]==mem[out+12:]
    return bytes(u.mem_read(0x600000,len(mem))),u.reg_read(UC_ARM_REG_FPSCR)

edges=[0,0x80000000,1,0x80000001,0x7fffff,0x807fffff,0x800000,0x80800000,
       0x3f800000,0xbf800000,0x40600000,0x7f7fffff,0xff7fffff,0x7f800000,
       0xff800000,0x7fc00000,0xffc00000,0x7f800001,0xff800001,0x7fffffff,0xffffffff]
rng=random.Random(0x30e678)
rotation_cases=0
for fpscr in [0,0x01000000,0x02000000,0x03000000]:
  for offset in [-12,-8,-4,0,4,8,12,16,64]:
    for case in range(128):
      words=[rng.choice(edges) if case<64 else rng.getrandbits(32) for _ in range(64)]
      mem=struct.pack('<64I',*words)
      a,b=[run_rotation(u,mem,offset,fpscr) for u in machines]
      if a!=b:raise AssertionError(('rotation',fpscr,offset,case))
      rotation_cases+=1

def run_tail(u,word,fpscr):
    seed_saved(u)
    u.mem_write(0x600088,struct.pack('<I',word))
    u.reg_write(UC_ARM_REG_R4,0x600000)
    u.reg_write(UC_ARM_REG_FPSCR,fpscr)
    before=snapshot_saved(u)
    u.emu_start(0x30e8a8,0x30e8c8,count=20)
    assert u.reg_read(UC_ARM_REG_PC)==0x30e8c8
    assert snapshot_saved(u)==before
    return struct.unpack('<I',u.mem_read(0x600088,4))[0],u.reg_read(UC_ARM_REG_FPSCR)
threshold_cases=0
negative_nan=[]
thresholds=edges+[0x405fffff,0x40600001,0x40400000,0xc0600000]
thresholds+=[rng.getrandbits(32) for _ in range(2048)]
for fpscr in [0,0x01000000,0x02000000,0x03000000]:
  for word in thresholds:
    a,b=[run_tail(u,word,fpscr) for u in machines]
    if a!=b:raise AssertionError(('threshold',fpscr,hex(word),a,b))
    signed=word if word<0x80000000 else word-0x100000000
    if signed<0x40600000:
      assert a[0]==0,hex(word)
    if word in [0xffc00000,0xff800001,0xffffffff]:negative_nan.append([hex(fpscr),hex(word),hex(a[0])])
    threshold_cases+=1
summary={'rotation_cases':rotation_cases,'threshold_cases':threshold_cases,'fpscr_modes':['0','0x01000000','0x02000000','0x03000000'],'rotation_alias_offsets':[-12,-8,-4,0,4,8,12,16,64],'memory_and_fpscr_mismatches':0,'negative_nan_outputs':negative_nan}

# Execute the complete control root with deterministic, explicitly modeled calls.
# This validates root control flow and call ABI under those models; it does not
# validate the real imported routines or an in-game actor lifecycle.
ACTOR=0x601000
FRONT=0x602000
STACK=0x608000
STOP=0x700000
imports={0x279ed4,0x337474,0x27c04c,0x24e9f8,0x271330,0x271300,0x2806a8,0x280610,0x271294}
contexts={}
def word(u,p):return struct.unpack('<I',u.mem_read(p,4))[0]
def cstring(u,p):
    out=bytearray()
    while len(out)<100:
        ch=u.mem_read(p+len(out),1)[0]
        if not ch:return out.decode('ascii')
        out.append(ch)
    raise AssertionError('unterminated model string')
def call_model(u,address,size,ctx):
    if 0x30e678<=address<0x30e8d8:
        ctx['coverage'].add(address)
        return
    if address not in imports:raise AssertionError(('unexpected external code',hex(address)))
    r0,r1,r2=[u.reg_read(reg) for reg in [UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2]]
    if address==0x279ed4:
        ctx['trace'].append(['gate',r0,r1]);u.reg_write(UC_ARM_REG_R0,ctx['gate'])
    elif address==0x337474:
        ctx['trace'].append(['quaternion',r0]);u.mem_write(r1,ctx['q'])
    elif address==0x27c04c:
        ctx['trace'].append(['front',r0]);u.reg_write(UC_ARM_REG_R0,FRONT)
    elif address==0x24e9f8:
        ctx['trace'].append(['audio',r0,cstring(u,word(u,r1+4)),r2])
    elif address==0x271330:
        ctx['trace'].append(['action',r0,cstring(u,r1)])
    elif address==0x271300:
        ctx['trace'].append(['vector_float',r0,u.reg_read(UC_ARM_REG_S0),bytes(u.mem_read(r0,12)).hex()])
    elif address==0x2806a8:
        ctx['trace'].append(['is_nerve',r0,r1]);u.reg_write(UC_ARM_REG_R0,int(ctx['nerve']==r1))
    elif address==0x280610:
        ctx['trace'].append(['set_nerve',r0,r1]);ctx['nerve']=r1
    elif address==0x271294:
        ctx['trace'].append(['final',r0]);u.reg_write(UC_ARM_REG_R0,1)
    u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
for u in machines:
    u.mem_map(STOP,0x1000)
    ctx={'trace':[],'coverage':set()};contexts[id(u)]=ctx
    u.hook_add(UC_HOOK_CODE,call_model,ctx)

def control(u,actor,front,q,gate,nerve,fpscr):
    ctx=contexts[id(u)];ctx.update(trace=[],q=q,gate=gate,nerve=nerve)
    u.mem_write(ACTOR,actor);u.mem_write(FRONT,front)
    u.mem_write(STACK-0x100,b'\xa5'*0x200)
    seed_saved(u)
    u.reg_write(UC_ARM_REG_SP,STACK);u.reg_write(UC_ARM_REG_LR,STOP)
    u.reg_write(UC_ARM_REG_R0,ACTOR);u.reg_write(UC_ARM_REG_FPSCR,fpscr)
    before=snapshot_saved(u)
    u.emu_start(0x30e678,STOP,count=2000)
    assert u.reg_read(UC_ARM_REG_PC)==STOP
    assert snapshot_saved(u)==before
    assert bytes(u.mem_read(STACK-0x100,0xe0))==b'\xa5'*0xe0
    assert bytes(u.mem_read(STACK,0x100))==b'\xa5'*0x100
    return bytes(u.mem_read(ACTOR,len(actor))),bytes(u.mem_read(FRONT,12)),ctx['trace'],ctx['nerve'],u.reg_read(UC_ARM_REG_FPSCR)

control_cases=0
for fpscr in [0,0x03000000]:
 for gate in [0,1]:
  for fuse in [0,1]:
   for t70 in [-1,0,1,2]:
    for t74 in [-1,0,1,2,89,90,91]:
     for nerve in [0,0x3f1e74,0x3f1e78,0x3f1e7c,0x3f1e80,0x3f1e84,0x3f1e88,0x3f1e8c]:
      actor=bytearray(b'\x5a'*0x8c)
      struct.pack_into('<ii',actor,0x70,t70,t74)
      actor[0x84]=fuse
      struct.pack_into('<I',actor,0x88,rng.choice(thresholds))
      front=struct.pack('<3I',*[rng.choice(edges) for _ in range(3)])
      q=struct.pack('<4I',*[rng.choice(edges) for _ in range(4)])
      a,b=[control(u,bytes(actor),front,q,gate,nerve,fpscr) for u in machines]
      if a!=b:raise AssertionError(('control',fpscr,gate,fuse,t70,t74,nerve))
      after=a[0]
      assert struct.unpack_from('<i',after,0x70)[0]==(t70-1 if t70>0 else t70)
      assert struct.unpack_from('<i',after,0x74)[0]==(t74-1 if fuse else t74)
      if fuse and t74<=1:
          assert after[0x84]==0
          if nerve not in [0x3f1e88,0x3f1e8c]:
              assert a[3]==0x3f1e8c and a[2][-1][0]=='set_nerve'
      control_cases+=1
coverage=contexts[id(machines[0])]['coverage']
summary.update(control_cases=control_cases,control_models='All nine external targets modeled; no real callee execution',control_memory_trace_fpscr_mismatches=0,callee_saved_integer_vfp_and_sp='preserved in every arithmetic and control case',control_visited_instruction_addresses=len(coverage),control_unvisited_instruction_addresses=[hex(x) for x in range(0x30e678,0x30e8d8,4) if x not in coverage])
print(json.dumps(summary,indent=2))
PY_REPLAY
python build/bomb_hei_replay/verify_math.py > build/bomb_hei_replay/replay_result.json
cat build/bomb_hei_replay/replay_result.json
```

Expected counts: 4,608 quaternion slices, 8,292 threshold slices, and 1,792 whole-root cases with modeled callees. Both mismatch counts are zero; SP, r4–r11 and s16–s31 are preserved. Whole-root instruction-address coverage is 152 with an empty unvisited-address list. The report describes the bounded input corpus and unverified real-helper/gameplay behavior.

## Inspect preservation and local status

```sh
python - <<'PY_INVENTORY'
import csv
from elftools.elf.elffile import ELFFile
with open('data/ver/eu/map.csv') as stream:
    accepted={row['Symbol'].strip() for row in csv.DictReader(stream) if row['Rank'].strip()=='O'}
with open('build/eu/obj/Game/backup/src/Enemy/BombHei.o','rb') as stream:
    elf=ELFFile(stream)
    functions=[symbol.name for symbol in elf.get_section_by_name('.symtab').iter_symbols()
               if symbol['st_info']['type']=='STT_FUNC' and isinstance(symbol['st_shndx'],int)]
assert not (set(functions)&accepted)
print('Zero already accepted definitions in the new object')
PY_INVENTORY
rg -n 'BombHei.h' Game lib
git status --short
```

Only the new source includes this class header. No prior accepted-function recheck is claimed or substituted with this inventory. The two source/header files and two Markdown notes are the complete proposed change.
