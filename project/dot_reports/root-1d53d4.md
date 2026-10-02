# Texture-environment setup: 001D53D4

Base: 45de738f8517a6fc8b198cd85963925bdf6f0267. Branch: dot/root-1d53d4.
Proposed exact claim: fn_001D53D4, complete 524-byte interval.
Source: Game/backup/src/Graphics/TextureEnvironmentSetup.cpp.
First source form committed at f7745e6f and built by `python make.py eu`, configured ARMCC 4.1 build 791.
Canonical command: `python tools/check.py fn_001D53D4 --object build/eu/obj/Game/backup/src/Graphics/TextureEnvironmentSetup.o`.
Result: `M -> O: The complete source-generated function interval matches byte for byte.` (exit 0).
The blank root row temporarily used fn_001D53D4 and rank M; original map restored before sealing.
No helper names, data rows, compiler flags, headers, tools, or other build inputs changed.

The receiver's first word is the program handle; 0x500 bypasses the uniform writes.
It configures texture-environment stages 1 and 2 through existing uniform helpers.
Literal uniform names and floating constants are ordinary C++ source, not binary stand-ins.
A scoped object saves graphics mode, selects 0x801, and restores the prior mode on exit.
All helper addresses already have mapped rows. No class identity claim beyond the observed local layout.
No replay or per-family broad preservation gate. Integrator intake/revalidation is still required.
