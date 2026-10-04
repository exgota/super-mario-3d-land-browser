namespace {
struct Value {
    unsigned char padding[0x14];
    float value;
};

struct Owner {
    unsigned char padding[0x20];
    Value* value;
};
}

extern "C" void fn_0025E50C(Owner* owner, float value)
{
    owner->value->value = value;
}
