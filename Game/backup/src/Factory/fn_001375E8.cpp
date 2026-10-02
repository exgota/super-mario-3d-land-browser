namespace
{
struct AreaObjectActivationFields
{
    unsigned char mPadding[0x48];
    bool mIsActive;
};
}

extern "C" void fn_001375E8(void* areaObject)
{
    static_cast<AreaObjectActivationFields*>(areaObject)->mIsActive = false;
}
