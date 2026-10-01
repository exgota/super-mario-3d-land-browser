# LiveActor data ownership

The existing `_ZTVN2al9LiveActorE` start at `0x003D7974` is already correct. Its complete table ends at `0x003D7A0C`, 152 bytes later. The current map row ends at `0x003D7A14`, including eight bytes owned by the following class's ABI header.

This endpoint defect does not change the vtable relocation in the constructor. Fresh ordinary ARMCC C++ from unchanged production source and header diagnostically equals the full 156-byte constructor at `0x0027FE9C..0x0027FF38` with the already named table base. No source correction or start-address repair is needed for that constructor. Only canonical committed project output and the project checker can establish acceptance.

The target SHA256 remains `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. This lane did not edit production, the map, ranks, ledger, target, tools, or shared state. Every output is under the ignored `build/phase_two_vtable_layouts/` directory.

## Independent retail identity

Retail constructor `0x0027FE9C..0x0027FF38` installs primary address point `0x003D797C` at complete-object offset zero. It installs effect point `0x003D79E4` at offset four, audio point `0x003D79F0` at offset eight, and stage-switch point `0x003D7A04` at offset twelve. It supplies object offset `0x54` to the independently named `LiveActorFlag` constructor at `0x001C70FC`. That constructor initializes nine byte flags. It then calls named `getLiveActorKit` at `0x00277250`, loads the all-actors group at kit offset `0x3C`, and dispatches the group's registration slot with this actor.

The primary header at `0x003D7974` has offset-to-top zero and a zero RTTI pointer. The effect header at `0x003D79DC` has offset-to-top -4 and a zero RTTI pointer. The audio header at `0x003D79E8` has offset-to-top -8 and a zero RTTI pointer. The stage-switch header at `0x003D79FC` has offset-to-top -12 and a zero RTTI pointer.

The primary address point has these 24 entries:

| Offset from primary point | Retail value | Source slot identity |
| --- | --- | --- |
| `0x00` | `0x003375F4` | getNerveKeeper |
| `0x04` | `0x001EBF28` | init |
| `0x08` | `0x001EBE64` | initAfterPlacement |
| `0x0C` | `0x0027FF70` | appear |
| `0x10` | `0x0027EA40` | makeActorAppeared |
| `0x14` | `0x0027EBF0` | kill |
| `0x18` | `0x0027D250` | makeActorDead |
| `0x1C` | `0x0026DF44` | movement |
| `0x20` | `0x0025AA34` | calcAnim |
| `0x24` | zero | draw |
| `0x28` | `0x002795C0` | startClipped |
| `0x2C` | `0x002797F4` | endClipped |
| `0x30` | `0x001EBC60` | attackSensor |
| `0x34` | `0x001EBC58` | receiveMsg |
| `0x38` | `0x003375DC` | getBaseMtx |
| `0x3C` | `0x00337604` | getEffectKeeper |
| `0x40` | `0x003375FC` | getAudioKeeper |
| `0x44` | zero | getStageSwitchKeeper |
| `0x48` | zero | initStageSwitchKeeper |
| `0x4C` | `0x001EBF2C` | control |
| `0x50` | zero | calcAndSetBaseMtx |
| `0x54` | `0x001EBCEC` | updateCollider |
| `0x58` | zero | Existing unnamed v22 slot |
| `0x5C` | zero | Existing unnamed v23 slot |

The effect secondary table has one entry, at `0x003D79E4`: thunk `0x003765A8`. It reads interface-relative offset `0x30`; since the interface is complete-object +4, it returns complete-object +`0x34`, the effect field. The ordinary primary effect getter at `0x00337604` independently reads complete-object +`0x34`. This identifies the unnamed function at `0x003765A8` as `_ZThn4_NK2al9LiveActor15getEffectKeeperEv`.

The audio secondary table has three entries at `0x003D79F0`, `0x003D79F4`, and `0x003D79F8`: zero, zero, and named thunk `0x00376A44`. The thunk reads interface-relative +`0x30`, or complete-object +`0x38`. The ordinary primary getter at `0x003375FC` independently reads complete-object +`0x38`. This identifies that unnamed eight-byte function as `_ZNK2al9LiveActor14getAudioKeeperEv`.

The stage-switch secondary table has two entries at `0x003D7A04` and `0x003D7A08`: named getter thunk `0x00375F28` and named initializer thunk `0x00375E4C`. The getter reads interface-relative +`0x30`, or complete-object +`0x3C`. The initializer explicitly subtracts twelve from its interface pointer, allocates eight bytes, calls the keeper constructor, and stores the keeper at complete-object +`0x3C`. Their observed this-adjustments independently establish the last secondary interface and the table's endpoint at `0x003D7A0C`.

## Adjacent owners

The preceding named `AreaShape` table at `0x003D7960..0x003D7974` is already correct. Its two header words are zero and its address point is `0x003D7968`. All three entries are the established pure-virtual handler `0x00377DA4`. Independently named `AreaShape` base constructor `0x0024E028..0x0024E054` installs `0x003D7968` and initializes its own shape fields. The following zero words at `0x003D7974` and `0x003D7978` therefore belong to LiveActor's header, not to extra AreaShape virtual entries.

The successor is an independently constructed 168-byte derived-LiveActor table at `0x003D7A0C..0x003D7AB4`. Its primary address point is `0x003D7A14`. Its primary table has 28 entries, with four additional slots after the inherited 24. Those added entries at `0x003D7A74`, `0x003D7A78`, `0x003D7A7C`, and `0x003D7A80` point to `0x001D9790`, `0x001D2100`, `0x001D979C`, and `0x00334690` respectively. Its effect secondary point is `0x003D7A8C`, audio point `0x003D7A98`, and stage-switch point `0x003D7AAC`. The respective header offset-to-top values are -4, -8, and -12, and all RTTI-pointer words are zero.

Constructor `0x001EC3BC..0x001EC3E8` calls base constructor `0x001D21FC`, then explicitly installs primary `0x003D7A14` and its three secondary points. Factory `0x00398DAC..0x00398DD4` allocates 196 bytes and tail-calls `0x001EC3BC`. Its intermediate base constructor `0x001D21FC..0x001D22BC` calls LiveActor's constructor, then installs another derived table and starts its additional fields at complete-object offset `0x60`. The LiveActor flag accesses ending at `0x5C`, and this independent derived-member boundary, corroborate the current 96-byte LiveActor layout. This lane found no standalone 96-byte allocation of the base class itself.

The next unrelated header begins at `0x003D7AB4`, with its address point at `0x003D7ABC`. Constructor `0x0028CB38..0x0028CB70` installs that point after calling its own base constructor and then processes its variadic format arguments. Its first entry is empty return `0x001EC3E8`, its second entry is zero, and its third entry is `0x0039E0E4`. This separate constructor and address point establish the successor's endpoint. No source name is invented for either adjacent class.

## Ordinary C++ corroboration

One fresh ARMCC 4.1 build 791 invocation copied `lib/al/src/LiveActor/alLiveActor.cpp` and `lib/al/include/LiveActor/alLiveActor.h` unchanged into the ignored probe directory. It emitted a 152-byte ordinary C++ table, with the same primary and three secondary ABI-header offsets. Its compiler-generated table relocations establish 24 primary entries, one effect entry, three audio entries, and two stage-switch entries. No assembly, instruction arrays, source edits, compiler-flag changes, or object edits were used.

The complete constructor section is 156 bytes including its literal pool. Applying standard relocations only in an in-memory diagnostic, using independently named imports, gives the same SHA256 as retail: `9c2e3e3a160f175dcb9424481d96869c37b2705bdc7eaf5718f039f598c3b828`. The imports are `_ZTVN2al9LiveActorE` at `0x003D7974`, `_ZN2al13LiveActorFlagC1Ev` at `0x001C70FC`, and `_ZN2al15getLiveActorKitEv` at `0x00277250`. All 156 bytes agree. This is one scratch diagnostic and zero accepted functions.

Commands, compiler environment, source/header/object hashes, binary-range hashes, every retail word and slot relocation, and the diagnostic comparison are in `ownership_evidence.json`. Its accompanying `retail_ownership.txt` is the focused retail disassembly. Reproduce from the repository root:

```sh
. ./development_environment.sh
python build/phase_two_vtable_layouts/audit_live_actor_ownership.py
```

## Data-only repair proposal

The existing two `dc` rows spanning `0x003D7974..0x003D7ABC` are respectively 160 and 168 bytes. Replace them with the true complete LiveActor table, the independently owned complete derived table, and the following header fragment. Preserve the exact 328-byte union interval and every byte. Keep all ranks U and every function interval unchanged.

```csv
0x003D7974,          ,0x003D7A0C,          ,U,dc,_ZTVN2al9LiveActorE,
0x003D7A0C,          ,0x003D7AB4,          ,U,dc,,
0x003D7AB4,          ,0x003D7ABC,          ,U,dc,,
```

The preceding AreaShape row needs no repair. Apply any endpoint repair in the root's separate metadata commit with this evidence in the decision log, as BRIEF hard rule 2 explicitly requires. The current constructor does not require this endpoint repair to resolve its table import, so canonical acceptance can proceed independently.

The two additional function names above are identity proposals for existing eight-byte function rows, not boundary changes or acceptance claims. They may be handled separately from the data repair.

## Uncertainty

Zero RTTI pointers and null function slots are directly observed. The null function values align with compiler-emitted ordinary C++ slots, and virtual-function elimination is a plausible explanation. This audit does not prove the original linker's mode or treat null entries as executable functions. The names of the two neighboring classes and semantic names for existing v22/v23 slots remain unresolved. Neither gap affects LiveActor table ownership or constructor byte diagnostics.
