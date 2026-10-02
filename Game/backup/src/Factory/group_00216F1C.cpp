namespace {
extern "C" void* fn_00216F34(void*, void*);
extern "C" void* fn_0026AD58(void*, void*);
}

namespace rp {
extern void* getCourseList();
}

extern "C" void* fn_00216F1C(void* self) {
    void* list = rp::getCourseList();
    return fn_00216F34(list, *reinterpret_cast<void**>(self));
}

extern "C" void* fn_0026AD40(void* self) {
    void* list = rp::getCourseList();
    return fn_0026AD58(list, *reinterpret_cast<void**>(self));
}
