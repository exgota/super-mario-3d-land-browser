namespace {
struct Storage { void** mVirtualFunctionTable; };
}

extern "C" Storage* fn_00267FDC(Storage*, const char*, int);
extern "C" Storage* fn_001BD418(Storage*, const char*, int);
extern "C" Storage* fn_00258C58(Storage*, const char*, int);
extern "C" Storage* _ZN2al7AreaObjC1EPKc(Storage*, const char*, int);
extern "C" Storage* _ZN2al6CameraC1EPKc(Storage*, const char*, int);
extern "C" Storage* fn_001E2AB8(Storage*, const char*, int);
extern "C" Storage* _ZN2al9AreaShapeC2Ev(Storage*, const char*, int);
extern "C" Storage* fn_001CD4BC(Storage*, const char*, int);
extern "C" __attribute__((noinline)) Storage* fn_001D790C(Storage*, const char*, int);
extern "C" Storage* fn_00241678(Storage*, const char*, int);
extern "C" Storage* fn_001D6F2C(Storage*, const char*, int);
extern "C" Storage* fn_0022C8EC(Storage*, const char*, int);
extern "C" Storage* fn_002C3150(Storage*, const char*, int);
extern "C" Storage* fn_002CB890(Storage*, const char*, int);

extern "C" void* dat_003C7E58[];
extern "C" void* dat_003CA4F4[];
extern "C" void* dat_003CC344[];
extern "C" void* dat_003CEDA8[];
extern "C" void* dat_003CF0B0[];
extern "C" void* dat_003CF214[];
extern "C" void* dat_003CFCF8[];
extern "C" void* dat_003D0290[];
extern "C" void* dat_003D1214[];
extern "C" void* dat_003D62D4[];
extern "C" void* dat_003D6A00[];
extern "C" void* dat_003D6BC0[];
extern "C" void* dat_003D7448[];
extern "C" void* dat_003D748C[];
extern "C" void* dat_003D726C[];
extern "C" void* dat_003D8A24[];
extern "C" void* dat_003D8F74[];
extern "C" void* dat_003D92EC[];

extern "C" Storage* fn_00140B54(Storage* storage, const char* name, int capacity)
{
    storage = fn_00267FDC(storage, name, capacity);
    storage->mVirtualFunctionTable = dat_003C7E58;
    return storage;
}

extern "C" Storage* fn_0015C940(Storage* storage, const char* name, int capacity)
{
    storage = fn_001BD418(storage, name, capacity);
    storage->mVirtualFunctionTable = dat_003CA4F4;
    return storage;
}

extern "C" Storage* fn_00171D64(Storage* storage, const char* name, int capacity)
{
    storage = fn_00258C58(storage, name, capacity);
    storage->mVirtualFunctionTable = dat_003CC344;
    return storage;
}

extern "C" Storage* fn_00195598(Storage* storage, const char* name, int capacity)
{
    storage = fn_00267FDC(storage, name, capacity);
    storage->mVirtualFunctionTable = dat_003CEDA8;
    return storage;
}

extern "C" Storage* fn_00196300(Storage* storage, const char* name, int capacity)
{
    storage = fn_00258C58(storage, name, capacity);
    storage->mVirtualFunctionTable = dat_003CF0B0;
    return storage;
}

extern "C" Storage* fn_001974C0(Storage* storage, const char* name, int capacity)
{
    storage = fn_00258C58(storage, name, capacity);
    storage->mVirtualFunctionTable = dat_003CF214;
    return storage;
}

extern "C" Storage* fn_0019FB10(Storage* storage, const char* name, int capacity)
{
    storage = _ZN2al7AreaObjC1EPKc(storage, name, capacity);
    storage->mVirtualFunctionTable = dat_003CFCF8;
    return storage;
}

extern "C" Storage* fn_001A249C(Storage* storage, const char* name, int capacity)
{
    storage = _ZN2al6CameraC1EPKc(storage, name, capacity);
    storage->mVirtualFunctionTable = dat_003D0290;
    return storage;
}

extern "C" Storage* fn_001B996C(Storage* storage, const char* name, int capacity)
{
    storage = fn_001E2AB8(storage, name, capacity);
    storage->mVirtualFunctionTable = dat_003D1214;
    return storage;
}

extern "C" Storage* fn_001C510C(Storage* storage, const char* name, int capacity)
{
    storage = _ZN2al9AreaShapeC2Ev(storage, name, capacity);
    storage->mVirtualFunctionTable = dat_003D62D4;
    return storage;
}

extern "C" Storage* fn_001D32B8(Storage* storage, const char* name, int capacity)
{
    storage = _ZN2al9AreaShapeC2Ev(storage, name, capacity);
    storage->mVirtualFunctionTable = dat_003D6A00;
    return storage;
}

extern "C" Storage* fn_001E3E58(Storage* storage, const char* name, int capacity)
{
    storage = fn_001D790C(storage, name, capacity);
    storage->mVirtualFunctionTable = dat_003D7448;
    return storage;
}

extern "C" __attribute__((noinline)) Storage* fn_001D790C(Storage* storage, const char* name, int capacity)
{
    storage = fn_001CD4BC(storage, name, capacity);
    storage->mVirtualFunctionTable = dat_003D6BC0;
    return storage;
}

extern "C" Storage* fn_001E42C8(Storage* storage, const char* name, int capacity)
{
    storage = fn_00241678(storage, name, capacity);
    storage->mVirtualFunctionTable = dat_003D748C;
    return storage;
}

extern "C" Storage* fn_00260408(Storage* storage, const char* name, int capacity)
{
    storage = fn_001D6F2C(storage, name, capacity);
    storage->mVirtualFunctionTable = dat_003D726C;
    return storage;
}

extern "C" Storage* fn_002B9378(Storage* storage, const char* name, int capacity)
{
    storage = fn_0022C8EC(storage, name, capacity);
    storage->mVirtualFunctionTable = dat_003D8A24;
    return storage;
}

extern "C" Storage* fn_002C4314(Storage* storage, const char* name, int capacity)
{
    storage = fn_002C3150(storage, name, capacity);
    storage->mVirtualFunctionTable = dat_003D8F74;
    return storage;
}

extern "C" Storage* fn_002CC358(Storage* storage, const char* name, int capacity)
{
    storage = fn_002CB890(storage, name, capacity);
    storage->mVirtualFunctionTable = dat_003D92EC;
    return storage;
}
