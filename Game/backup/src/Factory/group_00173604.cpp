namespace {
struct ControlFields {
    unsigned int mPadding[2];
    void* mControl;
};
}

extern "C" void fn_0017360C(void*);
extern "C" void fn_001C9E48(void*);
extern "C" void fn_001D34E0(void*);
extern "C" void fn_001D3574(void*);
extern "C" void fn_001D377C(void*);
extern "C" void fn_001D3AB0(void*);
extern "C" void fn_001E8DE4(void*);
extern "C" void fn_001E8E98(void*);
extern "C" void fn_001E90A0(void*);
extern "C" void fn_0025CB78(void*);
extern "C" void fn_00268E80(void*);
extern "C" void fn_00272AEC(void*);
extern "C" void fn_003329D4(void*);
extern "C" void fn_00336D24(void*);
extern "C" void fn_00336D70(void*);
extern "C" void fn_0033F9DC(void*);

namespace al {
class LiveActor;
void startAction(LiveActor*, const char*);
}

#define WRAPPER(name, target) \
extern "C" void name(void* audioKeeper) { \
    ControlFields* fields = static_cast<ControlFields*>(audioKeeper); \
    target(fields->mControl); \
}

WRAPPER(fn_00173604, fn_0017360C)
WRAPPER(fn_001C9E40, fn_001C9E48)
WRAPPER(fn_001D34D8, fn_001D34E0)
WRAPPER(fn_001D356C, fn_001D3574)
WRAPPER(fn_001D3774, fn_001D377C)
WRAPPER(fn_001D3AA8, fn_001D3AB0)
WRAPPER(fn_001E8DDC, fn_001E8DE4)
WRAPPER(fn_001E8E90, fn_001E8E98)
WRAPPER(fn_001E9098, fn_001E90A0)
WRAPPER(fn_0025CB70, fn_0025CB78)
WRAPPER(fn_00268E78, fn_00268E80)
WRAPPER(fn_00272AE4, fn_00272AEC)
WRAPPER(fn_003329CC, fn_003329D4)
WRAPPER(fn_00336D1C, fn_00336D24)
WRAPPER(fn_00336D68, fn_00336D70)
WRAPPER(fn_0033F9D4, fn_0033F9DC)

extern "C" void fn_0027F23C(void* audioKeeper, const char* action) {
    ControlFields* fields = static_cast<ControlFields*>(audioKeeper);
    al::startAction(static_cast<al::LiveActor*>(fields->mControl), action);
}
