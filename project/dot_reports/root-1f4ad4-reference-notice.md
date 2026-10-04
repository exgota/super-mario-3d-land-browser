Claims: none. Header reference/adaptation notice.
The Handle, HandleObject, WaitObject, InterruptEvent, EventBase and HidBase layout headers
are adapted from https://github.com/3dsdecomp/nnsdk at d50a4b89d13c929d2438abf92e5a48f039efffbf,
paths Include/nn/Handle.h, Include/nn/os/os_{HandleObject,WaitObject,InterruptEvent,EventBase}.h,
and Include/nn/hid/CTR/hid_HidBase.h.
The inspected tree has no LICENSE/NOTICE file; no additional license grant is inferred.
Original headers and exact blob hashes are retained in the reference packet.
Adaptations: current-project nn/types.h casing, qualified Handle include, and a resource getter.
RE-Pepper/ctrsdk82b1d4e162d69b434afb6aa0c2638eab90d959e1 also confirms HidBase size8 and resource+4,
with a GetResource accessor returning uptr. The chosen nnsdk storage is void*; our accessor keeps it.
RE-Pepper's README states that its code was referenced from a purchased/dumped studio debug binary.
That original notice remains retained; the owner explicitly authorized these named references.
No setup scripts, SDK assembly, raw function bytes or game data were copied.
