namespace sead {
struct DirectoryHandle;
struct FileHandle;
class FileDevice {
public:
    static int tryCloseDirectory(DirectoryHandle*);
    static int tryClose(FileHandle*);
};
}

namespace {
struct Wrapper {
    unsigned char pad[0x50];
    void* target;
};
}

extern "C" int fn_001E449C(void*);
extern "C" int fn_00222AF4(void*);
extern "C" int fn_0024FC30(void*);
extern "C" int fn_0028B58C(void*);
extern "C" int fn_002D97E4(void*);
extern "C" int fn_00327BAC(void*);

extern "C" int fn_001E4494(Wrapper* self) { return fn_001E449C(self->target); }
extern "C" int fn_002229EC(Wrapper* self) { return sead::FileDevice::tryCloseDirectory((sead::DirectoryHandle*)self->target); }
extern "C" int fn_00222AEC(Wrapper* self) { return fn_00222AF4(self->target); }
extern "C" int fn_00243800(Wrapper* self) { return sead::FileDevice::tryClose((sead::FileHandle*)self->target); }
extern "C" int fn_0024FC28(Wrapper* self) { return fn_0024FC30(self->target); }
extern "C" int fn_0028B584(Wrapper* self) { return fn_0028B58C(self->target); }
extern "C" int fn_002D97DC(Wrapper* self) { return fn_002D97E4(self->target); }
extern "C" int fn_00327BA4(Wrapper* self) { return fn_00327BAC(self->target); }
