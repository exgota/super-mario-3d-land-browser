# Partial response to main packet 0016A11C

Main packet at a059251507b899d48a6ab58b564cfa8a6d3d360f is preserved unchanged. This dot-produced followup records local reconstruction/checking, not an external Pro answer. The hard init target remains unresolved.

# CourseList init: request for source/codegen review

Target 0x0016A11C–0x0016A2B4. 404 compiled bytes versus 408; canonical U -> M size mismatch. Guarded NonMatching; no exact claim. Checker/base a5041a5091efbf53fdbf99c6950133f2ec41e998; source checkpoint1a1dcfe.

ARMCC 4.1 build 791. Canonical flags: `-DVERSION=EU -DNN_SWITCH_DISABLE_ASSERT_WARNING_FOR_SDK=1 -DNN_SWITCH_DISABLE_DEBUG_PRINT_FOR_SDK=1 -DNON_MATCHING=1 --cpu=MPCore --fpmode=fast --apcs=/interwork --depend_format=unix_quoted --diag_suppress=1608 --arm_only --no_exceptions --diag_style=gnu --arm_only --no_exceptions --diag_style=gnu --signed_chars --dollar --force_new_nothrow --no_rtti --no_debug_macros --no_depend_system_headers -O3 -Otime --gnu --split_sections --force_new_nothrow --multibyte_chars --enum_is_int --signed_chars --no_rtti_data --forceinline --remove_unneeded_entities --no_debug --preinclude=/workspace/scratch/73cdb2c524af/mario-latest/Game/project_globals.h --sys_include -c -D__BASE_FILE_NAME__="CourseList.cpp" -o --depend`. Include roots: Game/backup/include, lib/al/include, lib/CtrSDK/include, lib/sead/include. No flags changed. No scratch object establishes acceptance.

## Original interval (owner-supplied binary; disassembly only)
```asm
0016A11C: push {r4, r5, r6, r7, r8, lr}
0016A120: mov r6, r0
0016A124: sub sp, sp, #0x30
0016A128: mov r4, r1
0016A12C: mov r0, #8
0016A130: bl #0x2932b0
0016A134: cmp r0, #0
0016A138: mov r8, #0
0016A13C: moveq r5, #0
0016A140: beq #0x16a1d4
0016A144: mov r5, r0
0016A148: ldr r0, [pc, #0x158]
0016A14C: ldr r1, [pc, #0x158]
0016A150: mov r7, #1
0016A154: str r0, [sp, #4]
0016A158: str r1, [sp]
0016A15C: mov r1, sp
0016A160: mov r0, r4
0016A164: bl #0x290640
0016A168: mov r1, r0
0016A16C: add r0, sp, #0x20
0016A170: bl #0x2905f0
0016A174: mov r4, r0
0016A178: str r8, [r5]
0016A17C: add r0, sp, #0x10
0016A180: str r8, [r5, #4]
0016A184: bl #0x2910e8
0016A188: ldr r2, [pc, #0x120]
0016A18C: add r1, sp, #0x10
0016A190: mov r0, r4
0016A194: bl #0x290fb0
0016A198: cmp r0, #0
0016A19C: beq #0x16a1d4
0016A1A0: add r0, sp, #0x10
0016A1A4: bl #0x2910f8
0016A1A8: cmp r0, #0
0016A1AC: str r0, [r5, #4]
0016A1B0: beq #0x16a1d4
0016A1B4: lsl r0, r0, #2
0016A1B8: mov r1, r4
0016A1BC: bl #0x292a78
0016A1C0: str r0, [r5]
0016A1C4: ldr r0, [r5, #4]
0016A1C8: mov r4, #0
0016A1CC: cmp r0, #0
0016A1D0: bgt #0x16a1e0
0016A1D4: mov r7, #0
0016A1D8: stm r6, {r5, r8}
0016A1DC: b #0x16a290
0016A1E0: add r0, sp, #0x18
0016A1E4: bl #0x2910e8
0016A1E8: mov r2, r4
0016A1EC: add r1, sp, #0x18
0016A1F0: add r0, sp, #0x10
0016A1F4: bl #0x29107c
0016A1F8: cmp r0, #0
0016A1FC: ldreq r0, [r5]
0016A200: streq r8, [r0, r4, lsl #2]
0016A204: beq #0x16a228
0016A208: mov r1, r4
0016A20C: mov r0, #0xc
0016A210: bl #0x2932b0
0016A214: cmp r0, #0
0016A218: addne r1, sp, #0x18
0016A21C: blne #0x325b44
0016A220: ldr r1, [r5]
0016A224: str r0, [r1, r4, lsl #2]
0016A228: ldr r0, [r5, #4]
0016A22C: add r4, r4, #1
0016A230: cmp r0, r4
0016A234: bgt #0x16a1e0
0016A238: b #0x16a1d4
0016A23C: ldr r0, [r0]
0016A240: mov r4, #0
0016A244: ldr r5, [r0, r7, lsl #2]
0016A248: ldr r0, [r5, #4]
0016A24C: cmp r0, #0
0016A250: ble #0x16a28c
0016A254: ldr r0, [r5]
0016A258: ldr r0, [r0, r4, lsl #2]
0016A25C: ldr r0, [r0]
0016A260: bl #0x25bd18
0016A264: cmp r0, #0
0016A268: nop 
0016A26C: beq #0x16a27c
0016A270: ldr r0, [r6, #4]
0016A274: add r0, r0, #1
0016A278: str r0, [r6, #4]
0016A27C: ldr r0, [r5, #4]
0016A280: add r4, r4, #1
0016A284: cmp r0, r4
0016A288: bgt #0x16a254
0016A28C: add r7, r7, #1
0016A290: ldr r0, [r6]
0016A294: ldr r1, [r0, #4]
0016A298: cmp r1, r7
0016A29C: bgt #0x16a23c
0016A2A0: add sp, sp, #0x30
0016A2A4: pop {r4, r5, r6, r7, r8, pc}
0016A2A8: .word 0x003A294C
0016A2AC: .word 0x003D9C3C
0016A2B0: .word 0x003A2944
```

## Complete retained source
```cpp
#include "System/CourseList.h"


#include <Resource/alResource.h>
#include <System/Application.h>
#include <Util/alStringUtil.h>
#include <Yaml/alByamlIter.h>

#include "System/GameSystem.h"
#include "System/RootTask.h"

// Source-defined strings from the EU course list pool. Type, Normal and
// Miniature share an address anchor because the constructor derives Normal
// from Miniature - 8. Other strings retain their independently mapped rows.
static const char force_section( ".sdata_dat_003A2884" ) s_koopaCastle[12] = "KoopaCastle";
static const char force_section( ".sdata_dat_003A2890" ) s_type[8] = "Type";
static const char force_section( ".sdata_dat_003A2890" ) s_normal[8] = "Normal";
static const char force_section( ".sdata_dat_003A2890" ) s_miniature[12] = "Miniature";
static const char force_section( ".sdata_dat_003A28AC" ) s_koopaFortress[16] = "KoopaFortress";
static const char force_section( ".sdata_dat_003A28BC" ) s_koopaBattleShip[16] = "KoopaBattleShip";
static const char force_section( ".sdata_dat_003A28CC" ) s_championship[16] = "Championship";
static const char force_section( ".sdata_dat_003A28DC" ) s_kinopioHousePresent[20] = "KinopioHousePresent";
static const char force_section( ".sdata_dat_003A28F0" ) s_kinopioHouseAlbum[20] = "KinopioHouseAlbum";
static const char force_section( ".sdata_dat_003A2904" ) s_mysteryBox[12] = "MysteryBox";
static const char force_section( ".sdata_dat_003A2910" ) s_dokan[8] = "Dokan";
static const char force_section( ".sdata_dat_003A2918" ) s_empty[8] = "Empty";
static const char force_section( ".sdata_dat_003A2920" ) s_collectCoinNum[16] = "CollectCoinNum";
static const char force_section( ".sdata_dat_003A2930" ) s_scenario[12] = "Scenario";
static const char force_section( ".sdata_dat_003A293C" ) s_stage[8] = "Stage";
static const char force_section( ".sdata_dat_003A2944" ) s_worlds[8] = "Worlds";
struct CourseListResourceName
{
        char name[11];
        unsigned char padding;
};
static const CourseListResourceName force_section( ".sdata_dat_003A294C" ) s_courseList = {
        "CourseList", 0xFF
};
static_assert( sizeof( CourseListResourceName ) == 12, "CourseList name and alignment byte" );


CourseList::Course::Course( const al::ByamlIter* course )
    : mCourseType( CourseType_Normal ), mStageName( nullptr ), mScenario( -1 ),
      mMiniatureModelName( s_miniature ), mCoinCollectNum( 0 )

{
        const char* type = s_normal;
        course->tryGetStringByKey( &type, s_type );

        if ( al::isEqualString( type, s_koopaCastle ) )
                mCourseType = CourseType_KoopaCastle;
        else if ( al::isEqualString( type, s_koopaFortress ) )
                mCourseType = CourseType_KoopaFortress;
        else if ( al::isEqualString( type, s_koopaBattleShip ) )
                mCourseType = CourseType_KoopaBattleShip;
        else if ( al::isEqualString( type, s_championship ) )
                mCourseType = CourseType_Championship;
        else if ( al::isEqualString( type, s_kinopioHousePresent ) )
                mCourseType = CourseType_KinopioHousePresent;
        else if ( al::isEqualString( type, s_kinopioHouseAlbum ) )
                mCourseType = CourseType_KinopioHouseAlbum;
        else if ( al::isEqualString( type, s_mysteryBox ) )
                mCourseType = CourseType_MysteryBox;
        else if ( al::isEqualString( type, s_dokan ) )
                mCourseType = CourseType_Dokan;
        else if ( al::isEqualString( type, s_empty ) )
                mCourseType = CourseType_Empty;

        course->tryGetStringByKey( &mStageName, s_stage );
        course->tryGetIntByKey( &mScenario, s_scenario );
        course->tryGetStringByKey( &mMiniatureModelName, s_miniature );
        course->tryGetIntByKey( &mCoinCollectNum, s_collectCoinNum );
}

#ifdef NON_MATCHING
CourseList::World::World( const al::ByamlIter* world )
    : mCourses( nullptr ), mNumCourses( 0 ), mIsSpecialWorld( false )
{
        const char* type = "World";
        if ( world->tryGetStringByKey( &type, "Type" ) )
        {
                mIsSpecialWorld = al::isEqualString( type, "Special" );
                al::ByamlIter listIter;
                if ( world->tryGetIterByKey( &listIter, "Course" ) )
                {
                        mNumCourses = listIter.getSize();
                        if ( mNumCourses )
                        {
                                mCourses = new Course*[ mNumCourses ];
                                for ( int i = 0; i < mNumCourses; i++ )
                                {
                                        al::ByamlIter courseIter;
                                        if ( listIter.tryGetIterByIndex( &courseIter, i ) )
                                                mCourses[ i ] = new Course( &courseIter );
                                        else
                                                mCourses[ i ] = nullptr;
                                }
                        }
                }
        }
}


CourseList::List::List( const al::ByamlIter& courseListIter ) : mWorlds( nullptr ), mNumWorlds( 0 )
{
        al::ByamlIter worlds;
        if ( courseListIter.tryGetIterByKey( &worlds, s_worlds ) )
        {
                mNumWorlds = worlds.getSize();
                if ( mNumWorlds )
                {
                        mWorlds = new World*[ mNumWorlds ];
                        for ( int i = 0; i < mNumWorlds; i++ )
                        {
                                al::ByamlIter curWorld;
                                if ( worlds.tryGetIterByIndex( &curWorld, i ) )
                                        mWorlds[ i ] = new World( &curWorld );
                                else
                                        mWorlds[ i ] = nullptr;
                        }
                }
        }
}

#endif

CourseList::CourseList() : mCourseList( 0 )
{
        init( al::findOrCreateResource( "ObjectData/GameSystemDataTable" ) );
}

#ifdef NON_MATCHING
void CourseList::init( const al::Resource* gameSystemDataTable )
{
        mCourseList = new List( al::ByamlIter( gameSystemDataTable->getByml( s_courseList.name ) ) );
        mNumStages  = 0;

        for ( int i = 0; i < mCourseList->mNumWorlds; i++ )
        {
                World* world = mCourseList->mWorlds[ i ];
                for ( int j = 0; j < world->mNumCourses; j++ )
                {
                        Course* course = world->mCourses[ j ];
                        if ( Course::isCourseTypeStage( Course::CourseType( course->mCourseType ) ) )
                                mNumStages++;
                }
        }
}
#endif

CourseList* rp::getCourseList()
{
        return al::getApplication()->getRootTask()->getGameSystem()->getCourseList();
}

```

## Layout declarations
```cpp
#pragma once

namespace al
{
class Resource;
class ByamlIter;
} // namespace al

class CourseList
{
public:
        void init( const al::Resource* gameSystemDataTable );

private:
        struct Course
        {
                enum CourseType
                {
                        CourseType_Normal,
                        CourseType_KoopaCastle,
                        CourseType_KoopaFortress,
                        CourseType_KoopaBattleShip,
                        CourseType_Championship,
                        CourseType_KinopioHousePresent,
                        CourseType_KinopioHouseAlbum,
                        CourseType_MysteryBox,
                        CourseType_Dokan,
                        CourseType_Empty = 10
                };

                int         mCourseType;
                const char* mStageName;
                int         mScenario;
                const char* mMiniatureModelName;
                int         mCoinCollectNum;

                Course( const al::ByamlIter* course );

                static bool isCourseTypeStage( CourseType type );
        };

        struct World
        {
                Course** mCourses;
                int      mNumCourses;
                bool     mIsSpecialWorld;

                World( const al::ByamlIter* world );
        };

        struct List
        {
                World** mWorlds;
                int     mNumWorlds;

                List( const al::ByamlIter& courseListIter );
        };

        List* mCourseList;
        int   mNumStages;

public:
        CourseList();
};

namespace rp
{

CourseList* getCourseList();

} // namespace rp

```

## Evidence and attempts


The List allocation at `0x0016A12C` requests 8 bytes. Its world pointer table is at offset 0 and signed count at offset 4. The World allocation at `0x0016A20C` requests 12 bytes. Its constructor writes the course pointer table at offset 0, signed count at offset 4 and special-world flag byte at offset 8. The Course allocation at `0x00325C20` requests 20 bytes. The inline constructor initializes type 0, stage name null, scenario -1, model-name default Miniature and coin count 0, with five successive four-byte fields. All existing source layouts agree.

The World constructor requires a valid Type string before it processes Course. It compares Type with Special, then allocates and initializes the Course pointer array. Failed indexed-child lookup writes a null entry. Course type comparisons are ordered KoopaCastle=1, KoopaFortress=2, KoopaBattleShip=3, Championship=4, KinopioHousePresent=5, KinopioHouseAlbum=6, MysteryBox=7, Dokan=8 and Empty=10; unmatched values retain Normal=0. It then reads Stage, Scenario, Miniature and CollectCoinNum in that order.

The List constructor likewise looks up Worlds, allocates a pointer array and initializes each entry, preserving null entries on failed lookup. `init` counts courses with type at most Championship after construction. The existing helper at `0x0025BD18` uses an unsigned range comparison, accepting values 0 through 4 and rejecting negative enum bit patterns; the four-instruction leaf does not require a data import. The inherited signed comparison was corrected after the final readback described below.

The previously same-translation-unit `isCourseTypeStage` definition let ARMCC exploit its known register preservation despite `no_inline`, producing caller-saved loop registers unlike the retail caller. Its separate source file preserves the ordinary external call boundary without adding a guessed import or changing its implementation.




The parent committed checkpoint 1 as `a69ad8d` and ran the canonical build. Both strict checks reported `U -> M` because the complete section sizes differ: init is 404 bytes, World 644 bytes. The compiled 212-byte `.sdata_dat_003A2884` pool equals all corresponding retail bytes; both copies have SHA-256 `6977bc957a7bbf3f85a4155ad379869b6f43cd0f32eff93ed509568a06b233e5`. This data comparison is independent of code acceptance.

The single aggregate lets ARMCC retain one base address and derive most strings with additions, unlike the retail body. Diagnostic source probes therefore split the pool into fully initialized string arrays at their existing row anchors, keeping Type, Normal and Miniature together in one 28-byte section at `0x003A2890`. That section spans the existing Type/Normal row and Miniature row without changing either boundary. Each other string has its own address-encoded section; the final CourseList name uses a 12-byte struct containing its 11-byte terminated name and the observed 0xFF pad. All source data remains complete. This supersedes the single-aggregate representation described above for the next checkpoint.

The split arrays restore World's 692-byte baseline register allocation and its exact Normal = Miniature - 8 relationship. The independently inspected split compiled data equals the corresponding retail bytes. A diagnostic inclusion of the approved ARMCC `<new>` header had no effect on allocation arguments and is not retained. No artificial nothrow tag, heap argument, pointer cast or instruction-shaped expression was added.

Checkpoint 2 moves the List constructor into an ordinary separate translation unit. The retail init body shows the same sort of late-inlining differences previously recovered elsewhere in this project. This tests the existing canonical source-closure mechanism without assigning the List constructor an original address. Its complete definition remains source code; the checker must reject it if residual helper code remains.

The diagnostics used the same recorded compiler command with only source, output and dependency paths redirected to `/tmp`; ARMCC41INC was set to the approved compiler's include directory as in the normal build. An initial missing ARMCC41INC caused header-not-found errors before compilation. None of these scratch outputs counts as a match or replaces a canonical object.

### Checkpoint 2 result and final source form

The parent committed checkpoint 2 as `ae5aa23` and built it canonically. World's strict check reported `M -> M` for the 692-versus-700-byte section-size difference. The init source closure was rejected with `Residual helper code or another allocated extent remains after inlining.` Its compiled root is 240 bytes and the separately compiled List constructor is 208 bytes. The linker did not produce the original single 408-byte extent. The separate-constructor experiment is therefore not retained: List returns to the same translation unit in the final source form.

All fifteen individual compiled string sections from checkpoint 2 independently matched their corresponding original intervals byte for byte. Together they cover every byte of `0x003A2884–0x003A2958` without gaps or overlaps and retain the aggregate SHA-256 `6977bc957a7bbf3f85a4155ad379869b6f43cd0f32eff93ed509568a06b233e5`. Their sizes in address order are 12, 28, 16, 16, 16, 20, 20, 12, 8, 8, 16, 12, 8, 8 and 12 bytes. The 28-byte Type/Normal/Miniature section is the sole section spanning two original data rows; its first row is an address anchor only. The remaining sections exactly fit their original data rows. All meaningful string contents and observed padding are reconstructed, rather than merely importing an undersized placeholder.

The large functions remain correct semantic reconstructions and NonMatching proposals. World's residual code-generation issue is exactly the two retail allocator-argument moves plus the resulting branch/literal displacements. Both runtime allocators ignore their tag argument, but no unsupported explicit tag or fabricated argument was added merely to force those moves. init retains additional allocation/null-path, local-lifetime and loop-control scheduling differences. The same-translation-unit allocator header experiment did not resolve these differences, and the tested separate-constructor late-inline hypothesis failed the strict closure guard.


### Final canonical result

The parent committed the final large-function/source-data checkpoint as `7b00157` and ran the canonical ARMCC build and strict checks:

- `_ZN10CourseListC1Ev`, `0x0016A2B4–0x0016A31C`, 104 bytes: `U -> O: The complete source-generated function interval matches byte for byte.` This is the single exact function established in this family at this checkpoint.
- `_ZN10CourseList4initEPKN2al8ResourceE`, 404 compiled bytes versus 408 original bytes: `M -> M: The complete compiled section, including its literal pool, has a different size from the original interval.`
- `_ZN10CourseList5WorldC1EPKN2al9ByamlIterE`, 692 compiled bytes versus 700 original bytes: the same `M -> M` size-mismatch result.
- `_ZN10CourseList6Course17isCourseTypeStageENS0_10CourseTypeE`, 16 bytes: `m -> m: The linked candidate differs from the unchanged original interval.` It is not an exact match.

The final canonical CourseList.o again passed a complete read-only comparison of all fifteen string-data sections: 212 bytes, contiguous from `0x003A2884` to `0x003A2958`, aggregate SHA-256 `6977bc957a7bbf3f85a4155ad379869b6f43cd0f32eff93ed509568a06b233e5`. Its source provenance records CourseList.cpp SHA-256 `07b7eb7c46aaaa9a6b4cf0ba31bd3656d781ccbe319a90794cc38a3003fda0b6`.

The leaf mismatch prompted a final retail readback. Its actual instructions use unsigned HI/LS after comparison with 4. The inherited C++ used signed LE/GT, incorrectly treating negative enum bit patterns as stages. The final source changes only that semantic comparison to an unsigned cast and restores the leaf's `NON_MATCHING` guard. This correction is independent of the exact CourseList constructor, whose canonical input files and behavior are unchanged. No additional leaf match is claimed unless a subsequent parent check establishes it. The separate leaf source remains necessary to retain the retail external-call optimization boundary in init.

No further code-generation experiments are proposed for the two large functions in this pass. All constructor/traversal logic, class extents, course types, string contents, data padding and external call identities now have explicit retail evidence; their remaining mismatches require a new compiler or source-lifetime hypothesis. No tools, map intervals, compiler flags, binaries, maps or project ledgers were edited by this lane.

## Latest-main verification (2026-10-01)

Canonical checker/build base: `a5041a5091efbf53fdbf99c6950133f2ec41e998`. Source was committed locally as `1a1dcfe` before `python make.py eu`. No checker changes were made. The full compact build linked and exported successfully. The final unsigned leaf now matches; this supersedes its earlier mismatch above.

Commands (from the repository, with the approved toolchain environment):
```sh
python make.py eu
.venv/bin/python tools/check.py _ZN10CourseListC1Ev --object build/eu/obj/Game/backup/src/System/CourseList.o
.venv/bin/python tools/check.py _ZN10CourseList6Course17isCourseTypeStageENS0_10CourseTypeE --object build/eu/obj/Game/backup/src/System/CourseListCourse.o
.venv/bin/python tools/check.py _ZN10CourseList4initEPKN2al8ResourceE --object build/eu/obj/Game/backup/src/System/CourseList.o
.venv/bin/python tools/check.py _ZN10CourseList5WorldC1EPKN2al9ByamlIterE --object build/eu/obj/Game/backup/src/System/CourseList.o
```
Both first checks: `U -> O: The complete source-generated function interval matches byte for byte.` Exact subtotal: 104 + 16 = 120 bytes. Both last checks: `U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.` Those 408/700-byte targets are not accepted and stay guarded.

Local-only map changes name existing imports listed above, including Resource::getByml and ByamlIter::tryGetStringByKey. No row boundaries changed. Main must adopt/revalidate these independently established identities before importing the proposal. No map, ledger, STATE, tools, or binaries are included. `tools/diff.py` was unavailable because its dependency setup was not permitted; source diagnostics do not replace canonical `check.py` acceptance. See the two address-named packets for the unresolved source/codegen questions.

Question: identify a defensible source/lifetime/compiler explanation for the remaining mismatch. Do not change flags, target boundaries, oracle, or use assembly/byte arrays. Canonical tools/diff.py is unavailable; the strict object check above remains authoritative.
