namespace {
struct NothrowTag {};
struct ComponentA;
struct ComponentB;
struct ComponentC;

struct FloatPair {
    float first;
    float second;
    FloatPair(float a, float b) : first(a), second(b) {}
};

struct ComponentBStackArgs {
    FloatPair values;
    void* extra;
};

struct Header {
    const void* vtable;
    void* owner;
    Header(const void* table, void* actor) : vtable(table), owner(actor) {}
};

struct Keeper {
    Header header;
    ComponentA* first;
    ComponentB* second;
    ComponentC* third;
    bool flag0;
    bool flag1;
    bool flag2;
    bool flag3;
    float value;
};

extern "C" const unsigned char dat_003CB0CC[];
extern "C" void* _ZnwjRKSt9nothrow_t(unsigned int, const NothrowTag&);
extern "C" __softfp ComponentA* fn_00162DE4(
    void*, void*, void*, const void*, float, float);
extern "C" __softfp ComponentB* fn_001989E0(
    void*, void*, void*, const void*, ComponentBStackArgs);
extern "C" __softfp ComponentC* fn_00175CB4(
    void*, void*, const void*, void*, float, float, float);
}

inline void* operator new(unsigned int, void* storage) { return storage; }

extern "C" __softfp Keeper* fn_00162938(
    Keeper* self, void* context, void* owner, void* extra,
    float firstValue, const void* firstToken,
    const void* secondToken, const void* thirdToken,
    float secondValue, float value, float thirdValue) {
    self->first = 0;
    self->second = 0;
    self->third = 0;
    new (&self->header) Header(dat_003CB0CC, owner);
    self->flag0 = false;
    self->flag1 = false;
    self->flag2 = false;
    self->flag3 = false;
    self->value = value;

    void* allocation = _ZnwjRKSt9nothrow_t(
        0x30, *static_cast<const NothrowTag*>(firstToken));
    if (allocation)
        allocation = fn_00162DE4(
            allocation, owner, context, firstToken, firstValue, secondValue);
    self->first = static_cast<ComponentA*>(allocation);

    allocation = _ZnwjRKSt9nothrow_t(
        0x38, *static_cast<const NothrowTag*>(firstToken));
    if (allocation) {
        const ComponentBStackArgs args = {
            FloatPair(firstValue, secondValue), extra
        };
        allocation = fn_001989E0(
            allocation, owner, context, secondToken, args);
    }
    self->second = static_cast<ComponentB*>(allocation);

    allocation = _ZnwjRKSt9nothrow_t(
        0x28, *static_cast<const NothrowTag*>(firstToken));
    if (allocation)
        allocation = fn_00175CB4(
            allocation, owner, thirdToken, context,
            firstValue, secondValue, thirdValue);
    self->third = static_cast<ComponentC*>(allocation);
    return self;
}
