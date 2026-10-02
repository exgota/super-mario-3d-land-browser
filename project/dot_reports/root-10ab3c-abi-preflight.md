# InterruptReceiver shared declaration prerequisite

Branch: dot/root-10ab3c. Base: 8ae3d9d55e639057950efb7ba3a0c7ccc29c5f65.
Target: 0010AB3C, 540 bytes, named nn::gxlow::CTR::InterruptReceiver::ReceiverThreadFunc(unsigned).
Accepted Factory group_00107AD0.cpp defines a source-local InterruptReceiver class containing only WaitAnyHandlerDone().
The target's receiver accesses lock/owner/depth at 0/4/8, callbacks at 10, event handle 2C, queue pointer 34, completion event 68, phase 76, and stop byte 77.
An ordinary shared class definition must reconcile those fields and the static thread entry declaration with the existing accepted TU.
A draft using a private view and a manually spelled mangled export was committed locally, then withdrawn during review before any canonical check.
The draft build failed because __mrc was undeclared; no canonical checker ran. No further source attempt was made.
The final patch contains only this note; no draft source or compiled object is submitted.
No fake compatible class or assembly/SVC stand-in is proposed. The accepted Factory file remains untouched.
Mapped data 003E25C4 is present; this is a shared C++ type/declaration prerequisite, not a missing-row or checker-failure claim.
No exact, runtime, or demotion claim. Integrator-owned declaration cleanup is required before an ordinary class implementation is retried.
