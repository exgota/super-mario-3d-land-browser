namespace {
struct OffsetHolder {
    unsigned char padding[0x10];
    unsigned int offset;
};
}

extern "C" void *fn_0033F8B8(OffsetHolder *object) {
    return reinterpret_cast<unsigned char *>(object) + object->offset;
}

extern "C" void *fn_0033F8EC(OffsetHolder *object) {
    return reinterpret_cast<unsigned char *>(object) + object->offset;
}

extern "C" void *fn_0033F8F8(OffsetHolder *object) {
    return reinterpret_cast<unsigned char *>(object) + object->offset;
}
