# Query scratch state needs a row

Branch: dot/root-331fa8. Base: 3d69bf00a769676c77f07465163def56a559b48f.
Target: 00331FA8, 556 bytes, U. Exact claims: none.
Missing address: 004264CC. No exact or containing row exists in this base.
The target clears four-byte fields at +0x8 and +0xC, then reads +0xC after the query helper, establishing a 0x10-byte minimum readable/writable span.
The related target 003323C4 uses the same state. Complete object extent, semantic symbol name, and owner remain unproved.
No source, build, canonical check, or replay was attempted. No runtime failure or demotion claim.
