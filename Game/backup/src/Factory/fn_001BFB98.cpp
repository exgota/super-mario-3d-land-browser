namespace {
struct EffectKeeper;

struct IUseEffectKeeper {
    virtual EffectKeeper* getEffectKeeper() = 0;
};

extern "C" void _ZN2al12EffectKeeper13setActionNameEPKc(EffectKeeper*, const char*);
}

extern "C" void fn_001BFB98(IUseEffectKeeper* effectUser, const char* name)
{
    return _ZN2al12EffectKeeper13setActionNameEPKc(effectUser->getEffectKeeper(), name);
}
