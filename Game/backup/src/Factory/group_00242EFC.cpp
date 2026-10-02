namespace {
struct Link {
    unsigned int reserved;
    Link* next;
};
}

extern "C" void* fn_00242EFC(Link* self) {
    return self->next->next;
}

extern "C" void* fn_0032EABC(Link* self) {
    return self->next->next;
}

extern "C" void* fn_0032ED20(Link* self) {
    return self->next->next;
}

extern "C" void* fn_003305EC(Link* self) {
    return self->next->next;
}
