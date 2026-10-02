namespace {
struct Inner { void* words[9]; };
struct Middle { void* words[13]; };
struct Outer { void* words[11]; };
}

extern "C" void* fn_0024394C(void*);
extern "C" void* fn_00244020(void*);
extern "C" void* fn_0024AC7C(void*);
extern "C" void* fn_0026244C(void*);
extern "C" void* fn_002624EC(void*);
extern "C" void* fn_00262548(void*);
extern "C" void* fn_00262590(void*);
extern "C" void* fn_003307E8(void*);
extern "C" void* fn_00330820(void*);
extern "C" void* fn_00330854(void*);

extern "C" void* fn_0024393C(Outer* p) { return fn_0024394C(static_cast<Inner*>(static_cast<Middle*>(p->words[10])->words[0])->words[8]); }
extern "C" void* fn_00244010(Outer* p) { return fn_00244020(static_cast<Inner*>(static_cast<Middle*>(p->words[10])->words[0])->words[8]); }
extern "C" void* fn_0024AC6C(Outer* p) { return fn_0024AC7C(static_cast<Inner*>(static_cast<Middle*>(p->words[10])->words[0])->words[8]); }
extern "C" void* fn_0026243C(Outer* p) { return fn_0026244C(static_cast<Inner*>(static_cast<Middle*>(p->words[10])->words[0])->words[8]); }
extern "C" void* fn_002624DC(Outer* p) { return fn_002624EC(static_cast<Inner*>(static_cast<Middle*>(p->words[10])->words[0])->words[8]); }
extern "C" void* fn_00262538(Outer* p) { return fn_00262548(static_cast<Inner*>(static_cast<Middle*>(p->words[10])->words[0])->words[8]); }
extern "C" void* fn_00262580(Outer* p) { return fn_00262590(static_cast<Inner*>(static_cast<Middle*>(p->words[10])->words[0])->words[8]); }
extern "C" void* fn_003307D8(Outer* p) { return fn_003307E8(static_cast<Inner*>(static_cast<Middle*>(p->words[10])->words[0])->words[8]); }
extern "C" void* fn_00330810(Outer* p) { return fn_00330820(static_cast<Inner*>(static_cast<Middle*>(p->words[10])->words[0])->words[8]); }
extern "C" void* fn_00330844(Outer* p) { return fn_00330854(static_cast<Inner*>(static_cast<Middle*>(p->words[10])->words[0])->words[8]); }
