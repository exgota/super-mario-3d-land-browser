namespace {
struct Level2 { unsigned pad[12]; unsigned value; };
struct Level1 { Level2* next; };
struct Object { unsigned pad[10]; Level1* next; };
}

extern "C" unsigned fn_0024EB8C(unsigned);
extern "C" unsigned fn_0024947C(unsigned);
extern "C" unsigned fn_002624AC(unsigned);

extern "C" unsigned fn_001C2EE8(Object* p) {
    return fn_0024EB8C(p->next->next->value);
}
extern "C" unsigned fn_001D2C78(Object* p) {
    return fn_0024947C(p->next->next->value);
}
extern "C" unsigned fn_001DB350(Object* p) {
    return fn_002624AC(p->next->next->value);
}
