namespace sead {
struct MethodTreeNode {
    void detachAll();
};
}

extern "C" void fn_002DB344(void* self) {
    reinterpret_cast<sead::MethodTreeNode*>(static_cast<char*>(self) + 0x124)->detachAll();
}
