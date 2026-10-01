# Recreate the Byaml component replay

Use the final committed branch, the owner's verified local EU code/exheader, and the approved ARMCC 4.1/791/wibo toolchain. No downloads or game-data copies are required. Python needs the project's existing pyelftools and Unicorn packages.

First source `development_environment.sh`, set `DEVKITARM=/usr` on Linux, and run `python make.py eu`. The canonical provenance gate requires the C++ inputs to be committed. Save the second Python block below as `build/dot_byaml_components/replay.py`, then execute the first block from the repository root with the project virtual environment. It temporarily enrolls neutral root names for canonical checks and always restores the original map. It will refuse a different starting enrollment state. No checker or function boundaries change.

The replay script contains only generated test data and source logic. It reads the retail executable locally and the candidate AXF files produced by the unchanged project checker. It executes actual retail callees without replacing them. The candidate root overlay exists only in emulator memory.

## Canonical setup and map restoration

```python
from pathlib import Path
import json, subprocess
from tools.low.checkExactBytes import check_exact_bytes
root = Path.cwd()
map_path = root / "data/ver/eu/map.csv"
original_map = map_path.read_bytes()
addresses = ("002253C8", "0025BE14", "00278C6C", "0032B89C")
obj = root / "build/eu/obj/lib/al/src/Placement/alPlacementFunction.o"
try:
    lines = original_map.decode().splitlines(keepends=True)
    for index, line in enumerate(lines):
        if any(line.startswith("0x" + address + ",") for address in addresses):
            fields = line.rstrip("\n").split(",")
            assert fields[4].strip() == "U" and not fields[6].strip()
            fields[6] = "fn_" + fields[0][2:]
            lines[index] = ",".join(fields) + "\n"
    map_path.write_text("".join(lines))
    for address in addresses:
        symbol = "fn_" + address
        checked = subprocess.run(["python", "tools/check.py", symbol, "--object", str(obj)], check=False)
        assert checked.returncode == 1  # all four are deliberately reported NonMatching
        result = check_exact_bytes(symbol, obj, output_directory=root / "build/dot_byaml_components/final" / address)
        assert not result["exact"] and not result.get("rejected")
        assert result["reason"] == "The linked candidate differs from the unchanged original interval."
    subprocess.run(["python", "build/dot_byaml_components/replay.py"], check=True)
finally:
    map_path.write_bytes(original_map)
```

## Replay program

```python
#!/usr/bin/env python3
"""Bounded actual-ARM replay: compiled roots, unchanged retail callees, no stubs."""
import hashlib, itertools, json, struct, sys
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM, UC_HOOK_CODE, UC_HOOK_MEM_WRITE
from unicorn.arm_const import *
ROOT = Path(__file__).resolve().parents[2]
TARGET = (ROOT/'data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(TARGET).hexdigest() == 'e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
BASE, DATA, IT, META, OUT, KEY, STACK, STOP = 0x100000,0x600000,0x610000,0x610100,0x610200,0x610300,0x700000,0x7ff000
ROOTS = (0x2253c8,0x25be14,0x278c6c,0x32b89c)
KEYS = ('Node','PowerUpItemNum','X','Y','Z')
P = lambda n: struct.pack('<I',n&0xffffffff)
def byaml(entries, outer='direct'):
    buf=bytearray(0x500)
    buf[:16]=struct.pack('<HHIII',0x4259,1,0x20,0,0x100)
    table=bytearray(P((len(KEYS)<<8)|0xc2));off=4+4*len(KEYS)
    for key in KEYS:
        table+=P(off);off+=len(key)+1
    table+=b''.join(k.encode()+b'\0' for k in KEYS)
    buf[0x20:0x20+len(table)]=table
    def put_hash(pos, values):
        seq=sorted(values.items(),key=lambda kv:KEYS.index(kv[0]))
        buf[pos:pos+4]=P((len(seq)<<8)|0xc1)
        for i,(key,(typ,value)) in enumerate(seq):
            buf[pos+4+i*8:pos+12+i*8]=P((typ<<24)|KEYS.index(key))+P(value)
    if outer=='direct': put_hash(0x100,entries)
    elif outer=='hash': put_hash(0x100,{'Node':(0xc1,0x200)});put_hash(0x200,entries)
    elif outer=='array': put_hash(0x100,{'Node':(0xc0,0x200)});buf[0x200:0x204]=P(0xc0)
    elif outer=='null': put_hash(0x100,{'Node':(0xff,0)})
    elif outer=='wrong': put_hash(0x100,{'Node':(0xd1,42)})
    elif outer=='missing': put_hash(0x100,{})
    else: raise ValueError(outer)
    return bytes(buf)
def candidate(address):
    path=ROOT/f'build/dot_byaml_components/final/{address:08X}/candidate.axf'
    with path.open('rb') as f:
        e=ELFFile(f);sec=e.get_section_by_name('CANDIDATE_CODE');assert sec['sh_addr']==address
        return sec.data()
CANDIDATES={a:candidate(a) for a in ROOTS}
called=set(); total_instructions=0
class Runner:
    def __init__(self,address,compiled):
        self.address=address;self.uc=Uc(UC_ARCH_ARM,UC_MODE_ARM)
        self.uc.mem_map(BASE,0x400000);self.uc.mem_write(BASE,TARGET)
        self.uc.mem_map(DATA,0x20000);self.uc.mem_map(STACK,0x100000)
        if compiled:self.uc.mem_write(address,CANDIDATES[address])
        self.uc.reg_write(UC_ARM_REG_FPEXC,0x40000000)
        self.uc.hook_add(UC_HOOK_CODE,self.instruction)
        self.uc.hook_add(UC_HOOK_MEM_WRITE,self.write)
    def instruction(self,uc,addr,size,user):
        self.steps+=1
        if 0x100000<=addr<0x400000: called.add(addr)
    def write(self,uc,access,addr,size,value,user):
        if not (STACK<=addr<STACK+0x100000):self.writes.append((addr,size,value&((1<<(size*8))-1)))
    def run(self,blob,iter_mode='valid',metadata_null=False):
        self.steps=0;self.writes=[]
        u=self.uc;u.mem_write(DATA,blob);u.mem_write(IT,P(DATA)+P(DATA+0x100))
        if iter_mode=='invalid':u.mem_write(IT,bytes(8))
        if iter_mode=='null_root':u.mem_write(IT,P(DATA)+P(0))
        u.mem_write(META,P(0xdeadbeef)+P(0 if metadata_null else IT)+P(0xabcdef00))
        u.mem_write(OUT,bytes.fromhex('112233445566778899aabbccddeeff00'));u.mem_write(KEY,b'Node\0')
        u.mem_write(STACK+0xe0000,bytes(0x1000))
        for reg in (UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R6,UC_ARM_REG_R7,UC_ARM_REG_R8,UC_ARM_REG_R9,UC_ARM_REG_R10,UC_ARM_REG_R11,UC_ARM_REG_R12):u.reg_write(reg,0)
        u.reg_write(UC_ARM_REG_SP,STACK+0xe1000);u.reg_write(UC_ARM_REG_LR,STOP)
        u.reg_write(UC_ARM_REG_R0,META if self.address==0x32b89c else OUT)
        u.reg_write(UC_ARM_REG_R1,IT);u.reg_write(UC_ARM_REG_R2,KEY)
        u.emu_start(self.address,STOP,count=100000)
        assert u.reg_read(UC_ARM_REG_PC)==STOP,hex(u.reg_read(UC_ARM_REG_PC))
        assert u.reg_read(UC_ARM_REG_SP)==STACK+0xe1000
        return (u.reg_read(UC_ARM_REG_R0),bytes(u.mem_read(OUT,16)),bytes(u.mem_read(DATA,len(blob))),bytes(u.mem_read(META,12)),tuple(self.writes))
counts={hex(a):0 for a in ROOTS}; digest=hashlib.sha256();checks=0
runners={a:(Runner(a,False),Runner(a,True)) for a in ROOTS}
def check(a,blob,expected_return,expected_out=None,**kwargs):
    global checks,total_instructions
    left,right=runners[a]
    x,y=left.run(blob,**kwargs),right.run(blob,**kwargs)
    # LDM/STM and VFP copies can group writes differently; compare final memory, not bus order.
    assert x[:4]==y[:4],(hex(a),kwargs,x[:2],y[:2])
    assert x[0]==expected_return,(hex(a),kwargs,x[0],expected_return)
    if expected_out is not None:assert x[1][:len(expected_out)]==expected_out,(hex(a),kwargs,x[1],expected_out)
    # Every non-stack write is inside the expected output extent.
    extent=0 if a==0x32b89c else (8 if a==0x278c6c else 12)
    assert all(OUT<=addr and addr+size<=OUT+extent for addr,size,_ in x[4]+y[4])
    digest.update(P(a)+P(x[0])+x[1]+x[2]+x[3]);counts[hex(a)]+=1;checks+=1
    total_instructions+=left.steps+right.steps
# All component state combinations: absent, float, integer, Boolean, string, null.
states=(None,0xd2,0xd1,0xd0,0xa0,0xff)
words=(0x3f800000,0x80000000,0x7fc12345)
for types in itertools.product(states,repeat=3):
    entries={k:(t,v) for k,t,v in zip(('X','Y','Z'),types,words) if t is not None}
    out=b''.join(P(v if t==0xd2 else 0) for t,v in zip(types,words))
    check(0x2253c8,byaml(entries),int(0xd2 in types),out)
    check(0x25be14,byaml(entries,'hash'),int(0xd2 in types),out)
    check(0x278c6c,byaml(entries,'hash'),int(0xd2 in types[:2]),out[:8])
# Float bit preservation, including infinities, signed zero, subnormals and both NaN classes.
bits=(0,0x80000000,1,0x80000001,0x007fffff,0x00800000,0x7f7fffff,0xff7fffff,0x7f800000,0xff800000,0x7fc12345,0x7f812345,0xffc12345)
for v in bits:
    entries={k:(0xd2,v) for k in ('X','Y','Z')}
    for a,outer,n in ((0x2253c8,'direct',3),(0x25be14,'hash',3),(0x278c6c,'hash',2)):check(a,byaml(entries,outer),1,P(v)*n)
# Actual tryGetIterByKey treats a null/array value as a valid iterator. Components then fail and zeros copy.
for a,n in ((0x25be14,3),(0x278c6c,2)):
    for outer in ('null','array'):check(a,byaml({},outer),0,bytes(n*4))
    for outer in ('missing','wrong'):check(a,byaml({},outer),0,bytes.fromhex('112233445566778899aabbcc')[:n*4])
for a in ROOTS[:3]:
    for mode in ('invalid','null_root'):
        expected=bytes(12) if a==0x2253c8 else bytes.fromhex('112233445566778899aabbcc')[:(8 if a==0x278c6c else 12)]
        check(a,byaml({}),0,expected,iter_mode=mode)
# Metadata integer type contract and complete int bit patterns.
for typ in (None,0xd1,0xd2,0xd0,0xa0,0xff,0xc0,0xc1):
    for v in (0,1,2,0xffffffff,0x80000000,0x7fffffff):
        entries={} if typ is None else {'PowerUpItemNum':(typ,v)}
        check(0x32b89c,byaml(entries),v if typ==0xd1 else 2)
check(0x32b89c,byaml({}),2,metadata_null=True)
for mode in ('invalid','null_root'):check(0x32b89c,byaml({}),2,iter_mode=mode)
report={'cases':checks,'paired_executions':2*checks,'counts':counts,'instructions':total_instructions,'result_sha256':digest.hexdigest(),'called_instruction_addresses':len(called),'retail_sha256':hashlib.sha256(TARGET).hexdigest(),'candidate_sha256':{hex(a):hashlib.sha256(b).hexdigest() for a,b in CANDIDATES.items()},'actual_callees_executed':{hex(a):a in called for a in (0x278c30,0x27e068,0x28cab4,0x290fb0,0x2910e8,0x28aa60)},'limitations':'Constructed valid BYAML buffers; original callees execute without replacement. Not arbitrary-buffer safety, original asset loading, or gameplay.'}
(ROOT/'build/dot_byaml_components/replay_result.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
```
