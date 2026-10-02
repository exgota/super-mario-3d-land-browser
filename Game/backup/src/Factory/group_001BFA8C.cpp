namespace {

class EffectKeeper;

class IUseEffectKeeper {
public:
    virtual EffectKeeper* getEffectKeeper() = 0;
};

}

extern "C" void fn_001BFAA4(EffectKeeper* keeper);
extern "C" void fn_0024ADCC(EffectKeeper* keeper);

extern "C" void fn_001BFA8C(IUseEffectKeeper* effectUser)
{
    return fn_001BFAA4(effectUser->getEffectKeeper());
}

extern "C" void fn_0024ADB4(IUseEffectKeeper* effectUser)
{
    return fn_0024ADCC(effectUser->getEffectKeeper());
}
