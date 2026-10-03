namespace sead {
class MethodTreeNode {
public:
    void call();
};
}

extern "C" void fn_002E2FCC(void* self) {
    reinterpret_cast<sead::MethodTreeNode*>(static_cast<char*>(self) + 0x20)->call();
}
