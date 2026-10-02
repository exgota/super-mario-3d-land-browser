# Handle-wrapper creation needs an allocation-context row

Branch: `dot/root-1ed3b8`
Frozen base: `eeaca9ad7a606674b428b29630da497031ff913d`
Target: unnamed `0x001ED3B8`, 516 bytes, U.
Claims: none. No source candidate, build or canonical check attempted.

Missing data address: `0x003F7120`.
Size: unknown; no exact or containing map row establishes the object's extent.
Reason: after a successful handle-producing operation, the function passes this static allocation context to helper 0x0028E01C, then installs the returned handle wrapper's mapped dispatch table. The context is referenced directly; its owner and full layout remain unestablished. This is separate from the already mapped service-state and dispatch references.

Integrator action: independently establish the context's identity and size, add the row, then return this root for source reconstruction and canonical regrading. Moving on within the 516-byte tier. No map, rank, tool or data changes are proposed.
