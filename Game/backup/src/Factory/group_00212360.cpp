namespace al {
struct IUseNerve {};
struct Nerve {};
void setNerve(IUseNerve *, const Nerve *);
}
extern "C" const al::Nerve dat_003F1570;
extern "C" const al::Nerve dat_003F1574;

extern "C" void fn_00212360(al::IUseNerve *self, const al::Nerve *nerve) {
    *reinterpret_cast<const al::Nerve **>(reinterpret_cast<char *>(self) + 0x38) = nerve;
    al::setNerve(self, &dat_003F1570);
}
extern "C" void fn_00258194(al::IUseNerve *self, const al::Nerve *nerve) {
    *reinterpret_cast<const al::Nerve **>(reinterpret_cast<char *>(self) + 0x38) = nerve;
    al::setNerve(self, &dat_003F1574);
}
