# FileSelect operation ABI reconciliation

Source proposal from `dot/root-14aedc` at `57f41a5a8` supplied the 524-byte `fn_0014AEDC` operation update, but its own report correctly held final integration on the existing caller's incompatible placeholder declaration. The driver preserved and held that original request, then prepared this cleanup family. The address-only row name is recorded separately in evidence commit `03d7cf20867336fb937abb5e5e69942de5a9bdcd`; no boundary or rank change accompanies that naming.

## Source and type contract

`FileSelectOperation.h` provides one partial, binary-grounded state definition and shared declarations. It does not claim the complete FileSelect class, inheritance or original spelling of the state-view type. `fn_0014AEDC` returns void and receives that state pointer. The existing nerve execute caller `fn_0014AED4` receives `const al::Nerve*` and `al::NerveKeeper*`, retrieves the host through the established keeper accessor, and calls the void update. Its original 8-byte interval is preserved.

The two dialog predicates `fn_00264930` and `fn_00264950` now share the established `const al::IUseNerve*` type. Completion helper `fn_002744FC` shares `al::IUseNerve*`. Their existing 12-byte bodies move from the anonymous Factory placeholders into `FileSelectNerve.cpp`, where they call the typed al nerve APIs. The root and these reusable helpers remain separate compilation units. Combining them caused compiler inlining and changed the root; restoring that source boundary recovered all bytes without flags or forced inlining controls.

The old root declaration and thunk definition are removed from `group_001414C0.cpp`. The three helper definitions and their private nerve-object declarations are removed from `group_0025BBD4.cpp`. No other function in those files changes. A source search finds zero old integer/four-argument root declarations or old private `UseNerve` declarations for the three migrated helpers in this five-file family.

## Canonical verification

Frozen final source commit: `564cd53df97ac42468e191e3c473b5338961c4aa`. After a normal project build with the configured ARMCC toolchain, `tools/check.py <symbol> --object <project object>` checked every mapped definition in both touched Factory objects and both new FileSelect objects. All 49 complete intervals pass, totaling 1,000 bytes. This preserves 48 existing exact functions covering 476 bytes and locally verifies the new 524-byte root. The final check finished at 2026-10-02T17:49:24-04:00 in 30.55 seconds.

The final source hashes are:

- `Game/backup/include/Layout/FileSelectOperation.h`: `7ae6d4d825b41228f9d7782724b492361a9cd7e4fa17f70a58ecba6bdb19e518`
- `Game/backup/src/Layout/FileSelectOperation.cpp`: `9408f97e502e989ad152781acd4129acc93b203f22f78a6b1b562f819c9314e4`
- `Game/backup/src/Layout/FileSelectNerve.cpp`: `c3ab74a81686ec7422196c9556a8d4d8eee5fc3fc32f929bf13b1ed33a7f16e3`
- `Game/backup/src/Factory/group_001414C0.cpp`: `5b4174a7260208425ca920839c224d9f409af46f1178f703da7d24d0d21cc959`
- `Game/backup/src/Factory/group_0025BBD4.cpp`: `b4b071a539bffa2c29d9222565f02361e9a17524d4b51e3f4ba076ef4c46012e`

Commands start with `. ./development_environment.sh`, then `python make.py eu`. The four checked project objects are `build/eu/obj/Game/backup/src/Factory/group_001414C0.o`, `Factory/group_0025BBD4.o`, `Layout/FileSelectOperation.o` and `Layout/FileSelectNerve.o` under that same object prefix. The driver keeps complete canonical output and the per-symbol results locally in `logs/file_select_abi_final_checks.log` and `.json` in the factory workspace. Scratch rank changes are restored before submission.

This is a source/ABI proposal, not an acceptance receipt or runtime replay. The integrator must run the cleanup alone and preserve every O row before it awards the 524 new bytes. No complete class-layout or gameplay claim is made.
