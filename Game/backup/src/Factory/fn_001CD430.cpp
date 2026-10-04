extern "C" void fn_001CD430(void* storage, int count)
{
    void** buffer = new void*[count];
    reinterpret_cast<void***>(storage)[1] = buffer;
    reinterpret_cast<int*>(storage)[2] = count;
}
