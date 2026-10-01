# Native matrix packet layouts

Branch: `dot/matrix-native-layouts`. Base: `e2cbf8db72566da78fc16b77a5b90026bfd9ea0a`.

No new exact matches. One new structural form was compiled for Matrix33 makeZero. The two other packets remain blocked at their previous counts because inspection supplied no independently grounded change to their ABI or native record layout. No failed source is added to the final production tree.

## Measured results

| Packet | Retail bytes | Prior distinct forms | New forms | Cumulative forms | Result |
| --- | ---: | ---: | ---: | ---: | --- |
| `0027C510`, Matrix33 makeZero | 28 | 2 | 1 | 3 | Standard complete-record `memset` emits 44 bytes; strict size rejection |
| `0027C198`, Matrix33 + translation to Matrix34 | 32 | 3 | 0 | 3 | Native records and caller ABI reconfirmed; no new admissible form |
| `0027C880`, Matrix34 transposeTo | 60 | 2 | 0 | 2 | Three caller ABIs and shared literal reconfirmed; no new admissible form |

The existing `0027C198` packet's syntax-failed aggregate run is not an additional distinct source form. Its earlier counts remain four paired runs/eight invocations for three valid forms. The `0027C510` packet previously had two paired runs/four invocations. This lane adds one project ARMCC 4.1/791 invocation for that source, giving five compiler invocations cumulatively. The new build also compiles the rest of the project; those compiles are not new attempts at these three packets. No new 894 or 902 experiment was run.

Original EU image SHA256 was checked before work and is unchanged:
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.

The canonical build at source commit `238d709` completed `python make.py eu`, linked `RE-Pepper.axf`, and exported the compact scaffold image. No global, module or per-file compiler flags changed. The candidate was built in the existing `lib/al/src/Math/seadMatrixCopy.cpp` translation unit.

The unchanged checker output was:

```text
python tools/check.py _ZN4sead15Matrix33CalcCtrIfE8makeZeroERN2nn4math5MTX33E --object build/eu/obj/lib/al/src/Math/seadMatrixCopy.o
U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.
```

No byte-distance result is available because complete-section size rejects before linking the candidate interval. The local rank mutation made by the checker was discarded; no map or other acceptance metadata is part of this branch's final diff. No row name, extent, type, section identity or literal-pool identity was changed for this diagnostic.

Affected previously accepted roots in the same translation unit were checked against the committed candidate object:

```text
_ZN4sead15Matrix33CalcCtrIfE4copyERN2nn4math5MTX33ERKS4_
O -> O: The complete source-generated function interval matches byte for byte.
_ZN4sead15Matrix33CalcCtrIfE11transposeToERN2nn4math5MTX33ERKS4_
O -> O: The complete source-generated function interval matches byte for byte.
```

Those are preservation checks, not new matches. No whole-project exact preservation claim is made. The final production source is restored byte-for-byte to the base revision; no headers changed. A subsequent `python make.py eu -ca` from the final source completed clean linking/export, and both accepted Matrix33 roots passed again from its regenerated canonical object. The source files, map, checker, ledger and STATE were also checked byte-identical to the base revision.

## New form and complete compiler output

The hypothesis was a standard memory-clear helper. Retail makeZero writes positive floating zero to every word, and neighboring makeZero routines use the same three-word store groups regardless of matrix row width. A complete-record clear therefore tests an actual independent implementation strategy, unlike another scalar assignment ordering.

The candidate added `<cstring>`, the matching static declaration, and this explicit specialization to the existing source:

```cpp
template <>
void Matrix33CalcCtr<float>::makeZero( nn::math::MTX33& output )
{
        std::memset( &output, 0, sizeof(output) );
}
```

`MTX33` remains the existing `struct { float m[3][3]; }`, 36 bytes. The full committed candidate source can be recovered from `238d709:lib/al/src/Math/seadMatrixCopy.cpp`; its SHA256 is `d4c8153494a2599670c45c74c8493e56b161cf7e66b04d5d13d60f1ea0b47380`. The canonical object SHA256 was `7a5cc830b9dc5f6c2a3f50d76317d914a470e3f67b3bf3bdae49b9dc033635d2`. Object hashes include local path-dependent build material and are evidence identifiers, not portable acceptance claims.

The complete 44-byte emitted section, with no literal pool or relocations in this function, was:

```text
0000 mov r1, #0
0004 str r1, [r0]
0008 str r1, [r0, #4]
000C str r1, [r0, #8]
0010 str r1, [r0, #0xc]
0014 str r1, [r0, #0x10]
0018 str r1, [r0, #0x14]
001C str r1, [r0, #0x18]
0020 str r1, [r0, #0x1c]
0024 str r1, [r0, #0x20]
0028 bx lr
```

This is the same scalar-store shape as the earlier aggregate-zero form, despite the distinct source strategy. It does not supply a reason to repeat zero-source syntax variants. No bounded execution verification or new functionally verified NonMatching implementation is claimed.

## Native records and ABI evidence

The accepted copy/transpose siblings and the target intervals agree on ordinary contiguous records: MTX33 has nine floats/36 bytes; MTX34 has twelve floats/48 bytes; MTX44 has sixteen floats/64 bytes; VEC3 has three floats/12 bytes. The Matrix33-to-Matrix34 conversion inserts translation at destination offsets 12, 28 and 44. There is no observed padding, alignment gap, hidden receiver, or fourth source row to explain the mismatch.

At retail `00167950`, the direct caller of `0027C198` constructs R1 as resource pointer + 0x10 (basis), R2 as that pointer + 4 (translation), and R0 from its destination register. Immediately after the call it overwrites R1 and R0 and proceeds with another resource field. This independently supports the existing three-reference ABI; it supplies no evidence of a return contract to impose on the candidate. A scan of mapped ARM function intervals found this one direct BL call to the conversion. This is not a claim that no indirect or tail-call uses exist.

The three direct BL callers of `0027C880`, at `001C00A4`, `0024DE20`, and `002E49C0`, pass separate stack destinations and read/use those matrices after the call. None consumes R0 as a function result. This reconfirms the packet's void-signature choice but does not prove an original source return type, which is absent from the mangling.

## Adjacent kernel evidence

The neighboring copy family explains the desired register packing without changing record layout:

- `0027C178` loads all twelve MTX34 floats, including its translation slots, then writes three three-float basis groups to MTX33
- `0027C198` loads three MTX33 rows into register groups with gaps for the translations, fills those gaps from VEC3, and stores twelve contiguous floats
- `0027C1E4` repeats that exact basis/translation sequence and additionally loads VEC4 into the final four registers for MTX44
- `0027CA30` (in-place MTX44 transpose) loads fifteen floats even though several diagonal values are not stored, while `0027CA68` splits a sixteen-float load after twelve words

These patterns are consistent with a shared low-level kernel organization. They do not establish an alternative C++ record layout or license artificial liveness, volatile reads, ABI return constraints, inline assembly, or byte stand-ins.

The zero family is especially diagnostic:

- MTX33 makeZero (`0027C510`) initializes R1/R2/R3 separately to zero and makes three three-word stores
- MTX34 makeZero (`0027C8BC`) uses the same registers and four three-word stores, despite its four-float row stride
- MTX44 makeZero (`0027CAB4`) uses the same registers for five three-word stores and one last scalar word
- MTX34 and MTX44 makeIdentity (`0027C8DC`/`0027CADC`) reuse R1=0x3f800000, R2=0 and R3=0 across alternating three-word and two-word stores that cross row boundaries

The three-word chunk is consequently not evidence for three-member MTX34 rows. Making native record layouts follow those chunks would contradict the copy, transpose, and consumer offsets. Adding fake wide members or forcing three live zero values would have no independent ABI justification.

## Remaining concrete blockers

For makeZero, the standard helper hypothesis was tested and rejected. The native record is already established; the missing fact is an admissible C++ or compiler-stage explanation for the repeated three-register stores. Another aggregate, scalar, or row-loop permutation is not a new explanation.

For the basis-plus-translation copy, the original native register placement and postincrement row loads are established, but ordinary snapshot/aggregate forms in the packet do not reproduce them. No independently identified native constructor, inlined helper contract, or native record distinction was found in the permitted evidence. Inventing such a contract solely to shape registers would not resolve that gap.

For Matrix34 transpose, the zero load at `0027C884` addresses `0027C798`, inside the preceding native inverseTranspose interval (`0027C674..0027C79C`), not a separately mapped data object. The target interval owns no zero literal. Any new zero-bearing standalone compiler section still has to solve the real shared-literal ownership/closure problem in addition to register packing. The original target and map remain untouched; no fabricated pool alias was introduced.

Reopen these packets when there is independently grounded native helper/inlining or shared-literal compiler-stage evidence. They remain below the attempt cap; stopping here avoids spending those remaining attempts on the already rejected scalar/aggregate family. The complete measured result is zero exact additions and two preserved existing roots.
