# BSS objects used by group_00350028

This cleanup starts from `3d17628733ab941c39229f725ea9f496fe5dde70`.
The only oracle used is the owner's EU executable, whose SHA256 is
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
The exheader establishes BSS as `[0x003F39D4, 0x00431058)`; neither object
has initialized bytes in `code.bin`.

## Object at 0x0042FA14: 0x20 bytes

The initializer at `0x00381C44..0x00381E80` loads `0x0042FA08` from
literal `0x00381E10` at instruction `0x00381D34`. After initializing a
separate 12-byte object, instruction `0x00381D48` adds `0x0C` to that base,
giving `0x0042FA14`. At `0x00381D4C`, `vstmia r0, {s0-s7}` writes eight
four-byte floats. The last word starts at base+`0x1C` and ends at
base+`0x20`, establishing `[0x0042FA14, 0x0042FA34)`.

The next parameter object's independent initialization loads
`0x0042FA34` from literal `0x00381E54` at `0x00381D58` and calls
`0x001A6344` at `0x00381D74`. Actor initialization also uses this next
base and subtracts `0x20` at `0x00325980` before passing the resulting
`0x0042FA14` to `0x0026B9D8`. This independently corroborates the endpoint.

## Object at 0x0042F534: 0x20 bytes

The initializer at `0x0038503C..0x00385290` loads `0x0042F510` from
literal `0x00385250` at instruction `0x00385148`. Three 12-byte objects
are initialized, with cursor advances at `0x00385154`, `0x00385168`, and
`0x00385180`. The cursor is then `0x0042F510 + 3*0x0C = 0x0042F534`.
Instruction `0x00385184` calls `0x00211D5C`; that constructor writes
`vstmia r0, {s0-s7}` at `0x00211D7C` and returns without changing `r0`.
Its last four-byte word starts at base+`0x1C`, giving the exclusive
endpoint `0x0042F554` and size `0x20`.

Instruction `0x00385188` advances by `0x20` before initializing the next
object with `0x00211C4C`. Independently, actor initialization loads
`0x0042F554` from literal `0x001385C4` at `0x00138450`, subtracts `0x20`
at `0x0013845C`, and passes `0x0042F534` to `0x0026B9D8`.

## Consumers and map safety

The eight functions being cleaned pass these bases to `0x00258774`.
That helper retains its parameter in `r4` at `0x00258778` and reads
four-byte floats at offsets `0`, `4`, and `8` (`0x002587BC`,
`0x002587C8`, and `0x002587AC`). Other callers pass these parameters to
helpers that read offset `0x0C` and three floats beginning at offset
`0x10` (`0x0025976C`, `0x002597BC..0x002597C4`, and
`0x00269274..0x0026927C`). The largest evidenced word offset is `0x1C`
in each object's complete eight-word initialization; a 12-byte row
based solely on the cleanup helper's reads would truncate the objects.

The two added intervals overlap neither each other nor any existing
map row. The preceding mapped interval ends at `0x003F39D4`, and the
next begins at `0x0042FB10`. Every existing row remains unchanged.
The rows are unnamed `db` entries with rank `U`, empty Pool, Section,
Symbol, and SectionName fields:

```csv
0x0042F534,          ,0x0042F554,          ,U,db,,
0x0042FA14,          ,0x0042FA34,          ,U,db,,
```

The source-referenced `dat_` aliases therefore resolve at these starts.
`write_stubs` in `tools/pypstem/stepSplit.py` generates weak zero-filled
32-byte scaffold objects in their type-derived `.bss_dat_XXXXXXXX`
sections. These are placement scaffolds, not reconstructed initial values.
No binary, asset, function interval, existing rank, or ledger is changed.

To reproduce the size evidence, source `development_environment.sh`,
verify the SHA256, and disassemble the initializer and constructor code
intervals above using Capstone ARM mode on `code.bin` with virtual base
`0x00100000`. Read each specified literal as a little-endian 32-bit word.
Check map overlap with the half-open interval predicate
`existing_start < new_end and existing_end > new_start` for every row.
