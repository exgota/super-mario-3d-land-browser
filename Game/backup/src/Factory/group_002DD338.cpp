extern "C" void fn_002DD338() {}
extern "C" void fn_002DD4BC() {}
extern "C" void fn_002DD4C0() {}
extern "C" void fn_002DD4C4() {}
extern "C" void fn_002DDAFC() {}
extern "C" void fn_002DDC2C() {}

namespace {
struct EmptyBase {};
}

namespace sead {
template <int N> class FixedSafeString : virtual EmptyBase {
public:
    ~FixedSafeString();
};

template <> FixedSafeString<1024>::~FixedSafeString() {}
template <> FixedSafeString<256>::~FixedSafeString() {}
}

extern "C" void fn_002DE53C() {}
extern "C" void fn_002DE7E8() {}
extern "C" void fn_002DFCCC() {}
extern "C" void fn_002E1724() {}
extern "C" void fn_002E2484() {}
extern "C" void fn_002E39AC() {}
extern "C" void fn_002EC5D8() {}
extern "C" void fn_002F0438() {}
extern "C" void fn_002F1858() {}
extern "C" void fn_002F2998() {}
extern "C" void fn_002F2A64() {}
extern "C" void fn_002F2A68() {}
extern "C" void fn_002F2A6C() {}
extern "C" void fn_002F2A70() {}
extern "C" void fn_002F2C0C() {}
extern "C" void fn_002F2C10() {}
extern "C" void fn_002F2F98() {}
