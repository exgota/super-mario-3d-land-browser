# Reproduce the actor scale evidence

Use the final committed `dot/actor-scale-reader` source with the owner's local
verified EU executable/exheader, approved ARMCC4.1/791/wibo, and the project
Python environment. This recipe needs pyelftools and Unicorn2.1.4. No game
binary or assets are included. Work from the repository root.

```sh
. ./development_environment.sh
export DEVKITARM=/usr
python make.py eu -ca
```

Save the second Python block as `build/actor_scale/replay.py`, creating its parent
directory if needed, then execute the first block with the activated Python.
The first block names only the existing target row while preserving bounds,
pool and class. Checker-updated ranks and the diagnostic name are always restored.
It checks every previously accepted symbol defined by the affected object.

## Canonical check and restoration

```python
from pathlib import Path
import json, subprocess, sys
sys.path.insert(0, str(Path.cwd()))
from elftools.elf.elffile import ELFFile
from tools.low.checkExactBytes import check_exact_bytes
from tools.low.buildProvenance import verify_build_output
map_path = Path('data/ver/eu/map.csv')
original_map = map_path.read_bytes()
obj = Path('build/eu/obj/lib/al/src/Placement/alPlacementFunction.o')
report = {'provenance': verify_build_output(obj), 'preservation': {}}
with obj.open('rb') as stream:
    elf = ELFFile(stream)
    definitions = {symbol.name for symbol in elf.get_section_by_name('.symtab').iter_symbols()
                   if isinstance(symbol['st_shndx'], int)}
accepted = []
for line in original_map.decode().splitlines():
    fields = line.split(',')
    if len(fields) > 6 and fields[4].strip() == 'O' and fields[6].strip() in definitions:
        accepted.append(fields[6].strip())
try:
    lines = original_map.decode().splitlines(keepends=True)
    matches = 0
    for index, line in enumerate(lines):
        if line.startswith('0x0024EC80,'):
            fields = line.rstrip('\n').split(',')
            assert fields[1] == '0x0024ED20' and fields[2] == '0x0024ED2C'
            assert fields[4] == 'U' and fields[5] == 'f' and not fields[6]
            fields[6] = 'fn_0024EC80'
            lines[index] = ','.join(fields) + '\n'
            matches += 1
    assert matches == 1
    map_path.write_text(''.join(lines))
    checked = subprocess.run(['python','tools/check.py','fn_0024EC80','--object',str(obj)],
                             text=True,capture_output=True)
    print(checked.stdout, checked.stderr)
    assert checked.returncode == 1
    report['target_stdout'] = checked.stdout
    result = check_exact_bytes('fn_0024EC80', obj,
                 output_directory=Path('build/actor_scale/final/fn_0024EC80'))
    assert not result['exact'] and not result.get('rejected')
    assert result['evidence']['compiled_section_size'] == 172
    assert result['evidence']['different_bytes'] == 4
    report['target'] = result
    for symbol in accepted:
        checked = subprocess.run(['python','tools/check.py',symbol,'--object',str(obj)],
                                 text=True,capture_output=True)
        assert checked.returncode == 0, (symbol, checked.stdout, checked.stderr)
        report['preservation'][symbol] = checked.stdout
    Path('build/actor_scale/final/report.json').write_text(json.dumps(report,indent=2))
    print('Preserved accepted roots:', len(accepted))
finally:
    map_path.write_bytes(original_map)
assert map_path.read_bytes() == original_map
subprocess.run(['python','build/actor_scale/replay.py'], check=True)
```

## Actual-callee replay

The test executes the full original callees. The canonical candidate root is
overlaid only in emulator memory. The executable on disk is never changed.

```python
#!/usr/bin/env python3
"""Compare canonical ARM scale wrapper with retail and execute actual callees."""
import hashlib, itertools, json, struct
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM, UC_HOOK_CODE, UC_HOOK_MEM_WRITE
from unicorn.arm_const import *
ROOT = Path.cwd()
TARGET = (ROOT / 'data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(TARGET).hexdigest() == 'e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
ENTRY = 0x24ec80
BASE, DATA, IT, INFO, OUT, STACK, SP, STOP = 0x100000, 0x600000, 0x610000, 0x610100, 0x610200, 0x700000, 0x7e1000, 0x7ff000
KEYS = ('scale_x', 'scale_y', 'scale_z')
P = lambda n: struct.pack('<I', n & 0xffffffff)
with (ROOT/'build/actor_scale/final/fn_0024EC80/candidate.axf').open('rb') as f:
    e = ELFFile(f); sec = e.get_section_by_name('CANDIDATE_CODE')
    assert sec['sh_addr'] == ENTRY and sec['sh_size'] == 172
    CANDIDATE = sec.data()

def byaml(entries, array=False):
    buf = bytearray(0x500)
    buf[:16] = struct.pack('<HHIII', 0x4259, 1, 0x20, 0, 0x100)
    table = bytearray(P((len(KEYS)<<8) | 0xc2))
    offset = 4 + 4*(len(KEYS)+1)
    for key in KEYS:
        table += P(offset); offset += len(key)+1
    table += P(offset)
    table += b''.join(key.encode()+b'\0' for key in KEYS)
    buf[0x20:0x20+len(table)] = table
    if array:
        buf[0x100:0x104] = P(0xc0)
    else:
        seq = sorted(entries.items(), key=lambda kv:KEYS.index(kv[0]))
        buf[0x100:0x104] = P((len(seq)<<8) | 0xc1)
        for i,(key,(typ,value)) in enumerate(seq):
            buf[0x104+i*8:0x10c+i*8] = P((typ<<24)|KEYS.index(key)) + P(value)
    return bytes(buf)

REGS = [UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_R3,
        UC_ARM_REG_R4, UC_ARM_REG_R5, UC_ARM_REG_R6, UC_ARM_REG_R7,
        UC_ARM_REG_R8, UC_ARM_REG_R9, UC_ARM_REG_R10, UC_ARM_REG_R11,
        UC_ARM_REG_R12]
PRESERVED = REGS[4:12]
OBSERVE = {0x276aa0:1, 0x278c30:3}
CALLEES = (0x276aa0,0x278c30,0x28cab4,0x28aa60)
called=set();steps=0
class Runner:
    def __init__(self, compiled):
        self.u=u=Uc(UC_ARCH_ARM, UC_MODE_ARM)
        u.mem_map(BASE,0x400000);u.mem_write(BASE,TARGET)
        u.mem_map(DATA,0x20000);u.mem_map(STACK,0x100000)
        if compiled:u.mem_write(ENTRY,CANDIDATE)
        u.reg_write(UC_ARM_REG_FPEXC,0x40000000)
        u.hook_add(UC_HOOK_CODE,self.code)
        u.hook_add(UC_HOOK_MEM_WRITE,self.write)
    def code(self,u,address,size,user):
        self.steps+=1
        if address in CALLEES:called.add(address)
        if address in OBSERVE:
            sp=u.reg_read(UC_ARM_REG_SP)
            # Observe real entry SP, ABI arguments and the same caller stack bytes.
            self.calls.append((address,sp,tuple(u.reg_read(r) for r in REGS[:OBSERVE[address]]),bytes(u.mem_read(SP-0x100,0x100))))
    def write(self,u,access,address,size,value,user):
        if not (STACK<=address and address+size<=STACK+0x100000):
            self.writes.append((address,size,value&((1<<(size*8))-1)))
    def run(self,blob,output=OUT,mode='valid',seed=0):
        u=self.u;self.calls=[];self.writes=[];self.steps=0
        u.mem_write(DATA,bytes([0xa5])*0x20000)
        u.mem_write(DATA,blob)
        u.mem_write(IT,P(DATA)+P(DATA+0x100)+P(0xcccccccc)+P(0xdddddddd))
        if mode=='invalid':u.mem_write(IT,bytes(8))
        if mode=='null_root':u.mem_write(IT,P(DATA)+P(0))
        u.mem_write(INFO,P(IT)+P(0x12345678)*5)
        u.mem_write(OUT,bytes.fromhex('112233445566778899aabbccddeeff00'))
        u.mem_write(SP-0x1000,bytes([0x3c ^ seed])*0x1000)
        for i,r in enumerate(REGS):u.reg_write(r,(0x98760000+0x111*i+seed)&0xffffffff)
        preserved=tuple(u.reg_read(r) for r in PRESERVED)
        u.reg_write(UC_ARM_REG_R0,output);u.reg_write(UC_ARM_REG_R1,INFO)
        u.reg_write(UC_ARM_REG_SP,SP);u.reg_write(UC_ARM_REG_LR,STOP)
        before=bytes(u.mem_read(DATA,0x20000))
        u.emu_start(ENTRY,STOP,count=100000)
        assert u.reg_read(UC_ARM_REG_PC)==STOP
        assert u.reg_read(UC_ARM_REG_SP)==SP
        assert tuple(u.reg_read(r) for r in PRESERVED)==preserved
        assert all(output<=a and a+n<=output+12 for a,n,_ in self.writes)
        return (u.reg_read(UC_ARM_REG_R0),before,bytes(u.mem_read(DATA,0x20000)),tuple(self.calls),tuple(self.writes))

left,right=Runner(False),Runner(True)
counts={'type_combinations':0,'float_bits':0,'iterator_states':0,'alias_success':0,'alias_failure':0}
digest=hashlib.sha256();call_observations=0
lower_stack_observations=0;lower_stack_cases=0;first_lower_stack_difference=None

def check(category,entries,success,mode='valid',output=OUT,array=False,seed=0):
    global steps,call_observations,lower_stack_observations,lower_stack_cases,first_lower_stack_difference
    blob=byaml(entries,array)
    a=left.run(blob,output,mode,seed);b=right.run(blob,output,mode,seed)
    assert a[:3]==b[:3] and a[4]==b[4],(category,mode,hex(output),a[0],b[0])
    assert len(a[3])==len(b[3])
    lower_differed=False
    for observation,(retail,compiled) in enumerate(zip(a[3],b[3])):
        assert retail[:3]==compiled[:3]
        # The active caller frame is 32 bytes. Completed callees may leave
        # different R6/R7 spills below it; retain those observer differences.
        assert retail[3][-32:]==compiled[3][-32:]
        different=[i-0x100 for i,(x,y) in enumerate(zip(retail[3],compiled[3])) if x!=y]
        if different:
            assert max(different)<-32
            lower_stack_observations+=1;lower_differed=True
            if first_lower_stack_difference is None:
                first_lower_stack_difference={'category':category,'entries':entries,'call_index':observation,'callee':hex(retail[0]),'offsets_from_entry_sp':different}
    lower_stack_cases+=int(lower_differed)
    assert a[0]==int(success),(category,a[0],success)
    expected=bytearray(a[1])
    if success:
        values=b''.join(P(entries[key][1]) for key in KEYS)
        expected[output-DATA:output-DATA+12]=values
    assert a[2]==bytes(expected)
    # Short circuit: exactly the first failing lookup is reached.
    float_calls=[x for x in a[3] if x[0]==0x278c30]
    want=0
    if mode=='valid':
        for key in KEYS:
            want+=1
            if array or key not in entries or entries[key][0]!=0xd2:break
    elif mode=='null_root':want=1
    assert len(float_calls)==want,(category,mode,len(float_calls),want)
    counts[category]+=1;steps+=left.steps+right.steps;call_observations+=len(a[3])
    digest.update(P(a[0])+a[2])

states=(None,0xd2,0xd1,0xd0,0xa0,0xff)
values=(0x3f800000,0x80000000,0x7fc12345)
for types in itertools.product(states,repeat=3):
    entries={k:(t,v) for k,t,v in zip(KEYS,types,values) if t is not None}
    check('type_combinations',entries,all(t==0xd2 for t in types))
bits=(0,0x80000000,1,0x80000001,0x007fffff,0x00800000,0x7f7fffff,0xff7fffff,0x7f800000,0xff800000,0x7fc12345,0x7f812345,0xffc12345)
for value in bits:check('float_bits',{k:(0xd2,value) for k in KEYS},True)
for mode in ('invalid','null_root'):
    for seed in (0,1,2):check('iterator_states',{k:(0xd2,v) for k,v in zip(KEYS,values)},False,mode=mode,seed=seed)
check('iterator_states',{},False,array=True)
# Raw ARM alias observations, not a general native-C++ strict-aliasing claim.
for output in (IT,IT+4,INFO,INFO+4,DATA,DATA+0x20,DATA+0x100,DATA+0x108,DATA+0x110,OUT+4):
    for seed in (0,1,2):
        check('alias_success',{k:(0xd2,v) for k,v in zip(KEYS,values)},True,output=output,seed=seed)
        check('alias_failure',{'scale_x':(0xd2,values[0]),'scale_y':(0xd1,42)},False,output=output,seed=seed)
assert set(CALLEES)<=called
report={'cases':sum(counts.values()),'paired_executions':2*sum(counts.values()),'counts':counts,'instructions':steps,'call_entry_observations_per_side':call_observations,'lower_stack_observations_differing':lower_stack_observations,'cases_with_lower_stack_differences':lower_stack_cases,'first_lower_stack_difference':first_lower_stack_difference,'result_sha256':digest.hexdigest(),'candidate_sha256':hashlib.sha256(CANDIDATE).hexdigest(),'actual_callees_executed':[hex(a) for a in sorted(called)],'limitations':'Constructed valid BYAML buffers with real retail callees; raw ARM alias tests and caller-stack snapshots. No arbitrary-buffer safety, native-language alias proof, asset loading, hardware, gameplay, or exact-match credit.'}
(ROOT/'build/actor_scale/replay_result.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2))
```
