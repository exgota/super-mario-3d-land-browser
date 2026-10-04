extern "C" int fn_002B9B50(void*, int*);

extern "C" int fn_002B9B44(void* arg0, int* arg1)
{
    return fn_002B9B50(arg0, arg1 ? arg1 + 1 : 0);
}
