namespace {
struct Owner {
    unsigned char pad[0x20];
    void *resource;
};
}

extern "C" void *fn_001DE7C0(Owner *owner)
{
    return static_cast<unsigned char *>(owner->resource) + 0x98;
}
