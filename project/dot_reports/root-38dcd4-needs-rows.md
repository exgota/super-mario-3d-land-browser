# Static record initialization needs a row

Branch: dot/root-38dcd4. Base: f8e79c19657c5dd4d57631b6456bd353e8fe1726.
Target: 0038DCD4, 532 bytes, currently U. Exact claims: none.
Missing address: 0x0042FDD4. No exact or containing row exists in this base's map.
The routine writes eight consecutive records at stride0x1c, including each record's final 4-byte field at +0x18.
Minimum writable extent is 0xe0 bytes (224). Complete object extent, type, and owner remain unproved.
Each record has pointer/integer fields, byte flags, and a floating field; this does not establish a complete class definition.
Separate pointer stores span003F2B60..003F2B78 (24 bytes), crossing six distinct 4-byte rows at003F2B60,64,68,6C,70,74.
Every stored word has individual row coverage; neither one combined array row nor a single owning array identity is established.
Any source treating that span as one array needs separate identity/extent justification; no boundary merge is proposed here.
No source, build, checker, replay, or preservation claim was produced.
Integrator should establish the missing row before revisiting this target.
