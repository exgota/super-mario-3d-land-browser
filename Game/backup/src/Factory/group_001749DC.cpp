namespace {
struct VTable { unsigned int pad[4]; };
struct Object { unsigned int pad[4]; void* field10; };
struct Root { unsigned int pad; Object* field4; };
}
extern "C" void fn_001749DC(Root* p) {
    Object* a = p->field4;
    Object* b = *reinterpret_cast<Object**>(reinterpret_cast<char*>(a) + 0x10);
    VTable* v = *reinterpret_cast<VTable**>(b);
    reinterpret_cast<void (*)(void*)>(*reinterpret_cast<void**>(reinterpret_cast<char*>(v) + 0x10))(b);
}
extern "C" void fn_00181940(Root* p) {
    Object* a = p->field4;
    Object* b = *reinterpret_cast<Object**>(reinterpret_cast<char*>(a) + 0x10);
    VTable* v = *reinterpret_cast<VTable**>(b);
    reinterpret_cast<void (*)(void*)>(*reinterpret_cast<void**>(reinterpret_cast<char*>(v) + 0x10))(b);
}
extern "C" void fn_001A4760(Root* p) {
    Object* a = p->field4;
    Object* b = *reinterpret_cast<Object**>(reinterpret_cast<char*>(a) + 0x10);
    VTable* v = *reinterpret_cast<VTable**>(b);
    reinterpret_cast<void (*)(void*)>(*reinterpret_cast<void**>(reinterpret_cast<char*>(v) + 0x10))(b);
}
extern "C" void fn_001A7334(Root* p) {
    Object* a = p->field4;
    Object* b = *reinterpret_cast<Object**>(reinterpret_cast<char*>(a) + 0x10);
    VTable* v = *reinterpret_cast<VTable**>(b);
    reinterpret_cast<void (*)(void*)>(*reinterpret_cast<void**>(reinterpret_cast<char*>(v) + 0x10))(b);
}
