namespace al {
class IUseNerve;
class Nerve;
extern bool isNerve(const IUseNerve *, const Nerve *);
}

extern "C" const unsigned char dat_003F1CF4;
extern "C" const unsigned char dat_003F1CE8;
extern "C" const unsigned char dat_003F1D08;
extern "C" const unsigned char dat_003F1D10;
extern "C" const unsigned char dat_003F1D00;
extern "C" const unsigned char dat_003F1D04;
extern "C" const unsigned char dat_003F1CFC;
extern "C" const unsigned char dat_003F1D0C;

extern "C" bool fn_00376094(const void *self) { return al::isNerve(reinterpret_cast<const al::IUseNerve *>(reinterpret_cast<const char *>(self) - 0x14), reinterpret_cast<const al::Nerve *>(&dat_003F1CF4)); }
extern "C" bool fn_003760E4(const void *self) { return al::isNerve(reinterpret_cast<const al::IUseNerve *>(reinterpret_cast<const char *>(self) - 0x14), reinterpret_cast<const al::Nerve *>(&dat_003F1CE8)); }
extern "C" bool fn_003760F4(const void *self) { return al::isNerve(reinterpret_cast<const al::IUseNerve *>(reinterpret_cast<const char *>(self) - 0x14), reinterpret_cast<const al::Nerve *>(&dat_003F1D08)); }
extern "C" bool fn_00376104(const void *self) { return al::isNerve(reinterpret_cast<const al::IUseNerve *>(reinterpret_cast<const char *>(self) - 0x14), reinterpret_cast<const al::Nerve *>(&dat_003F1D10)); }
extern "C" bool fn_00376114(const void *self) { return al::isNerve(reinterpret_cast<const al::IUseNerve *>(reinterpret_cast<const char *>(self) - 0x14), reinterpret_cast<const al::Nerve *>(&dat_003F1D00)); }
extern "C" bool fn_00376174(const void *self) { return al::isNerve(reinterpret_cast<const al::IUseNerve *>(reinterpret_cast<const char *>(self) - 0x14), reinterpret_cast<const al::Nerve *>(&dat_003F1D04)); }
extern "C" bool fn_00376184(const void *self) { return al::isNerve(reinterpret_cast<const al::IUseNerve *>(reinterpret_cast<const char *>(self) - 0x14), reinterpret_cast<const al::Nerve *>(&dat_003F1CFC)); }
extern "C" bool fn_00376194(const void *self) { return al::isNerve(reinterpret_cast<const al::IUseNerve *>(reinterpret_cast<const char *>(self) - 0x14), reinterpret_cast<const al::Nerve *>(&dat_003F1D0C)); }
