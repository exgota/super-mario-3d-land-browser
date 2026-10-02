namespace {
class EmptyStageBase {};
}

namespace nn {
class Result {
    unsigned int value;
};

namespace dbg {
void PrintResult(Result) {}
}

namespace os {
class Thread {
public:
    class TypeInfo {
    public:
        template <typename T> static void Destroy(void*);
    };
};

template <> void Thread::TypeInfo::Destroy<void (*)()>(void*) {}
template <> void Thread::TypeInfo::Destroy<int>(void*) {}
template <> void Thread::TypeInfo::Destroy<unsigned int>(void*) {}
}

namespace gr {
namespace CTR {
class Combiner {
public:
    class Stage : virtual public EmptyStageBase {
    public:
        Stage();
    };
};

Combiner::Stage::Stage() {}
}
}
}

extern "C" {
void fn_00116738() {}
void fn_00116744() {}
void fn_001173A0() {}
void fn_0011AEB4() {}
void fn_0011FB98() {}
void fn_0012E930() {}
void fn_00130294() {}
void fn_001314B4() {}
void fn_0013A800() {}
void fn_0013D944() {}
void fn_0013D948() {}
void fn_0013D94C() {}
void fn_0013D950() {}
void fn_0013DE4C() {}
void fn_0013E1A4() {}
void fn_0014097C() {}
void fn_00140980() {}
void fn_00141090() {}
void fn_00141094() {}
void fn_00141224() {}
}
