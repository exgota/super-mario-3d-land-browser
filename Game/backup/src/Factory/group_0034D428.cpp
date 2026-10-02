namespace {
typedef void (*Method)(void*);
struct Wrapper { void* object; };
}
#define BODY(name) extern "C" void name(void*, Wrapper* w) { ((Method)(*(void**)((char*)*(void**)w->object + 0x14)))(w->object); }
BODY(fn_0034D428)
BODY(fn_00350A50)
BODY(fn_0035AE24)
BODY(fn_0035AE44)
BODY(fn_0035C3DC)
BODY(fn_0036290C)
BODY(fn_003697AC)

