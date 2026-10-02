namespace {
typedef unsigned int Word;
}

extern "C" Word fn_0027FF40(void *, Word);

extern "C" Word fn_0012D5DC(void *self) {
    return fn_0027FF40(self, *reinterpret_cast<Word *>(static_cast<char *>(self) + 0x6c));
}
