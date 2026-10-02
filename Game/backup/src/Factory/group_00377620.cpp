namespace al {
struct IUseNerve;
struct Nerve;
void setNerve(IUseNerve *, const Nerve *);
bool isNerve(const IUseNerve *, const Nerve *);
}

extern "C" unsigned char dat_003F3130;
extern "C" unsigned char dat_003F15CC;
extern "C" unsigned char dat_003F26BC;
extern "C" unsigned char dat_003F15F4;

extern "C" void fn_00377620(void *self) {
    al::setNerve(reinterpret_cast<al::IUseNerve *>(static_cast<char *>(self) - 0x60), reinterpret_cast<const al::Nerve *>(&dat_003F3130));
}

extern "C" bool fn_00377B78(void *self) {
    return al::isNerve(reinterpret_cast<const al::IUseNerve *>(static_cast<const char *>(self) - 0x60), reinterpret_cast<const al::Nerve *>(&dat_003F15CC));
}

extern "C" bool fn_00377C70(void *self) {
    return al::isNerve(reinterpret_cast<const al::IUseNerve *>(static_cast<const char *>(self) - 0x60), reinterpret_cast<const al::Nerve *>(&dat_003F26BC));
}

extern "C" bool fn_00377CC0(void *self) {
    return al::isNerve(reinterpret_cast<const al::IUseNerve *>(static_cast<const char *>(self) - 0x60), reinterpret_cast<const al::Nerve *>(&dat_003F15F4));
}
