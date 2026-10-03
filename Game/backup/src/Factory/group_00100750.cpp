namespace {
struct AppMemoryInfo {
    unsigned char pad[0x40];
    unsigned int value;
};
extern "C" AppMemoryInfo dat_003EFAE4;
}

namespace nn { namespace os {
unsigned int GetAppMemorySize() {
    return *reinterpret_cast<volatile unsigned int *>(0x1FF80000u + 0x40u);
}
} }

extern "C" unsigned int fn_0032A00C() {
    return dat_003EFAE4.value;
}
