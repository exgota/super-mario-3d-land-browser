# Actor initialization needs two parameter rows

Branch: `dot/root-307010`
Frozen base: `eeaca9ad7a606674b428b29630da497031ff913d`
Target: unnamed `0x00307010`, 516 bytes, U.
Claims: none. No source candidate, build or canonical check attempted.

Missing data addresses: `0x0042F644` and `0x0042F660`.
Sizes: both unknown; their spacing does not establish either exact extent.
Reason: the routine passes the first object to constructor 0x0027B54C and the second to constructor 0x0027AF28 while setting up two actor states. Fresh main has neither exact nor containing rows at these addresses. Other visible external data references already have rows, including SafeString's vtable address point and the state nerves.

Integrator action: independently establish both parameter objects and their extents, add the rows, then return this root for source reconstruction and canonical regrading. Moving to another unowned 516-byte tie. No map, rank, tool or data changes are proposed.
