namespace {
struct ForwardTarget {
    unsigned int pad;
    unsigned int pad2;
    void* value;
};
}

extern "C" unsigned int fn_001CC810(void*, void*, unsigned int, unsigned int);

extern "C" unsigned int fn_002627A4(void* first, ForwardTarget* target, unsigned int value)
{
    return fn_001CC810(first, target->value, value, value);
}
