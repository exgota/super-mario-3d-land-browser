namespace {
typedef unsigned int Word;

struct Interface {
    Word **vtable;
};

struct Object {
    unsigned char pad[0x1c];
    Interface *interface;
};

typedef Word (*Method)(Interface *);
}

extern "C" Word fn_0013E024(void *self) {
    Interface *interface = static_cast<Object *>(self)->interface;
    Method method = reinterpret_cast<Method>(interface->vtable[1]);
    return method(interface);
}

extern "C" Word fn_0018A380(void *self) {
    Interface *interface = static_cast<Object *>(self)->interface;
    Method method = reinterpret_cast<Method>(interface->vtable[1]);
    return method(interface);
}
