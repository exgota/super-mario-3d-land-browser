# Player-relative vector setup needs a data row

Branch: dot/root-15f010. Base: f8e79c19657c5dd4d57631b6456bd353e8fe1726.
Target: 0015F010, 532 bytes, currently U. Exact claims: none.
Missing address: 0x00430488; no exact or containing row exists in this map.
The target passes this address as the second vector input to helper0027CB48.
That helper reads three 32-bit floating components at offsets0,4,8, establishing a12-byte minimum span.
The vector is added to the player position before subsequent relative-vector calculations.
Complete object extent, owning class, and semantic vector name are not established.
No source, build, canonical checker, replay, or preservation claim was produced.
Integrator should establish the actual row before this target is retried.
