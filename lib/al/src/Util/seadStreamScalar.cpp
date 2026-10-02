// Clean reconstruction of the retail scalar stream dispatch family.
// This translation unit describes observed prefixes only. It owns no tables.
namespace sead {

struct StreamFormat;
struct StreamSrc;

struct StreamFormatDispatchPrefix {
    unsigned char (*readU8)(StreamFormat*, StreamSrc*, unsigned int);
    unsigned short (*readU16)(StreamFormat*, StreamSrc*, unsigned int);
    void (*unknown08)();
    unsigned long long (*readU64)(StreamFormat*, StreamSrc*, unsigned int);
    signed char (*readS8)(StreamFormat*, StreamSrc*, unsigned int);
    short (*readS16)(StreamFormat*, StreamSrc*, unsigned int);
    int (*readS32)(StreamFormat*, StreamSrc*, unsigned int);
    void (*unknown1C)();
    void (*unknown20)();
    void (*unknown24)();
    void (*unknown28)();
    void (*readMemBlock)(StreamFormat*, StreamSrc*, void*, unsigned int);
    void (*writeU8)(StreamFormat*, StreamSrc*, unsigned int, unsigned char);
    void (*writeU16)(StreamFormat*, StreamSrc*, unsigned int, unsigned short);
    void (*unknown38)();
    void (*writeU64)(StreamFormat*, StreamSrc*, unsigned int, unsigned long long);
    void (*writeS8)(StreamFormat*, StreamSrc*, unsigned int, signed char);
    void (*writeS16)(StreamFormat*, StreamSrc*, unsigned int, short);
    void (*writeS32)(StreamFormat*, StreamSrc*, unsigned int, int);
    void (*unknown4C)();
    void (*unknown50)();
    void (*unknown54)();
    void (*unknown58)();
    void (*writeMemBlock)(StreamFormat*, StreamSrc*, const void*, unsigned int);
    void (*unknown60)();
    void (*unknown64)();
    void (*unknown68)();
    void (*flush)(StreamFormat*, StreamSrc*);
};

struct StreamFormat {
    const StreamFormatDispatchPrefix* dispatch;
};

struct StreamSourceDispatchPrefix {
    void (*unknown00)();
    void (*unknown04)();
    void (*unknown08)();
    void (*unknown0C)();
    void (*unknown10)();
    void (*flush)(StreamSrc*);
};

struct StreamSrc {
    const StreamSourceDispatchPrefix* dispatch;
};

class Stream {
protected:
    void* mDispatch;
    StreamFormat* mFormat;
    StreamSrc* mSource;
    unsigned char mEndian;
};

class ReadStream : public Stream {
public:
    void readU8(unsigned char& value);
    void readU16(unsigned short& value);
    void readU64(unsigned long long& value);
    void readS8(signed char& value);
    void readS16(short& value);
    void readS32(int& value);
    void readMemBlock(void* buffer, unsigned int size);
};

class WriteStream : public Stream {
public:
    void writeU8(unsigned char value);
    void writeU16(unsigned short value);
    void writeU64(unsigned long long value);
    void writeS8(signed char value);
    void writeS16(short value);
    void writeS32(int value);
    void writeMemBlock(const void* buffer, unsigned int size);
    void flush();
};

void ReadStream::readU8(unsigned char& value) {
    value = mFormat->dispatch->readU8(mFormat, mSource, mEndian);
}
void ReadStream::readU16(unsigned short& value) {
    value = mFormat->dispatch->readU16(mFormat, mSource, mEndian);
}
void ReadStream::readU64(unsigned long long& value) {
    value = mFormat->dispatch->readU64(mFormat, mSource, mEndian);
}
void ReadStream::readS8(signed char& value) {
    value = mFormat->dispatch->readS8(mFormat, mSource, mEndian);
}
void ReadStream::readS16(short& value) {
    value = mFormat->dispatch->readS16(mFormat, mSource, mEndian);
}
void ReadStream::readS32(int& value) {
    value = mFormat->dispatch->readS32(mFormat, mSource, mEndian);
}
void ReadStream::readMemBlock(void* buffer, unsigned int size) {
    mFormat->dispatch->readMemBlock(mFormat, mSource, buffer, size);
}
void WriteStream::writeU8(unsigned char value) {
    mFormat->dispatch->writeU8(mFormat, mSource, mEndian, value);
}
void WriteStream::writeU16(unsigned short value) {
    mFormat->dispatch->writeU16(mFormat, mSource, mEndian, value);
}
void WriteStream::writeU64(unsigned long long value) {
    mFormat->dispatch->writeU64(mFormat, mSource, mEndian, value);
}
void WriteStream::writeS8(signed char value) {
    mFormat->dispatch->writeS8(mFormat, mSource, mEndian, value);
}
void WriteStream::writeS16(short value) {
    mFormat->dispatch->writeS16(mFormat, mSource, mEndian, value);
}
void WriteStream::writeS32(int value) {
    mFormat->dispatch->writeS32(mFormat, mSource, mEndian, value);
}
void WriteStream::writeMemBlock(const void* buffer, unsigned int size) {
    mFormat->dispatch->writeMemBlock(mFormat, mSource, buffer, size);
}
void WriteStream::flush() {
    mFormat->dispatch->flush(mFormat, mSource);
    mSource->dispatch->flush(mSource);
}

// Observed RAM source prefix. The complete class extent is not established.
class RamStreamSrc : public StreamSrc {
public:
    unsigned int read(void* buffer, unsigned int size);
    unsigned int write(const void* buffer, unsigned int size);
    int skip(int size);
    void rewind();
private:
    unsigned char* mBuffer;
    unsigned int mCapacity;
    unsigned int mPosition;
};

extern "C" void* nnnstdMemCpy(void* destination, const void* source, unsigned int size);

unsigned int RamStreamSrc::read(void* buffer, unsigned int size) {
    if (mPosition + size > mCapacity)
        size = mCapacity - mPosition;
    nnnstdMemCpy(buffer, mBuffer + mPosition, size);
    mPosition += size;
    return size;
}
unsigned int RamStreamSrc::write(const void* buffer, unsigned int size) {
    if (mPosition + size > mCapacity)
        size = mCapacity - mPosition;
    nnnstdMemCpy(mBuffer + mPosition, buffer, size);
    mPosition += size;
    return size;
}
int RamStreamSrc::skip(int size) {
    if (size > 0 && mPosition + static_cast<unsigned int>(size) > mCapacity)
        size = static_cast<int>(mCapacity - mPosition);
    if (size < 0 && mPosition < 0u - static_cast<unsigned int>(size))
        size = static_cast<int>(0u - mPosition);
    mPosition += static_cast<unsigned int>(size);
    return size;
}
void RamStreamSrc::rewind() {
    mPosition = 0;
}

} // namespace sead
