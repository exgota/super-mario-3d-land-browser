# BombHei private product layout: negative result on 8991

## Result and scope

The retained BombHei::control proposal is reverified at **704 complete bytes with 49 differing bytes** on frozen main `8991bec8cb27ff1a2dac0003f059d270a0b17493`. One new evidence-derived private product-layout form produces the **same entire 12,372-byte object**, and therefore the same complete linked candidate. Stop after this one unsuccessful form. This report proposes no source or header changes and no newly authored matching or NonMatching body. The source patch is explicitly empty. There are zero accepted roots, zero accepted bytes and zero accepted bytes/hour from this pass.

Work used a new isolated dot worktree. The original `dot/bomb-hei-control` and main were not edited. The experiment commits are local `b34156f9bd848ffe026ea656a5155b356bfb76cc` (exact old baseline imported onto 8991) and `838a354ac3aecdc39887340aa1f29cc54fab2736` (the one private layout form), retained on `dot/bomb-product-layout`. The publishable report-only branch starts separately at 8991. The coordinator owns publication.

The source/header input was read from immutable local `4f2a1c0` and checked against its documented hashes. At 02:04UTC the coordinator independently read back published `dot/bomb-hei-control` commit `3fa7ad61311567397ca5db385b5e73d2823e37f9` through GitHub and verified its tree `eae9bbdb6d82df735e0a429e890a02f40ff2e0ea` equals local `4f2a1c0^{tree}`. That remote commit object was not available in this local Git database; the source/header hashes here are direct local observations. Exact inputs and the test delta are embedded below.

## Why this test was bounded

Read the Bomb packet, all four original packet forms, the four subsequent dot forms and both complete dot reports, all three ranked Pro forms and their grade, and the accepted HitSensorDirector response at `project/pro_responses/001D1DA0.md`. Hit's private `x,z,y` destination record corrected destination-register identities while preserving public Vector3 layout and correct assignments. That is evidence that an aggregate representation can matter in this compiler, not evidence that its exact fix transfers here.

Bomb's retained returned-Quatf form already assigns product x/y/z/w to s0/s1/s2/s3, exactly as retail does. Its residual problem is input assignment, load coalescing and issue order. Retail's product chains issue x,w,z,y; the retained form issues x,y,z,w. Retail loads rotation x/y/z/w into s9/s4/s5/s7, and vector x/y/z into s10/s8/s6. The baseline loads rotation into s5/s4/s6/s7 and vector into s10/s9/s8, including an extra add and a paired z/w load. Its first output accumulator uses s7 where retail uses s6. The last 52 bytes of the quaternion block already match, including alias-dependent rotation reloads.

The single new hypothesis was a private four-float product declared x,w,z,y, following retail's observed chain issue order, with constructor parameters still x,y,z,w and each component assigned correctly. It retained the returned-value lifetime, all arithmetic grouping, all reference aliases, all output stores, and every public input layout. This tested whether destination initialization could change the surviving input load/coalescing schedule. No other member permutation, renamed-local experiment, dummy store, volatile/register qualifier, padding, compiler flag, helper address or oracle change was tried.

The compiled object is wholly identical. This provides no support for using this private layout to alter Bomb's allocation; it is consistent with the layout distinction disappearing during optimization. The compiler phase responsible was not instrumented. Do not infer that all possible aggregate representations are equivalent, or spend a blind permutation sweep on that uncertainty.

## Attempts and canonical evidence

Historical packet forms 1–4 measured 69/55/56/55 differing bytes under paired 791/894 diagnostics. Historical dot forms 5–8 measured 49/80/60 differences and a 708-byte size failure; dot restored form 5. Pro's three ranked forms emitted 716/700/704 bytes; its third form reproduced the same 49-byte linked baseline. Those are historical results, not new compiles in this pass.

This pass built only ARMCC 4.1 build 791 through the normal project build. Both inputs were committed first. The baseline used `python make.py eu -ca` with no existing build output; the new form used `python make.py eu`. Both completed normal linking and code export. Baseline check time was 2026-10-02T02:00:23.157719Z; new-form check time was 02:01:11.467971Z. Investigation began at 01:57:52Z. No separately instrumented build-duration claim is made.

Both unchanged `tools/check.py --object` calls return exit 1 and:

```text
U -> m: The linked candidate differs from the unchanged original interval.
```

Both unchanged `tools/low/checkExactBytes.py` diagnostics return exit 1, `exact: false`, no rejection, full size 704 and 49 differing bytes. Only the independently identified existing 0030E678 row's name is temporarily set. Its pool, end, Type and all other rows remain unchanged. The complete original map is restored in a finally block after each check.

All 49 differences remain in 0030E6A8..0030E708. The final 52 quaternion bytes, all 556 bytes outside the 148-byte quaternion interval, and the complete 96-byte literal pool agree. These partial agreements add no exact credit.

| Item | SHA256 |
| --- | --- |
| Frozen baseline source | `916533c6d9bfd7046f3eb417d5d795555c41b58837df7c5ac639641ac4935ff3` |
| One new private-layout source | `bba96160a14ce5fb0c2cbf280379fd5875385cdb2fa8a3fcc659ed3f1d8787e2` |
| Frozen Bomb header, both builds | `1f1185d8a879d64b18e265154dd24c469aa7659b6f2a4e5cec12a4245b4d7fc1` |
| Full baseline and new-form object | `b25c7a2481d397f04df8f92f624142b66e03f432bbde839e87d5ebedaab0525c` |
| Both full original-address candidate intervals | `1adfba5f10cc022d50346ff99a4d5acfe8844795110d0d854f56985b2ed634d1` |
| Original EU code | `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64` |
| Restored 8991 map | `2dde00f86cc0bc6f174f3a281853a4faacea1f16b6b628d602bea687b4cdb805` |
| Unchanged project checker | `e177e0abe130e645e75c5f0dc24abb19dbe83310de55f1f099ec8fc385e07317` |
| Unchanged exact-byte helper | `aef81a3a21c49bb17b8a19f139d02607899257e04dbe461ece2992cc726fc2e2` |
| ARMCC 4.1/791 executable | `d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d` |
| Empty proposed source patch | `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855` |

The historical 3de-based object hash was `05d4c6e8b98ec37b133005f8c249daecf2414b49e6e0e1392eec4fe964b1060c`; do not require that whole-object hash after importing onto current main. This pass records the actual 8991 object hash above. Its root's linked bytes are exactly the historical baseline.

## Public layouts and preservation limits

For faithful baseline reproduction only, the old Bomb header was imported byte-for-byte. Relative to 8991, that existing refinement splits opaque bytes60..7F into an unchanged 16-byte prefix and evidenced words70/74/78/7C, uses float88, declares the control override and asserts size8C. It is frozen across both builds. This report submits none of that header delta. Current-main Vector3, Quat and LiveActor headers remain byte-for-byte unchanged; no public-layout change was used to chase code generation.

The current 717-root/736-definition preservation gate was **not run in this pass**. No changed source/header is submitted and no current-wide preservation claim is made. Other branches' gates are not substituted. Both project builds link, but linking alone does not prove prior-root preservation.

## Fresh replay, separately scoped

The old replay corpus was rerun twice: once against the newly built baseline candidate and once against the newly built final experimental candidate, each obtained from its unchanged checker-linked object. Results are identical, and no historical replay result is substituted.

- Arithmetic-only scope: 4,608 quaternion-region pairs plus 8,292 signed-word threshold pairs, **12,900 pairs per object**. Real original/candidate instructions execute without external calls. The quaternion fixtures cover nine output/rotation alias offsets, random raw words and IEEE edge values. Four FPSCR modes cover flush-to-zero/default-NaN combinations. Output memory and full FPSCR agree; outside-output memory guards and callee-saved integer/VFP registers and SP agree
- Whole-control scope: **1,792 pairs per object**, all 152 original body instruction addresses visited. Actor/front memory, ordered call traces, modeled nerve state and FPSCR agree. Stack guards, r4–r11, s16–s31 and SP remain intact
- All nine whole-control external endpoints are deterministic models: 00279ED4 gate; 00337474 quaternion provider; 0027C04C front getter; 0024E9F8 audio; 00271330 action; 00271300 vector/float helper; 002806A8 nerve query; 00280610 nerve set; 00271294 final helper. No real imported routine executes in this scope
- Signed-word threshold cases retain negative quiet/signaling NaN behavior: FFC00000, FF800001 and FFFFFFFF produce positive zero in every tested FPSCR mode. The source remains the old union-based signed-word comparison, not a newly substituted IEEE comparison

Each object has zero observed mismatches in each scope. The two objects are byte-identical, but the final object's replay was actually rerun. No extra fixture was needed because this experiment emitted no behavioral change. This bounded corpus is not exhaustive paths, real-callee validation, native-compiler validation, signed timer-overflow validation or gameplay equivalence.

## Full reproduction from committed inputs

Use a fresh isolated worktree at 8991 with the approved local toolchain, Python/Unicorn dependencies and the owner's local EU code/exheader. Do not use the experiment's files without committing them before the build. Save this report outside the new worktree or pass its absolute path to the extraction command. Only five embedded files below are extracted: two source inputs and three ignored build helpers/data. Nothing under tools is created or edited.

```sh
git worktree add -b dot/bomb-product-reproduce ../mario-bomb-product-reproduce 8991bec8cb27ff1a2dac0003f059d270a0b17493
cd ../mario-bomb-product-reproduce
# Provide the approved .venv, data/compilers, and local data/ver/eu/code.bin/exh.bin.
python3 - /absolute/path/to/bomb-product-layout.md <<'PY_EXTRACT'
import re, sys
from pathlib import Path
text = Path(sys.argv[1]).read_text()
allowed = {
 'Game/backup/src/Enemy/BombHei.cpp',
 'Game/backup/include/Enemy/BombHei.h',
 'build/bomb_product_layout/form1.patch',
 'build/bomb_product_layout/check_attempt.py',
 'build/bomb_product_layout/replay.py',
}
blocks = re.findall(r'<!-- reproduce-file: ([^>]+) -->\n```[^\n]*\n(.*?)\n```', text, re.S)
assert {name for name, body in blocks} == allowed
for name, body in blocks:
 path = Path(name); path.parent.mkdir(parents=True, exist_ok=True)
 path.write_text(body + '\n')
PY_EXTRACT
git add Game/backup/src/Enemy/BombHei.cpp Game/backup/include/Enemy/BombHei.h
git commit -m 'Reproduce frozen BombHei baseline'
. ./development_environment.sh
export DEVKITARM=/usr
python make.py eu -ca > build/bomb_product_layout/baseline-build.log 2>&1
python build/bomb_product_layout/check_attempt.py baseline
python build/bomb_product_layout/replay.py baseline > build/bomb_product_layout/baseline/replay.json
git apply build/bomb_product_layout/form1.patch
git add Game/backup/src/Enemy/BombHei.cpp
git commit -m 'Test one retail-derived private product order'
python make.py eu > build/bomb_product_layout/form1-build.log 2>&1
python build/bomb_product_layout/check_attempt.py form1
python build/bomb_product_layout/replay.py form1 > build/bomb_product_layout/form1/replay.json
python - <<'PY_VERIFY'
from pathlib import Path
import hashlib, json
p = Path('build/bomb_product_layout')
assert (p/'baseline/BombHei.o').read_bytes() == (p/'form1/BombHei.o').read_bytes()
for label in ['baseline', 'form1']:
 m = json.loads((p/label/'manifest.json').read_text())
 assert m['checker_exit'] == 1 and m['diagnostic_exit'] == 1 and m['map_restored']
 assert m['result']['compiled_section_size'] == 704 and m['result']['different_bytes'] == 49
 assert m['result']['object_sha256'] == 'b25c7a2481d397f04df8f92f624142b66e03f432bbde839e87d5ebedaab0525c'
 assert m['result']['linked_section_sha256'] == '1adfba5f10cc022d50346ff99a4d5acfe8844795110d0d854f56985b2ed634d1'
 r = json.loads((p/label/'replay.json').read_text())
 assert [r['rotation_cases'], r['threshold_cases'], r['control_cases']] == [4608,8292,1792]
 assert r['memory_and_fpscr_mismatches'] == r['control_memory_trace_fpscr_mismatches'] == 0
 assert r['control_visited_instruction_addresses'] == 152 and not r['control_unvisited_instruction_addresses']
assert hashlib.sha256(Path('data/ver/eu/map.csv').read_bytes()).hexdigest() == '2dde00f86cc0bc6f174f3a281853a4faacea1f16b6b628d602bea687b4cdb805'
print('Baseline and one tested form: identical full objects; 704 bytes/49 differences; both bounded replay scopes pass')
PY_VERIFY
```

The embedded extractor was run in a temporary directory: both baseline hashes matched, the one test patch applied cleanly, and its output matched the recorded experimental source hash. Both runner scripts parse, and the final verification block passed against the observed evidence. The following inputs are exact, including terminal newlines.

<!-- reproduce-file: Game/backup/src/Enemy/BombHei.cpp -->
```cpp
#include "Enemy/BombHei.h"
#include <LiveActor/alActorPoseKeeper.h>
#include <Nerve/alNerveFunction.h>
#include <Audio/alAudioKeeper.h>
#include <prim/seadSafeString.h>

#ifdef NON_MATCHING

extern "C" bool fn_00279ED4( const al::LiveActor*, int );
extern "C" void fn_00337474( const al::LiveActor*, sead::Quatf* );
extern "C" void fn_0024E9F8( al::IUseAudioKeeper*, const sead::SafeString&, int );
extern "C" void fn_00271330( al::LiveActor*, const char* );
extern "C" void fn_00271300( sead::Vector3f*, float );
extern "C" bool fn_00271294( al::LiveActor* );
extern "C" const al::Nerve dat_003F1E8C;
extern "C" const al::Nerve dat_003F1E88;
extern "C" const al::Nerve dat_003F1E84;
extern "C" const al::Nerve dat_003F1E80;
extern "C" const al::Nerve dat_003F1E74;
extern "C" const al::Nerve dat_003F1E78;
extern "C" const al::Nerve dat_003F1E7C;

inline sead::Quatf multiplyQuaternionVector( const sead::Quatf& rotation, const sead::Vector3f& vector )
{
        return sead::Quatf(
                rotation.y * vector.z - rotation.z * vector.y + rotation.w * vector.x,
                rotation.z * vector.x - rotation.x * vector.z + rotation.w * vector.y,
                rotation.x * vector.y - rotation.y * vector.x + rotation.w * vector.z,
                -rotation.x * vector.x - rotation.y * vector.y - rotation.z * vector.z );
}

inline void rotateVectorByQuaternion( sead::Vector3f& vector, const sead::Quatf& rotation )
{
        const sead::Quatf product = multiplyQuaternionVector( rotation, vector );
        vector.x = product.x * rotation.w - product.y * rotation.z + product.z * rotation.y - product.w * rotation.x;
        vector.y = product.y * rotation.w + product.x * rotation.z - product.z * rotation.x - product.w * rotation.y;
        vector.z = product.y * rotation.x - product.x * rotation.y + product.z * rotation.w - product.w * rotation.z;
}

// NON_MATCHING: quaternion register allocation remains under investigation.
void BombHei::control()
{
        if ( fn_00279ED4( this, 0 ) )
        {
                sead::Quatf rotation;
                fn_00337474( this, &rotation );
                rotateVectorByQuaternion( *al::getFrontPtr( this ), rotation );
        }
        if ( _70 > 0 )
                --_70;
        if ( _84 )
        {
                fn_0024E9F8( this, "SeEmLvBombHeiFuse", 2 );
                if ( _74 <= 90 )
                {
                        fn_0024E9F8( this, "SeEmLvBombHeiBlinkFast", 2 );
                        if ( _74 == 90 )
                                fn_00271330( this, "Blink" );
                }
                if ( --_74 <= 0 )
                {
                        _84 = false;
                        if ( !al::isNerve( this, &dat_003F1E8C ) && !al::isNerve( this, &dat_003F1E88 ) )
                        {
                                al::setNerve( this, &dat_003F1E8C );
                                return;
                        }
                }
        }
        if ( al::isNerve( this, &dat_003F1E84 ) || al::isNerve( this, &dat_003F1E80 ) ||
             al::isNerve( this, &dat_003F1E74 ) || al::isNerve( this, &dat_003F1E78 ) ||
             al::isNerve( this, &dat_003F1E7C ) )
        {
                fn_00271300( al::getFrontPtr( this ), _88 );
                // Retail uses signed integer ordering of the IEEE-754 word. In
                // particular, negative NaNs take the +0 path.
                union { float value; int bits; } rate;
                rate.value = _88;
                _88 = rate.bits < 0x40600000 ? 0.0f : _88 * 0.96f;
        }
        fn_00271294( this );
}

#endif
```

<!-- reproduce-file: Game/backup/include/Enemy/BombHei.h -->
```cpp
#pragma once

#include <MapObj/alMapObjActor.h>

class BombHei : public al::MapObjActor
{
private:
        u8    _60[ 0x10 ];
        int   _70;
        int   _74;
        int   _78;
        float _7C;
        float _80;
        bool  _84;
        float _88;

public:
        virtual void control();
        virtual void init( const al::ActorInitInfo& info );
        virtual void makeActorAppeared();
        virtual void attackSensor( al::HitSensor* me, al::HitSensor* other );
        virtual bool receiveMsg( u32 msg, al::HitSensor* other, al::HitSensor* me );

public:
        BombHei( const sead::SafeString& name );
};

static_assert_( sizeof( BombHei ) == 0x8C );
```

<!-- reproduce-file: build/bomb_product_layout/form1.patch -->
```diff
diff --git a/Game/backup/src/Enemy/BombHei.cpp b/Game/backup/src/Enemy/BombHei.cpp
index 9845ec9..d34df38 100644
--- a/Game/backup/src/Enemy/BombHei.cpp
+++ b/Game/backup/src/Enemy/BombHei.cpp
@@ -20,9 +20,22 @@ extern "C" const al::Nerve dat_003F1E74;
 extern "C" const al::Nerve dat_003F1E78;
 extern "C" const al::Nerve dat_003F1E7C;

-inline sead::Quatf multiplyQuaternionVector( const sead::Quatf& rotation, const sead::Vector3f& vector )
+namespace
 {
-        return sead::Quatf(
+
+// Private Hamilton-product storage follows the observed accumulator issue order.
+// The public quaternion and vector layouts are unchanged.
+struct QuaternionVectorProduct
+{
+        float x, w, z, y;
+
+        QuaternionVectorProduct( float xValue, float yValue, float zValue, float wValue )
+            : x( xValue ), w( wValue ), z( zValue ), y( yValue ) {}
+};
+
+inline QuaternionVectorProduct multiplyQuaternionVector( const sead::Quatf& rotation, const sead::Vector3f& vector )
+{
+        return QuaternionVectorProduct(
                 rotation.y * vector.z - rotation.z * vector.y + rotation.w * vector.x,
                 rotation.z * vector.x - rotation.x * vector.z + rotation.w * vector.y,
                 rotation.x * vector.y - rotation.y * vector.x + rotation.w * vector.z,
@@ -31,12 +44,14 @@ inline sead::Quatf multiplyQuaternionVector( const sead::Quatf& rotation, const

 inline void rotateVectorByQuaternion( sead::Vector3f& vector, const sead::Quatf& rotation )
 {
-        const sead::Quatf product = multiplyQuaternionVector( rotation, vector );
+        const QuaternionVectorProduct product = multiplyQuaternionVector( rotation, vector );
         vector.x = product.x * rotation.w - product.y * rotation.z + product.z * rotation.y - product.w * rotation.x;
         vector.y = product.y * rotation.w + product.x * rotation.z - product.z * rotation.x - product.w * rotation.y;
         vector.z = product.y * rotation.x - product.x * rotation.y + product.z * rotation.w - product.w * rotation.z;
 }

+} // namespace
+
 // NON_MATCHING: quaternion register allocation remains under investigation.
 void BombHei::control()
 {
```

<!-- reproduce-file: build/bomb_product_layout/check_attempt.py -->
```python
from pathlib import Path
import subprocess,sys,hashlib,json,shutil,datetime
label=sys.argv[1]
out=Path('build/bomb_product_layout')/label
out.mkdir(parents=True,exist_ok=True)
source=Path('Game/backup/src/Enemy/BombHei.cpp')
header=Path('Game/backup/include/Enemy/BombHei.h')
obj=Path('build/eu/obj/Game/backup/src/Enemy/BombHei.o')
paths=[source,header,obj,Path('tools/check.py'),Path('tools/low/checkExactBytes.py'),Path('data/ver/eu/code.bin'),Path('data/ver/eu/map.csv')]
assert hashlib.sha256(paths[-2].read_bytes()).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
for p in [source,header]:assert p.read_bytes()==subprocess.check_output(['git','show','HEAD:'+str(p)])
meta={'commit':subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip(),'utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'hashes':{str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in paths}}
shutil.copy2(source,out/'BombHei.cpp');shutil.copy2(header,out/'BombHei.h');shutil.copy2(obj,out/'BombHei.o')
mp=paths[-1]; original=mp.read_bytes();rows=original.decode().splitlines(True);found=0
for i,row in enumerate(rows):
 if row.startswith('0x0030E678,'):
  cols=row.split(',');assert cols[1].strip()=='0x0030E8D8' and cols[2].strip()=='0x0030E938'
  assert cols[6].strip() in ['','_ZN7BombHei7controlEv'];cols[6]='_ZN7BombHei7controlEv';rows[i]=','.join(cols);found+=1
assert found==1
try:
 mp.write_text(''.join(rows))
 check=subprocess.run([sys.executable,'tools/check.py','_ZN7BombHei7controlEv','--object',str(obj)],stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True)
 (out/'check.txt').write_text(check.stdout);meta['checker_exit']=check.returncode;print(check.stdout)
 diag=subprocess.run([sys.executable,'tools/low/checkExactBytes.py','_ZN7BombHei7controlEv',str(obj)],stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True)
 (out/'exact.json').write_text(diag.stdout);meta['diagnostic_exit']=diag.returncode
 r=json.loads(diag.stdout);e=r.get('evidence',{});meta['result']={k:e.get(k) for k in ['compiled_section_size','object_sha256','linked_section_sha256','different_bytes']};meta['result']['reason']=r['reason'];print(json.dumps(meta['result'],indent=2))
 if 'output_directory' in e:
  d=Path(e['output_directory']);shutil.copytree(d,out/'linked',dirs_exist_ok=True)
finally:mp.write_bytes(original)
assert mp.read_bytes()==original
meta['map_restored']=True
(out/'manifest.json').write_text(json.dumps(meta,indent=2)+'\n')
```

<!-- reproduce-file: build/bomb_product_layout/replay.py -->
```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM, UC_HOOK_CODE
from unicorn.arm_const import *
import json, random, struct, sys

root = Path.cwd()
result = json.loads((Path('build/bomb_product_layout') / sys.argv[1] / 'exact.json').read_text())
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
```

## Complete residual instruction diff

```diff
--- retail
+++ baseline_and_form1
@@ -10,30 +10,30 @@
 0030E69C bl #0x337474
 0030E6A0 mov r0, r4
 0030E6A4 bl #0x27c04c
-0030E6A8 vldr s6, [r0, #8]
+0030E6A8 vldr s8, [r0, #8]
 0030E6AC vldr s4, [sp, #4]
-0030E6B0 vldr s10, [r0]
-0030E6B4 vldr s9, [sp]
-0030E6B8 vmul.f32 s0, s4, s6
-0030E6BC vldr s8, [r0, #4]
-0030E6C0 vldr s5, [sp, #8]
-0030E6C4 vnmul.f32 s3, s9, s10
-0030E6C8 vmul.f32 s2, s9, s8
-0030E6CC vmul.f32 s1, s5, s10
-0030E6D0 vldr s7, [sp, #0xc]
-0030E6D4 vmls.f32 s0, s5, s8
-0030E6D8 vmls.f32 s3, s4, s8
+0030E6B0 add r1, sp, #8
+0030E6B4 vldr s9, [r0, #4]
+0030E6B8 vmul.f32 s0, s4, s8
+0030E6BC vldmia r1, {s6, s7}
+0030E6C0 vldr s10, [r0]
+0030E6C4 vldr s5, [sp]
+0030E6C8 vmul.f32 s1, s6, s10
+0030E6CC vmul.f32 s2, s5, s9
+0030E6D0 vnmul.f32 s3, s5, s10
+0030E6D4 vmls.f32 s0, s6, s9
+0030E6D8 vmls.f32 s1, s5, s8
 0030E6DC vmls.f32 s2, s4, s10
-0030E6E0 vmls.f32 s1, s9, s6
+0030E6E0 vmls.f32 s3, s4, s9
 0030E6E4 vmla.f32 s0, s7, s10
-0030E6E8 vmls.f32 s3, s5, s6
-0030E6EC vmla.f32 s2, s7, s6
-0030E6F0 vmla.f32 s1, s7, s8
-0030E6F4 vmul.f32 s6, s0, s7
-0030E6F8 vmls.f32 s6, s1, s5
-0030E6FC vmla.f32 s6, s2, s4
-0030E700 vmls.f32 s6, s3, s9
-0030E704 vstr s6, [r0]
+0030E6E8 vmla.f32 s1, s7, s9
+0030E6EC vmla.f32 s2, s7, s8
+0030E6F0 vmls.f32 s3, s6, s8
+0030E6F4 vmul.f32 s7, s0, s7
+0030E6F8 vmls.f32 s7, s1, s6
+0030E6FC vmla.f32 s7, s2, s4
+0030E700 vmls.f32 s7, s3, s5
+0030E704 vstr s7, [r0]
 0030E708 vldr s4, [sp, #0xc]
 0030E70C vldmia sp, {s5, s6, s7}
 0030E710 vmul.f32 s4, s1, s4
```

## Local evidence

Ignored evidence is frozen in `build/bomb_product_layout/`: baseline/form1 source snapshots, canonical objects, checker outputs, exact-byte JSON, linked outputs, build logs, replay source/results, form1 provenance, the test/header patches, and `hashes.json`. There are37 frozen entries. The publishable deliverable is this report only. No private binary or ignored artifact is committed.
