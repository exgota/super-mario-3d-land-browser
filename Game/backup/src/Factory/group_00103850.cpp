namespace nn {
namespace os {
class Thread {
public:
    class TypeInfo {
    public:
        template <typename T, typename U>
        static void Copy(const void* source, void* destination) {
            if (destination != 0)
                *reinterpret_cast<U*>(destination) = *reinterpret_cast<const T*>(source);
        }
    };
};
}
}

extern "C" void fn_003923FC(const void* source, void* destination) {
    if (destination != 0)
        *reinterpret_cast<unsigned int*>(destination) = *reinterpret_cast<const unsigned int*>(source);
}

template <> void nn::os::Thread::TypeInfo::Copy<void (*)(), void (*)()>(const void* source, void* destination) {
    if (destination != 0)
        *reinterpret_cast<void (**)()>(destination) = *reinterpret_cast<void (* const*)()>(source);
}
template <> void nn::os::Thread::TypeInfo::Copy<int, int>(const void* source, void* destination) {
    if (destination != 0)
        *reinterpret_cast<int*>(destination) = *reinterpret_cast<const int*>(source);
}
template <> void nn::os::Thread::TypeInfo::Copy<unsigned int, unsigned int>(const void* source, void* destination) {
    if (destination != 0)
        *reinterpret_cast<unsigned int*>(destination) = *reinterpret_cast<const unsigned int*>(source);
}
