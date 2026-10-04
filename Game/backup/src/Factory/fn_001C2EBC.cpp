namespace {
struct ByteRecord {
    unsigned char padding[0x10];
    signed char value;
};

struct RecordOwner {
    unsigned char padding[0x44];
    ByteRecord *record;
};
}

extern "C" signed char fn_001C2EBC(RecordOwner *self) {
    return self->record->value;
}
