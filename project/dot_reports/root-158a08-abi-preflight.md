# Item stock layout constructor: actor-init helper prerequisite

- Branch `dot/root-158a08`; base `6b0e2a1385b814b70344023d9816424edcc7d832`.
- Target `0x00158A08..0x00158C44`, 572 bytes, remains U.
- The target calls the accepted LayoutActor constructor, creates six 0x60-byte LiveActor children through the public LiveActor constructor, and passes each child to 267F2C at 158AF4.
- That call carries the original actor-init-info argument, a stack SafeString, and a non-null string pointer as its fourth argument.
- Accepted Factory fn_0019BB88 declares 267F2C with private Actor/ActorInitInfo/SafeString types and an integer fourth parameter. Accepted group_00127BF0 uses a pointer fourth parameter but distinct private class types. No coherent ordinary/public import exists for this helper.
- The separate 27E6C8 call is not a blocker: accepted CourseSelectMap.cpp already declares lowercase fn_0027e6c8(al::LayoutActor*, const char*, const char*, int), which can be reused unchanged. This observation qualifies the older 321698 preflight and was returned to that lane for renewed ownership screening.
- Shared 267F2C repair remains integrator-owned. No integer/pointer adapter, duplicate alias, Factory change or source attempt.
- All directly referenced external data rows are present. No build or canonical check ran; only this short report is included.
