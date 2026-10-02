namespace {
struct Data;
}

extern "C" void fn_002805B8(void*, int, int, void*);
extern "C" Data dat_003F1614;
extern "C" Data dat_003F1620;
extern "C" Data dat_003F1618;
extern "C" Data dat_003F161C;
extern "C" Data dat_003F1628;
extern "C" Data dat_003F162C;
extern "C" Data dat_003F1630;

extern "C" void fn_00375C1C(void* p, int a, int b, int) { fn_002805B8((char*)p - 0x64, a, b, &dat_003F1614); }
extern "C" void fn_00375C30(void* p, int a, int b, int) { fn_002805B8((char*)p - 0x64, a, b, &dat_003F1620); }
extern "C" void fn_00375C40(void* p, int a, int b, int) { fn_002805B8((char*)p - 0x64, a, b, &dat_003F1618); }
extern "C" void fn_00375C50(void* p, int a, int b, int) { fn_002805B8((char*)p - 0x64, a, b, &dat_003F161C); }
extern "C" void fn_00375D00(void* p, int a, int b, int) { fn_002805B8((char*)p - 0x64, a, b, &dat_003F1628); }
extern "C" void fn_00375D20(void* p, int a, int b, int) { fn_002805B8((char*)p - 0x64, a, b, &dat_003F162C); }
extern "C" void fn_00375D30(void* p, int a, int b, int) { fn_002805B8((char*)p - 0x64, a, b, &dat_003F1630); }
