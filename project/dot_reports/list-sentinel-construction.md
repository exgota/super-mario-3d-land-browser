# PlayerActionMultiCondition constructor follow-up

**Zero new exact functions or bytes.** The unchanged main baseline remains 44 bytes with 8 differing bytes. Two additional list-initialization hypotheses worsened the candidate and were withdrawn. The restored baseline passes 512 bounded original/candidate/independent-model ARM1176 pairs.

Target `_ZN26PlayerActionMultiConditionC1Ev`,00252054..00252080. Base/checker commit `e2cbf8db72566da78fc16b77a5b90026bfd9ea0a`; original packet00252054 had two source forms. Cumulative count is four distinct forms, plus a restoration build.

1. Existing packet: implicit list construction and explicit list construction, both44 bytes/8 differences.
2. New form3, committed9a886ea: reset sentinel links in the ListImpl constructor body after size initialization. Canonical clean build/link/export passed; checker `U -> m`,44 bytes/15 differences. Worse.
3. New form4, committedb2f99e2: explicit link assignments inside the ListNode constructor, preserving field order and list layout. Normal project build passed; checker `m -> m`,44 bytes/15 differences. Worse.
4. Restored exact main header at360fc6c and rebuilt; checker `m -> m`,44 bytes/8 differences. No header/source changes are proposed for intake. This report validates the existing source and records failed follow-ups; it is not newly authored matching code.

Final canonical command:

```sh
. ./development_environment.sh
export DEVKITARM=/usr
python make.py eu
python tools/check.py _ZN26PlayerActionMultiConditionC1Ev --object build/eu/obj/Game/backup/src/Player/PlayerActionMultiCondition.o
```

Output: `m -> m: The linked candidate differs from the unchanged original interval.` Checker blobs: check.py `7c0ccd93b7387a2f9afa9b304b5254f34149b318`, checkExactBytes.py `ce8fc0a5d747c1521d84a1ca1fbeaeab09e3bb47`. No local map identity, boundary, original bytes, tools, compiler flags or main source patches are needed. Only the checker-updated local rank differs and is not published.

The remaining difference is store order: retail stores next/count together before previous, while the retained source stores previous before next/count. No new layout/alias evidence justifies further variants now.

## Bounded behavior evidence

[Replay source](list-sentinel-construction-replay.md) runs complete retail and canonical candidate code on Unicorn2.1.4 ARM1176 with512 randomized aligned object addresses and prior-memory states. All data memory, returned this pointer, SP and callee-saved registers agree with an independent five-word model: vptr003D10DC; previous/next=this+4; size0; node-value offset4. No helper is mocked. This supports final-state behavior only, excluding concurrency/intermediate observation, MMIO or volatile storage, invalid/faulting addresses and physical hardware.

Original interval SHA-256 `ff4938092acd497ba85272f1e2e6d0f6bb3fa3291dd385bf947409af0ea498c2`; candidate `6642839660f3bee59a11504c95fdf26520cea09d8a45108161af25825aaa0e43`. Both44 bytes,8 differences. The existing table row is unchanged; no claim is made about its disputed trailing bytes.
