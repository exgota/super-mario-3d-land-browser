# FileSelect operation state update

Branch: `dot/root-14aedc`
Base: `45de738f8517a6fc8b198cd85963925bdf6f0267`
Final source commit: `b584d276e072b6970a5db855538d0a7af557223f`
Root: `fn_0014AEDC`, complete interval `0x0014AEDC..0x0014B0E8`, 524 bytes including its 16-byte pool.
Result: local exact pass; integrator acceptance and Factory ABI reconciliation remain pending.

After sourcing the approved `mario-dot/development_environment.sh` and setting `DEVKITARM=/usr`, the final normal `python make.py eu` completed link and code export with configured ARMCC 4.1/791. Then:

`python tools/check.py fn_0014AEDC --object build/eu/obj/Game/backup/src/Layout/FileSelectOperation.o`

`M -> O: The complete source-generated function interval matches byte for byte.`

The only scratch map edit named the existing root row `fn_0014AEDC` and set it M. No boundaries or data rows changed. The complete original map was restored after checking. The integrator must supply that root name on its existing row before checking.

Four meaningful source forms were checked: direct calls/uncached slot pointers (552 bytes); a signed three-slot loop/cached pointers (516); direct calls with a full-width enable argument/cached pointers (520); and a natural inline three-slot helper with a boolean argument (524, exact). The first form also required two declaration-only build corrections because ARMCC rejects an imported abstract object when its complete class is already visible; the final nerve imports precede that definition. No flags, tools, configuration, padding, assembly or artificial register constraints changed.

Constructor `0014BBAC` identifies the FileSelect layout and initializes signed mode at `+2d` and selected/other indices at `+3c/+40` to -1. The update advances two selectors at `+44/+48`, selects a dialog from `+54` using that signed mode, handles save-file copy/deletion and restores selection states. Slot indices are at `+8`, slot state bytes at `+c`, and the two completion states at `+74/+78`. The same three-call slot-control pattern occurs in twelve retail routines, supporting the inline helper. All four literal targets already have rows: `003B7ECC`/`003B7EDC` are the delete/copy save labels and `003F192C`/`003F18F0` are nerve objects. They remain typed external imports.

ABI limitation: `Factory/group_001414C0.cpp` still declares `uint32_t fn_0014AEDC(uint32_t, const uint32_t*, uint32_t, uint32_t)` and returns it from the accepted 8-byte thunk `0014AED4`. The observed root is a void nerve update: it consumes only r0 and tail-calls the established void `al::setNerve` on its three transition exits. This proposal deliberately defines that observed void contract at the established symbol, with no invented return or renamed alias. The normal build and exact root checker pass, but they do not establish cross-translation-unit C++ type compatibility. Factory was left unchanged at the coordinator's request; its placeholder declaration and corresponding thunk return need reconciliation before final source integration.

Targeted coordinator verification also checked the unchanged accepted caller with `python tools/check.py fn_0014AED4 --object build/eu/obj/Game/backup/src/Factory/group_001414C0.o`: `O -> O: The complete source-generated function interval matches byte for byte.` The 524-byte root independently repeated M -> O. Both checks exited zero, and the full map was restored. This verifies the two intervals, not C++ declaration compatibility; the ABI cleanup above remains required.

The preserved object and adjacent `.provenance.json` are under `build/eu/obj/Game/backup/src/Layout/FileSelectOperation.*` in the isolated worktree `/workspace/scratch/73cdb2c524af/mario-root-14aedc`.
Source SHA256: `a4560f2da3c866d928b92c2d66aa753f5cceaaf5ad9714a3d5941badddd48a07`
Object SHA256: `b66aed61d5d42c42f1e9fa4d311b6d5cb79593e1667ad62d522f9710a9063f30`
Provenance SHA256: `cf08c0c0ecfe7a2a77f57926ae172b51f22863adb1f626c326669270eebcece7`

Only the family source and this report are proposed. No replay, all-O audit, map/rank/ledger edits or game bytes are included.
