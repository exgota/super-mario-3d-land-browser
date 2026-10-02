namespace {
struct C {
    char pad[0x10];
    signed char value;
};

struct B {
    C* next;
};

struct A {
    char pad[0x40];
    B* next;
};
}

extern "C" int fn_00267244(A* value) {
    return value->next->next->value;
}
