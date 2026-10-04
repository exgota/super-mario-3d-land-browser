extern "C" void fn_001F3C54(void* object, float value)
{
    *reinterpret_cast<float*>(reinterpret_cast<unsigned char*>(object) + 0x858) = value;
}
