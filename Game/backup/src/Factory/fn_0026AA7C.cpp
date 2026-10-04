extern "C" int fn_0026AA7C(void* self, void* argument) {
    typedef int (*Callback)(void*, void*);
    Callback callback = reinterpret_cast<Callback>(
        reinterpret_cast<void**>(*reinterpret_cast<void***>(self))[15]);
    return callback(self, argument);
}
