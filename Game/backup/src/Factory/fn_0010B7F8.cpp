extern "C" void nnnstdMemCpy(void*, const void*, unsigned int);

namespace sead {
template <typename T>
class SafeStringBase {
public:
    virtual ~SafeStringBase();
    virtual void assureTermination() const;
    const T* mString;

    const T* cstr() const {
        assureTermination();
        return mString;
    }

    int calcLength() const {
        assureTermination();
        int length = 0;
        while (length < 0x10000 && mString[length] != 0)
            ++length;
        if (length >= 0x10000)
            return 0;
        return length;
    }
};
}

namespace {
class FileNameBuffer : public sead::SafeStringBase<char> {
public:
    int mBufferSize;

    void copyAt(int position, const sead::SafeStringBase<char>& source) {
        char* buffer = const_cast<char*>(mString);
        int length = calcLength();
        int copyLength = source.calcLength();
        if (position + copyLength >= mBufferSize)
            copyLength = mBufferSize - position - 1;
        if (copyLength <= 0)
            return;
        char* destination = buffer + position;
        const char* sourceString = source.cstr();
        nnnstdMemCpy(destination, sourceString, copyLength);
        if (position + copyLength > length)
            buffer[position + copyLength] = 0;
    }

    void copy(const sead::SafeStringBase<char>& source) {
        if (this == &source)
            return;
        const_cast<char*>(mString)[0] = 0;
        copyAt(0, source);
    }
};
}

namespace al {
class FileEntryBase {
public:
    void* mUnknown;
    FileNameBuffer mFileName;
    void setFileName(const sead::SafeStringBase<char>& name);
};

void FileEntryBase::setFileName(const sead::SafeStringBase<char>& name) {
    mFileName.copy(name);
}
}
