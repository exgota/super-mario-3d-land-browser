namespace {
typedef unsigned short Halfword;
}

extern "C" Halfword* fn_00110EC0(void* object);

extern "C" Halfword* fn_0011091C(void* object) {
    Halfword* data = fn_00110EC0(object);
    if (data)
        data = data + *data + 1;
    return data;
}
