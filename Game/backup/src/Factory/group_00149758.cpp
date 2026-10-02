namespace {
typedef unsigned char Byte;
}

extern "C" void* fn_00149760(void*);
extern "C" void* fn_00151F2C(void*);
extern "C" void* fn_00160770(void*);
extern "C" void* fn_00166138(void*);
extern "C" void* fn_0016C6E4(void*);
extern "C" void* fn_0017E034(void*);

extern "C" void* fn_00149758(void* p) { return fn_00149760(static_cast<Byte*>(p) - 0x60); }
extern "C" void* fn_00151F24(void* p) { return fn_00151F2C(static_cast<Byte*>(p) - 0x60); }
extern "C" void* fn_00160768(void* p) { return fn_00160770(static_cast<Byte*>(p) - 0x60); }
extern "C" void* fn_00166130(void* p) { return fn_00166138(static_cast<Byte*>(p) - 0x60); }
extern "C" void* fn_0016C6DC(void* p) { return fn_0016C6E4(static_cast<Byte*>(p) - 0x60); }
extern "C" void* fn_0017E02C(void* p) { return fn_0017E034(static_cast<Byte*>(p) - 0x60); }
