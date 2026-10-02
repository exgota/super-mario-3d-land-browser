extern "C" int fn_0025BB60(int);
extern "C" int fn_0026B42C(int, int);

extern "C" int fn_002744E4(int value)
{
    int result = fn_0025BB60(value);
    return fn_0026B42C(result, value);
}
