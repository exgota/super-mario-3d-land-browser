# Fugumannen rail-direction prerequisite

Owned existing Fugumannen.cpp atd7350e2, recordedcd45814. All dedicated/shared headers unchanged. No committed NN/sead source/header currently defines nn::math::VEC3 or sead::Vector3CalcCtr, so these source-local declarations do not conflict with an existing definition. The qualified names come from the already committed original multScalar symbol0027CC64. Its provider loads three floats through r1, multiplies by s0, stores three floats through r0 and returns. Other original consumer00240E30 uses this same provider with a12-byte stack vector at00240EDC..00240EFC, then copies three integer words to another vector before using named add0027CB48. This independently witnesses three contiguous floats, references in r0/r1 and float scalar in s0. No implementation, alias or guessed method address is supplied for multScalar.

Both Fugumannen target callbacks originally receive NerveKeeper and load its host from the first word. The Move callback reads actor+60, already the typed speed field grounded by the accepted0x68 ctor/init. They copy three words from alreadyO getRailDir0027C140 to a12-byte local, conditionally call the named multScalar(-1.0f), then call neutral00240E08 with actor/direction reference/scalar4.0f. Provider00240E08 saves r1 and s0, obtains a host direction via0027F3B4, restores r1/s0 and falls through into00240E30. That continuation explicitly consumes three words from the supplied reference. The source-local anonymous import uses this observed nn::math::VEC3 float3-reference ABI; no public original type name is claimed for the neutral wrapper. Only this owned CPP declares it. No aliasing cast or layout prefix is used.

The two preexisting unaccepted exeMove/exeMove2 source helpers are made inline, so their intended logic expands into the two Nerve targets and their unsupported standalone emissions must disappear. Root explicitly included this intentional helper closure; they add zero original roots/bytes. Every other18-symbol baseline contract remains protected. No accepted root or shared math header changes. Component initialization uses ordinary typed C++ and the real named external scalar provider.

Target names/rows, state-object names and class/table identities already exist in the frozen map. No identity/rank/boundary or data proposal is needed. Copies of complete original target/provider/independent-consumer disassemblies are in the prior read-only screen package. Four unsuccessful meaningful forms max; all physical failures remain distinct and retained. No scratch exact credit.

## Second ordinary source form

The source-local VEC3 uses one existing clean sead::Vector3f member. This ordinary composition is twelve bytes with four-byte alignment and three consecutive float words, the independently observed external-reference storage ABI. Only the member type's default copy transfers the three words; there is no cast, alias, union, byte array, assembly, imported layout implementation or shared-header edit. The source-local member spelling is reconstruction vocabulary, not a claim that it was the original NN public field name. The qualified VEC3 and Vector3CalcCtr names remain the independently named original provider types. The first component-copy form remains frozen separately.

The Move callback's provider at00273E28 already has the committed neutral identity fn_00273e28; the prior shared-header spelling al::moveSyncRailTurn has no established map identity and is not used. Original00273E28..00273E5C/52 pushes r4-r6/lr, saves the actor in r4, sets s1 to a literal zero and calls0027CD3C with the original scalar in s0. It then follows actor+40 to the rail keeper, rail keeper+4 to the rail state, adds4 to its tangent and calls0027F394 with the actor and this tangent reference. This provider independently establishes actor-in-r0 and speed-in-s0 inputs. The Move callback loads the scalar from its already accepted actor+60 speed field. The return is unused; the neutral declaration preserves the existing void caller contract without assigning semantics to a possible returned register. This is a whole-row existing import, not an alias or address rescue. The original provider interval and its four-byte zero pool are retained as separate evidence.

## Original complete provider and independent consumer windows

### 0027CC64

```text
0027CC64 vldmia   r1, {s1, s2, s3}
0027CC68 vmul.f32 s4, s1, s0
0027CC6C vmul.f32 s5, s2, s0
0027CC70 vmul.f32 s6, s3, s0
0027CC74 vstmia   r0, {s4, s5, s6}
0027CC78 bx       lr
Pool: 
```

### 00240E08

```text
00240E08 push     {r4, lr}
00240E0C mov      r4, r1
00240E10 vpush    {d8}
00240E14 vmov.f32 s16, s0
00240E18 bl       #0x27f3b4
00240E1C vmov.f32 s0, s16
00240E20 vpop     {d8}
00240E24 mov      r1, r4
00240E28 pop      {r4, lr}
00240E2C mov      r0, r0
Pool: 
```

### 00240E30

```text
00240E30 push     {r4, r5, lr}
00240E34 mov      r5, r0
00240E38 vpush    {d8, d9}
00240E3C sub      sp, sp, #0x54
00240E40 vmov.f32 s18, s0
00240E44 ldr      r2, [r1]
00240E48 ldrd     r0, r1, [r1, #4]
00240E4C vldr     s16, [pc, #0x10c]
00240E50 str      r2, [sp, #0x14]
00240E54 strd     r0, r1, [sp, #0x18]
00240E58 vstr     s16, [sp, #0x18]
00240E5C add      r0, sp, #0x14
00240E60 bl       #0x27d5c4
00240E64 cmp      r0, #0
00240E68 movne    r0, #1
00240E6C bne      #0x240f54
00240E70 ldr      r2, [pc, #0xec]
00240E74 mov      r1, r5
00240E78 mov      r0, sp
00240E7C bl       #0x278a24
00240E80 vldr     s1, [sp]
00240E84 vldr     s6, [sp, #0x14]
00240E88 vldr     s4, [sp, #4]
00240E8C vldr     s5, [sp, #0x18]
00240E90 vmul.f32 s1, s1, s6
00240E94 vldr     s2, [sp, #8]
00240E98 vldr     s3, [sp, #0x1c]
00240E9C vldr     s17, [pc, #0xc4]
00240EA0 mov      r0, sp
00240EA4 add      r1, sp, #0x14
00240EA8 vmov.f32 s0, s17
00240EAC vmla.f32 s1, s4, s5
00240EB0 vmla.f32 s1, s2, s3
00240EB4 vcmpe.f32 s1, s16
00240EB8 vmrs     apsr_nzcv, fpscr
00240EBC bge      #0x240f0c
00240EC0 bl       #0x27a724
00240EC4 cmp      r0, #0
00240EC8 beq      #0x240f0c
00240ECC ldr      r2, [pc, #0x98]
00240ED0 mov      r1, r5
00240ED4 add      r0, sp, #0x44
00240ED8 bl       #0x278a24
00240EDC vmov.f32 s0, s17
00240EE0 add      r4, sp, #0x14
00240EE4 add      r1, sp, #0x44
00240EE8 add      r0, sp, #0x38
00240EEC bl       #0x27cc64
00240EF0 add      r2, sp, #0x38
00240EF4 ldm      r2, {r0, r1, r3}
00240EF8 add      r2, sp, #0x20
00240EFC stm      r2, {r0, r1, r3}
00240F00 mov      r1, r4
00240F04 mov      r0, r1
00240F08 bl       #0x27cb48
00240F0C vldr     s0, [pc, #0x5c]
00240F10 mov      r1, r5
00240F14 add      r3, sp, #0x14
00240F18 vmul.f32 s0, s18, s0
00240F1C mov      r2, sp
00240F20 mov      r0, r1
00240F24 bl       #0x257b08
00240F28 vldr     s0, [pc, #0x44]
00240F2C vstr     s16, [sp, #0x2c]
00240F30 vstr     s0, [sp, #0x30]
00240F34 mov      r4, r0
00240F38 mov      r1, r5
00240F3C add      r2, sp, #0x2c
00240F40 vstr     s16, [sp, #0x34]
00240F44 vldr     s0, [pc, #0x2c]
00240F48 mov      r0, r1
00240F4C bl       #0x26a7e8
00240F50 mov      r0, r4
00240F54 add      sp, sp, #0x54
00240F58 vpop     {d8, d9}
00240F5C pop      {r4, r5, pc}
Pool: 00000000e00543000ad7233cc805430035fa8e3c0000803fcdcc4c3e
```

### 0027C140

```text
0027C140 ldr      r0, [r0, #0x40]
0027C144 ldr      r0, [r0, #4]
0027C148 add      r0, r0, #0x10
0027C14C bx       lr
Pool: 
```

### 00273E28

```text
Original EU provider, existing neutral identity fn_00273e28
Range: 0x00273E28..0x00273E5C/52, pool0x00273E58
Oracle sha256: e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64
00273E28: 70402de9  push {r4, r5, r6, lr}
00273E2C: 0040a0e1  mov r4, r0
00273E30: 080adfed  vldr s1, [pc, #0x20]
00273E34: c02300eb  bl #0x27cd3c
00273E38: 401094e5  ldr r1, [r4, #0x40]
00273E3C: 0050a0e1  mov r5, r0
00273E40: 0400a0e1  mov r0, r4
00273E44: 041091e5  ldr r1, [r1, #4]
00273E48: 041081e2  add r1, r1, #4
00273E4C: 502d00eb  bl #0x27f394
00273E50: 0500a0e1  mov r0, r5
00273E54: 7080bde8  pop {r4, r5, r6, pc}
00273E58: 00000000  literal +0.0f
```
