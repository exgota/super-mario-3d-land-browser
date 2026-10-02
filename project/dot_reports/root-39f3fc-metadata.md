# Separate template metadata evidence, light-area sort

This note proposes no map commit and changes no boundary. It does not establish a match. Source and canonical object remain frozen at the four-form cutoff. The parent identified that the initial check procedure exceeded its temporary-names-only scope by changing Type f to ft on two existing rows; the corrected check below keeps both original classifications.

## Corrected names-only check

On2026-10-02 at01:30UTC, `build/root39f3fc/check.py` reran the normal unchanged `tools/check.py --object` for both emitted symbols. It temporarily assigns names to the existing root, heap-helper and Light-copy rows, preserving their original Type f. Both comparisons reach the complete-size failure. This is the authoritative reported failure for the frozen source; it is not inferred from the earlier ft run.

```text
0x39f3fc U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.

0x204950 U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.

```

## Earlier classification-dependent diagnostic

The earlier executed procedure temporarily set root0039F3FC and helper00204950 to Type ft and assigned those same three existing-row names. Its result must be read as dependent on proposed classification metadata, pending main's separate review; it cannot itself establish unchanged-classification canonical eligibility. Its observed output was:

```text
0x39f3fc U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.

0x204950 U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.

```

The newer names-only rerun happens to reach the same failure; that does not retroactively make the earlier procedure names-only. Neither run records acceptance, and both restored the map byte-for-byte.

## Independent evidence and proposed classification

The existing map rows are:

- `0x0039F3FC,0x0039FE34,0x0039FE3C,          ,U,f,,`
- `0x00204950,          ,0x00204E28,          ,U,f,,`
- Original Light copy remains the existing Type f interval0024E054..0024E0B8

The committed explicit C++ instantiation generates the following actual ARMCC object sections, as independently read from the canonical ELF section and symbol tables:

- `t._ZSt16__introsort_loopIPN9dot39f3fc4AreaEiPFbRKS1_S4_EEvT_S7_T0_T1_`, root2636bytes
- `t._ZSt13__adjust_heapIPN9dot39f3fc4AreaEiS1_PFbRKS1_S4_EEvT_T0_S8_T1_T2_`, helper1808bytes

Root control flow, the independent lighting caller, existing callee row, 0x128 stride and the helper's five-argument by-value ABI support the specialization identification described in the main report. The generated section prefix is an observed compiler fact. The proposed f-to-ft classification would select these t. sections in normal linking if main separately adopts the descriptive source identities. It is not proof of the original type spelling, not a boundary repair, and not permission to credit a nonmatching body. Main must review this metadata independently; no map edit is included in the patch.

No compiler, source, object, flag or oracle changed for the correction. The final source SHA remains `e8d26f39de21af91e72f5387e6533dc3b2e918a0fce2a1630a451336a4e82ec4`, canonical object SHA `8e329d332fe5b1a054157219edde52ae9fb3ab90f10170ac823655bfee41f5d8`, and restored map SHA `d517e7aa3eefdbb859e781d7d974f8daf4fbcb33b964edb7d0a5ef6a11db8bd9`.

Raw terminal-log SHA-256 values (including color escapes):

- Names-only: `1596949765ed59fa4929643d8988fd99cdf2f37f3ecfc25f48c983b8e6c42c5a`
- Earlier ft diagnostic: `1596949765ed59fa4929643d8988fd99cdf2f37f3ecfc25f48c983b8e6c42c5a`
