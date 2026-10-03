extern "C" void fn_001981B4(void *self, int value) {
    *reinterpret_cast<int *>(reinterpret_cast<char *>(self) + 4) = 5 - value;
}
