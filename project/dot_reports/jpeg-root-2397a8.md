# JPEG decoder marker and scan driver

`dot/jpeg-root-2397a8`, based on main `57f902421f6874f132d5ea971d1aa5b224c5a528`.
The unchanged U root is `0x002397A8..0x0023A854`, with final pool at
`0x0023A834`: 4,268 bytes including four embedded format-dispatch tables.
This is a complete guarded **NonMatching source proposal**, not an exact match.
It adds zero exact functions or bytes. No original SDK name is asserted.

The source checkpoint is `fe740a3`. The configured CtrSDK compiler is ARMCC
4.0 build 902. Both `make.py eu` and the final `make.py eu -ca` clean build
compile 42 Game, 120 Actor and 2 SDK sources, link and export. The clean build
reproduces the same canonical object SHA-256 below. The unchanged canonical
committed-source checker reports:

```
U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.
```

The final compiler object contains exactly one executable section,
`i.fn_002397A8`, 3,956 bytes, 312 bytes shorter than the target. No out-of-line
source helper remains. There were two distinct compiled source forms: the
initial offset-view version was 4,512 bytes; recovering the typed context
removed field-address temporaries and reduced its stack allocation from
0x84 to 0x24. Retail allocates 0x2C. Further matching needs the original
control-flow, access and register-allocation structure, not cosmetic cycling.
No compiler flags, checker, boundaries, pools or map classes were changed.

## Verified ABI and ownership

The root takes one `nn::jpeg::CTR::JpegMpDecoderContext*` in r0 and has a void
observed contract. All five direct callers ignore r0 after it returns and read
the error byte at context+0x44. They load the context from the first word of a
wrapper and initialize it through 0x0023A854 first. The initializer clears
0x2370 bytes, independently establishing the complete storage extent. Their
parameter block aliases wrapper+8; the replay retains this real caller alias.

| Original caller | Call instruction | Observed use |
| --- | --- | --- |
| 0x001FBC94 | 0x001FBD10 | Decode to caller output and return byte count |
| 0x001FBD54 | 0x001FBE44 | Scaled decode; accepted scale 1 through 4 |
| 0x00218BAC | 0x00218C14 | Header-only metadata-section extraction |
| 0x00218DAC | 0x00218E14 | Header-only metadata record extraction |
| 0x0021DF28 | 0x0021DF9C | Header-only validation with second-image option |

`lib/CtrSDK/include/retail/jpeg_DecoderRoot.h` records observed fields and an
opaque workspace; it does not import or claim the original complete SDK class
layout. Important fields are input pointer/size at +0/+4, output capacity +8,
restart countdown/interval +0xC/+0x10, output pointer +0x14, dimensions and
constraints +0x18..+0x22, DC predictors +0x24..+0x28, bit count and sampling
+0x2A..+0x30, output dimensions +0x38/+0x3C, block-output callback +0x40,
status word +0x44, output format +0x48, header/second-image/scale options
+0x4B..+0x4D, cursor +0x54, and marker masks/options +0x5C/+0x60/+0x64.
The named assembly helper signatures independently establish the context type.
All descriptive field names are reconstruction names.

The raw Huffman count/symbol records begin at +0x500, +0x524, +0x638 and
+0x65C; their derived records begin at +0x770, +0xA20, +0xCD0 and +0xF80.
The code work arrays are 16-bit codes at +0x1D00 and signed byte sizes at
+0x1F02. The implementation retains the original guards, inclusive 256-index
checks, first-error preservation and mutation order around failed markers.

The three callback tables are existing whole 0x90-byte rows at 0x003B4A30,
0x003B4AC0 and 0x003B4B50. The index is format*4 + verticalSampling*2 +
horizontalSampling - 3. The first selects aligned full blocks, the second
handles partial blocks, and the third handles reduced output. Original table
entries and original output routines execute in the replay; they are not copied
into C++ or replaced with callback models.

The 0x1800-byte default-Huffman copy begins at the existing data row
0x003B2BE0 and continues into the adjacent existing row 0x003B3A39; it ends
at 0x003B43E0. The 64-byte ordering table at that end is represented as
`dat_003B3A39 + 0x9A7`, an interior view, not an invented map identity.
The quantization scaling coefficients are the first 64 signed halves of the
existing 0x90-byte row at 0x003B49A0. No constant data or row was committed.

## Full control flow

The root validates SOI, resets restart state, and iterates JPEG markers. APP1
and APP2 call the original metadata parsers 0x001FDCE8 and 0x001FE5E0.
SOF0/SOF1 recover dimensions, sampling, output stride/size and the callback.
DHT copies count/symbol records, generates canonical codes, derives min/max
and symbol offsets, fills eight-bit lookup tables, and updates marker masks.
DQT supports the observed eight- and sixteen-bit paths with exact integer
wrap/truncation operations and ordering. DRI initializes the restart counters.

SOS either returns in header-only mode or validates required markers, installs
optional default Huffman tables, enters the original entropy/IDCT helpers,
decodes luminance and chrominance blocks, handles restart intervals, invokes the
selected original output routine, and finalizes status. A repeated SOI can
restart parsing for the requested second image. EOI, unknown markers, bounds,
missing markers and pre-existing status retain their original control paths.
The source includes the entire root after every embedded dispatch table.

## Bounded ARM evidence

Both roots run on Unicorn's ARM1176 model. The original owner binary stays at
its original addresses. The unmodified canonical ARMCC object is linked at
0x00600000 solely for replay, with imports resolved to existing map rows.
Real callers execute their complete original initializer and wrapper bodies;
only entry to the assigned root redirects to the candidate in that execution.
The root comparison does not compare the unused return register. Caller-chain
comparisons do compare their meaningful returned byte count or Boolean.

Final source passes **1,908 returning original/candidate pairs**:

| Group | Returning pairs |
| --- | ---: |
| Full original decode wrapper, three sampling modes, four dimensions, nine output formats | 108 |
| Original scaled wrapper, scales 1 through 3 | 81 |
| All three original header/metadata wrappers | 9 |
| Marker errors with clear/preserved error | 336 |
| Every truncation boundary of one generated JPEG | 810 |
| Default Huffman choices, quantization, Huffman guards, restart markers, APP metadata, second image, dimensions/status and caller parameters | 503 |
| Additional initial-marker/DRI/second-image/missing-marker/status/entropy paths | 52 |
| Explicit successful output/byte-count proof for all nine formats | 9 |

One additional deliberately invalid direct-context case uses output format 255,
bypassing the initializer's allowed-format check. Both executions fault while
fetching address 0xFFA6. This is retained separately as a paired fault and adds
no returning-pair or equivalence credit. The final edge JSON records it as a
failure rather than silently treating it as a pass. No mismatch was observed
among the returning cases.

Every returning pair compares the entire 0x4000-byte mapped context region,
0x10000-byte input region, 0x100000-byte output region, wrapper and metadata
result buffers, stack-pointer restoration and r4-r11 restoration. The retained
context extent exceeds the initializer's 0x2370-byte object to catch writes
outside it. The ordinary generated JPEG decode proof returns 1,024, 1,536 or
2,048 bytes depending on output format, with clear error bytes and identical
output hashes. Restart fixtures use actual codec-generated DRI/RST markers.
The combined returning/fault corpus visits 968 of the 1,023 inferred executable
instruction words after excluding the four nine-word dispatch tables. This is
coverage evidence, not a proof of every branch or general equivalence.

All reached callees execute untouched original code: the two metadata parsers,
size helper 0x002396F0, named assembly convert/get-matrix/decode-block helpers,
0x0028F0A0 memcpy, initializers, wrapper methods and table-selected block-output
routines. **There are no modeled callees.** Inputs are deterministic JPEGs
created by Pillow plus deliberately constructed/malformed marker streams;
they are not captured initialized game inputs. The tests do not prove arbitrary
input/context/output overlap, alignment-fault configuration, asynchronous
mutation, malformed contexts outside caller constraints, exhaustive metadata
formats, or game initialization/rendering/gameplay. No original game assets or
executable bytes are included in the notes.

## Reproduction and integration

The [replay appendix](jpeg-root-2397a8-replay.md) contains the standalone
Python source and bootstrap link step. It requires the authorized local binary,
the installed project compiler, Pillow, Unicorn and pyelftools. Generated
fixtures and all binaries remain under ignored build storage.

Run the normal project build, then temporarily name only the existing target's
blank symbol cell `fn_002397A8` for the diagnostic:

```
. ./development_environment.sh
python make.py eu
python tools/check.py fn_002397A8 --object build/eu/obj/lib/CtrSDK/sources/jpeg_DecoderRoot.o
```

The checker alone writes the measured temporary rank. Restore the map exactly
afterward. The branch contains source, header and notes only; the target remains
unnamed U in the committed map. Main owns naming, acceptance and publication.
The guarded source adds no callback/data definitions to the compact scaffold.

Final source SHA-256:
`66463cf9b3302c59e3c502e400f57c2e9e4db2e82593d5d1e9a01577c2971595`.
Header SHA-256:
`df0d3588e1c37acbd7c56fa7477128b477d03a67cb73fdf2dc4790f52bbdc078`.
Canonical object SHA-256:
`df3e84263fbf4e62365671a1563dc463bbc644c92d4197466e9b6a4da4415548`.
Owner binary SHA-256:
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
Committed/restored map SHA-256:
`ac7eef756278c86c41472bab66291afed57b6bcbed2336eb2b75aae157ff7cca`.
