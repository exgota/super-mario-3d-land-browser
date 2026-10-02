# Actor registry and primary table evidence

The owned EU executable reproduces the earlier 268 name/creator pairs and 222 heuristic links. Those 268 entries combine 214 actors, 15 cameras, 14 demos, 15 start events and 10 areas. The earlier filter required map Type exactly `f`, excluding 11 actor creators and 26 area creators with other function flags. The number 268 is not the actor registry size, and 222 is not a count of proven C++ classes.

The complete ActorFactory registry contains 225 entries. Its lookup loads the table at 0x003B99F0 and compares the loop index against 225. Each registry creator was independently traced through allocation, the null guard, the construction call or tail call, and an unconditional primary table installation at receiver offset zero. Native ARM decoding confirms all 225 transfers and stores. The semantic records are sorted by registry identifier, not original table order.

`actor_catalog.json` contains the historical snapshot, the 225 construction records, and an exact-address-point lookup for worker context. Thirteen entries have a separately evidenced current project C++ identity. Registry spelling alone proves no original C++ class name. Shared virtual targets do not establish unique method ownership. Three records retain explicit limits for later calls on the receiver; this scan does not prove complete object layouts, secondary tables, every group boundary, or final dynamic state after every possible call.

The RailDot row at 0x003D3998..0x003D3AC8 also contains the Seagull construction's primary address point at 0x003D3A30. RailDot's independently named constructor installs 0x003D3998. The catalog records both stores and leaves the map untouched. Worker annotations must resolve the actual primary address point rather than assigning the entire row to one class.

## Reproduce

Supply the owner's decompressed EU executable locally at `data/ver/eu/code.bin`, with the BRIEF SHA-256. From the repository root:

```sh
. ./development_environment.sh
python tools/factory/inspect_actor_registry.py > /tmp/sm3dl_actor_registry_verification.json
```

The default revision is the frozen evidence baseline `e2336037d4f709acc082b3322daedadf9500564a`. The output's `actor_records` must equal `project/actor_catalog.json` at `actor_registry.entries`; `legacy_records` must equal `historical_heuristic_scan.entries`. The driver independently reran the stored program and obtained exact equality for all 225 and 268 records. The program uses Python's standard library, Git, c++filt and arm-none-eabi-objdump. It reads the owned binary privately and emits only semantic identifiers, addresses, offsets and relationships.

No binary bytes, raw table order, disassembly, ranks or function boundaries are published. No byte-exact function credit is claimed. The driver deployed packet integration on 2026-10-02 after three reference-safety checks and direct verification of four packet modes. New jobs receive bounded references for applicable targets. See `project/packet_reference_deployment.md` for the observed rollout and limits.
