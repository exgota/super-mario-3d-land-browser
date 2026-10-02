namespace {
struct FinalObject { unsigned char pad[0x28]; void* value; };
struct MiddleObject { FinalObject* value; };
struct RootObject { unsigned char pad[0x28]; MiddleObject* value; };
}

extern "C" void* fn_0026252C(void*);
extern "C" void* fn_0024E9A8(void*);
extern "C" void* fn_00243A00(void*);
extern "C" void* fn_0024AD3C(void*);

extern "C" void* fn_0026ACDC(RootObject* object) { return fn_0026252C(object->value->value->value); }
extern "C" void* fn_00271330(RootObject* object) { return fn_0024E9A8(object->value->value->value); }
extern "C" void* fn_00271340(RootObject* object) { return fn_00243A00(object->value->value->value); }
extern "C" void* fn_00271350(RootObject* object) { return fn_0024AD3C(object->value->value->value); }
