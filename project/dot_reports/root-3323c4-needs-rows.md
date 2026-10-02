# Second query shares missing scratch state

Branch: dot/root-3323c4. Base: 3d69bf00a769676c77f07465163def56a559b48f.
Target: 003323C4, 556 bytes, U. Exact claims: none.
Missing address: 004264CC. No exact or containing row exists in this base.
This target also clears fields +0x8/+0xC and reads +0xC after its query helper, independently confirming the same 0x10-byte minimum span recorded in root-331fa8-needs-rows.md.
One justified data row may serve both dependent roots; no full allocation extent or owner identity is asserted.
No source, build, canonical check, or replay was attempted. No runtime failure or demotion claim.
