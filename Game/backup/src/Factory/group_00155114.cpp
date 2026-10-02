namespace {
struct InitRecord { unsigned char bytes[12]; };
}
extern "C" unsigned char dat_003C9804;
extern "C" unsigned char dat_003D0804;
extern "C" void fn_00155114(InitRecord *self, int value) {
    *reinterpret_cast<unsigned int *>(self->bytes + 8) = value;
    *reinterpret_cast<unsigned long long *>(self->bytes) =
        static_cast<unsigned int>(reinterpret_cast<unsigned long>(&dat_003C9804));
}
extern "C" void fn_001A7140(InitRecord *self, int value) {
    *reinterpret_cast<unsigned int *>(self->bytes + 8) = value;
    *reinterpret_cast<unsigned long long *>(self->bytes) =
        static_cast<unsigned int>(reinterpret_cast<unsigned long>(&dat_003D0804));
}
