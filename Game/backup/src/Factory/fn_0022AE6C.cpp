extern "C" int fn_0022AE78(void*, int, int, int);

extern "C" int fn_0022AE6C(void* self, int arg1, int arg2)
{
    self = static_cast<char*>(self) + 8;
    return fn_0022AE78(self, arg1, arg2, 0xcc);
}
