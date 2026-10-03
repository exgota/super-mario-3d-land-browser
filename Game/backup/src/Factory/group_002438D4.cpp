#include "Math/alVectorNormalizationImports.h"
#include <math/seadVectorCalcCtr.h>

extern "C" {
void fn_0013615C(void*);
void fn_00268E64(void*);
void _ZN2al13NerveExecutor11updateNerveEv(void*);
void fn_002592F8(void*);
void* _ZN2rp14getPlayerActorEv();
void fn_0026C1B0(void*);
void fn_002D5668(void*);
unsigned int _ZN2nn5gxlow3CTR15GetPhysicalAddrEj(unsigned int);
void _ZN2nn5gxlow3CTR11WriteHWRegsEjPKvj(unsigned int, const void*, unsigned int);
void fn_0022F800(void*);
void fn_00136818(void*);
void _ZN2al9LiveActor4killEv(void*);
void _ZdlPv(void*);
void* _ZN4sead10FileDeviceD1Ev(void*);
void _ZdaPv(void*);
void fn_00222B28(void*);

void fn_002438D4(void* self) { fn_0013615C(self); }
void fn_00252B00(void* self) { fn_00268E64(self); }
void fn_0025C040(void* self) { _ZN2al13NerveExecutor11updateNerveEv(self); }
void fn_0026CC14(void* self) { fn_002592F8(self); }
void* fn_0026EFD8() { return _ZN2rp14getPlayerActorEv(); }
void fn_00277224(void* self) { fn_0026C1B0(self); }
void fn_002775AC(void* self) { _ZN2al13NerveExecutor11updateNerveEv(self); }
float fn_00279ABC(nn::math::VEC3& vector) { return sead::Vector3CalcCtr<float>::normalize(vector); }
void fn_0027DFE8(void* self) { _ZN2al13NerveExecutor11updateNerveEv(self); }
void fn_0028058C(void* self) { fn_002D5668(self); }
unsigned int fn_00289C1C(unsigned int address) { return _ZN2nn5gxlow3CTR15GetPhysicalAddrEj(address); }
void nngxlowWriteHWRegs(unsigned int address, const void* data, unsigned int size) { _ZN2nn5gxlow3CTR11WriteHWRegsEjPKvj(address, data, size); }
void fn_002A7828(void* self) { fn_0022F800(self); }
void fn_002A7B94(void* self) { fn_0022F800(self); }
void fn_002CD5DC(void* self) { fn_00136818(self); }
void fn_002D67C0(void* self) { _ZN2al9LiveActor4killEv(self); }
void fn_002DAB20(void* memory) { _ZdlPv(memory); }
void* fn_002DB2A8(void* self) { return _ZN4sead10FileDeviceD1Ev(self); }
void fn_002DDA14(void* memory) { _ZdaPv(memory); }
void fn_002DDAA4(void* memory) { _ZdaPv(memory); }
void fn_002DE418(void* memory) { _ZdlPv(memory); }
void* fn_002DF460(void* self) { return _ZN4sead10FileDeviceD1Ev(self); }
void fn_002DF9F4(void* self) { fn_00222B28(self); }
void* fn_002E00EC(void* self) { return _ZN4sead10FileDeviceD1Ev(self); }
void* fn_002E2FC8(void* self) { return _ZN4sead10FileDeviceD1Ev(self); }
}
