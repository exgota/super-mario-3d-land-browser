extern "C" void *fn_002C6348(void *object, int index)
{
    if (index <= 15)
        return *(void **)((char *)object + 0x88 + index * 4);
    return 0;
}

extern "C" void *fn_003407C8(void *object, int index)
{
    if (index <= 15)
        return *(void **)((char *)object + 0x88 + index * 4);
    return 0;
}
