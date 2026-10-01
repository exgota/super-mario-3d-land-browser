# Shader binary ingestion reservation

Branch `dot/program-state-root`, based on main `57f902421f6874f132d5ea971d1aa5b224c5a528`, reserves the unnamed U root `0x002478D8..0x00248A48` (4,464 bytes). This is an initial identity/ABI checkpoint, not a match claim. No source form has yet been compiled.

The independent callers at `0x001D4C54`, `0x002B577C`, `0x002B5858` and `0x002CBA44` supply a five-argument shader-binary upload shape: signed shader count in r0, handle array in r1, format 0x6000 in r2, binary pointer in r3, and binary length at caller sp. The body uses count, handle array and binary pointer; format and length are unused. No caller consumes a return value. The address name will be retained until the public API identity is separately accepted.

The body allocates a 0x24-byte shared resource through the existing allocator slot `dat_003E2654`, copies instruction words and the low word of each eight-byte operand descriptor, and allocates 0xE8-byte stage records. It interprets each binary stage's constant, output, variable and string tables, then attaches the shared resource to handles in the context's 512 shader buckets at +0x808. Independent release root `0x0020F690` confirms resource arrays at +0/+8/+0x10, stage pointer ownership at +0x30/+0x58/+0xE0, resource reference count +0x18, and list links +0x1C/+0x20.

Configured module is `lib/CtrSDK`, ARMCC 4.0 build 902, with unchanged project flags. The owner executable hashes to `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. Only the private owner dump is used. Source, headers and this report are the branch's permitted changes; map, tools, compiler flags, ledger, STATE and game data stay untouched.

The active linker worker owns `0x00245D50`; its independently recovered stage contract agrees on 0xE8 stride, constants +0x30/+0x34, outputs +0x38, flags +0x54, uniforms +0x58/+0x5C, attributes +0x60 and strings +0xE0. Neither worker edits the other's tree. Main owns integration and acceptance. Exact and bounded behavioral evidence remain pending.
