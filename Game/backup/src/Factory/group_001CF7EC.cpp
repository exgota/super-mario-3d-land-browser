extern "C" void fn_001CF7EC() {}
extern "C" void fn_001D20FC() {}
extern "C" void fn_001D2180() {}
extern "C" void fn_001D4B58() {}
extern "C" void fn_001D7888() {}
extern "C" void fn_001DA28C() {}
extern "C" void fn_001DA2D8() {}
extern "C" void fn_001DA2DC() {}
extern "C" void fn_001DA2E0() {}
extern "C" void fn_001DCAF0() {}
extern "C" void fn_001E224C() {}
extern "C" void fn_001E2518() {}
extern "C" void fn_001E30E8() {}
extern "C" void fn_001E3E70() {}
extern "C" void fn_001E7E74() {}
extern "C" void fn_001E7E78() {}
extern "C" void fn_001E8B6C() {}
extern "C" void fn_001E94E4() {}
extern "C" void fn_001EBC54() {}
extern "C" void fn_001EC3E8() {}
extern "C" void fn_001ECE0C() {}
extern "C" void fn_001ED2C0() {}

namespace {
class CriticalSectionBase {};
}

namespace nn {
namespace os {
class CriticalSection : virtual public CriticalSectionBase {
public:
    ~CriticalSection();
};

CriticalSection::~CriticalSection() {}
}
}

extern "C" void fn_001F47A4() {}
extern "C" void fn_001F607C() {}
