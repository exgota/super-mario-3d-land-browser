# Main-owned built-in uniform table identity proposal

Proposed BSS identity: **0x00420160..0x00420F4C, 3,564 bytes**,
297 records of three 32-bit words. This note changes no map, boundary, rank,
data definition or target byte. Main owns any metadata change after review.

The source-only program-link proposal refers to this object as
`dat_00420160[297]`. The current map has no BSS identity for it, and canonical
checking correctly rejects that unresolved ABS32 import. This is independently
identified data, not a helper address selected to rescue a source section.

## Independent initialization evidence

The complete original initializer begins at `0x001064E4`. Its literal at
`0x00106B24` is `0x003A2F7C`, the source table of four-word records. The first
word is an ID. The word at `0x00106B28` is destination `0x00420160`.

- `0x00106510..0x00106520` reads the first ID and checks -1 termination
- `0x00106528..0x00106538` reads ID and value, computes `ID * 12`, and stores
  source word 1 to destination record word 0
- `0x0010653C..0x00106544` copies source word 2 to destination record word 1
- `0x00106548..0x0010654C` copies source word 3 to destination record word 2
- `0x00106550..0x00106558` advances by 16 bytes and checks the next ID

The owner's unchanged executable contains 297 source rows, each ID0..296 exactly
once. Its terminator begins at `0x003A420C`. The record-data bytes are not copied
into this branch.

The next initializer loop at `0x0010655C..0x0010658C` separately populates a
register-index table. Its destination literal at `0x00106B30` is `0x00420F4C`,
which is exactly `0x00420160 + 297 * 12`. Thus the neighboring independently
initialized object provides the upper boundary, in addition to the exact ID
coverage and both consumers below.

## Independent consumers

The program linker `0x00245D50` appends297 built-in locations. At
`0x002467A0..0x002467D4` it loads `0x00420160` from the literal at `0x00246D1C`,
selects a 12-byte record, and reads its word 1 as an enum type. Types
`0x8B50..0x8B55` determine the low two bits of the location token.

A separate location-name lookup loads the same base from `0x0024574C`. At
`0x00245700..0x0024570C` it subtracts the built-in location base, scales by 12,
and reads word 2 as a name pointer before calling the original strcmp at
`0x00245714`. This corroborates the record stride and a second field without
reusing the program-link code path.

## Scope of the proposal

The proposed row is one whole independently bounded BSS table. Its populated
contents require the original initializer (or a separate clean reconstruction),
not a copied binary array. Adding this identity alone will not make the linker
match: the final complete ARMCC section is 5,908 bytes against 6,524 retail bytes.
No exact reconstruction credit is requested for this note or table identity.
