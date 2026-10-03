namespace {
struct Object {
    void* pad[10];
    void* field_28;
};
}

extern "C" void* fn_00129080(void*);
extern "C" void* fn_001678BC(void*);
extern "C" void* fn_00243E74(void*);

extern "C" void* fn_00129074(Object* self) {
    return fn_00129080(*static_cast<void**>(self->field_28));
}

extern "C" void* fn_001678B0(Object* self) {
    return fn_001678BC(*static_cast<void**>(self->field_28));
}

extern "C" void* fn_00243E68(Object* self) {
    return fn_00243E74(*static_cast<void**>(self->field_28));
}
