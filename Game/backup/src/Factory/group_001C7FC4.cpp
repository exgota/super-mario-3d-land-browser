extern "C" void fn_0024CC44(void*);
extern "C" void fn_001CD01C();
extern "C" void fn_001CD04C();

extern "C" void fn_001C7FC4(void* self) {
    fn_0024CC44(*reinterpret_cast<void**>(reinterpret_cast<char*>(self) + 0x18));
    fn_001CD01C();
}

extern "C" void fn_001C825C(void* self) {
    fn_0024CC44(*reinterpret_cast<void**>(reinterpret_cast<char*>(self) + 0x18));
    fn_001CD04C();
}
