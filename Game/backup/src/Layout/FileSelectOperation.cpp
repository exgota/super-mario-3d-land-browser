namespace al { class Nerve; }
extern "C" const al::Nerve dat_003F192C;
extern "C" const al::Nerve dat_003F18F0;

#include <Layout/alLayoutActor.h>
#include <Nerve/alNerveFunction.h>
#include <stdint.h>
#include <stddef.h>

namespace {

struct UseNerve;
struct SaveData;

struct FileSelectSlot {
    SaveData* data;
    void* unknown04;
    int index;
    unsigned char state;
};

// FileSelect constructor 0014BBAC initializes mode and both slot indices to -1.
struct FileSelect {
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
    UseNerve* copyState;
    UseNerve* deleteState;
};

static_assert(offsetof(FileSelect, mode) == 0x2d, "");
static_assert(offsetof(FileSelect, slots) == 0x30, "");
static_assert(offsetof(FileSelect, dialogs) == 0x54, "");
static_assert(offsetof(FileSelect, copyState) == 0x74, "");
static_assert(offsetof(FileSelectSlot, index) == 8, "");
static_assert(offsetof(FileSelectSlot, state) == 0xc, "");

extern "C" void fn_0027DFE8(void*);
extern "C" void fn_00264E2C(FileSelect*, bool, bool);
extern "C" void fn_00264D34(FileSelectSlot*, bool);
extern "C" bool fn_00264950(const UseNerve*);
extern "C" bool fn_00264930(const UseNerve*);
extern "C" void fn_0016ACEC(int);
extern "C" void* fn_0026493C();
extern "C" void fn_0014E22C(void*, int);
extern "C" void fn_00264CFC(const char*);
extern "C" uint32_t fn_001A57DC();
extern "C" void fn_0014C3E0(int, int);
extern "C" void fn_0014E1D4(void*, int, int);
extern "C" void fn_0026495C(FileSelectSlot*);
extern "C" void fn_002744FC(UseNerve*);
extern "C" const char dat_003B7ECC[];
extern "C" const char dat_003B7EDC[];

inline void setSlotsEnabled(FileSelect* self, bool enabled) {
    fn_00264D34(self->slots[0], enabled);
    fn_00264D34(self->slots[1], enabled);
    fn_00264D34(self->slots[2], enabled);
}

}

// The accepted Factory thunk still has a broad placeholder return/signature.
// This body follows the observed void nerve-update contract; that declaration
// needs integrator reconciliation before treating the combined C++ ABI as final.
extern "C" void fn_0014AEDC(FileSelect* self) {
    fn_0027DFE8(self->firstSelector);
    fn_0027DFE8(self->secondSelector);
    al::LayoutActor* dialog = self->dialogs[self->mode];
    if (al::isFirstStep(reinterpret_cast<al::IUseNerve*>(self))) {
        fn_00264E2C(self, false, false);
        setSlotsEnabled(self, false);
        dialog->appear();
    }

    if (fn_00264950(reinterpret_cast<const UseNerve*>(dialog))) {
        if (self->mode != 0 && self->mode != 1) {
            FileSelectSlot* selected = self->slots[self->selectedSlot];
            fn_0016ACEC(selected->index);
            fn_0014E22C(fn_0026493C(), selected->index);
            fn_00264CFC(dat_003B7ECC);
            fn_001A57DC();
            al::setNerve(reinterpret_cast<al::IUseNerve*>(self), &dat_003F192C);
        } else {
            FileSelectSlot* selected = self->slots[self->selectedSlot];
            FileSelectSlot* other = self->slots[self->otherSlot];
            fn_0014C3E0(other->index, selected->index);
            fn_0014E1D4(fn_0026493C(), other->index, selected->index);
            fn_00264CFC(dat_003B7EDC);
            fn_001A57DC();
            FileSelectSlot* restored = self->slots[self->otherSlot];
            restored->state = 1;
            fn_0026495C(restored);
            self->otherSlot = -1;
            al::setNerve(reinterpret_cast<al::IUseNerve*>(self), &dat_003F192C);
        }
    } else if (fn_00264930(reinterpret_cast<const UseNerve*>(dialog))) {
        switch (self->mode) {
        case 0:
        case 1: {
            FileSelectSlot* restored = self->slots[self->otherSlot];
            restored->state = 1;
            fn_0026495C(restored);
            self->otherSlot = -1;
            fn_002744FC(self->copyState);
            break;
        }
        case 2: {
            FileSelectSlot* restored = self->slots[self->selectedSlot];
            restored->state = 1;
            fn_0026495C(restored);
            self->selectedSlot = -1;
            fn_002744FC(self->deleteState);
            break;
        }
        }
        self->mode = -1;
        al::setNerve(reinterpret_cast<al::IUseNerve*>(self), &dat_003F18F0);
    }
}
