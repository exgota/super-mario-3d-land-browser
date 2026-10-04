extern "C" void fn_002B9E94(void* self, void* value, float scale) {
    *reinterpret_cast<void**>(static_cast<char*>(self) + 0x34) = value;
    *reinterpret_cast<float*>(static_cast<char*>(self) + 0x38) = scale;
}
