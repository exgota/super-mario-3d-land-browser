namespace {

struct ValueHolder {
    unsigned int field_0;
    unsigned int field_4;
    unsigned int value;
};

struct ValueSource {
    virtual void slot_0() = 0;
    virtual unsigned int getValue() = 0;
};

}

extern "C" void fn_001DC870(ValueHolder* self, ValueSource* source) {
    self->value = source->getValue();
}

extern "C" void fn_00242DF0(ValueHolder* self, ValueSource* source) {
    self->value = source->getValue();
}
