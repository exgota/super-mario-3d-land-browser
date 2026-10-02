namespace {
extern "C" const void* _ZTVN4sead14SafeStringBaseIcEE[];
struct SafeString {
    const void* vtable;
    const char* text;
    SafeString(const char* value) : text(value) {
        vtable = _ZTVN4sead14SafeStringBaseIcEE + 2;
    }
};
struct Context {
    char pad[12];
    void* resource;
    void* effect;
};
extern "C" {
int fn_00255C60();
int fn_002D0D70();
int fn_002D0E8C();
int fn_00255C48();
int fn_002748F4();
void fn_00255C78(Context*, int, int, int);
void fn_0027109C(void*, const SafeString&);
void fn_00270460(void*, int, int, int);
extern const int dat_003EFAB4[];
extern const char dat_003ADE98[];
}
inline int rank(int value) {
    return value == 7 ? 100 : dat_003EFAB4[value];
}
}

extern "C" int fn_0018AA2C(Context* self, int argument) {
    int state;
    if (fn_00255C60())
        state = 2;
    else if (fn_002D0D70())
        state = 3;
    else if (fn_002D0E8C())
        state = 4;
    else if (fn_00255C48())
        state = 5;
    else
        return 0;

    int current = rank(fn_002748F4());
    int nextRank = rank(state);
    if (current <= nextRank) {
        fn_00255C78(self, state, argument, 0);
        return 1;
    }

    fn_0027109C(self->resource ? static_cast<char*>(self->resource) + 4 : 0,
               SafeString(dat_003ADE98));
    fn_00270460(self->effect, argument, 10, 0);
    return 0;
}
