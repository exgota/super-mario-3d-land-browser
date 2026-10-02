# Audio-channel update needs a data row

Branch: `dot/root-1fae8c`
Frozen base: `72a019d5a5a46bae2b0c65e152b781b928ed4154`
Target function: `0x001FAE8C`, 512 bytes, currently unnamed and U.
Claims: none. No source candidate, build or canonical check was attempted.

Missing data address: `0x00426B54`.
Size: unknown; do not infer an exact extent from the next address or invent a row.
Reason: the function repeatedly passes this static audio-driver object to helpers at 0x001F9018, 0x001F9224, 0x001F9734 and 0x001F9068. The frozen map has neither a row beginning here nor a containing row. The first helper accesses a channel pointer at object-relative 0x10b0 plus bank/channel indexing, demonstrating that this is a structure rather than a four-byte scalar. Its complete bounds and public name remain unestablished.

Integrator action: establish the object's identity and size independently, add the data row, then return this function for source reconstruction and canonical regrading. No map, rank, tool or data changes are proposed. Moving to the next unowned 512-byte root as requested.
