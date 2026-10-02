namespace
{
struct NerveStateBaseLayout
{
    unsigned char mPadding[8];
    bool mIsDead;
};
}

extern "C" void fn_001A82BC(NerveStateBaseLayout* self)
{
    self->mIsDead = false;
}

extern "C" void fn_0025A834(NerveStateBaseLayout* self)
{
    self->mIsDead = false;
}
