#pragma once

#include <Nerve/alNerveKeeper.h>
#include <stddef.h>

namespace al { class LayoutActor; }

// A partial, binary-grounded state view. This does not claim the complete FileSelect class.

struct FileSelectSlot {
    void* data;
    void* unknown04;
    int index;
    unsigned char state;
};

// FileSelect constructor 0014BBAC initializes mode and both slot indices to -1.
struct FileSelectOperationState {
    unsigned char layoutActor[0x2d];
    signed char mode;
    unsigned char padding2e[2];
    FileSelectSlot* slots[3];
    int selectedSlot;
    int otherSlot;
    void* firstSelector;
    void* secondSelector;
    void* unknown4c;
    void* unknown50;
    al::LayoutActor* dialogs[4];
    void* unknown64[4];
    al::IUseNerve* copyState;
    al::IUseNerve* deleteState;
};

static_assert(offsetof(FileSelectOperationState, mode) == 0x2d, "");
static_assert(offsetof(FileSelectOperationState, slots) == 0x30, "");
static_assert(offsetof(FileSelectOperationState, dialogs) == 0x54, "");
static_assert(offsetof(FileSelectOperationState, copyState) == 0x74, "");
static_assert(offsetof(FileSelectSlot, index) == 8, "");
static_assert(offsetof(FileSelectSlot, state) == 0xc, "");

extern "C" void fn_0014AEDC(FileSelectOperationState* state);
extern "C" void fn_0014AED4(const al::Nerve* nerve, al::NerveKeeper* keeper);
extern "C" bool fn_00264930(const al::IUseNerve* state);
extern "C" bool fn_00264950(const al::IUseNerve* state);
extern "C" void fn_002744FC(al::IUseNerve* state);
