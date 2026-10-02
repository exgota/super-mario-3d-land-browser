namespace {
struct Object {
    unsigned char pad[0x18];
    void* next;
};
}

extern "C" void* fn_0018F9A4(void*);
extern "C" void* fn_001C7F34(void*);
extern "C" void* fn_001EA4DC(void*);
extern "C" void* fn_001EA5C0(void*);
extern "C" void* fn_001EA618(void*);
extern "C" void* fn_001EA67C(void*);
extern "C" void* fn_00248ED0(void*);
extern "C" void* fn_002490CC(void*);
extern "C" void* fn_00337064(void*);

extern "C" void* fn_0018F99C(Object* self) { return fn_0018F9A4(self->next); }
extern "C" void* fn_001C7F2C(Object* self) { return fn_001C7F34(self->next); }
extern "C" void* fn_001EA4D4(Object* self) { return fn_001EA4DC(self->next); }
extern "C" void* fn_001EA5B8(Object* self) { return fn_001EA5C0(self->next); }
extern "C" void* fn_001EA610(Object* self) { return fn_001EA618(self->next); }
extern "C" void* fn_001EA674(Object* self) { return fn_001EA67C(self->next); }
extern "C" void* fn_00248EC8(Object* self) { return fn_00248ED0(self->next); }
extern "C" void* fn_002490C4(Object* self) { return fn_002490CC(self->next); }
extern "C" void* fn_0033705C(Object* self) { return fn_00337064(self->next); }
