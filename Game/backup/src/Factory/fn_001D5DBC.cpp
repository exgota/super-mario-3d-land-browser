namespace {

struct Result {
    int code;
    Result(int value = 0) : code(value) {}
    int failure() const { return code & 0x80000000; }
    int isSuccess() const {
        int value = static_cast<const volatile Result*>(this)->code;
        return 1 + (value >> 31);
    }
};

struct Owner {
    unsigned char padding[0x50];
    Result status;
};

struct ConstructionTable {
    const int* primary;
    unsigned char padding[0x1c];
    const int* virtualTable;
    const int* secondary;
};

extern "C" const ConstructionTable dat_003D7B04;
extern "C" const char dat_003A2CD4[];
extern "C" const unsigned int _ZTVN4sead14SafeStringBaseIcEE[];

struct SafeString {
    const void* table;
    const char* text;
    SafeString(const char* string)
        : table(_ZTVN4sead14SafeStringBaseIcEE + 2), text(string) {}
};

struct ReadValue {
    volatile float value;
    int status;
    ReadValue() {}
};

struct Stream;
extern "C" int fn_0024545C(Stream*, const SafeString*, int, int);
extern "C" void fn_0024542C(Stream*, int);
extern "C" int fn_00287C64(void*, ReadValue*);
extern "C" void fn_00291B40(int, unsigned int);

struct StreamState {
    void* words[5];

    void read(ReadValue* value) {
        int result = fn_00287C64(this, value);
        unsigned int failure = static_cast<unsigned int>(result) >> 31;
        if (failure)
            fn_00291B40(result, failure);
    }
};

struct Stream {
    const int* primary;
    const int* secondary;
    StreamState state;
    unsigned int padding;
    ReadValue value;

    Stream() {
        state.words[0] = 0;
        state.words[1] = 0;
        state.words[2] = 0;
        state.words[3] = 0;
        state.words[4] = 0;
        primary = dat_003D7B04.primary;
        *reinterpret_cast<const int**>(reinterpret_cast<char*>(this) + primary[-12]) = dat_003D7B04.virtualTable;
        secondary = dat_003D7B04.secondary;
    }

    ~Stream() { fn_0024542C(this, 0); }
    void readValue() { state.read(&value); }
    int open(int mode, const SafeString& name) {
        return fn_0024545C(this, &name, mode, 1);
    }
};

}

extern "C" int fn_001D5DBC(Owner* self, float* output, int mode) {
    Stream stream;
    self->status = stream.open(mode, dat_003A2CD4);
    if (self->status.failure() < 0)
        return false;
    stream.readValue();
    int readStatus = stream.value.status;
    float readFloat = stream.value.value;
    self->status = Result();
    if (readStatus < 0)
        return false;
    *output = readFloat;
    return self->status.isSuccess();
}
