#include <Player/ObservedSquatAction.h>
using namespace observed_squat;
extern "C" void fn_002531A8(ActionPrefix*);
extern "C" void fn_00253134(ActionPrefix*);
extern "C" bool fn_00252FB4(ActionPrefix*);
extern "C" void fn_0019DDAC(ActionPrefix*);
extern "C" bool fn_0019E22C(ActionPrefix*);
extern const char dat_003B1534[];

extern "C" void fn_0019DF5C(ActionPrefix* action) {
    switch (action->state->value) {
    case 0:
        fn_002531A8(action);
        if (action->services->animator->isAnim(sead::SafeString(dat_003B1534))) {
            if (!action->services->animator->isAnimEnd()) return;
        }
        if (action->services->predicate08->test() && action->services->predicate24->test()) {
            action->state->set(3);
            return;
        }
        break;
    case 1:
        fn_002531A8(action);
        fn_00253134(action);
        if (fn_00252FB4(action)) return;
        if (action->services->predicate08->test() && action->services->predicate24->test())
            action->state->set(3);
        else if (action->progress.bits==0x3F800000u)
            action->state->set(2);
        return;
    case 2:
        fn_002531A8(action);
        fn_00253134(action);
        if (fn_00252FB4(action)) return;
        if (action->services->predicate08->test() && action->services->predicate24->test())
            action->state->set(3);
        return;
    case 3:
        fn_002531A8(action);
        fn_00253134(action);
        if (fn_00252FB4(action)) return;
        break;
    case 4:
        fn_0019DDAC(action);
        fn_00253134(action);
        if (action->state->value==4 && action->counter<4) return;
        if (action->services->predicate08->test() && action->services->predicate24->test()) {
            action->state->set(3);
            return;
        }
        break;
    default:
        return;
    }
    fn_0019E22C(action);
}
