extern "C" int fn_001F0734(int, int, int, int);
extern "C" int fn_002F5480(int, int, int, int);

extern "C" int fn_001F072C(int a, int b, int c)
{
    return fn_001F0734(a, b, c, 1);
}

extern "C" int fn_002F5478(int a, int b, int c)
{
    return fn_002F5480(a, b, c, 1);
}
