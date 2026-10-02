namespace alProjectInterface {
extern void* getSystemKit();
}

namespace {
struct SystemKit {
    int padding;
    void* member;
};
}

extern "C" void* fn_001BB904(void*);

extern "C" void* fn_001BB8F0() {
    return fn_001BB904(
        reinterpret_cast<SystemKit*>(alProjectInterface::getSystemKit())->member);
}
