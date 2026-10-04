extern "C" void fn_002C6960( void* );
extern "C" void fn_002C6000( void* );
extern "C" void fn_00229434( void* );

extern "C" void fn_003766CC( void* self_ptr )
{
        char* base = (char*)self_ptr - 76;
        unsigned char a = *(unsigned char*)(base + 8);
        if ( a != 0 )
        {
                a = *(unsigned char*)(base + 9);
                if ( a == 0 )
                        return;
        }
        else
        {
                return;
        }
        if ( *(void**)(base + 104) != 0 || *(float*)(base + 108) > 0.0f )
        {
                fn_002C6960( base );
        }
        else
        {
                if ( *(unsigned char*)(base + 10) == 0 )
                {
                        fn_002C6000( base );
                }
        }
        for ( int i = 0; i < 16; ++i )
        {
                void* p;
                if ( i > 15 )
                {
                        p = 0;
                }
                else
                {
                        p = *(void**)(base + i * 4 + 136);
                }
                if ( p != 0 )
                {
                        fn_00229434( p );
                }
        }
}
