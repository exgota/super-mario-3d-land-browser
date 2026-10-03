extern "C" void fn_0022AC0C(void* self, unsigned char value, unsigned int word) {
    *reinterpret_cast<unsigned char*>(static_cast<char*>(self) + 0x98) = value;
    *reinterpret_cast<unsigned int*>(static_cast<char*>(self) + 0x50) = word;
}
