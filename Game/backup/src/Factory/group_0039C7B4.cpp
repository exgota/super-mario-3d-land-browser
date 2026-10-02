namespace {
struct FunctionPointer {
    unsigned int address;
    unsigned int adjustment;
};
struct Functor {
    const unsigned int* vtable;
    void* parent;
    FunctionPointer function;
};
extern "C" void* _ZnwjRKSt9nothrow_t(unsigned int size);
extern "C" unsigned int dat_003D5D7C;
extern "C" unsigned int dat_003D5D8C;
extern "C" unsigned int dat_003D5D9C;
extern "C" unsigned int dat_003D5DAC;
extern "C" unsigned int dat_003D5DBC;
extern "C" unsigned int dat_003D5DCC;
extern "C" unsigned int dat_003D5DDC;
extern "C" unsigned int dat_003D5DEC;
extern "C" unsigned int dat_003D5DFC;
extern "C" unsigned int dat_003D5E0C;
extern "C" unsigned int dat_003D5E1C;
extern "C" unsigned int dat_003D5E2C;
extern "C" unsigned int dat_003D5E4C;
extern "C" unsigned int dat_003D5E5C;
extern "C" unsigned int dat_003D5E6C;
extern "C" unsigned int dat_003D5E7C;
extern "C" unsigned int dat_003D5E8C;
extern "C" unsigned int dat_003D5E9C;
}

extern "C" Functor* fn_0039C81C(const Functor* source)
{
    Functor* result = static_cast<Functor*>(_ZnwjRKSt9nothrow_t(sizeof(Functor)));
    if (result) {
        result->vtable = &dat_003D5D7C;
        result->parent = source->parent;
        result->function = source->function;
    }
    return result;
}

extern "C" Functor* fn_0039C884(const Functor* source)
{
    Functor* result = static_cast<Functor*>(_ZnwjRKSt9nothrow_t(sizeof(Functor)));
    if (result) {
        result->vtable = &dat_003D5D8C;
        result->parent = source->parent;
        result->function = source->function;
    }
    return result;
}

extern "C" Functor* fn_0039C8EC(const Functor* source)
{
    Functor* result = static_cast<Functor*>(_ZnwjRKSt9nothrow_t(sizeof(Functor)));
    if (result) {
        result->vtable = &dat_003D5D9C;
        result->parent = source->parent;
        result->function = source->function;
    }
    return result;
}

extern "C" Functor* fn_0039C954(const Functor* source)
{
    Functor* result = static_cast<Functor*>(_ZnwjRKSt9nothrow_t(sizeof(Functor)));
    if (result) {
        result->vtable = &dat_003D5DAC;
        result->parent = source->parent;
        result->function = source->function;
    }
    return result;
}

extern "C" Functor* fn_0039C9BC(const Functor* source)
{
    Functor* result = static_cast<Functor*>(_ZnwjRKSt9nothrow_t(sizeof(Functor)));
    if (result) {
        result->vtable = &dat_003D5DBC;
        result->parent = source->parent;
        result->function = source->function;
    }
    return result;
}

extern "C" Functor* fn_0039CA24(const Functor* source)
{
    Functor* result = static_cast<Functor*>(_ZnwjRKSt9nothrow_t(sizeof(Functor)));
    if (result) {
        result->vtable = &dat_003D5DCC;
        result->parent = source->parent;
        result->function = source->function;
    }
    return result;
}

extern "C" Functor* fn_0039CA8C(const Functor* source)
{
    Functor* result = static_cast<Functor*>(_ZnwjRKSt9nothrow_t(sizeof(Functor)));
    if (result) {
        result->vtable = &dat_003D5DDC;
        result->parent = source->parent;
        result->function = source->function;
    }
    return result;
}

extern "C" Functor* fn_0039CAF4(const Functor* source)
{
    Functor* result = static_cast<Functor*>(_ZnwjRKSt9nothrow_t(sizeof(Functor)));
    if (result) {
        result->vtable = &dat_003D5DEC;
        result->parent = source->parent;
        result->function = source->function;
    }
    return result;
}

extern "C" Functor* fn_0039CB5C(const Functor* source)
{
    Functor* result = static_cast<Functor*>(_ZnwjRKSt9nothrow_t(sizeof(Functor)));
    if (result) {
        result->vtable = &dat_003D5DFC;
        result->parent = source->parent;
        result->function = source->function;
    }
    return result;
}

extern "C" Functor* fn_0039CBC4(const Functor* source)
{
    Functor* result = static_cast<Functor*>(_ZnwjRKSt9nothrow_t(sizeof(Functor)));
    if (result) {
        result->vtable = &dat_003D5E0C;
        result->parent = source->parent;
        result->function = source->function;
    }
    return result;
}

extern "C" Functor* fn_0039CC2C(const Functor* source)
{
    Functor* result = static_cast<Functor*>(_ZnwjRKSt9nothrow_t(sizeof(Functor)));
    if (result) {
        result->vtable = &dat_003D5E1C;
        result->parent = source->parent;
        result->function = source->function;
    }
    return result;
}

extern "C" Functor* fn_0039CC94(const Functor* source)
{
    Functor* result = static_cast<Functor*>(_ZnwjRKSt9nothrow_t(sizeof(Functor)));
    if (result) {
        result->vtable = &dat_003D5E2C;
        result->parent = source->parent;
        result->function = source->function;
    }
    return result;
}

extern "C" Functor* fn_0039CD64(const Functor* source)
{
    Functor* result = static_cast<Functor*>(_ZnwjRKSt9nothrow_t(sizeof(Functor)));
    if (result) {
        result->vtable = &dat_003D5E4C;
        result->parent = source->parent;
        result->function = source->function;
    }
    return result;
}

extern "C" Functor* fn_0039CDCC(const Functor* source)
{
    Functor* result = static_cast<Functor*>(_ZnwjRKSt9nothrow_t(sizeof(Functor)));
    if (result) {
        result->vtable = &dat_003D5E5C;
        result->parent = source->parent;
        result->function = source->function;
    }
    return result;
}

extern "C" Functor* fn_0039CE34(const Functor* source)
{
    Functor* result = static_cast<Functor*>(_ZnwjRKSt9nothrow_t(sizeof(Functor)));
    if (result) {
        result->vtable = &dat_003D5E6C;
        result->parent = source->parent;
        result->function = source->function;
    }
    return result;
}

extern "C" Functor* fn_0039CE9C(const Functor* source)
{
    Functor* result = static_cast<Functor*>(_ZnwjRKSt9nothrow_t(sizeof(Functor)));
    if (result) {
        result->vtable = &dat_003D5E7C;
        result->parent = source->parent;
        result->function = source->function;
    }
    return result;
}

extern "C" Functor* fn_0039CF04(const Functor* source)
{
    Functor* result = static_cast<Functor*>(_ZnwjRKSt9nothrow_t(sizeof(Functor)));
    if (result) {
        result->vtable = &dat_003D5E8C;
        result->parent = source->parent;
        result->function = source->function;
    }
    return result;
}

extern "C" Functor* fn_0039CF6C(const Functor* source)
{
    Functor* result = static_cast<Functor*>(_ZnwjRKSt9nothrow_t(sizeof(Functor)));
    if (result) {
        result->vtable = &dat_003D5E9C;
        result->parent = source->parent;
        result->function = source->function;
    }
    return result;
}

