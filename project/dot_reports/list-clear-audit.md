# ListImpl::clear register-correspondence audit

Zero exact additions. On main5025a6cd5ec8531fb1bc40ff4b570a0c01ef201c, the unchanged committed ListImpl::clear source canonically remains56 bytes with seven differing bytes. No new source form is justified by this audit, so the prior five structures remain the count.

The unchanged command `python tools/check.py _ZN4sead8ListImpl5clearEv --object build/eu/obj/lib/al/src/Util/seadList.o` reports `m -> m: The linked candidate differs from the unchanged original interval.` The object was built by the project during a clean current-main build with an unrelated isolated audio experiment; the List source/header and compiler inputs are unchanged main code. No target, pool, map boundary or tool was modified.

Original56-byte SHA-256: aaa2c66325d60fad97c393cfb849295f8e19569a65e402042373aa47dc63405d. Canonical candidate:6deda507b9f8d002cb7f02c5e1f0a56511e0a2464c033c640df663c7180bc87e. The latter equals the body whose612 bounded pairs are already documented in packet0021EF6C; that suite was not repeated or counted as new work here.

## Structural observation

All fourteen decoded instructions agree exactly under the single R2/R3 register permutation. Branch destinations, conditions, memory offsets, operation order and all other registers agree. The original's zero temporary is R3 and removed-node temporary is R2; the source exchanges them. Neither instruction stream has a multi-register transfer containing both swapped registers, so this substitution does not reverse two stored values.

The zero register is defined before every path can use it. The removed-node register is defined at the start of each loop iteration before either node store uses it, and is not used on the empty-list path. Thus no incoming R2/R3 value supplies observable input. The fields accessed and changed by each instruction correspond under that temporary-register mapping. This supports the existing bounded NonMatching interpretation under the void caller contract; it does not turn different machine bytes into an exact match or establish physical timing, concurrent observation, malformed-list fault behavior or gameplay.

The current packet's reference to an accepted PlayerActionMultiCondition constructor should be read cautiously: its252054 constructor remains U/nonmatching on this main snapshot. Its independent field accesses and already accepted append routine support the list layout; no constructor exact credit is used in this audit.

## Reproducer

Run the normal project build and canonical clear check first. The following reads the local owner-supplied binary and the latest complete canonical candidate. It does not alter any code, use a replacement oracle, or assign a rank.

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from capstone import Cs, CS_ARCH_ARM, CS_MODE_ARM
import hashlib, re
root = Path.cwd()
data = (root / 'data/ver/eu/code.bin').read_bytes()
assert hashlib.sha256(data).hexdigest() == 'e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
start, length = 0x21ef6c, 56
original = data[start - 0x100000:start - 0x100000 + length]
path = max((root / 'build/exact_checks/eu').glob('function_0021EF6C_*/candidate.axf'), key=lambda p: p.stat().st_mtime)
with path.open('rb') as stream:
    elf = ELFFile(stream)
    candidate = next(s.data() for s in elf.iter_sections() if s['sh_addr'] == start and s['sh_size'])
assert len(candidate) == length
assert hashlib.sha256(candidate).hexdigest() == '6deda507b9f8d002cb7f02c5e1f0a56511e0a2464c033c640df663c7180bc87e'
assert sum(a != b for a, b in zip(original, candidate)) == 7
engine = Cs(CS_ARCH_ARM, CS_MODE_ARM)
a = [(i.mnemonic, i.op_str) for i in engine.disasm(original, start)]
b = [(i.mnemonic, i.op_str) for i in engine.disasm(candidate, start)]
def swap(text):
    return re.sub(r'\br[23]\b', lambda m: 'r3' if m[0] == 'r2' else 'r2', text)
assert len(a) == len(b) == 14
assert [(mnemonic, swap(operands)) for mnemonic, operands in a] == b
assert not any('{' in operands and re.search(r'\br2\b', operands) and re.search(r'\br3\b', operands) for _, operands in a)
print('14 instructions correspond under R2/R3 permutation; seven bytes still differ')
```

No new compiler flag, return type, volatile access, helper boundary or arbitrary liveness change is proposed. Further matching needs independent source/API/compiler evidence rather than another register-only spelling.
