namespace {

struct Object;

extern "C" void fn_001C1EE8(Object* self, const void* data);
extern "C" unsigned char dat_0041E334[];
extern "C" unsigned char dat_0041E340[];

}

extern "C" void fn_002CF7CC(Object* self) {
    fn_001C1EE8(self, reinterpret_cast<const void*>(0x0041E340));
}

extern "C" void fn_002D0574(Object* self) {
    fn_001C1EE8(self, reinterpret_cast<const void*>(0x0041E334));
}
