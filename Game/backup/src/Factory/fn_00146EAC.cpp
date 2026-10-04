namespace {

struct ValueSource {
    virtual void reserved() = 0;
    virtual void* getValue() = 0;
};

struct ValueHolder {
    unsigned char reserved[0x1c];
    void* value;
};

}

extern "C" void fn_00146EAC(ValueHolder* holder, ValueSource* source) {
    holder->value = source->getValue();
}
