# Static geometry buffer needs a row

Branch: dot/root-2e1868. Base: e2336037d4f709acc082b3322daedadf9500564a.
Target: 002E1868, 544 bytes, U. Exact claims: none.
Missing address: 00424680. No exact or containing row exists in the frozen map.
The guarded initialization writes through offset 0x11C, establishing a 0x120-byte minimum writable span; the subsequent optional copy also requests 0x120 bytes from this base.
The repeated 0x24-byte record pattern suggests eight geometry records, but full object ownership, symbol identity, and complete allocation extent are not established.
The guard storage at 003EF1A4 and separate copy source 003A873A have existing rows; neither covers the missing buffer.
No source, build, canonical check, or replay was attempted. Integrator should establish the justified data row before retrying this target.
