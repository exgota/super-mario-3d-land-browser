namespace {
struct LookupObject {
    unsigned char padding[0x48];
    void* value;
};
}

extern "C" {
extern LookupObject dat_003EFEE4;
extern LookupObject dat_003EFAE4;

void* fn_00328BEC() { return dat_003EFEE4.value; }
void* fn_00329E2C() { return dat_003EFAE4.value; }
}
