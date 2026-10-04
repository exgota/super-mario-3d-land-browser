// WIP: Function at 0x00325B04 (64 bytes)
// Target: dispatch-like, calls 0x280428, sets pointers from 0x003d5784,
// zeros at +0x60, +0x64, +0x68. No float constant.
// Status: Not verified (toolchain objdump unavailable). Structure based on
// disassembly analysis. Identical structure to 00321E8C, different table.
struct Fn00325B04 { char _00[0x6c]; };
extern "C" {
Fn00325B04* fn_00280428( Fn00325B04* );
extern const unsigned char dat_003D5784[];
}
extern "C" void fn_00325B04( Fn00325B04* self )
{
        self = fn_00280428( self );
        const unsigned char* table = dat_003D5784;
        ((int*)self)[24] = 0;
        ((const void**)self)[0] = (const void*)table;
        const unsigned char* t74 = table + 0x74;
        ((const void**)self)[3] = (const void*)(t74 + 0x14);
        ((int*)self)[25] = 0;
        ((int*)self)[26] = 0;
        const void** p04 = (const void**)((char*)self + 4);
        p04[0] = (const void*)(table + 0x68);
        p04[1] = (const void*)t74;
}
