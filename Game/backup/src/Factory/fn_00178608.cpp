namespace {

class ValueSource {
public:
    virtual void reserved() = 0;
    virtual int getValue() = 0;
};

struct ValueHolder {
    char padding[0x20];
    int value;
};

}

extern "C" void fn_00178608(ValueHolder* holder, ValueSource* source) {
    holder->value = source->getValue();
}
