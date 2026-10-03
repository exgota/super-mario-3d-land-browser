namespace {
struct RelativeAddress {
    unsigned int unknown;
    int displacement;
};
}

extern "C" void *fn_0033E5AC(void *self) {
    return static_cast<char *>(self) + reinterpret_cast<RelativeAddress *>(self)->displacement;
}

extern "C" void *fn_0033E61C(void *self) {
    return static_cast<char *>(self) + reinterpret_cast<RelativeAddress *>(self)->displacement;
}

extern "C" void *fn_00341994(void *self) {
    return static_cast<char *>(self) + reinterpret_cast<RelativeAddress *>(self)->displacement;
}
