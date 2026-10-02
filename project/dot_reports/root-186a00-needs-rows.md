# Vector initialization dependency needs a row

Branch: dot/root-186a00. Base: 8ae3d9d55e639057950efb7ba3a0c7ccc29c5f65.
Target: 00186A00, 536 bytes, currently U. Exact claims: none.
Missing address: 0x004305D4. No exact or containing row exists in this base's map.
The target copies three consecutive words at offsets 0, 4, 8 into a local vector, establishing a 12-byte minimum readable span.
This target alone establishes access bounds, not the semantic vector name or owner.
Published root-316014-needs-rows.md independently corroborates sead Vector3<float>::ey at 004305D4 from its static initializer, alongside ex at 004305C8.
The same missing address affects 00158D70, 00316014, and this target; one justified row may unblock all three.
No source, build, canonical checker, replay, or preservation claim was produced.
Integrator should establish the actual row before this target is retried.
