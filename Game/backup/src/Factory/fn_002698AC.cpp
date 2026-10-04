extern "C" void fn_002698B8(void* self);

extern "C" void fn_002698AC(void* self, void* argument) {
    typedef void (*Dispatch)(void*, void*);
    Dispatch dispatch = reinterpret_cast<Dispatch>(
        *reinterpret_cast<void**>(
            *reinterpret_cast<void***>(self) + 2));
    return dispatch(self, argument);
}
