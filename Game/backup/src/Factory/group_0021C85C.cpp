namespace {
typedef unsigned int Word;
}

extern "C" void fn_00218F80(void *, Word, Word);
extern "C" void fn_00218EB8(void *, Word, Word);

extern "C" void fn_0021C85C(void *self, Word value) {
    fn_00218F80(static_cast<unsigned char *>(self) + 4, 8, value);
}

extern "C" void fn_0021D028(void *self, Word value) {
    fn_00218EB8(static_cast<unsigned char *>(self) + 4, 8, value);
}
