extern "C" unsigned int fn_0026AE18(unsigned int, unsigned int, unsigned int, unsigned char);

extern "C" unsigned int fn_0026AE0C(unsigned int* value, unsigned int arg1,
                                    unsigned int arg2, unsigned int) {
    return fn_0026AE18(*value, arg1, arg2,
                       reinterpret_cast<unsigned char*>(value)[7]);
}
