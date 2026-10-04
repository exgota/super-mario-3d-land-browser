namespace {
struct Object;
struct Context;
}

extern "C" int fn_0033B3C4(Object*);
extern "C" int fn_0021600C(Context*);
extern "C" int fn_002A5448(Context*);

extern "C" int fn_0033C820(Object* object, Context* context)
{
    int first = fn_0033B3C4(object);
    int second = fn_0021600C(context);
    return first & second;
}

extern "C" int fn_0033C8F0(Object* object, Context* context)
{
    int first = fn_0033B3C4(object);
    int second = fn_002A5448(context);
    return first & second;
}
