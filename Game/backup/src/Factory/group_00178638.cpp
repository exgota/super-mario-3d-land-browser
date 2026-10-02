namespace al {
struct Nerve {};
struct IUseNerve {};
void setNerve(IUseNerve *, const Nerve *);
}
namespace {
struct Object {
    unsigned char pad_00[8];
    unsigned char field_08;
    unsigned char pad_09[7];
    unsigned int field_10;
    unsigned int field_14;
};
}
extern "C" unsigned int dat_003F34F4;
extern "C" unsigned int dat_003F34E8;
extern "C" void fn_00178638(Object *self) {
    self->field_08 = 0;
    self->field_10 = 0;
    self->field_14 = 0;
    al::setNerve(reinterpret_cast<al::IUseNerve *>(self), reinterpret_cast<const al::Nerve *>(&dat_003F34F4));
}
extern "C" void fn_001A65FC(Object *self) {
    self->field_08 = 0;
    self->field_10 = 0;
    self->field_14 = 0;
    al::setNerve(reinterpret_cast<al::IUseNerve *>(self), reinterpret_cast<const al::Nerve *>(&dat_003F34E8));
}
