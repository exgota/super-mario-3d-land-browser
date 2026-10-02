# Spawn-position coefficients need a data row

Branch: `dot/root-320858`
Frozen base: `45de738f8517a6fc8b198cd85963925bdf6f0267`
Target: unnamed `0x00320858`, 520 bytes, U.
Claims: none. No source candidate, build or canonical check attempted.

Missing data address: `0x0042F98C`.
Observed minimum span: 0x0c bytes, from float loads at offsets 0, 4 and 8. Complete extent, owner and original type are unestablished.
Reason: the routine uses these three coefficients to scale basis vectors while computing a spawned actor's position. Fresh main has no exact or containing row. The observed three-float use does not by itself establish a complete standalone vector boundary.

Integrator action: independently establish the data object and its bounds, add the row, then return the root for source reconstruction and canonical regrading. Moving to another unowned 520-byte tie. No map, rank, tool or data changes are proposed.
