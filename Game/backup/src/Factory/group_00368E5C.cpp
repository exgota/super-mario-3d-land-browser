namespace {
struct SlotOwner {
    void *slot;
};

extern "C" void *dat_003ADEB0;
extern "C" void *fn_001BFA04(void *, void *);
}

extern "C" void *fn_00368E5C(void *, SlotOwner *owner) {
    void *value = *reinterpret_cast<void **>(static_cast<char *>(owner->slot) + 0xC);
    if (value)
        value = static_cast<char *>(value) + 8;
    return fn_001BFA04(value, &dat_003ADEB0);
}

extern "C" void *fn_00368FA0(void *, SlotOwner *owner) {
    void *value = *reinterpret_cast<void **>(static_cast<char *>(owner->slot) + 0xC);
    if (value)
        value = static_cast<char *>(value) + 8;
    return fn_001BFA04(value, &dat_003ADEB0);
}
