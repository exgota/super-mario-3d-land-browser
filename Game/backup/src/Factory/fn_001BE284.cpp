extern "C" int fn_00250564(void* object);

extern "C" void fn_001BE284(void* object)
{
    void* target = *static_cast<void**>(object);
    if (fn_00250564(target) == 0)
        static_cast<unsigned char*>(object)[8] = 1;
}
