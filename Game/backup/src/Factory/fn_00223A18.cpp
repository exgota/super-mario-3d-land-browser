namespace sead {
class MethodTreeNode {
public:
    void detachAll();
};
}

extern "C" void fn_00223A18(void* self) {
    return reinterpret_cast<sead::MethodTreeNode*>(static_cast<char*>(self) + 0x6c)->detachAll();
}
