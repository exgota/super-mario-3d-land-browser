# TreeNode push-front: canonical exact result

Base: `e87d556cd4d993606a03e205b4604d863d718bd4`. Branch: `dot/tree-push-front`.
Reservation: `64016b5`. Source checkpoint: `32b0d23`.

`TreeNode::pushFrontChild` at `0x002F2AD4..0x002F2B14` passes the unmodified
`tools/check.py --object`: **64 complete bytes exact**. The same canonical
object preserves the accepted detach100, recursive child-clear64 and constructor24
intervals, all exact. This proposal adds one candidate root,64 bytes; main owns
acceptance. No map, configuration, tools, ledger, STATE or binary change is committed.

## Recovered context

The accepted source already has a TU-local four-link TreeNode class and the
exact detach, clear and constructor methods in `lib/al/src/Util/seadTreeNode.cpp`.
The new insertion body is the packet's ordinary C++ body. The only new context
is `__attribute__((noinline))` on the existing detach definition, whose body is
unchanged. This preserves the retail call boundary at `0x002F2AE4` while leaving
the leaf implementation visible to the compiler in the same translation unit.

Retail contains a BL to the separately accepted detach method, so prohibiting
inlining is independently supported by the call itself. No original source
annotation is claimed. No stack padding, ABI annotation, volatile, synthetic
callee, opcode stand-in or flag adjustment is used. With the real leaf body
visible but uninlined, ARMCC emits the retail three-register save/restore mask.
The compiler's internal explanation is inferred; the emitted bytes are checked.

This is the fourth tree context including the packet's three: declaration-only
64/2 differing bytes, ordinary shared-tail inlining104, accepted branch-local
inlining144, then visible accepted body with the native call boundary64/exact.
No second no-inline form or alternate flags/compiler was tested here.

The original child links are detached first. If this node has no child, it takes
the new child and sets the parent. Otherwise the new child takes the old first
child as next sibling, the old first child points back to it, then first-child
and parent fields update. No null guard or tree invariant absent from retail
was added.

## Canonical verification

The source was committed before `python make.py eu`. A subsequent clean
`python make.py eu -ca` compiled, archived, linked and exported successfully.
The final four object checks passed again after that clean build:

```
_ZN4sead8TreeNode14pushFrontChildEPS0_    U -> O: The complete source-generated function interval matches byte for byte.
_ZN4sead8TreeNode13detachSubTreeEv       O -> O: The complete source-generated function interval matches byte for byte.
_ZN4sead8TreeNode27clearChildLinksRecursively_Ev O -> O: The complete source-generated function interval matches byte for byte.
_ZN4sead8TreeNodeC2Ev                   O -> O: The complete source-generated function interval matches byte for byte.
```

These are all four definitions in the affected source. The existing constructor
C2 row still uses its accepted C1 section identity. This is a focused companion
check, not a fresh audit of all651 accepted roots.

Hashes after the clean build:

- Source: `7baffec4b181a3f1ad57356bc67a46ca099fe864e861dfd62aa01a83ce9983b4`
- Canonical ARMCC4.1/791 object: `67ec951741f64dd8655f78ed9c8a103a95f43e42b5b8591df4dac549db21eb64`
- Original EU executable: `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`
- Restored baseline map: `a8cd649c372ae92c33ae26d4c4a90dc84f257e549e6465b292e419a2a2ce9b97`
- Unchanged check.py: `e177e0abe130e645e75c5f0dc24abb19dbe83310de55f1f099ec8fc385e07317`
- Unchanged checkExactBytes.py: `aef81a3a21c49bb17b8a19f139d02607899257e04dbe461ece2992cc726fc2e2`

## Reproduction

Use the committed branch and the owner's local EU executable/exheader with the
approved compiler and venv. No binary is distributed with this report. From the
repository root, after making those ignored local inputs available:

```sh
. ./development_environment.sh
export DEVKITARM=/usr
python make.py eu -ca
```

Run this complete Python block with the activated project venv. It restores the
original map after the checker updates its diagnostic ranks. No row names,
bounds, classes or pools are changed, and no new rows are added.

```python
from pathlib import Path
import hashlib, json, subprocess
from tools.low.checkExactBytes import check_exact_bytes
from tools.low.buildProvenance import verify_build_output
map_path = Path('data/ver/eu/map.csv')
original_map = map_path.read_bytes()
obj = Path('build/eu/obj/lib/al/src/Util/seadTreeNode.o')
assert verify_build_output(obj)['compiler'] == '4.1/791'
symbols = [
    '_ZN4sead8TreeNode14pushFrontChildEPS0_',
    '_ZN4sead8TreeNode13detachSubTreeEv',
    '_ZN4sead8TreeNode27clearChildLinksRecursively_Ev',
    '_ZN4sead8TreeNodeC2Ev',
]
report = {}
try:
    for symbol in symbols:
        run = subprocess.run(['python', 'tools/check.py', symbol, '--object', str(obj)],
                             text=True, capture_output=True)
        print(symbol, run.stdout, run.stderr)
        assert run.returncode == 0
        result = check_exact_bytes(symbol, obj,
                    output_directory=Path('build/tree_final') / symbol)
        assert result['exact'] and result['evidence']['different_bytes'] == 0
        report[symbol] = {'stdout': run.stdout, 'check': result}
finally:
    map_path.write_bytes(original_map)
assert map_path.read_bytes() == original_map
Path('build/tree_final/report.json').write_text(json.dumps(report, indent=2))
print('Canonical object SHA256:', hashlib.sha256(obj.read_bytes()).hexdigest())
```

Ignored detailed evidence is in `build/tree_final/report.json` and its four
checker-produced original-address ELF directories. The old three packet
contexts were not recompiled. This pass began2026-10-01 at21:59UTC; the clean
build and final checks ended22:04UTC. Times overlap the scale packet work and
are not separate per-function labor estimates.
