namespace {
struct Triple {
    unsigned int first;
    unsigned int second;
    unsigned int third;
};
}

extern "C" void fn_0018FDB8(void* object, unsigned int first, unsigned int second, unsigned int third)
{
    Triple value = {first, second, third};
    *reinterpret_cast<Triple*>(reinterpret_cast<char*>(object) + 0x14) = value;
}
