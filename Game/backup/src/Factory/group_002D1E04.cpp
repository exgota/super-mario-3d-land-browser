namespace {
struct DispatchTable { void* unused; void (*slot)(void*); };
struct Subobject { DispatchTable* table; };
struct Object { void* unused; Subobject* subobject; };
}
#define BODY \
    Object* self = (Object*)arg; \
    Subobject* subobject = self->subobject; \
    DispatchTable* table = subobject->table; \
    return table->slot(subobject)
extern "C" void fn_002D1E04(void* arg) { BODY; }
extern "C" void fn_002D27E8(void* arg) { BODY; }
extern "C" void fn_002D2C40(void* arg) { BODY; }
extern "C" void fn_002D410C(void* arg) { BODY; }
extern "C" void fn_002D411C(void* arg) { BODY; }
