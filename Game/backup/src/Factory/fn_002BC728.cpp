namespace
{
struct ExecuteOrder
{
    unsigned int _0;
    const char* _4;
};
}

extern "C" bool fn_00240210(const char* string, const char* kind);

extern "C" bool fn_002BC728(const ExecuteOrder* order, const char* kind)
{
    return fn_00240210(order->_4, kind);
}
