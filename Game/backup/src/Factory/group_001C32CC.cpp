namespace {
struct Root {
    char pad[0x28];
    void *next;
};
struct Middle {
    void *next;
};
struct Leaf {
    char pad[0x24];
    void *value;
};
}

extern "C" void *fn_001C32DC(void *);
extern "C" void *fn_0026246C(void *);
extern "C" void *fn_0033076C(void *);

extern "C" void *fn_001C32CC(Root *self) {
    return fn_001C32DC(static_cast<Leaf *>(static_cast<Middle *>(self->next)->next)->value);
}

extern "C" void *fn_0026245C(Root *self) {
    return fn_0026246C(static_cast<Leaf *>(static_cast<Middle *>(self->next)->next)->value);
}

extern "C" void *fn_0033075C(Root *self) {
    return fn_0033076C(static_cast<Leaf *>(static_cast<Middle *>(self->next)->next)->value);
}
