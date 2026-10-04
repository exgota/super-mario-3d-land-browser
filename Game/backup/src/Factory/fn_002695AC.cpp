#include <prim/seadSafeString.h>

struct Presentation;

extern "C" void fn_002695AC(Presentation* self, const sead::SafeString& name) {
    typedef void (*Dispatch)(Presentation*, const sead::SafeString&);
    Dispatch dispatch = reinterpret_cast<Dispatch>(
        (*reinterpret_cast<void***>(self))[24]);
    return dispatch(self, name);
}
