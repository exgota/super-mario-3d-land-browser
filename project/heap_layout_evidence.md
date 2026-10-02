# Clean sead Heap base layout

Root lane `root/heap-layout`, original base `0800d9a`. No function implementation, rank, boundary, flag, table or matching credit belongs to this header family. Inputs are the clean local headers and the owner's verified EU executable, SHA-256 `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. No external sead implementation or removed history was consulted.

The original constructor at `0x0028B6A4..0x0028B7F0` calls the independently named `IDisposer` constructor `0x00293374` on the unadjusted receiver, then replaces its virtual pointer. This establishes the primary polymorphic disposer base. The disposer constructor initializes its heap pointer at +4 and two link pointers at +8/+0xC. Its destructor at `0x00271D58` reads the same heap field and calls the named Heap removal operation. Base size is 0x10.

| Offset | Size | Declaration | Independent evidence |
| --- | ---: | --- | --- |
| 0x00 | 0x10 | IDisposer base | Base constructor call; named disposer constructor and destructor |
| 0x10 | 8 | SafeString mName | Installs a string virtual pointer at +0x10 and copies incoming name's +4 pointer to +0x14; established clean SafeString shape |
| 0x18 | 4 | void* mStartAddress | Stores incoming address; FrameHeap creator reads it to form both allocation endpoints |
| 0x1C | 4 | u32 mSize | Constructor preserves a stack word through VFP; FrameHeap creator loads it as an integer and subtracts 0x78 to compute its usable region |
| 0x20 | 4 | Heap* mParent | Parent controls insertion into its child list and conditional locking |
| 0x24 | 0x10 | OffsetList<Heap> mChildren | Circular sentinel at +0x24/+0x28, count +0x2C, node offset +0x30; constructor sets node offset 0x34 |
| 0x34 | 8 | ListNode mChildNode | Two zero words; parent insertion uses child receiver plus stored offset 0x34 |
| 0x3C | 0x10 | OffsetList<IDisposer> mDisposers | Sentinel +0x3C/+0x40, count +0x44, offset +0x48; named append/remove use offset 8 into each disposer |
| 0x4C | 1 | u8 mDirectionEncoding | Constructor stores low byte of incoming direction; FrameHeap creator compares the byte with forward encoding 1 |
| 0x4D | 3 | Reserved bytes | Keep the observed alignment gap without semantic fields |
| 0x50 | 0x1C | Opaque lock storage | Constructor 0x00291240 constructs its own 0x10 disposer prefix and atomic state at +0x10/+0x14/+0x18; lock/unlock receive Heap+0x50 |
| 0x6C | 4 | u32 mFlags | Initializes 4 or 5; bit 0 gates every observed lock/unlock pair |

The original FrameHeap creator `0x00105EBC..0x00106030` requests at least 0x78 bytes, calls this base constructor, then writes its own fields at +0x70 and +0x74. Combined with the base's final word at +0x6C, this independently bounds the complete base extent at 0x70. FrameHeap's derived fields are outside this submission; the existing derived declaration remains incomplete.

The header now inherits IDisposer and declares the observed base storage. The disposer header forward-declares Heap to avoid an include cycle. Existing virtual declarations and ordering remain identical: destructor at slots 0/4, reserved slots 8..0x20, freeAll at 0x24. The original `MemorySystem::freeAllSequenceHeap` at 0x001C0ECC independently dispatches through +0x24.

`mDirectionEncoding` preserves the raw byte, including reverse 0xFF, without asserting signed reads that these functions do not show. Critical-section storage retains four-byte alignment and 28 bytes without declaring its unproved class identity or lifecycle. The nested lists establish storage and stored node offsets; this patch does not extend the existing OffsetList insertion API. No heap allocation, ownership, lock, constructor or destructor implementation is supplied.

The self-contained Pro question is `.integrator/pro_queue/layout/sead_heap_base.md`; its response is advisory. Root must validate the committed final headers with ARMCC layout probes, a clean build and relevant existing callers before submission. Full-map preservation belongs to the integrator. Native heap services and runtime behavior remain unverified.
