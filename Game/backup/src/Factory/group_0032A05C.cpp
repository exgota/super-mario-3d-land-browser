namespace {
struct FloatHolder {
    unsigned char padding[0x34];
    float value;
};
}

extern "C" FloatHolder dat_003EFAE4;
extern "C" FloatHolder dat_003EFEE4;

extern "C" float fn_0032A05C() { return dat_003EFAE4.value; }
extern "C" float fn_0032A88C() { return dat_003EFEE4.value; }
