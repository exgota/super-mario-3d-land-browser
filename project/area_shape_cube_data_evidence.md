# Separate AreaShapeCube data ownership proposal

The named table row `_ZTVN2al13AreaShapeCubeE` currently spans `0x003D62B8..0x003D62D4` (28 bytes). Independent retail constructor and adjacent-table evidence establishes its own extent as `0x003D62B8..0x003D62CC` (20 bytes): an eight-byte ABI header followed by three virtual entry slots.

Minimal separate metadata proposal: shrink that named row's end to `0x003D62CC`, then add an anonymous `dc` slice at `0x003D62CC..0x003D62D4` containing the following table's two-word ABI header. Preserve all existing anonymous rows, function boundaries, ranks and table names. This proposal is data-only and has not been applied.

The Cube constructor at `0x001C50EC` calls the known AreaShape base constructor, stores its flag at `+0x14`, and installs address point `0x003D62C0` from literal `0x001C5108`. The two words before that address point, at `0x003D62B8/+4`, are zero offset-to-top and zero type pointer. Its entries are the accepted Cube predicate `0x00330868`, zero, and the unnamed function `0x0033091C`. The latter uses the same Cube byte flag at `+0x14`, supporting its class relationship. Its semantic method name and signature remain unknown; no source implementation is proposed.

The immediately following constructor `0x001C510C..0x001C5124` calls the same AreaShape base constructor and installs address point `0x003D62D4` from its pool at `0x001C5120`. That independently establishes its ABI header at `0x003D62CC/+4`, both zero. The three following entry slots at `0x003D62D4/+4/+8` hold `0x003311D8`, zero, and `0x00331234`, followed by another two-zero header. The first method calls AreaShape's existing local-position helper, which independently supports the shared base. The following class is unnamed here.

The base AreaShape constructor independently installs address point `0x003D7968`; its existing named row `0x003D7960..0x003D7974` already has the correct 20-byte extent and three entries pointing to established `__cxa_pure_virtual` at `0x00377DA4`. No base-row repair is requested.

Ordinary C++ compiler tables are each 20 bytes. The current Cube header leaves two methods pure, so its raw generated entries differ from the target's zero/unnamed slots. This audit does not claim table-byte equality or complete Cube class reconstruction. Constructor equality does not supply that missing semantic evidence. `vtable_shape.json` records the ordinary table sizes and relocations from both untouched compiler objects.

`identity_evidence.json` and `retail_identity.txt` freeze the original intervals, header words, constructor address points, source/header hashes and relevant consumers. No root-reserved neighboring string/table identity is named or reconstructed by this proposal. Root owns review and any separate evidence commit.
