#include <prim/seadSafeString.h>
struct Fn003184F4 { char _00[0x68]; };
extern "C" {
Fn003184F4* _ZN2al11MapObjActorC1ERKN4sead14SafeStringBaseIcEE( Fn003184F4*, const sead::SafeString& );
extern const unsigned char dat_003D4200[];
}
extern "C" void fn_003184F4( Fn003184F4* self, const sead::SafeString& name )
{
        self = _ZN2al11MapObjActorC1ERKN4sead14SafeStringBaseIcEE( self, name );
        const unsigned char* table = dat_003D4200;
        const void* tables[4];
        tables[0] = table;
        tables[1] = table + 0x68;
        tables[2] = table + 0x74;
        tables[3] = table + 0x88;
        for ( int i = 0; i < 2; ++i )
        {
                ((const void**)self)[i] = tables[i];
        }
        const void** p08 = (const void**)((const char*)self + 8);
        p08[0] = tables[2];
        p08[1] = tables[3];
        ((float*)self)[24] = 10.0f;
        ((int*)self)[25] = 0;
}
