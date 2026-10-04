extern "C" void fn_002697FC(void* self) {
    typedef void (*Dispatch)(void*);
    Dispatch dispatch = reinterpret_cast<Dispatch>(
        *reinterpret_cast<void**>(
            *reinterpret_cast<void***>(self) + 30));
    return dispatch(self);
}
