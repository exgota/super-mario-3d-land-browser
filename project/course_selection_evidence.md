# Course selection map update

The recovered C++ function fn_00159fec covers the unchanged retail interval
0x00159FEC-0x0015A2C0, 724 bytes. Its literal pool starts at 0x0015A288.
The two source files are committed under Game/backup's Layout source and
header directories. The diagnostic lane changed no production files; root
integration establishes only the existing function and constructor identities.

## Ownership and layout

The constructor at 0x0015A36C calls the known al::LayoutActor constructor
at 0x0027E648 and loads the resource name "CourseSelectMap". It creates a
marker group from "CourseSelectMapButton", sets up Point1-Point8 markers,
and initializes PicMedal1-PicMedal8 panes. The owner constructor at
0x0018899C allocates 0x68 bytes at 0x00188A10 and calls this constructor at
0x00188A2C. This establishes the representation's total allocation size.
The original C++ class name is not established. CourseSelectMapLayout is
an explicit recovered representation, not an asserted retail identity.

| Offset | Representation | Binary observation |
| --- | --- | --- |
| 0x00-0x2F | al::LayoutActor | Base constructor and clean local sizeof(LayoutActor). |
| 0x30-0x4F | Eight floats | Constructor stores an s0 return value for PicPoint1-PicPoint8. |
| 0x50 | Marker-group pointer | Constructor stores the child group; the update selects markers through it. |
| 0x54 | Unrecovered integer | Constructor initializes the word to -1. This function does not read it. |
| 0x58 | World-selection interface pointer | Constructor stores its third argument. Update invokes virtual slot 0x08 and uses its integer result as the world index. |
| 0x5C | Unrecovered bool | Constructor clears it. Adjacent function 0x0015A2C0 stores a boolean and uses it as a display override. |
| 0x5D-0x64 | Eight marker-visibility booleans | Update stores one result per marker. Function 0x0015A2C0 reads them. |
| 0x65-0x67 | Alignment padding | The allocation ends at 0x68. |

WorldSelectionInterface records the observed slot at 0x08. Its first two
virtual declarations reserve offsets only. Their signatures and the original
interface identity are not claimed. The recovered update is called by
0x0015A33C after layout appearance and by 0x002580EC through the owning
scene's member at 0x188.

## Recovered behavior

The function gets the selected world's course count, displays at most eight
markers and hides unused markers. It gets the first global course index for
that world and classifies each displayed course through fn_00260860.
Classification values 0-7 select image pairs at indices 0,6,2,14,4,8,12,10
respectively. Other values skip that marker's update. fn_002607f4 chooses
the member of each image pair.

fn_00265524 determines marker visibility. The function updates each marker's
display and PointN animation, stores the visibility result, and suppresses
the special image pair at indices 10 and 11. fn_003270f0 controls whether
PicMedalN is shown for classification zero. The final WorldLine pane uses
the WorldLine action when fn_00260780 returns true for the world's last
course, and the Wait action otherwise. These final action strings were
verified directly from the retail PC-relative addresses.

The helper at 0x0027BEC8 is the 32-character variadic StringTmp constructor
ABI: it sets the string pointer to this+0x0C, stores capacity 0x20 at this+8,
builds the buffer and invokes formatV with the format and variable arguments.
The source uses the existing clean al::StringTmp<32> API. Temporary expressions
retain the constructor's returned pointer through the assurance call, as the
retail caller does. Its generated constructor body is not claimed matched.
Its imported identity must be recorded independently for the caller's strict
link: _ZN2al9StringTmpILi32EEC1EPKcz at 0x0027BEC8.

The function keeps all other unknown imports address-named. Their declarations
retain the observed argument and return registers without assigning unrecovered
semantic names. The table at 0x003F1850 has eight name pointers Point1-Point8.
The table at 0x003F1870 has sixteen image-name pointers. Existing unchanged
map boundaries give sizes 32 and 64 bytes respectively. Both tables remain
external dat_ declarations; no original data values enter the compiled source.

## Diagnostic compiler evidence

The split integration copies compile successfully under ARMCC 4.1/791 and
4.1/894, using unchanged project flags and preinclude. Both produce the same
complete 724-byte function. Diagnostic relocation resolution yields a byte
for byte match to the retail interval under both versions. This is not an
M0 discriminator and is not an accepted O.

Final source SHA256:
5d8cf9270d4eba9fdd8611fbbed0b4db9fbf8a2470d50b7bd3b58aa068c3939b.
The paired aggregate header SHA256, including the prepared header directory,
is 64311c4ca1ca382dee1c5128c8f313cf9dec721e2445d39acdfe92cc4e6a1e11.
Full commands, compiler versions, source snapshots, raw and resolved section
bytes, relocation lists and target hashes are preserved in
course_select_map_compiler_evidence.json and the ignored probe history.

Eighteen compiler invocations were measured across nine paired evaluations.
None failed and none had unstable source or header inputs. Measured wall
time from the first scratch source through the final pair was 10.92 minutes.
Earlier target inspection is excluded. These are Phase 0 reconstruction
attempts, not capped pilot iterations.

## Compiler-local switch marker and acceptance gate

ARMCC emits a local STT_FUNC symbol __switch$$ with size zero and value 180
inside fn_00159fec's section. Eight ABS32 relocations use it as the base of
the compiler-generated case table. The ignored diagnostic driver resolves
this local symbol from the original function placement plus the ELF symbol's
value. It preserves the ARMCC section and relocation addends. No object or
target bytes are edited.

At package preparation, check_exact_bytes was not changed. Its shared-section guard rejected
any nonzero STT_FUNC value, including this zero-size compiler-local marker.
The canonical function name is also currently absent from the map, so this
lane did not submit its scratch object for accepted checking. Root integration
must independently name the function and constructor import, commit the C++,
compile it directly with the project build, and apply the canonical source
provenance gate. If the local-label guard needs correction, that is a separate
tooling decision with compiler-produced evidence. The original intervals and
byte comparison must remain unchanged.
