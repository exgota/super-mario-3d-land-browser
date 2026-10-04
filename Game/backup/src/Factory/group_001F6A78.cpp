namespace {
struct FloatSetting {
    unsigned char enabled;
    float value;
};
}

extern "C" {
extern FloatSetting dat_0042AE74;
extern FloatSetting dat_00426B54;
__attribute__((noinline)) void fn_001F7D14(FloatSetting*, float);
void fn_0023AF5C(FloatSetting*, float);

void fn_001F6A78(FloatSetting* self, float value) {
    if (self->enabled) {
        self->value = value;
        fn_001F7D14(reinterpret_cast<FloatSetting*>(0x0042AE74), value);
    }
}

void fn_001F7D14(FloatSetting* self, float value) {
    if (self->enabled) {
        self->value = value;
        fn_0023AF5C(reinterpret_cast<FloatSetting*>(0x00426B54), value);
    }
}
}
