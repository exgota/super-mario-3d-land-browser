# Clean sead declarations

This module was written from the existing `Game/` and `lib/al` call sites, the project's EU symbol map, and the owner's decompressed EU executable. No removed library source, removed Git history, RedPepper `Library/`, or external sead implementation was consulted.

The headers provide the smallest interface currently required to compile the game sources. They do not establish matching sead functions. Unrecovered runtime operations remain declarations. The full SDK object layouts and a port runtime are not implemented.

## Binary evidence

Addresses below are virtual addresses in the EU executable. The executable's load base is `0x00100000`; subtract it to locate instruction bytes in `data/ver/eu/code.bin`.

| Interface | Evidence | Reconstructed portion |
| --- | --- | --- |
| `Vector3<float>` | `al::lerpVec` at `0x00269790`; pose setters copy three floats | `x`, `y`, `z` at offsets 0, 4, 8; size 12 |
| `Quat<float>` | `__sti___12_seadQuat_cpp` at `0x00381350`; `ActorPoseKeeperTQSV::getScale` at `0x00335074` | Four floats; unit initialized to `(0, 0, 0, 1)` at `0x00430500`; size 16 |
| `Matrix34<float>` | `al::calcSideDir`, `calcUpDir`, `calcFrontDir` at `0x0027D0E0`, `0x0027D0A0`, `0x00278660` | Three rows of four floats; column reads follow a 16-byte row stride; size 48 |
| `SafeStringBase<char>` | `al::MapObjActor` constructor at `0x00280428` | Virtual table pointer at 0; character pointer at 4; `cstr()` calls a virtual termination hook at virtual table offset 8 before reading the pointer |
| `BufferedSafeStringBase<char>` and `FixedSafeString` | `FixedSafeString<128>` assignment at `0x001090CC`; `StringTmp<256>` constructor at `0x0028E350` | Capacity at 8; inline buffer at 12; final buffer byte is terminated; fixed size is capacity plus 12 |
| `FixedSafeString<64>` | `al::Sequence::unk1` at `0x00318A4C` reads scene pointer at `0x54`; `NerveExecutor` occupies 8 bytes | String member size `0x4C`, placing the scene pointer at `8 + 0x4C` |
| Game-facing `PtrArray` | `al::LiveActorGroup` constructor at `0x00277DA0`; `registerActor` at `0x001CAB14`; `HitSensorKeeper::invalidate` at `0x001CCDB0` | Capacity at 0, count at 4, buffer at 8; size 12; append stores a pointer then increments count |
| `ListNode` and `ListImpl` | `ListNode::insertFront_` at `0x0028E948`; `ListImpl::clear` at `0x0021EF6C` | Previous and next pointers at 0 and 4; sentinel node followed by count at 8; circular sentinel initialization |
| `OffsetList` and `OffsetListNode<T*>` | `PlayerActionMultiCondition` constructor at `0x00252054`; append at `0x00252008` | OffsetList size 16, stored node offset at 12; pointer payload node size 12; list-node offset 4 |
| `IDisposer` and Application singleton prefix | `IDisposer` constructor at `0x00293374`; `Application::createInstance` at `0x0010025C` | Virtual pointer, heap pointer, and two node pointers fill 16 bytes; Application's ordinary fields start at `0x10`; singleton and disposer pointers are consecutive at `0x003E23C4` and `0x003E23C8` |
| Heap dispatch | `MemorySystem::freeAllSequenceHeap` at `0x001C0ECC` | `freeAll()` dispatches through virtual table offset `0x24`; preceding slots have reserved declarations without inferred semantics |
| Controller lookup | `al::isPadTrigger` at `0x0024CA4C` | ControllerMgr count at `0xD0`, controller buffer at `0xD8`; controller trigger mask at 4; out-of-range comparison treats index as unsigned |
| Player action RTTI declaration | `PlayerActionGraph::move` at `0x0018153C` | `PlayerAction::update()` uses virtual table offset `0x10`, consistent with one leading RTTI slot and the destructor before move/update |

Named trigger wrappers establish the following pad indices: A 0, B 1, X 3, Y 4, Select 12, Touch 15, Up 16, Down 17, Left 18, Right 19, LeftStickUp 20, LeftStickDown 21, LeftStickLeft 22, LeftStickRight 23. For example, `isPadTriggerX` at `0x00225C44` passes mask 8, and `isPadTriggerLeftStickRight` at `0x00263850` passes mask `0x00800000`.

## Limits

The L, R, Start, Home, and Minus indices remain declarations until their wrappers are identified. Heap root lookup, current-heap changes, allocation APIs, disposer lifetime, runtime type information bodies, and formatting bodies remain declarations. ControllerMgr's leading bytes and the Heap virtual slots before `freeAll()` are opaque. Quaternion component labels follow the existing game-facing API; the size and unit ordering are observed.

The vector constant addresses recovered from static initialization and pose getters are zero `0x004305F8`, ones `0x004305EC`, ex `0x004305C8`, ey `0x004305D4`, and ez `0x004305E0`. These constants are declared, not defined, in this header-only reconstruction.

Compile and byte-exact check results are maintained in the project's attempt ledger and decision log. This module has no independent matching claim.
