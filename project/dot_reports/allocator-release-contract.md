# Allocator release and storage contract preservation

Claims: none. This is a prerequisite proposal, with zero new matched bytes.
Base: `3e8b2608ac46560fc6078ce9b9a851d975861f6b`.
Connector observation: 2026-10-04 08:02:47 UTC; subsequent `git fetch origin main`
confirmed that same immutable base. No publication or production intake occurred.

Only `group_0024C880.cpp`, `group_0010EA24.cpp` and the new
`Game/backup/include/Util/AllocatorStorage.h` change. Before editing, all 266
connector-listed dot tips and 294 local dot refs were screened; no competing
variant owned these files. This does not prove absence of private factory leases.

The release path at 00283D70 advances the receiver by four bytes and forwards
its allocation pointer to 00283D78, which consumes that pointer through
`nn::fnd::detail::FreeToHeap` at 001F33E0. The accepted one-argument declaration
omitted a real argument. Both declarations now transport receiver and allocation
as `void*`; no other release or allocator API was invented.

003F0380 and 003F0384 are four-byte allocator pointer cells. A single shared
forward declaration owns their neutral `observed_allocator::Allocator` identity;
its original name and complete interface remain unknown. No class layout,
virtual members, provider, manager or texture interface is introduced.
`void* fn_0021DFB8()` and `void* fn_0021CDE4()` still return cell addresses.
All unrelated declarations, PublicPtrArray and other Factory files preserve.

A pristine normal project build and the committed form-1 normal build linked.
The unchanged project checker reports O-to-O for all 16 accepted definitions
in the two objects, totaling 160 complete bytes. The new header has exactly one
actual consumer, group_0010EA24.cpp. Project provenance and symbol scans found
no other committed-source consumer. Generated stubs were recorded separately.
All 1,116 project objects, including generated stubs, and the complete linked
ELF, map, linker script and exported code.bin are byte-identical to baseline.
Compiler, configuration, map, ranks, ledger, oracle and guard source preserve.

The fixed validity-aware complete-section diagnostic gives aggregate (0, 0).
The strict linked adapter passes all eight accessors. It refuses all eight tail
wrappers because linker relaxation changes their branch relocations; those
refusals are retained. Unchanged pristine asm-differ object mode compares every
word of all 16 complete sections against canonical-exact baseline ARMCC objects.
That diagnostic grants no exact credit; the project checker is authoritative.
Form 1 succeeds; no second form is warranted. Root 002FF3DC remains at four
historical forms used, with its missing historical source/proof qualification.
Provider and manager contract blockers remain outside this proposal.

Actual read-only Supervisor.merge_submission rejects this ordinary dot branch
because it changes existing Factory files. Separately calling the unchanged
shared-type gate passes with no violations or immutable-base decoding failure.
No fetch or merge was executed by the guard harness; refs, HEAD, index and map
were preserved. No integrator identity or operator approval was simulated.
The real operator-approved integrator route is still required for intake:
an `integrator/` submission with `operator_approved: true`, then the integrator's
full preservation/build/check cycle. This is not canonical acceptance.

Final source SHA-256 values:
- AllocatorStorage.h: `31ffa76f309fdb506d3f0fd30c80d0bd5f0f31661f473d873f3837351846d848`
- group_0010EA24.cpp: `b62fda896bb1d4ef318732e68344e6c1bf25def89f70f58d0c5fc26462372562`
- group_0024C880.cpp: `d1df5c444e84561d22b0b50772cd4931a59cc527bb1f63077e4679369a6cb58f`
