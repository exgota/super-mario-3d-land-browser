namespace {
typedef unsigned int Word;
}

extern "C" void fn_003037E8(Word);

extern "C" void fn_00357624(void *, Word *object) {
    fn_003037E8(*reinterpret_cast<Word *>(*reinterpret_cast<Word *>(object) + 0xC));
}

extern "C" void fn_0035E5CC(void *, Word *object) {
    fn_003037E8(*reinterpret_cast<Word *>(*reinterpret_cast<Word *>(object) + 0xC));
}
