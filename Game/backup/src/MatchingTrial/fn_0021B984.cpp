extern "C" bool fn_0021B984( int* out, const char* str, int len )
{
        if ( len <= 0 )
        {
                return false;
        }
        else
        {
                *out = 0;
                int bias = -'0';
                int c;
                while ( --len >= 0 && ( c = *str ) != 0 )
                {
                        unsigned int d = (unsigned int)( c - '0' );
                        if ( d >= 10 )
                                return false;
                        ++str;
                        *out = c + ( bias + *out * 10 );
                }
                return true;
        }
}
