namespace
{
    namespace al
    {
        class Resource;
    }

    class StageResourceKeeper
    {
    public:
        al::Resource** mResources;
    };
}

extern "C" al::Resource* fn_00335360(const StageResourceKeeper* self)
{
    return self->mResources[1];
}
