# 0x001EFEFC — `fn_001EFEFC`

Written by factory job 3303 (gpt-6-luna medium, run finished).

- Class guess: standalone utility function; confidence: high. Evidence: unmangled `fn_` symbol and a two-argument scalar transform with no object access.
- Struct offsets: none observed.
- Callees: none.
- Data references: none.
- Inferred signature: `extern "C" std::uint32_t fn_001EFEFC(std::uint32_t value, std::uint32_t bits)`.
- Source revision: job 3303 source revision; ARMCC 4.1 build 791 with `-O3 -Otime --arm_only --gnu --signed_chars --enum_is_int --force_new_nothrow`.
- Source: `((value & ~0x00f00000u) << 8) | bits`.
- Canonical checker result: MATCHED on attempt 1 of 5; complete 12-byte function interval accepted.
- Limits: one observed matching form; this does not establish a general compiler rule.
