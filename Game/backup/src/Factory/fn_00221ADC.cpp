namespace sead {
class MethodTreeNode {
public:
    void call();
};
}

extern "C" void fn_00221ADC(void* self) {
    reinterpret_cast<sead::MethodTreeNode*>(
        reinterpret_cast<char*>(self) + 0x394)->call();
}
