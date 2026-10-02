# Material command-list builder 001141CC

This is a clean-room interpretation of the owner's original EU executable. It
uses no SDK implementation or symbols from removed libraries. Names below are
descriptive hypotheses, not recovered source identifiers.

## Observed layout and flow

The root accepts one owner pointer. Owner +0x6C is a material pointer, +0x60 is an
unsigned index, and +0x70 begins the indexed list-pointer area. The complete owner
layout and list capacity are unknown. The original performs no index bound check.

Material +0x690 and +0x6AC are two adjacent 28-byte records. Each observed record
has an unused word at +0, a handle at +4, a queried buffer/base value at +8, a
signed resource identifier at +12, a byte completion flag at +16, and two words
at +20/+24 passed as output pointers to 00281360. The final two names length and
capacity in the draft describe their likely roles, not independently proven API
semantics. The resource release call occurs only when signed identifier > 0.

At entry the root zeroes seven material fields: 0x6C8, 0x6CC, 0x710, 0x734,
0x738, 0x75C and 0x774. Two counted arrays use counts at +0x6C8/+0x6CC and
entries at +0x6D0/+0x6F0. A third uses count +0x75C and entries +0x760. The
recorded entry is the queried command offset rounded down to four bytes, plus a
captured base, with 32-bit wrapping arithmetic. Field +0x774 holds one similar
uncounted location. Entry capacity and malformed count safety are unproven.

Both passes query selector 0x207, generate/bind one handle, allocate with alignment
argument 0x20, query selector 0x206, restore the previous handle, save a current
handle into 003EF074, and select the new one. The requested sizes are 0x1000 and
0x800. Existing positive resources are released. The saved state at 003EF078 is
passed by pointer during preparation and by value during restoration. Entry query
0x201 conditionally invokes 00281A54; finalization conditionally invokes 00284894
with the same saved value. glGetError results are ignored in both passes.

The root captures the initial material pointer for reset and record lifecycle.
It reloads owner +0x6C before each pass. In pass one, it reloads that pointer once
more before the +0x75C array operation, after applying the +0xE8 substate. A helper
may change owner +0x6C; the other captured pointers remain in use. These distinct
reloads are retained by the source and exercised by constructed mutations.

## First pass

The sign bit of material +0x68C chooses between a table indexed by +0x5DC and the
word at 003EF06C. State blocks at +0x414, +0xE8, +0, +0x3A0, +0x15C and +0x244
are applied in the observed order. The +0x770 word is emitted and its command
location is saved at +0x774. Nonzero +0x578 selects an additional counted location
and mode-1 command; it is read again later to select a mode-2 command.

The signed color index at +0x564 is capped above at 5, without a lower clamp. If
002877FC returns zero, the selected RGBA word is reduced to 5/6/5 precision then
expanded by bit replication, with alpha forced to 255. Nonzero return preserves
the table color. Negative indices are not silently clamped by this reconstruction.

Nonzero +0x448 enables another state block. Its signed color index +0x648 is
capped above at 11, without a lower clamp. Each RGB byte is shifted right by one,
while alpha is retained. The root then processes +0x448 and applies +0x74.

## Second pass

A nonnull owner list selected by the owner +0x60 index is processed, and the helper
return is saved at material +0x734 before applying +0x2B8. The +0x47C block and
+0x32C state are unconditional. Nonzero +0x4B0 selects a command word indexed by
material +0x5EC, processes +0x4B0 and applies +0x1D0. Both passes emit the same
24-byte final command template.

## External identities

All sixteen call destinations are existing unchanged function-map rows:
00281360, 0028140C, 00281874, 00281978, 002819A8, 002819D0, 00281A54,
002847E4, 00284894, 002877FC, 00288284 (nngxAdd3DCommand), 0028CA18
(glGetError), 0028ED00, 0028F618, 0028F6F0 and 0028F864.

Data/table references come directly from original literal loads: 003A7A68,
003A7D38, 003A7D58, 003A7E30, 003A7E40, 003A822C, 003A8284,
003A84E4, 003A84F4, 003A850C, 003A8524, 003A8534, 003EF06C,
003EF074, 003EF078, 003EF084, 003EF0BC, 003EF0F4 and 004244A8.
003A850C is independently the literal base 003A8284 plus 0x288.
The context at 004244A8 contributes its word at +0x50. It has no separate current
map row. The draft retains the actual observed address rather than inventing a
symbol or changing map boundaries. No data bytes are copied into the source.

## Limits

The source remains NON_MATCHING. ARMCC 4.1/791 in lib/al is its reproducible carrier,
not proof of the original library/compiler identity. All imported API bodies are
modeled during paired replay. It establishes equality on constructed fixtures of
call order, documented arguments, emitted packet bytes and model-visible effects;
it does not validate actual allocation, GPU submission, rendering or gameplay.
