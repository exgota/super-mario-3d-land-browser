extern "C" void fn_001DDB44(unsigned int, unsigned int size, unsigned int, unsigned int,
                              void *ptr) {
    if (size == 0x10000)
        ::operator delete(ptr);
}

extern "C" void fn_002E24CC(unsigned int, unsigned int size, unsigned int, unsigned int,
                              void *ptr) {
    if (size == 0x10000)
        ::operator delete(ptr);
}
