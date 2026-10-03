extern "C" float fn_0033013C(void *, float, float, float);

extern "C" float fn_00330134(void *self, float first, float second) {
    return fn_0033013C(self, first, second, *reinterpret_cast<float *>(reinterpret_cast<char *>(self) + 0x58));
}
