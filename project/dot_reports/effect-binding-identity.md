# Effect binding identity investigation

Base: `e2cbf8db72566da78fc16b77a5b90026bfd9ea0a`.
Branch: `dot/effect-binding-identity`.
Targets: `0x001EA31C..0x001EA364` and `0x001EA364..0x001EA3AC`,
72 bytes each. Both remain unnamed U rows.

This is a notes-only negative result. No new C++ candidate, build, checker
invocation, accepted function, or functional NonMatching result is claimed.
The two previously compiled structures per target remain the complete attempt
count; this investigation adds zero compile-diff attempts. No map, rank,
boundary, source, tool, compiler flag, ledger, or STATE change was made.

The original executable SHA256 was independently verified as
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
The examined map SHA256 is
`140aeaee501c03d5d2b3b3b83a13db8b99af029fc86c54527592998fc8ccd3e9`.
Only the approved executable, current source, packets, and unchanged tools
were consulted. No external implementation or library source was used.

## Result of the independent helper search

Each seven-instruction binding tail has exactly one four-byte-aligned
occurrence in the entire 3,096,576-byte executable:

| Tail interval | Complete-byte occurrences | Existing containing function |
| --- | --- | --- |
| `0x001EA348..0x001EA364` | `0x001EA348` only | `0x001EA31C..0x001EA364` |
| `0x001EA390..0x001EA3AC` | `0x001EA390` only | `0x001EA364..0x001EA3AC` |

A scan of every aligned word in all 18,055 existing function intervals,
including bytes after their first Pool marker, finds 18 direct ARM branches
to the matrix root and 12 to the translation root. It finds zero direct
branches to either interior tail. The scan decodes branch immediates directly,
so an interleaved pool or a disassembler stopping on an unrecognized word does
not truncate the examined interval. Literal words are conservatively included;
the resulting root calls were also observed in surrounding instruction context.

A separate search at every byte offset in the entire executable finds zero
little-endian address words for `0x001EA348`, `0x001EA349`, `0x001EA390`, or
`0x001EA391`. Thus neither tail acquired independent function-pointer evidence,
even when considering a Thumb-tagged value. This does not exclude a pointer
assembled arithmetically at runtime or prove the original source boundaries.

The wider immediate STRB scan finds no 28-byte mapped function containing a
store at offset `0x42` or `0x43`. Its smallest unrelated candidate is the
52-byte root `0x001E94B0`, which calls a virtual predicate, copies receiver
byte `0x42` to `0x41`, and sets `0x42`. It is not an effect-set binding helper.
This is a bounded signature search, not a claim that all possible semantically
equivalent lowerings were disproved.

The prior helper form still explains the retail frame teardown and adjacent
tail well: a 44-byte wrapper followed by a 28-byte helper can produce the
complete 72-byte interval. The packets already measured this byte equality
and its retained helper section. That is evidence for an unresolved original
source/linker boundary, not permission to add a function at either interior
address. The unchanged closure checker requires the selected root's sole
memory entry to own the full interval and every unmapped helper extent to
disappear. Repeating the adjacent-helper structure or merely changing its
class/namespace spelling does not answer that requirement.

## New configuration evidence

The resource parser at `0x002715E8..0x00271D54` gives independent serialized
field identities. It loads `EffectData/EffectData`, selects `EffectDataTable`,
then parses an `EffectList`. At `0x002716E4..0x00271720` it allocates an array
with 0x50-byte elements and passes constructor `0x001BB860` to the array
construction routine. The constructor address is loaded from literal
`0x00271A28`. At `0x00271750..0x0027175C`, element indexing independently uses
the same 0x50-byte stride.

The keeper constructor at `0x001BFC98` independently uses this stride at
`0x001BFD10..0x001BFD18`, allocates a 0x68-byte effect set, and passes each
configuration to set constructor `0x001EA3AC`. That set constructor saves the
configuration at set+0x0C, agreeing with both packet targets.

The parser repeatedly uses a view beginning at configuration+0x0C, held in
r5 after `0x00271834`. The constructor also forms configuration+0x0C at
`0x001BB89C`. This supports a convenient transform/configuration view, but
does not by itself prove a distinct C++ subobject or recover an original
class name.

| Configuration offset | Serialized key / meaning established here | Key address | Parser store |
| --- | --- | --- | --- |
| `+0x0C` | `DrawGroup`, an integer classification | `0x00271A6C` | `0x00271864`, `0x00271884` |
| `+0x10` | `JointName`, borrowed string pointer | `0x00271AA4` | Output argument at `0x002718D8` |
| `+0x3C` | `EmitIgnoreRotate` | `0x00271AB0` | `0x002718FC` |
| `+0x3D` | `EmitIgnoreScale` | `0x00271AC4` | `0x00271910` |
| `+0x3E` | `DeleteAtClipping` | `0x00271AD4` | `0x00271924` |
| `+0x3F` | `FollowPos` | `0x00271AE8` | `0x00271938` |
| `+0x40` | `FollowMtx` | `0x00271AF4` | `0x0027194C` |
| `+0x41` | `NeedProgramInfo` | `0x00271B00` | `0x00271960` |
| `+0x44` | `DirectPos` | `0x00271B10` | `0x0027197C` |
| `+0x45` | Successful `PosOffset` read; data goes at `+0x14..+0x1C` | `0x00271B1C` | `0x002719BC` |

The seven boolean resource queries call `0x00271210` before conditionally
storing one. Constructor `0x001BB860` initializes configuration bytes
`+0x3C..+0x46` to zero, including both packet targets' flags at `+0x42/+0x43`.
The resource parser does not fill those two flags in the observed parsing
path. The binding routines subsequently set them on the shared configuration.
They are distinct from serialized `FollowPos` and `FollowMtx`; do not rename
the binding flags as those serialized flags.

This resolves two deliberately unknown names in packet `001EA220`: its
configuration byte `+0x3F` is `FollowPos`, and `+0x40` is `FollowMtx`. The
separate runtime consumer already supports those interpretations by applying
translation or matrix updates to live entries. The broader role of the
matrix-binding override at `+0x43` remains unresolved. No source names or
map symbols were changed based on the new resource labels.

## Reproducible scan

Run from the repository root with its existing Python environment. This reads
the unchanged owner dump and map; it writes nothing and does not compile or
check a candidate. No copied target bytes are embedded in the procedure.

```python
import csv
import hashlib
import struct
from pathlib import Path

data = Path('data/ver/eu/code.bin').read_bytes()
header = Path('data/ver/eu/exh.bin').read_bytes()
base = struct.unpack_from('<I', header, 16)[0]
assert hashlib.sha256(data).hexdigest() == (
    'e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64')
rows = [{k.strip(): v.strip() for k, v in row.items()}
        for row in csv.DictReader(Path('data/ver/eu/map.csv').open())]
functions = [row for row in rows if 'f' in row['Type']]

def occurrences(needle, stride):
    return [hex(base + offset)
            for offset in range(0, len(data) - len(needle) + 1, stride)
            if data[offset:offset + len(needle)] == needle]

for start, end in ((0x1EA348, 0x1EA364), (0x1EA390, 0x1EA3AC)):
    print('complete tail', hex(start),
          occurrences(data[start - base:end - base], 4))
    for address in (start, start | 1):
        print('address word', hex(address),
              occurrences(struct.pack('<I', address), 1))

targets = {address: [] for address in
           (0x1EA31C, 0x1EA348, 0x1EA364, 0x1EA390)}
for row in functions:
    for address in range(int(row['Start'], 16), int(row['End'], 16), 4):
        word = struct.unpack_from('<I', data, address - base)[0]
        if word & 0x0E000000 != 0x0A000000 or word >> 28 == 15:
            continue
        immediate = word & 0xFFFFFF
        if immediate & 0x800000:
            immediate -= 0x1000000
        destination = address + 8 + 4 * immediate
        if destination in targets:
            targets[destination].append(hex(address))
print('function intervals', len(functions))
for destination, sources in targets.items():
    print(hex(destination), len(sources), sources)
```

Full local scan output and disassembled resource/consumer context are retained
in ignored `build/effect_binding_identity/`. The final scan is
`complete_identity_scan.json`; no ignored diagnostic is part of this commit.

## Revisit condition

Park both roots at their existing two-attempt state. A useful next attempt
needs independent evidence of an original standalone callee at an existing
mapped function boundary, or a genuinely different source closure whose
unmapped helper fully disappears under the unchanged canonical gate. An
evidence-backed boundary correction would be a separate main-lane decision
under the hard rules; this report does not propose one. The new configuration
names are reusable immediately in the neighboring effect-update work, but
they do not solve these two roots' structural mismatch.
