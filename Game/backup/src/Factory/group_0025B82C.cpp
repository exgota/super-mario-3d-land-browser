namespace {
struct Item {
    unsigned char pad0[8];
    void* value;
    unsigned char pad1[20];
};

struct Owner {
    unsigned char pad0[0x30];
    unsigned count;
    Item* items;
};

extern "C" void fn_0026B434(void*, void*);
extern "C" void fn_0026B04C(void*, void*);
}

extern "C" void fn_0025B82C(Owner* self, unsigned index, void* arg) {
    Item* item = self->items;
    if (self->count > index)
        item += index;
    fn_0026B434(item->value, arg);
}

extern "C" void fn_0025BB1C(Owner* self, unsigned index, void* arg) {
    Item* item = self->items;
    if (self->count > index)
        item += index;
    fn_0026B04C(item->value, arg);
}
