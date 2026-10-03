namespace sead {
class MethodTreeNode {
public:
    void detachAll();
};
}

extern "C" void fn_002DD498(void *self) {
    reinterpret_cast<sead::MethodTreeNode *>(
        reinterpret_cast<char *>(self) + 0x6c)->detachAll();
}
