namespace {
struct ExecuteOrder
{
    unsigned int _0;
    const char* _4;
};
}

extern "C" bool fn_0033F51C(const char*, const char*);
extern "C" bool fn_003329D4(const char*, const char*);
extern "C" bool fn_0024BA48(const char*, const char*);
extern "C" bool fn_00216EDC(const char*, const char*);
extern "C" bool fn_002519B0(const char*, const char*);

extern "C" bool fn_00240210(const ExecuteOrder* order, const char* kind)
{
    return fn_0033F51C(order->_4, kind);
}

extern "C" bool fn_002770B8(const ExecuteOrder* order, const char* kind)
{
    return fn_003329D4(order->_4, kind);
}

extern "C" bool fn_00327F78(const ExecuteOrder* order, const char* kind)
{
    return fn_0024BA48(order->_4, kind);
}

extern "C" bool fn_0032E384(const ExecuteOrder* order, const char* kind)
{
    return fn_00216EDC(order->_4, kind);
}

extern "C" bool fn_00335A48(const ExecuteOrder* order, const char* kind)
{
    return fn_002519B0(order->_4, kind);
}
