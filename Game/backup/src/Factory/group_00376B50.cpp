namespace {
typedef unsigned char Byte;
}

namespace al {
class LiveActor;
void* getTrans(const LiveActor*);
}

extern "C" void* fn_00216EDC(void*);

#define DEFINE_FORWARD(name) \
extern "C" void* name(void* self) { \
    return fn_00216EDC(static_cast<Byte*>(self) - 0x60); \
}

DEFINE_FORWARD(fn_00376B50)
DEFINE_FORWARD(fn_00376B58)
DEFINE_FORWARD(fn_00376B60)
DEFINE_FORWARD(fn_00376D00)
DEFINE_FORWARD(fn_00376F18)
DEFINE_FORWARD(fn_0037732C)
DEFINE_FORWARD(fn_003776C0)
DEFINE_FORWARD(fn_00377730)
DEFINE_FORWARD(fn_00377868)
DEFINE_FORWARD(fn_003778E8)
DEFINE_FORWARD(fn_00377950)
DEFINE_FORWARD(fn_00377958)
DEFINE_FORWARD(fn_00377B70)
DEFINE_FORWARD(fn_00377B98)
DEFINE_FORWARD(fn_00377BB0)

extern "C" void* fn_00377C68(void* self) {
    return al::getTrans(reinterpret_cast<const al::LiveActor*>(static_cast<Byte*>(self) - 0x60));
}
