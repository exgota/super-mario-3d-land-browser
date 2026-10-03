namespace {
struct OffsetObject {
    unsigned char padding[0xC];
    unsigned int offset;
};
}

extern "C" void* fn_0033F4C0(void* object) {
    OffsetObject* value = static_cast<OffsetObject*>(object);
    return static_cast<unsigned char*>(object) + value->offset;
}

extern "C" void* fn_003419A0(void* object) {
    OffsetObject* value = static_cast<OffsetObject*>(object);
    return static_cast<unsigned char*>(object) + value->offset;
}

extern "C" void* fn_00341D3C(void* object) {
    OffsetObject* value = static_cast<OffsetObject*>(object);
    return static_cast<unsigned char*>(object) + value->offset;
}
