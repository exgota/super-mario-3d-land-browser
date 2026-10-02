namespace al {
struct Nerve {};
struct IUseNerve {};
int isNerve(const IUseNerve*, const Nerve*);
}

extern "C" unsigned char dat_003F011C;
extern "C" unsigned char dat_003F00F0;
extern "C" unsigned char dat_003F00AC;
extern "C" unsigned char dat_003F0040;

extern "C" bool fn_0032B74C(const al::IUseNerve* self) {
    return al::isNerve(self, reinterpret_cast<const al::Nerve*>(&dat_003F011C));
}
extern "C" bool fn_0032C7DC(const al::IUseNerve* self) {
    return al::isNerve(self, reinterpret_cast<const al::Nerve*>(&dat_003F00F0));
}
extern "C" bool fn_0032E050(const al::IUseNerve* self) {
    return al::isNerve(self, reinterpret_cast<const al::Nerve*>(&dat_003F00AC));
}
extern "C" bool fn_0032E86C(const al::IUseNerve* self) {
    return al::isNerve(self, reinterpret_cast<const al::Nerve*>(&dat_003F0040));
}
