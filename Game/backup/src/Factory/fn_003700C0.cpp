namespace {
struct Object {
    unsigned char pad[12];
    void* nerve;
};
}

namespace al {
struct IUseNerve {};
struct Nerve {};
void setNerve(IUseNerve*, const Nerve*);
}

extern "C" int fn_00142770(void*);
extern "C" const al::Nerve dat_003F354C;

extern "C" void fn_003700C0(void*, Object** object) {
    Object* value = *object;
    if (fn_00142770(value->nerve)) {
        al::setNerve(reinterpret_cast<al::IUseNerve*>(value), &dat_003F354C);
    }
}
