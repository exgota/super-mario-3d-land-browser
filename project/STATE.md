# Root Azahar floating-point control lane state

Owner priority: browser demo, then 100% byte exact. Root works only on port runtime, with at most one subagent. Only the integrator changes main, ranks and ledger. All game data, generated translations, captures and downloaded tools stay ignored.

This branch, root/azahar-floating-point-control, starts at main 3d176287. Root owns the source-only Azahar ARM64 guest-to-host floating-point control correction and its reproduction/evidence documents. The single helper verifies it against original ARM execution and stable first-swap captures. It must leave the settled shared oracle source and historical captures intact.

Accepted source families: heap layout 8126842; runtime draw order ab98515; static recompiler 8ad4868; Azahar reference 6c30c2d; floating-point helpers a8cb16ae. No matching credit is added by these port families. Calendar section proposal was rejected because the constructor is not exact.

Native platform checkpoint root-azahar-static-platform-8ddc8cad4 is submitted and pending. Do not resubmit it. It reaches the first top-screen swap through compiled C, 226,970,847 guest instructions, zero interpreter/JIT fallbacks, 2,437,712 linked translated bytes. Strict comparison remains false: 18 PICA words and event ticks differ.

The rounding discrepancy is now isolated to pinned Dynarmic's ARM64 host FPSCR propagation. The diagnostic stock run produces the differing viewport values under the same guest FPSCR as native, with raw capture strictly equal to the historical stock reference. A controlled sequence changing guest rounding within one Run preserves the old host mode; a fresh Run uses the requested mode and agrees with original Unicorn. The candidate public-source correction updates host FPCR when guest VMSR updates the guest control register.

The final published probe and parent verifier pass all 96 corrected cases with zero result/status/host-control differences and zero fallback. Two corrected stock replays match strictly; every native PICA byte now matches. The full audit measured 16 historical word differences, correcting the prior 18 estimate. Exactly 473 native event tick values still differ.

Next: submit this verified correction family with no claims. Keep historical reference data and failures. Then resolve basic-block scheduling and proceed to rendering. Milestone 1 remains in progress; 2 through 6 are not started. No owner question blocks this evidence-backed runtime correction.
