extern "C" void* fn_00272554(void*);
extern "C" void* fn_00191E1C(void*, int, void*);

extern "C" void* fn_00191DFC(void* self)
{
    void* result = fn_00272554(self);
    return fn_00191E1C(result, 0, static_cast<char*>(self) + 0x468);
}
