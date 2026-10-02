namespace al { class Nerve; }
extern "C" const al::Nerve dat_003F192C;
extern "C" const al::Nerve dat_003F18F0;

#include <Layout/FileSelectOperation.h>
#include <Layout/alLayoutActor.h>
#include <Nerve/alNerveFunction.h>
#include <stdint.h>
#include <stddef.h>

namespace {

extern "C" void fn_0027DFE8(void*);
extern "C" void fn_00264E2C(FileSelectOperationState*, bool, bool);
extern "C" void fn_00264D34(FileSelectSlot*, bool);
extern "C" void fn_0016ACEC(int);
extern "C" void* fn_0026493C();
extern "C" void fn_0014E22C(void*, int);
extern "C" void fn_00264CFC(const char*);
extern "C" uint32_t fn_001A57DC();
extern "C" void fn_0014C3E0(int, int);
extern "C" void fn_0014E1D4(void*, int, int);
extern "C" void fn_0026495C(FileSelectSlot*);
extern "C" const char dat_003B7ECC[];
extern "C" const char dat_003B7EDC[];

inline void setSlotsEnabled(FileSelectOperationState* self, bool enabled) {
    fn_00264D34(self->slots[0], enabled);
    fn_00264D34(self->slots[1], enabled);
    fn_00264D34(self->slots[2], enabled);
}

}

// Operation-state update from dot/root-14aedc, with a shared typed caller contract.
extern "C" void fn_0014AEDC(FileSelectOperationState* self) {
    fn_0027DFE8(self->firstSelector);
    fn_0027DFE8(self->secondSelector);
    al::LayoutActor* dialog = self->dialogs[self->mode];
    if (al::isFirstStep(reinterpret_cast<al::IUseNerve*>(self))) {
        fn_00264E2C(self, false, false);
        setSlotsEnabled(self, false);
        dialog->appear();
    }

    if (fn_00264950(reinterpret_cast<const al::IUseNerve*>(dialog))) {
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
    } else if (fn_00264930(reinterpret_cast<const al::IUseNerve*>(dialog))) {
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
