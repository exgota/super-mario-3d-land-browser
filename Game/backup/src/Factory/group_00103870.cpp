namespace nn { namespace os { class Thread { public: class TypeInfo { public:
    template<class T> static void Invoke(void (*)(unsigned int), void const*);
    template<class T> static void Destroy(void*);
}; }; } }

template<> void nn::os::Thread::TypeInfo::Invoke<void (*)()>(void (*)(unsigned int), void const*);
template<> void nn::os::Thread::TypeInfo::Invoke<int>(void (*)(unsigned int), void const*);
template<> void nn::os::Thread::TypeInfo::Invoke<unsigned int>(void (*)(unsigned int), void const*);

template<> void nn::os::Thread::TypeInfo::Invoke<void (*)()>(void (*f)(unsigned int), void const* p) {
    f(*static_cast<unsigned int const*>(p));
}

template<> void nn::os::Thread::TypeInfo::Invoke<int>(void (*f)(unsigned int), void const* p) {
    f(*static_cast<unsigned int const*>(p));
}

template<> void nn::os::Thread::TypeInfo::Invoke<unsigned int>(void (*f)(unsigned int), void const* p) {
    f(*static_cast<unsigned int const*>(p));
}
