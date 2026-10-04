namespace {
namespace al {
class EffectKeeper;

class IUseEffectKeeper {
public:
    virtual EffectKeeper* getEffectKeeper() = 0;
};
}
}

extern "C" void fn_001BFAE0(al::EffectKeeper* keeper, const char* name);

extern "C" void fn_0026EDBC(al::IUseEffectKeeper* effectUser, const char* name)
{
    fn_001BFAE0(effectUser->getEffectKeeper(), name);
}
