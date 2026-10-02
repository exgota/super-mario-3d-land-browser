# Rail-moving actor action collector needs a row

Branch: dot/root-176480. Base: e2336037d4f709acc082b3322daedadf9500564a.
Target: 00176480, 544 bytes, U. Exact claims: none; actor class identity remains unresolved.
Missing address: 0042FDC8. No exact or containing row exists in the frozen map.
At 0017649C the target passes this address as the collector argument to named al::initNerveAction(LiveActor*, const char*, alNerveFunction::NerveActionCollector*, int).
The clean existing NerveActionCollector header declares three four-byte fields (count, first node, last node), giving a 12-byte type-size expectation; the exact object's independent allocation extent remains unproved.
Integrator should establish the collector row and its justified size before retrying.
Existing project/functor-v0m-cleanup.md separately records unresolved host/callback identities for this target's Functor table; that old note is context, not an active reservation or exhausted attempt.
No source, build, canonical check, or replay was attempted. No runtime fault, byte mismatch, or demotion claim.
