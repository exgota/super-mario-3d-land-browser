# Matrix33 inverse bounded replay

This diagnostic runs committed-source canonical ARMCC output against the original local EU interval. It is separate from the exact checker and grants no matching credit. Use the already installed Unicorn 2.1.4 and pyelftools. It reads no network resources and embeds no game bytes.

The final source is in `lib/al/src/Math/seadMatrixCopy.cpp`. Extract the Python block below into ignored `build/matrix_inverse_replay/replay.py`. From the repository root, source the environment and use the normal project build. On the dot Linux host, set `DEVKITARM=/usr` after sourcing `development_environment.sh`.

```sh
. ./development_environment.sh
export DEVKITARM=/usr
python make.py eu -ca
python tools/check.py _ZN4sead15Matrix33CalcCtrIfE7inverseERN2nn4math5MTX33ERKS4_ --object build/eu/obj/lib/al/src/Math/seadMatrixCopy.o
python build/matrix_inverse_replay/replay.py build/eu/obj/lib/al/src/Math/seadMatrixCopy.o build/matrix_inverse_replay/result.json
```

The exact check is expected to exit 1 because 404 bytes exceed 260. Run checks sequentially: the CLI changes local map ranks, which must not be committed from this lane. Replay reads every provenance input and refuses changed source/header/build inputs. To reproduce packet form 1, use an isolated worktree at `2cd498e`, build there, and run this same harness; its complete section is 308 bytes and its behavior differences are recorded in the companion report. The final tree deliberately retains the representation-preserving fourth form.

Comparisons include all data/sentinel bytes and FPSCR; NZCV is additionally counted separately. Same-object input/output is included. Partial overlaps, enabled floating exceptions, physical hardware, and gameplay are outside this check. The canonical object's R0 is compared as an observed register effect, not a recovered public return type.

```python
from pathlib import Path
import hashlib, json, random, struct, sys
from collections import Counter
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM
from unicorn.arm_const import *
from elftools.elf.elffile import ELFFile

ROOT = Path.cwd()
OBJ = Path(sys.argv[1])
OUT = Path(sys.argv[2])
SYMBOL = '_ZN4sead15Matrix33CalcCtrIfE7inverseERN2nn4math5MTX33ERKS4_'
START, END = 0x27c23c, 0x27c340
EXPECTED = 'e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
record = json.loads(OBJ.with_suffix('.provenance.json').read_text())
assert hashlib.sha256(OBJ.read_bytes()).hexdigest() == record['object_sha256']
for name, sha in record['inputs'].items():
    assert hashlib.sha256((ROOT / name).read_bytes()).hexdigest() == sha, name
binary = (ROOT / 'data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(binary).hexdigest() == EXPECTED
retail = binary[START - 0x100000:END - 0x100000]
with OBJ.open('rb') as stream:
    elf = ELFFile(stream)
    section = elf.get_section_by_name('i.' + SYMBOL)
    candidate = section.data()
    section_index = elf.get_section_index(section.name)
    assert not any(s.name.startswith('.rel') and s['sh_info'] == section_index and s['sh_size']
                   for s in elf.iter_sections()), 'Unexpected relocation'

DATA, STACK, STOP = 0x1000000, 0x2000000, 0x71000000
class Engine:
    def __init__(self, code):
        self.u = u = Uc(UC_ARCH_ARM, UC_MODE_ARM)
        u.ctl_set_cpu_model(UC_CPU_ARM_1176)
        u.mem_map(START & ~4095, 4096)
        u.mem_write(START, code)
        u.mem_map(DATA, 4096)
        u.mem_map(STACK, 4096)
        u.mem_map(STOP, 4096)
        u.reg_write(UC_ARM_REG_C1_C0_2, 0xf << 20)
        u.reg_write(UC_ARM_REG_FPEXC, 1 << 30)

    def run(self, words, mode, inplace):
        u = self.u
        u.mem_write(DATA, b'\xa5' * 1024)
        u.mem_write(STACK, b'\x6b' * 4096)
        source = DATA + 256
        output = source if inplace else DATA + 768
        u.mem_write(source, struct.pack('<9I', *words))
        u.reg_write(UC_ARM_REG_CPSR, 0x10)
        u.reg_write(UC_ARM_REG_R0, output)
        u.reg_write(UC_ARM_REG_R1, source)
        u.reg_write(UC_ARM_REG_SP, STACK + 2048)
        u.reg_write(UC_ARM_REG_LR, STOP)
        u.reg_write(UC_ARM_REG_FPSCR, mode)
        for i in range(4, 12):
            u.reg_write(UC_ARM_REG_R0 + i, 0xab000000 + i)
        for i in range(16, 32):
            u.reg_write(UC_ARM_REG_S0 + i, 0x3f800000 + i)
        u.emu_start(START, STOP, count=200)
        assert u.reg_read(UC_ARM_REG_PC) == STOP
        assert u.reg_read(UC_ARM_REG_SP) == STACK + 2048
        assert all(u.reg_read(UC_ARM_REG_R0 + i) == 0xab000000 + i for i in range(4, 12))
        assert all(u.reg_read(UC_ARM_REG_S0 + i) == 0x3f800000 + i for i in range(16, 32))
        assert bytes(u.mem_read(STACK, 1984)) == b'\x6b' * 1984
        assert bytes(u.mem_read(STACK + 2048, 2048)) == b'\x6b' * 2048
        return (bytes(u.mem_read(DATA, 1024)), u.reg_read(UC_ARM_REG_FPSCR),
                struct.unpack('<9I', u.mem_read(output, 36)), u.reg_read(UC_ARM_REG_R0))

def bits(value):
    return struct.unpack('<I', struct.pack('<f', value))[0]

identity = (0x3f800000, 0, 0, 0, 0x3f800000, 0, 0, 0, 0x3f800000)
cases = []
def add(group, name, values, raw=False):
    cases.append((group, name, tuple(values if raw else map(bits, values))))

add('named', 'identity', identity, True)
add('named', 'negative_identity', [-1,0,0,0,-1,0,0,0,-1])
add('named', 'zero', [0] * 9)
add('named', 'negative_zero', [0x80000000] * 9, True)
add('named', 'singular_duplicate_rows', [1,2,3,1,2,3,4,5,6])
add('named', 'upper_triangular', [1,2,3,0,4,5,0,0,6])
add('named', 'dense', [1,2,3,0,1,4,5,6,0])
add('named', 'reflection', [-1,0,0,0,1,0,0,0,1])
add('named', 'tiny_determinant', [1,0,0,0,1,0,0,0,1e-38])
special = [0, 0x80000000, 0x3f800000, 0xbf800000, 0x40000000,
           0x7f800000, 0xff800000, 0x7fc00001, 0xffc12345,
           0x7f800001, 0xff800001, 1, 0x80000001, 0x007fffff,
           0x807fffff, 0x00800000, 0x80800000, 0x7f7fffff, 0xff7fffff]
for value in special:
    for pos in range(9):
        words = list(identity)
        words[pos] = value
        add('special', 'identity_%08x_at_%d' % (value, pos), words, True)

# All zero signs around the diagonal stress negative-product and branch semantics.
for mask in range(512):
    words = [0x80000000 if (mask >> i) & 1 else 0 for i in range(9)]
    for i in (0, 4, 8):
        words[i] |= 0x3f800000
    add('diagonal_signs', 'signs_%03x' % mask, words, True)

rng = random.Random(START)
for i in range(256):
    add('finite', 'finite_%d' % i, [rng.uniform(-8,8) for _ in range(9)])
for i in range(256):
    row = [rng.uniform(-8,8) for _ in range(3)]
    add('dependent_rows', 'dependent_%d' % i, row + row + [rng.uniform(-8,8) for _ in range(3)])
for i in range(512):
    add('raw', 'raw_%d' % i, [rng.getrandbits(32) for _ in range(9)], True)

# Distinct signs, payloads, and signaling classes expose operand-order changes.
nans = [0x7fc12345, 0xffc54321, 0x7f812345, 0xff854321]
for first in range(9):
    for second in range(first + 1, 9):
        for a in nans:
            for b in nans:
                words = list(identity)
                words[first], words[second] = a, b
                add('nan_pairs', 'nan_%d_%d_%08x_%08x' % (first, second, a, b), words, True)
for i in range(64):
    add('all_nan', 'all_nan_%d' % i, [rng.choice(nans) for _ in range(9)], True)

modes = {r + ('_FZ' if fz else '') + ('_DN' if dn else ''):
         (rounding << 22) | (fz << 24) | (dn << 25)
         for rounding, r in enumerate(('RN','RP','RM','RZ'))
         for fz in (0,1) for dn in (0,1)}
engines = [Engine(retail), Engine(candidate)]
stats = {}; failures = []; named = []; alias_failures = [0,0]; nzcv_diffs = 0
for mode_name, mode in modes.items():
    for case_index, (group, name, words) in enumerate(cases):
        runs = []
        for inplace in (False, True):
            results = [engine.run(words, mode, inplace) for engine in engines]
            runs.append(results)
            left, right = results
            memory_equal = left[0] == right[0]
            status_equal = ((left[1] ^ right[1]) & 0x0fffffff) == 0
            nzcv_diffs += bool((left[1] ^ right[1]) & 0xf0000000)
            key = mode_name + '/' + group
            stat = stats.setdefault(key, {'pairs':0,'memory_failures':0,'status_failures':0,
                                          'both_pass':0,'r0_differences':0})
            stat['pairs'] += 1
            stat['memory_failures'] += not memory_equal
            stat['status_failures'] += not status_equal
            stat['both_pass'] += memory_equal and status_equal
            stat['r0_differences'] += left[3] != right[3]
            evidence = {'case_index':case_index,'name':name,'group':group,'mode':mode_name,
                        'inplace':inplace,'input':['%08x'%v for v in words],
                        'outputs':[['%08x'%v for v in result[2]] for result in results],
                        'fpscr':['%08x'%result[1] for result in results],
                        'memory_equal':memory_equal,'status_equal':status_equal}
            if not memory_equal or not status_equal:
                if len(failures) < 30: failures.append(evidence)
            if group == 'named' and not inplace:
                named.append(evidence)
        for i in (0,1):
            alias_failures[i] += runs[0][i][1:3] != runs[1][i][1:3]

totals = {key:sum(s[key] for s in stats.values())
          for key in ('pairs','memory_failures','status_failures','both_pass','r0_differences')}
summary = {'symbol':SYMBOL,'source':record['source'],
           'source_sha256':record['inputs'][record['source']],
           'object_sha256':record['object_sha256'],
           'candidate_bytes':len(candidate),'candidate_sha256':hashlib.sha256(candidate).hexdigest(),
           'original_bytes':len(retail),'original_sha256':hashlib.sha256(retail).hexdigest(),
           'cases':len(cases),'modes':modes,'totals':totals,'stats':stats,
           'nzcv_differences':nzcv_diffs,'within_engine_alias_failures':alias_failures,
           'first_failures':failures,'named_cases':named,
           'scope':'Unicorn 2.1.4 ARM1176; synthetic binary32 matrices; all 1024 sentinel/input/output bytes, non-NZCV FPSCR, R4-R11/S16-S31/SP and stack guards; distinct and same-object output/input; no partial overlap or hardware/gameplay claim.'}
OUT.write_text(json.dumps(summary, indent=2)+'\n')
print(json.dumps({k:v for k,v in summary.items() if k not in ('stats','modes','first_failures','named_cases')},indent=2))
print(json.dumps(failures[:3],indent=2))
```
