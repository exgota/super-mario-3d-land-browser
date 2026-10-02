# Shared graphics declarations: compiled integration proposal

## Producer attribution correction, 2026-10-02

The old shv_ValidatorTail.h comment incorrectly attributed lookup/exclusion-mask initialization to 00108690. That root consumes the 189-entry lookup: literal00108F70 is00420F4C,00108E08 loads the base,00108E4C reads indexed entries, and00108E64 bounds the loop at189. It does not write the lookup. Independently identified producer001064E4 (_shm_initializeShaderManager) loads00420F4C at00106570 and writes index/register pairs from003A421C through00106580. Its later blocks at001069D0/00106A28/00106ABC initialize the three exclusion masks at00421240/58/70. Exact whole-object extent and map ownership still require separate evidence and main review; this correction does not establish accepted metadata.

This update changes this report and comments only. All non-comment header text is byte-identical. No build or checker was rerun for the comment correction; earlier source/check hashes and measurements describe their named historical commits. It adds no exact or behavioral credit and makes no claim that accepted coverage was invalidated. The absolute-address accessors remain qualified diagnostic scaffolding, not canonical BSS ownership.

This branch gives seven reconstructed graphics roots one compatible declaration of their two shared C globals. A diagnostic translation unit including all nine interface/helper headers previously fails with incompatible-declaration errors; it now compiles. Every allocated output section remains identical to the individually tested objects, including bytes, flags, alignment, symbol definitions, relocation types/offsets/targets, and linked-section relationships. Whole object hashes differ because of non-runtime metadata; whole-object byte equality is not claimed.

Base: `57f902421f6874f132d5ea971d1aa5b224c5a528`. Committed source checkpoint: `645490e6f279b57c326047f11dcedf6f142f31d7`. This is a separate integration proposal, not production intake or an exact match. New main `e87d556cd4d993606a03e205b4604d863d718bd4` changes no CtrSDK source, compiler configuration or byte-checker implementation; its651-root preservation has not been rerun here.

## Declaration contract

`retail/GraphicsGlobals.h` owns the external declarations. The16-byte context-slot record has two unclassified words, an opaque ProgramContext pointer at+8, and a32-bit geometry-enabled cache at+12. The render-control global is an opaque RenderControl pointer. These names are descriptive. The complete pointed allocations remain unrecovered. ARM32 size/offset assertions protect the observed slot layout.

State writers adapt those opaque pointers to their existing typed views; shader, linker, binary-loader and texture-cache roots adapt them to their existing views. They no longer redeclare the same C symbols with conflicting struct, array and pointer types. The integer writer's extended state-view header is shared with the float writer; its added fields leave all prior offsets unchanged. No function algorithm or data contents were changed.

## Mechanical evidence

The final ordinary `python make.py eu -ca` compiles all seven proposed roots under unchanged SDK ARMCC4.0/902, links the compact scaffold and exports. A header-only compiler diagnostic uses the exact module command and include environment; it is not a fabricated canonical function object and was never supplied to the checker.

The seven committed-source objects have identical allocated sections and relocations to their tested reference builds. This preserves the machine code on which earlier bounded behavior results were based; those replay counts are not counted again as new executions. The unchanged canonical checker still gives the same nonmatching outcomes:

| Root | Complete source bytes | Target bytes | Canonical result |
|---|---:|---:|---|
| Float state0020ACAC |19040|18916|Size mismatch|
| Integer state00206474 |15484|18488|Unmapped BSS import first; also size mismatch|
| Full validator0037B8D0 |16284|14352|Size mismatch|
| Partial validator00377FD0 |16276|14408|Size mismatch|
| Program linker00245D50 |5908|6524|Unmapped BSS import first; also size mismatch|
| Shader binary002478D8 |4348|4464|Size mismatch|
| Texture cache003910C0 |3312|4188|Size mismatch|

Partial-validator clarification:16260 is its ELF function-symbol size. The complete section is16276 bytes, including16 trailing pool bytes. Its frozen replay loaded whole allocated sections, so no tested bytes or behavioral results change with this reporting correction.

The pre-existing accepted SDK priority conversion root repeats O→O. No accepted source includes the new declaration header, and no existing source/header outside these proposals changed. This is focused preservation, not a new audit of all618 or651 accepted roots.

## Remaining integration limits

These roots remain U in the restored main map and therefore are not production implementations in the compact image. An intermediate incremental build after diagnostic checks had marked mismatch roots M; that activation attempted to link the full validator and reported undefined `__cb_multiWriteReg` and `__cb_fillRegs`. Both have existing named fg rows at0028A14C and0028A304. No row/classification, stub generator or helper body was patched. Final clean linking succeeds with the original map restored. Header/source compatibility does not establish activated-root linking or complete runtime integration.

The integer scratch import0042013C and program-link table00420160 also remain explicit metadata prerequisites. Their separate reports distinguish an observed prefix from a separately bounded whole table. Main owns metadata and production coordination. No game data, tool/configuration, map, rank, ledger or STATE change is committed. New exact credit is zero.

All canonical outputs, source-object fingerprints, allocated-section comparisons, header diagnostics and input checkpoints are preserved in [evidence.json](graphics-declaration-integration/evidence.json). The [reproduction appendix](graphics-declaration-integration-replay.md) explains reference builds and supplies the comparison and header-diagnostic scripts. Original per-root reports retain their distinct real-callee, allocator-model, alias and malformed-input limits.
