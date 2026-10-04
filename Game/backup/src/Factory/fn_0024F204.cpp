namespace {
struct Record {
    unsigned int unknown0[2];
    unsigned int value8;
    unsigned int valueC;
};
}

extern "C" void fn_0024F204(Record *record) {
    record->value8 = record->valueC;
}
