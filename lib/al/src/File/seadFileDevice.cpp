// Source-local views of the independently observed retail file-device prefixes.
// No complete class extent, public spelling or table definition is claimed.
namespace sead {

class FileDevice;
class FileHandle;
class DirectoryHandle;
struct DirectoryEntry;

struct HandleBase {
    void* dispatch;
    unsigned char unknown04[12];
    FileDevice* mDevice;
};

class FileHandle : public HandleBase {
private:
    unsigned char unknown14[32];
    int mReadDivisionSize;
    friend class FileDevice;
};
class DirectoryHandle : public HandleBase {};

struct FileDeviceDispatchPrefix {
    void (*unknown00)();
    void (*unknown04)();
    void (*unknown08)();
    bool (*canHandle)(FileDevice*, const HandleBase*);
    void (*unknown10)();
    void (*unknown14)();
    void (*unknown18)();
    bool (*close)(FileDevice*, FileHandle*);
    bool (*read)(FileDevice*, unsigned int*, FileHandle*, unsigned char*, unsigned int);
    bool (*write)(FileDevice*, unsigned int*, FileHandle*, const unsigned char*, unsigned int);
    void (*unknown28)();
    void (*unknown2C)();
    void (*unknown30)();
    void (*unknown34)();
    void (*unknown38)();
    void (*unknown3C)();
    void (*unknown40)();
    bool (*closeDirectory)(FileDevice*, DirectoryHandle*);
    bool (*readDirectory)(FileDevice*, unsigned int*, DirectoryHandle*, DirectoryEntry*, unsigned int);
};

class FileDevice {
public:
    bool tryCloseDirectory(DirectoryHandle* handle);
    bool tryReadDirectory(unsigned int* readCount, DirectoryHandle* handle, DirectoryEntry* entries, unsigned int size);
    bool tryRead(unsigned int* readSize, FileHandle* handle, unsigned char* buffer, unsigned int size);
    bool tryClose(FileHandle* handle);
    bool tryWrite(unsigned int* writeSize, FileHandle* handle, const unsigned char* buffer, unsigned int size);
private:
    const FileDeviceDispatchPrefix* mDispatch;
    unsigned char unknown04[72];
    unsigned char mEnabled;
};

bool FileDevice::tryCloseDirectory(DirectoryHandle* handle) {
    if (!mEnabled)
        return false;
    if (!handle)
        return false;
    if (!mDispatch->canHandle(this, handle))
        return false;
    bool result = mDispatch->closeDirectory(this, handle);
    if (result)
        handle->mDevice = 0;
    return result;
}
bool FileDevice::tryReadDirectory(unsigned int* readCount, DirectoryHandle* handle, DirectoryEntry* entries, unsigned int size) {
    if (!mEnabled)
        return false;
    if (!handle)
        return false;
    if (!mDispatch->canHandle(this, handle))
        return false;
    unsigned int count = 0;
    bool result = mDispatch->readDirectory(this, &count, handle, entries, size);
    if (readCount)
        *readCount = count;
    if (count > size)
        return false;
    return result;
}
__attribute__((noinline)) bool FileDevice::tryRead(unsigned int* readSize, FileHandle* handle, unsigned char* buffer, unsigned int size) {
    if (!mEnabled)
        return false;
    if (!handle)
        return false;
    if (!mDispatch->canHandle(this, handle))
        return false;
    if (!buffer)
        return false;
    unsigned int total;
    if (handle->mReadDivisionSize) {
        total = 0;
        for (;;) {
            int current = handle->mReadDivisionSize;
            if (current > static_cast<int>(size))
                current = static_cast<int>(size);
            unsigned int actual = 0;
            if (!mDispatch->read(this, &actual, handle, buffer, static_cast<unsigned int>(current)))
                goto read_failed;
            total += actual;
            if (static_cast<unsigned int>(current) > actual)
                break;
            size -= static_cast<unsigned int>(current);
            buffer += actual;
            if (!size)
                break;
        }
        if (readSize)
            *readSize = total;
        return true;
    }
    return mDispatch->read(this, readSize, handle, buffer, size);
read_failed:
    if (readSize)
        *readSize = total;
    return false;
}
bool FileDevice::tryClose(FileHandle* handle) {
    if (!mEnabled)
        return false;
    if (!handle)
        return false;
    if (!mDispatch->canHandle(this, handle))
        return false;
    bool result = mDispatch->close(this, handle);
    if (result)
        handle->mDevice = 0;
    return result;
}
bool FileDevice::tryWrite(unsigned int* writeSize, FileHandle* handle, const unsigned char* buffer, unsigned int size) {
    if (!mEnabled)
        return false;
    if (!handle || !buffer)
        return false;
    if (!mDispatch->canHandle(this, handle))
        return false;
    return mDispatch->write(this, writeSize, handle, buffer, size);
}

} // namespace sead

// Private ABI views for the archive-device callback family. Only observed
// storage and call slots are modeled; no complete class or table is defined.
namespace sead { namespace fileDeviceImplementation {
struct ArchiveResourceView;
struct StringView;
struct FileInformation { unsigned int index; unsigned int size; };
struct FileState { unsigned char* data; FileInformation information; unsigned int cursor; };
struct StringDispatch {
    void (*unknown00)();
    void (*unknown04)();
    void (*assureTermination)(const StringView*);
};
struct StringView { const StringDispatch* dispatch; const char* string; };
struct ArchiveResourceDispatch {
    void (*unknown00)();
    void (*unknown04)();
    void (*unknown08)();
    void (*unknown0C)();
    void (*unknown10)();
    void (*unknown14)();
    unsigned char* (*openFile)(ArchiveResourceView*, const StringView*, FileInformation*);
    void (*unknown1C)();
    void (*unknown20)();
    void (*unknown24)();
    int (*openDirectory)(ArchiveResourceView*, void*, const StringView*);
    int (*closeDirectory)(ArchiveResourceView*, void*);
    unsigned int (*readDirectory)(ArchiveResourceView*, void*, DirectoryEntry*, unsigned int);
};
struct ArchiveResourceView { const ArchiveResourceDispatch* dispatch; };
struct ArchiveDeviceView { unsigned char unknown00[80]; ArchiveResourceView* archive; };
struct FileHandleView {
    unsigned char unknown00[20];
    FileState state;
    FileState* getState() { return &state; }
};
struct DirectoryHandleView {
    unsigned char unknown00[20];
    unsigned char state[1];
    void* getState() { return state; }
};
} }
using namespace sead::fileDeviceImplementation;

// A private ordinary accessor. It must disappear from every callback at link.
__attribute__((weak)) void* getHandleBuffer(ArchiveDeviceView*, void* handle) {
    return static_cast<unsigned char*>(handle) + 20;
}

extern "C" bool fn_002DF11C(ArchiveDeviceView* device, unsigned int* size, const StringView* path) {
    if (!device->archive)
        return false;
    path->dispatch->assureTermination(path);
    if (!path->string)
        return false;
    FileInformation information = {0, 0};
    unsigned char* data = device->archive->dispatch->openFile(device->archive, path, &information);
    if (!data)
        return false;
    *size = information.size;
    return true;
}
extern "C" bool fn_002DF198(ArchiveDeviceView* device, bool* exists, const StringView* path) {
    if (!device->archive)
        return false;
    path->dispatch->assureTermination(path);
    if (!path->string)
        return false;
    unsigned char* data = device->archive->dispatch->openFile(device->archive, path, 0);
    *exists = data != 0;
    return true;
}
extern "C" int fn_002DF298(ArchiveDeviceView* device, DirectoryHandleView* handle) {
    ArchiveResourceView* archive = device->archive;
    if (!archive)
        return 0;
    return archive->dispatch->closeDirectory(archive, getHandleBuffer(device, handle));
}
extern "C" bool fn_002DF440(ArchiveDeviceView*, FileHandleView*) { return true; }
extern "C" bool fn_002DF448(ArchiveDeviceView*, unsigned int*, FileHandleView*, const unsigned char*, unsigned int) { return false; }
