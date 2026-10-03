namespace sead
{
class Heap;
}

extern "C"
{
extern sead::Heap* dat_003EF8BC;
extern sead::Heap* dat_003EF8C0;
}

extern void* operator new[](unsigned int, sead::Heap*, int);

extern "C" void* fn_002DDA18(unsigned int size)
{
    return operator new[](size, dat_003EF8BC, 4);
}

extern "C" void* fn_002DDAA8(unsigned int size)
{
    return operator new[](size, dat_003EF8C0, 4);
}
