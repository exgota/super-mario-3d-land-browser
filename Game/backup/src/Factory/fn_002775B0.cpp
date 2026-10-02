namespace al {
class NerveExecutor {
public:
    void updateNerve();
};
}

extern "C" void fn_002775B0(void* self) {
    reinterpret_cast<al::NerveExecutor**>(
        reinterpret_cast<char*>(self) + 0x4c)[0]->updateNerve();
}
