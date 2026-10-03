namespace {
struct Inner24B9 { unsigned char pad[8]; void* value; };
struct Outer24B9 { Inner24B9* next; };
struct Inner2724 { unsigned char pad[8]; void* value; };
struct Outer2724 { Inner2724* next; };
}
extern "C" void* fn_0024B9B4(void*);
extern "C" void* fn_00272420(void*);
extern "C" void* fn_0024B9A8(Outer24B9* p) {
    return fn_0024B9B4(p->next->value);
}
extern "C" void* fn_00272414(Outer2724* p) {
    return fn_00272420(p->next->value);
}
