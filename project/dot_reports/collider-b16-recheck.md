# Frozen Collider proposal recheck on b16

## Result and discrepancy

The frozen direct-field proposal preserves all 35 accepted canonical definitions
(2,500 complete bytes) whose actual ARMCC dependencies include `alCollider.h`.
The baseline and candidate each pass 35/35 ordinary project object checks.
Canonical `al::isCollidedGround` remains exactly its complete 36-byte retail
interval on both sides. The candidate clean project build links and exports.

`Collider::onInvalidate` remains NonMatching: 88 complete bytes, eight differing
bytes, zero size delta. This recheck adds **zero exact functions and zero exact
bytes**. No new C++ form, scheduling trial, header adjustment, or semantic claim
was made. The two proposed source files are byte-for-byte copies of the completed
proposal on local `22b76f7a1df175170a24fe835e5a0f33172390c7`.

The current STATE sentence that Collider is held because its header changes four
accepted predicate bytes accurately describes the **older aggregate getter in
packet 0024C9EC**, whose expression is `mResults[0].mDistance`. That packet itself
proposes preserving the direct `mGroundDistance` expression as the next hypothesis.
The completed proposal already implements that distinct field representation and
leaves the direct getter expression unchanged. This b16 verification supplies
current evidence for that completed proposal; it does not invalidate the older
packet's measured failure. Main still owns source acceptance and any status change.

The published hash `8a92f95d6f3c619c4dfc98118074c810c8d3c8a2` supplied in the
handoff is absent from this local Git object store, so it was not used as evidence.
The frozen source identity below comes from the present local completed branch,
its committed blobs, and exact SHA-256 comparison. No network publication occurred.

## Frozen inputs and scope

- Base: `b16e2a0cc32e0cdacac5ba94feabe67432433391`
- Reapplied source commit: `f0dc655ebd39f80b7fa6a71c5054574395317613`
- Verification branch: `dot/collider-b16-recheck`
- Header SHA-256: `bab9d41db4e27648f3cdd354eed67a3e4c752d3b137c7c5ca0e1732de5267cd5`
- Source SHA-256: `91475d777b132897405e77dbffa3f33bac80b4b21cb40dd2a3da04fd2ccff7b1`
- Baseline map SHA-256, restored exactly after checks:
  `e77614ca3a67b34978ecbeb88fcaf668b5b0a181c0af62baec2bf5f3925faee5`
- Owner executable SHA-256:
  `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`

Only the existing two source/header files and this recheck's two notes are proposed.
The original packet, response, source form history, map, ledger, STATE, tools,
configuration, flags and eight current Pro reservations are unchanged. The
reservations are 001E37D8, 0030E678, 001D1DA0, 0016A11C, 0024F344, 00268EB0,
001EA220 and 00252B1C. This is verification for the current owning lane and makes
no exclusive family claim.

This pass uses configured ARMCC 4.1/791. It does not repeat 894 probing, bounded
functional replay, or the full 667-root gate. The earlier direct-field report
retains its historical replay scope and limits. Root's ordinary intake gate is
still required before acceptance.

## Baseline reuse audit

The baseline objects come from `mario-tree-b16-family`, head
`1454120a760becbea77c2629f160f807355669c7`. They were consumed only after all
relevant inputs were proven identical to pristine b16; the baseline worktree's
unrelated tree proposal is not taken as an input.

Actual ARMCC `.d` files identify three dependent baseline translation units:

| Object | Accepted definitions | Recorded input paths |
|---|---:|---:|
| `alCollisionUtil.o` | 1 | 15 |
| `alLiveActor.o` | 25 | 34 |
| `alLiveActorFunction.o` | 9 | 41 |

All 90 per-object input references, representing 49 unique paths, equal both their
recorded provenance SHA-256 and pristine b16 blob bytes. Every recorded source
input also equals its committed HEAD blob. Dependency-path sets equal the
provenance input sets after adding `data/config.json`. Object hashes, compiler
hashes, configured compiler command, input stability and project-build provenance
were checked. The ordinary checker independently enforces its committed-input
provenance gate for every definition. All 47 available tracked build/check tool
files equal pristine b16; optional configuration override files are absent.

The candidate discovers those same three translation units plus the new
`alCollider.cpp`, with zero previously accepted definitions in the new unit.
Its 96 input references cover 50 unique paths. Every input except the exact two
proposal files equals pristine b16. Those two files equal the frozen proposal.
The accepted `(object, symbol)` set is identical between baseline and candidate.
All input paths/hashes, commands, object/provenance/dependency hashes, tool hashes,
each individual checker exit/output, and final interval hashes are recorded in
[the machine-readable evidence](collider-b16-recheck-evidence.json).

Object file hashes differ between baseline and candidate; preservation is claimed
for the canonically linked complete accepted intervals, not whole object files.

## Canonical outcomes

Both baseline and candidate `isCollidedGround` at `00262BA4..00262BC8` have SHA-256
`6cf95be804831b23cfdcd60b71a6247949816278529fbd82789e1ab0c775e2c1`, equal to retail.
Both ordinary checks report:

```text
O -> O: The complete source-generated function interval matches byte for byte.
```

Candidate `onInvalidate` at `0024C9EC..0024CA44` reports:

```text
U -> m: The linked candidate differs from the unchanged original interval.
```

The candidate complete interval hash is
`6ac301dadcd4eac8ca50c924d1dd8bc560709f49c9db4e5d8e24382636221486`;
retail is `5a272a14661478a6257f4874ce7a541a6f20993effd102d8dc31cc8a15363f40`.
The candidate matches the earlier completed proposal's linked hash exactly.
The temporary checker rank change was discarded by restoring the original map
bytes in a `finally` block. Target rank remains U in the submitted branch.

The candidate started without a build directory. `python make.py eu -ca`
compiled 44 Game, 129 al and one SDK sources, then linked/exported with exit 0.
Build duration was not separately timed. Verification measured 64.234031 seconds,
including input auditing and 71 checks; baseline's 35 checks took 28.205604 seconds,
and candidate's 35 accepted checks plus the target took 33.102814 seconds.
The measured script ended at `2026-10-02T00:01:26.016786+00:00`.

## Reproduction

Use the exact base and source commit above, the project's normal Python packages,
ARMCC/wibo installation, ARM binutils, and authorized local `code.bin`/`exh.bin`.
No game binary or object is distributed in these notes. Do not copy objects into
a different checkout: the provenance checker requires their recorded build paths.

Create separate pristine baseline and proposal worktrees, set up each ignored
local toolchain/data environment, and source its environment before building:

```sh
git worktree add --detach /tmp/collider-baseline b16e2a0cc32e0cdacac5ba94feabe67432433391
git worktree add --detach /tmp/collider-candidate f0dc655ebd39f80b7fa6a71c5054574395317613
```

In each checkout, provide `.venv`, `data/compilers`, `data/ver/eu/code.bin` and
`data/ver/eu/exh.bin` from authorized setup. These may reuse the original dot
Python/compiler/data installation; build outputs must remain checkout-local.
Then run in each root:

```sh
. ./development_environment.sh
export DEVKITARM=/usr TMP=/tmp
sha256sum data/ver/eu/code.bin
python make.py eu -ca
```

The recorded run reused the already built baseline after the exact audit above,
and clean-built only the proposal. A fresh baseline build is the alternative if
that audit fails or the original baseline objects are unavailable.

Save the following executed verification script outside the repository as
`/tmp/collider-b16-recheck.py`. The original run used local proposal reference
`22b76f7`; if that historical local commit is unavailable, set `FROZEN` to
`f0dc655ebd39f80b7fa6a71c5054574395317613`. The two referenced source blobs were
independently verified byte-identical at both commits with the SHA-256s above.
From the candidate root with its environment active, run:

```sh
python /tmp/collider-b16-recheck.py /tmp/collider-baseline /tmp/collider-candidate /tmp/collider-b16-evidence
```

For the measured run, those first two arguments instead named the actual
`mario-tree-b16-family` and `mario-collider-b16-recheck` worktrees. The script audits
all inputs before consuming a baseline object, discovers all affected accepted
definitions, checks both sides, checks the target, restores exact map bytes, and
asserts the complete ground/target interval sizes and mismatch counts. A failed
input assertion is a reason to use a fresh baseline; it is not permission to
relax the check.

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
import csv,datetime,hashlib,json,os,shlex,subprocess,sys,time

BASE='b16e2a0cc32e0cdacac5ba94feabe67432433391'
FROZEN='22b76f7'
HEADER='lib/al/include/Collision/alCollider.h'
SOURCE='lib/al/src/Collision/alCollider.cpp'
GROUND='_ZN2al16isCollidedGroundEPKNS_9LiveActorE'
TARGET='_ZN2al8Collider12onInvalidateEv'
base=Path(sys.argv[1]).resolve();candidate=Path(sys.argv[2]).resolve()
out=Path(sys.argv[3]).resolve();out.mkdir(parents=True,exist_ok=True)
sha=lambda b:hashlib.sha256(b).hexdigest()
def run(root,*args):return subprocess.check_output(args,cwd=root)
def blob(root,ref,path):return run(root,'git','show',ref+':'+path)
def now():return datetime.datetime.now(datetime.timezone.utc).isoformat()
def inspect(root,which):
 records=[]
 for dep in sorted((root/'build/eu/obj').rglob('*.d')):
  deps=[]
  for line in dep.read_text().splitlines():
   fields=shlex.split(line)
   if len(fields)>1:
    p=Path(fields[1]).resolve()
    if p.is_relative_to(root):deps.append(str(p.relative_to(root)))
  if HEADER not in deps:continue
  obj=dep.with_suffix('.o');pro=dep.with_suffix('.provenance.json');record=json.loads(pro.read_text())
  assert record['schema']==1 and record['build_step']=='tools.pypstem.stepBuild' and record['language']=='C++' and record['inputs_stable']
  assert record['object_sha256']==sha(obj.read_bytes())
  assert record['compiler_sha256']==sha((root/record['compiler']).read_bytes())
  assert set(deps)|{'data/config.json'}==set(record['inputs'])
  proof=[]
  for name,digest in sorted(record['inputs'].items()):
   actual=(root/name).read_bytes();reference=blob(root,BASE if which=='baseline' or name not in (HEADER,SOURCE) else FROZEN,name)
   assert digest==sha(actual)==sha(reference),(which,name,digest,sha(actual),sha(reference))
   assert actual==blob(root,'HEAD',name)
   proof.append({'path':name,'sha256':digest,'reference':BASE if which=='baseline' or name not in (HEADER,SOURCE) else FROZEN})
  with obj.open('rb') as stream:
   elf=ELFFile(stream);table=elf.get_section_by_name('.symtab')
   definitions=sorted({s.name for s in table.iter_symbols() if s['st_info']['type']=='STT_FUNC' and isinstance(s['st_shndx'],int) and s.name in accepted})
  records.append({'object':str(obj.relative_to(root)),'object_sha256':sha(obj.read_bytes()),'dependency_sha256':sha(dep.read_bytes()),'provenance_sha256':sha(pro.read_bytes()),'compiler':record['compiler'],'compiler_sha256':record['compiler_sha256'],'command':record['command'],'inputs':proof,'accepted_definitions':definitions})
 return records

started=now();t0=time.monotonic()
map_bytes=blob(base,BASE,'data/ver/eu/map.csv')
rows=[{k.strip():v.strip() for k,v in row.items()} for row in csv.DictReader(map_bytes.decode().splitlines())]
accepted={row['Symbol']:row for row in rows if row['Rank']=='O'}
evidence={'started':started,'frozen_base':BASE,'baseline_checkout':str(base),'baseline_head':run(base,'git','rev-parse','HEAD').decode().strip(),'candidate_checkout':str(candidate),'candidate_source_head':run(candidate,'git','rev-parse','HEAD').decode().strip(),'map_sha256':sha(map_bytes),'target_sha256':sha((candidate/'data/ver/eu/code.bin').read_bytes()),'phases':{}}
for root,which in [(base,'baseline'),(candidate,'candidate')]:
 assert (root/'data/ver/eu/map.csv').read_bytes()==map_bytes
 for p in ['data/config.user.json','data/ver/eu/config.json']:assert not (root/p).exists()
 records=inspect(root,which)
 phase={'objects':records,'checks':[],'started':now()};evidence['phases'][which]=phase
 # Verify all executing tracked build/check tools against pristine b16.
 names=run(root,'git','ls-tree','-r','--name-only',BASE,'--','tools','make.py','development_environment.sh').decode().splitlines()
 phase['tool_inputs']={}
 for name in names:
  path=root/name
  if not path.is_file():continue
  data=path.read_bytes();assert data==blob(root,BASE,name),name
  phase['tool_inputs'][name]=sha(data)
 (out/'input-audit.json').write_text(json.dumps(evidence,indent=2)+'\n')
 # Audit completes before any checker can consume the reusable baseline objects.
 started_check=time.monotonic()
 snapshot=(root/'data/ver/eu/map.csv').read_bytes()
 try:
  for record in records:
   for symbol in record['accepted_definitions']:
    result=subprocess.run([sys.executable,'tools/check.py',symbol,'--object',record['object']],cwd=root,capture_output=True,text=True)
    check={'symbol':symbol,'object':record['object'],'exit':result.returncode,'output':result.stdout+result.stderr}
    phase['checks'].append(check)
    if result.returncode:print(json.dumps(check),flush=True)
  if which=='candidate':
   obj='build/eu/obj/lib/al/src/Collision/alCollider.o'
   result=subprocess.run([sys.executable,'tools/check.py',TARGET,'--object',obj],cwd=root,capture_output=True,text=True)
   phase['target_check']={'symbol':TARGET,'object':obj,'exit':result.returncode,'output':result.stdout+result.stderr}
 finally:
  (root/'data/ver/eu/map.csv').write_bytes(snapshot)
 phase['seconds']=time.monotonic()-started_check
 phase['ended']=now();phase['map_restored']=sha((root/'data/ver/eu/map.csv').read_bytes())==sha(map_bytes)
 phase['passes']=sum(r['exit']==0 for r in phase['checks']);phase['checks_count']=len(phase['checks'])
 assert all(c['exit']==0 for c in phase['checks'])
 assert any(c['symbol']==GROUND for c in phase['checks'])
 print(json.dumps({'phase':which,'objects':len(records),'checks':phase['checks_count'],'passes':phase['passes'],'seconds':phase['seconds']}),flush=True)
 (out/'evidence.json').write_text(json.dumps(evidence,indent=2)+'\n')
assert {(r['object'],s) for r in evidence['phases']['baseline']['objects'] for s in r['accepted_definitions']}=={(r['object'],s) for r in evidence['phases']['candidate']['objects'] for s in r['accepted_definitions']}
for root,which in [(base,'baseline'),(candidate,'candidate')]:
 evidence['phases'][which]['canonical_intervals']={}
 for symbol in [GROUND]+([TARGET] if which=='candidate' else []):
  row=next(r for r in rows if r['Symbol']==symbol);start=int(row['Start'],16);end=int(row['End'],16)
  paths=list((root/'build/exact_checks/eu').glob('function_%08X_*/candidate.axf'%start));p=max(paths,key=lambda p:p.stat().st_mtime)
  with p.open('rb') as stream:
   elf=ELFFile(stream);section=elf.get_section_by_name('CANDIDATE_CODE');assert section['sh_addr']==start;data=section.data()
  original=(root/'data/ver/eu/code.bin').read_bytes()[start-0x100000:end-0x100000]
  evidence['phases'][which]['canonical_intervals'][symbol]={'start':hex(start),'end':hex(end),'complete_size':len(data),'retail_size':len(original),'candidate_sha256':sha(data),'retail_sha256':sha(original),'different_bytes':sum(a!=b for a,b in zip(data,original)),'size_delta':len(data)-len(original),'axf_sha256':sha(p.read_bytes())}
  assert len(data)==len(original)
  assert sum(a!=b for a,b in zip(data,original))==(8 if symbol==TARGET else 0)
evidence['ended']=now();evidence['seconds']=time.monotonic()-t0
(out/'evidence.json').write_text(json.dumps(evidence,indent=2)+'\n')
print(json.dumps({'ended':evidence['ended'],'total_seconds':evidence['seconds'],'target':evidence['phases']['candidate']['canonical_intervals'][TARGET]}),flush=True)
```
