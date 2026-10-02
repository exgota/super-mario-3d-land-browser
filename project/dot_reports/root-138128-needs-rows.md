# Motion parameters need rows

Branch: dot/root-138128. Base: 3d69bf00a769676c77f07465163def56a559b48f.
Target: 00138128, 556 bytes, U. Exact claims: none.
Missing addresses: 0042F51C and 0042F528. Neither has an exact or containing row in this base.
The target reads a float at +0x8 from each base, establishing a 0x0C-byte minimum span for each referenced record. These adjacent addresses are not merged into one object.
Names, complete allocation extents, and independent owners remain unproved. Other source/ABI prerequisites were not exhaustively screened after these gaps.
No source, build, canonical check, or replay was attempted. No runtime failure or demotion claim.
