# Static initializer needs a data row

Branch: `dot/root-380314`
Frozen base: `eeaca9ad7a606674b428b29630da497031ff913d`
Target: unnamed `0x00380314`, 516 bytes, U.
Claims: none. No source candidate, build or canonical check attempted.

Missing data address: `0x0042F3C0`.
Size: full extent unknown. Direct stores establish at least 0xb8 bytes; a further subobject at +0xe8 is passed to an initializer, so the full object extends beyond that direct-store range. This is evidence of usage, not a proposed exact boundary.
Reason: the frozen map has no exact or containing row. The routine initializes two vectors, then a compound static settings object starting at +0x18, and registers its lifetime. The separate nerve/vtable dependencies already have rows. Do not invent the missing global's identity, extent or name.

Integrator action: independently establish the data object and add its row, then return this root for source construction and canonical regrading. Moving to another unowned 516-byte tie. No map, rank, tool or data changes are proposed.
