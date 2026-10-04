# Count-first array preservation intake

The owner requested finishing ready frog deliveries before the October 4 factory pause. This imports the two source files from `a3fe9ded33e16b0702beea152e8a1fffa886d575` unchanged. It claims no new exact bytes.

Base: `ae1ac070e5df7d68282269b26d7b3ae6d14eb0e1`. Source commit: `ddc060185f9501ea69ce51af6a194cacd6543f08`. The original helper at 0026AC60 stores count at offset zero, capacity at offset four and the entry buffer at offset eight. The new neutral shared storage preserves that order. The public sead::PtrArray declaration is unchanged.

The normal ARMCC project build linked, and the canonical checker independently preserved the full 260-byte fn_001B4708 interval. Compiler provenance identified the changed source and new header closure as one object with one accepted definition. The shared-type gate passed. Source hashes equal the supplier hashes, and the scratch map was restored exactly. No map, rank, ledger, target, checker or compiler-configuration change is included.

Private driver receipts: count_first_array_intake.json and count_first_array_intake_checks.json. Integrator full preservation remains required; any accepted-root demotion rejects this proposal. No complete dependent root is claimed unblocked.
