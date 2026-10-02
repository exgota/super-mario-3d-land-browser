# Static parameter initialization needs a row

Branch: dot/root-3888e8. Base: e2336037d4f709acc082b3322daedadf9500564a.
Target: 003888E8, 544 bytes, U. Exact claims: none.
Missing address: 004300D0. No exact or containing row exists in the frozen map.
The target passes this base to helper 00211D5C, which directly stores eight floats at that input pointer: independently established minimum writable span 0x20 bytes.
The root then chains additional subobject initializers and parameter writes. The complete combined object extent and semantic owner remain unproved; 0x20 is only the first initializer's conservative lower bound.
Separate dispatch-pointer destinations 003F302C..003F3058 each have individual four-byte row coverage. Their combined array identity is not asserted.
No source, build, canonical checker, or replay was attempted. Integrator should establish the actual parameter-object row before this root is retried.
