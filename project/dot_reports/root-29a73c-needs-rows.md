# Guarded matrix initialization needs a row

Branch: dot/root-29a73c. Base: 713975727c0447ea8cdea708a9ab13cc539a92d0.
Target: 0029A73C, 556 bytes, U. Exact claims: none.
Missing address: 00430C68. No exact or containing row exists in the frozen map.
The guarded initializer writes twelve floats at offsets 0x00..0x2C, then copies twelve words from that same base into an object matrix, establishing a 0x30-byte minimum span.
The separate guard at 003F389C has a four-byte row. The matrix's symbol, independent owner, and complete allocation extent remain unproved.
This address is distinct from other missing matrix dependencies; no objects are merged merely because their initializer values resemble each other.
No source, build, canonical check, or replay was attempted. Integrator should establish the justified data row before retrying.
