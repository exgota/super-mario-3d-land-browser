namespace {

typedef unsigned int u32;
typedef unsigned long long u64;

struct Stream;
struct SafeString;

extern "C" {
extern void** dat_003D7B04[10];
extern void* _ZTVN4sead14SafeStringBaseIcEE[];
int fn_002203C4(Stream*, const SafeString&, const SafeString&, u32);
int fn_00287C64(void*, u64*);
void fn_0024542C(Stream*, int);
}

struct Stream {
    void** vtable;
    void** secondary;
    u32 fields[5];

    Stream() {
        fields[0] = 0;
        fields[1] = 0;
        fields[2] = 0;
        fields[3] = 0;
        fields[4] = 0;
        vtable = dat_003D7B04[0];
        *reinterpret_cast<void***>(reinterpret_cast<char*>(this) +
            reinterpret_cast<int*>(vtable)[-12]) = dat_003D7B04[8];
        secondary = dat_003D7B04[9];
    }

    ~Stream() {
        fn_0024542C(this, 0);
    }
};

struct SafeString {
    const char* text;

    explicit SafeString(const char* value)
        : text(value) {
        *reinterpret_cast<void***>(this) = _ZTVN4sead14SafeStringBaseIcEE + 2;
    }

    virtual ~SafeString() {}
};

struct Device {
    void** vtable;
    char padding[0x4c];
    int result;
};

typedef const char* (*GetName)(Device*);

}

extern "C" bool fn_002E27E8(Device* device, u32* output, const SafeString& path) {
    Stream stream;
    device->result = fn_002203C4(&stream,
        SafeString(reinterpret_cast<GetName>(device->vtable[20])(device)), path, 1);
    if ((device->result & static_cast<int>(0x80000000u)) < 0)
        return false;

    u64 size = 0;
    device->result = fn_00287C64(stream.fields, &size);
    if (static_cast<long long>(size) < 0)
        return false;

    *output = static_cast<u32>(size);
    return device->result >= 0;
}
