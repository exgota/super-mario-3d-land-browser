namespace {
struct MemberFunction {
    unsigned int first;
    unsigned int second;
};

struct Functor {
    const void* table;
    void* parent;
    MemberFunction function;
};

extern "C" void* _ZnwjRKSt9nothrow_t(unsigned int size);
extern "C" const unsigned char dat_003D5A4C;
extern "C" const unsigned char dat_003D5A5C;
extern "C" const unsigned char dat_003D5A6C;
extern "C" const unsigned char dat_003D5A7C;
extern "C" const unsigned char dat_003D5A8C;
extern "C" const unsigned char dat_003D5AAC;
extern "C" const unsigned char dat_003D5ABC;
extern "C" const unsigned char dat_003D5ACC;
extern "C" const unsigned char dat_003D5ADC;
extern "C" const unsigned char dat_003D5AEC;
extern "C" const unsigned char dat_003D5AFC;
extern "C" const unsigned char dat_003D5B0C;
extern "C" const unsigned char dat_003D5B1C;
extern "C" const unsigned char dat_003D5B2C;
extern "C" const unsigned char dat_003D5B3C;
extern "C" const unsigned char dat_003D5B4C;
extern "C" const unsigned char dat_003D5B5C;
extern "C" const unsigned char dat_003D5B6C;
extern "C" const unsigned char dat_003D5B7C;
extern "C" const unsigned char dat_003D5B8C;
extern "C" const unsigned char dat_003D5B9C;
extern "C" const unsigned char dat_003D5BAC;
extern "C" const unsigned char dat_003D5BBC;
extern "C" const unsigned char dat_003D5BCC;
}

#define CLONE(address, table_address) \
    extern "C" Functor* fn_##address(const Functor* source) { \
        Functor* result = static_cast<Functor*>(_ZnwjRKSt9nothrow_t(sizeof(Functor))); \
        if (result) { \
            result->table = &table_address; \
            result->parent = source->parent; \
            result->function = source->function; \
        } \
        return result; \
    }

CLONE(0039B364, dat_003D5A4C)
CLONE(0039B3CC, dat_003D5A5C)
CLONE(0039B434, dat_003D5A6C)
CLONE(0039B49C, dat_003D5A7C)
CLONE(0039B504, dat_003D5A8C)
CLONE(0039B5D4, dat_003D5AAC)
CLONE(0039B63C, dat_003D5ABC)
CLONE(0039B6A4, dat_003D5ACC)
CLONE(0039B70C, dat_003D5ADC)
CLONE(0039B774, dat_003D5AEC)
CLONE(0039B7DC, dat_003D5AFC)
CLONE(0039B844, dat_003D5B0C)
CLONE(0039B8AC, dat_003D5B1C)
CLONE(0039B914, dat_003D5B2C)
CLONE(0039B97C, dat_003D5B3C)
CLONE(0039B9E4, dat_003D5B4C)
CLONE(0039BA4C, dat_003D5B5C)
CLONE(0039BAB4, dat_003D5B6C)
CLONE(0039BB1C, dat_003D5B7C)
CLONE(0039BB84, dat_003D5B8C)
CLONE(0039BBEC, dat_003D5B9C)
CLONE(0039BC54, dat_003D5BAC)
CLONE(0039BCBC, dat_003D5BBC)
CLONE(0039BD24, dat_003D5BCC)

#undef CLONE
