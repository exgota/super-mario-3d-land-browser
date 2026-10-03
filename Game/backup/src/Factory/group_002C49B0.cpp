extern "C" void fn_002C49B8(void *);
extern "C" void fn_002C5AE8(void *);

extern "C" void fn_002C49B0(void *p) {
    fn_002C49B8(static_cast<char *>(p) - 0x40);
}

extern "C" void fn_002C5AE0(void *p) {
    fn_002C5AE8(static_cast<char *>(p) - 0x40);
}
