// NonMatching: complete clean-room reconstruction of EU 0x0028F9DC..0x002905F0.
// Descriptive address name retained; see project/dot_reports/display-initializer-28f9dc.md.
#include <retail/DisplayInitializer.h>

#ifdef NON_MATCHING
extern "C" {
void fn_0028D1F0(void*, u32);
int _sta_initializeState(void*);
int _fb_initializeFBManager(void*);
int _vb_initializeVBManager(void*);
int _shm_initializeShaderManager(void*);
int _tx_initializeTexManager(void*);
int _vldtr_initializeStateValidator(void*);
u32 glGetError();
void fn_0028F6F0(u32);
void fn_0028C500(s32, const u32*);
void nngxlowInitialize();
int nngxlowIsFirstInitialization();
u32 nngxlowGetNumSpeculativeRequests();
void nngxlowRegisterInterruptHandler(void (*)(), u32);
void sys_PSC0Callback(); void sys_PSC1Callback();
void sys_PDC0Callback(); void sys_PDC1Callback();
void sys_PPFCallback(); void sys_P3DCallback();
void fn_0028AEF0();
void nngxlowWriteHWRegs(u32, const u32*, u32);
void nngxlowWriteHWRegsWithMask(u32, const u32*, const u32*, u32);
void nngxSplitDrawCmdlist();
u32 fn_0028E240(u32);
void fn_0028A814();
void fn_0028C640();
void nngxlowUnlock();
void nngxlowYieldThread();
}
using namespace retail_display;
static_assert_(sizeof(Context) == 0x180);
static_assert_(sizeof(CommandList) == 0x3c);
static_assert_(sizeof(AllocatorState) == 0x14);

extern "C" int fn_0028F9DC(Allocate allocate, Release release)
{
    u32 id = 1;
    if (!allocate || !release || dat_0041CFA0.initialized)
        return 0;
    fn_0028D1F0(&dat_0041CFA0, 0x180);
    dat_0041CFA0.width0 = 240;
    dat_0041CFA0.height0 = 400;
    dat_0041CFA0.width1 = 240;
    dat_0041CFA0.height1 = 320;
    dat_003E2654.allocate = allocate;
    dat_003E2654.release = release;
    dat_003E2654.enabled1 = 1;
    dat_003E2654.enabled0 = 1;
    dat_0041CFA0.flags = 0x700;
    u8 *framebuffer, *vertices, *shaders, *textures, *validator;
    u8* managers = static_cast<u8*>(allocate(0x10000, 0x100, 0, 0x2fdc));
    dat_003E2654.managers = managers;
    if (!managers) goto failed;
    framebuffer = managers + 0x758;
    vertices = managers + 0x768;
    shaders = managers + 0xfac;
    textures = managers + 0x2720;
    validator = managers + 0x2fcc;
    if (_sta_initializeState(managers) < 0) goto failed;
    if (_fb_initializeFBManager(framebuffer) < 0) goto failed;
    if (_vb_initializeVBManager(vertices) < 0) goto failed;
    if (_shm_initializeShaderManager(shaders) < 0) goto failed;
    if (_tx_initializeTexManager(textures) < 0) goto failed;
    glGetError();
    fn_0028F6F0(id);
    {
        CommandList* list = dat_0041CFA0.current;
        if (list) {
            if (list->buffer && dat_003E2654.release)
                dat_003E2654.release(0x10000, 0x105, list->id, list->buffer);
            list->recordCapacity = 16;
            list->used = 0;
            list->capacity = 0x10000;
            list->recordCount = 0;
            list->unknown10 = 0;
            list->processedCount = 0;
            list->submittedCount = 0;
            u8* buffer = dat_003E2654.allocate
                ? static_cast<u8*>(dat_003E2654.allocate(0x10000, 0x105, list->id, 0x101c0)) : 0;
            list->buffer = buffer;
            list->records = buffer + 0x10000;
            fn_0028D1F0(list->records, 0x1c0);
            dat_003E2E30 = list->buffer;
            dat_003E2E34 = list->buffer + list->capacity;
        }
        if (dat_0041CFA0.current) dat_0041CFA0.current->mode = 0x300;
    }
    if (glGetError()) goto failed;
    if (_vldtr_initializeStateValidator(validator) < 0) goto failed;
    nngxlowInitialize();
    {
        int first = nngxlowIsFirstInitialization();
        dat_0041CFA0.speculativeRequests = nngxlowGetNumSpeculativeRequests();
        nngxlowRegisterInterruptHandler(sys_PSC0Callback, 0);
        nngxlowRegisterInterruptHandler(sys_PSC1Callback, 1);
        nngxlowRegisterInterruptHandler(sys_PDC0Callback, 2);
        nngxlowRegisterInterruptHandler(sys_PDC1Callback, 3);
        nngxlowRegisterInterruptHandler(sys_PPFCallback, 4);
        nngxlowRegisterInterruptHandler(sys_P3DCallback, 5);
        nngxlowRegisterInterruptHandler(fn_0028AEF0, 6);
        if (first) {
            u32 value, mask;
#define WRITE(address, data) do { value = (data); nngxlowWriteHWRegs((address), &value, 4); } while (0)
#define MASK(address, data, bits) do { value = (data); mask = (bits); nngxlowWriteHWRegsWithMask((address), &value, &mask, 4); } while (0)
            WRITE(0x401000u, 0x0u);
            WRITE(0x401080u, 0x12345678u);
            WRITE(0x4010c0u, 0xfffffff0u);
            WRITE(0x4010d0u, 0x1u);
            WRITE(0x400400u, 0x1c2u);
            WRITE(0x400404u, 0xd1u);
            WRITE(0x400408u, 0x1c1u);
            WRITE(0x40040cu, 0x1c1u);
            WRITE(0x400410u, 0x0u);
            WRITE(0x400414u, 0xcfu);
            WRITE(0x400418u, 0xd1u);
            WRITE(0x40041cu, 0x1c501c1u);
            WRITE(0x400420u, 0x10000u);
            WRITE(0x400424u, 0x19du);
            WRITE(0x400428u, 0x2u);
            WRITE(0x40042cu, 0x192u);
            WRITE(0x400430u, 0x192u);
            WRITE(0x400434u, 0x192u);
            WRITE(0x400438u, 0x1u);
            WRITE(0x40043cu, 0x2u);
            WRITE(0x400440u, 0x1960192u);
            WRITE(0x400444u, 0x0u);
            WRITE(0x400448u, 0x0u);
            WRITE(0x40045cu, 0x19000f0u);
            WRITE(0x400460u, 0x1c100d1u);
            WRITE(0x400464u, 0x1920002u);
            WRITE(0x400470u, 0x80340u);
            WRITE(0x40049cu, 0x0u);
            WRITE(0x400500u, 0x1c2u);
            WRITE(0x400504u, 0xd1u);
            WRITE(0x400508u, 0x1c1u);
            WRITE(0x40050cu, 0x1c1u);
            WRITE(0x400510u, 0xcdu);
            WRITE(0x400514u, 0xcfu);
            WRITE(0x400518u, 0xd1u);
            WRITE(0x40051cu, 0x1c501c1u);
            WRITE(0x400520u, 0x10000u);
            WRITE(0x400524u, 0x19du);
            WRITE(0x400528u, 0x52u);
            WRITE(0x40052cu, 0x192u);
            WRITE(0x400530u, 0x192u);
            WRITE(0x400534u, 0x4fu);
            WRITE(0x400538u, 0x50u);
            WRITE(0x40053cu, 0x52u);
            WRITE(0x400540u, 0x1980194u);
            WRITE(0x400544u, 0x0u);
            WRITE(0x400548u, 0x11u);
            WRITE(0x40055cu, 0x14000f0u);
            WRITE(0x400560u, 0x1c100d1u);
            WRITE(0x400564u, 0x1920052u);
            WRITE(0x400570u, 0x80300u);
            WRITE(0x40059cu, 0x0u);
            WRITE(0x400468u, 0x18300000u);
            WRITE(0x40046cu, 0x18300000u);
            WRITE(0x400568u, 0x18300000u);
            WRITE(0x40056cu, 0x18300000u);
            WRITE(0x400494u, 0x18300000u);
            WRITE(0x400498u, 0x18300000u);
            WRITE(0x400478u, 0x1u);
            WRITE(0x400578u, 0x1u);
            MASK(0x400c18u, 0x0u, 0xff00u);
            WRITE(0x400004u, 0x70100u);
            MASK(0x40001cu, 0x0u, 0xffu);
            MASK(0x40002cu, 0x0u, 0xffu);
            WRITE(0x400050u, 0x22221200u);
            MASK(0x400054u, 0xff2u, 0xffffu);
            WRITE(0x400474u, 0x10501u);
            WRITE(0x400574u, 0x10501u);
#undef WRITE
#undef MASK
        }
        nngxSplitDrawCmdlist();
        CommandList* list = dat_0041CFA0.current;
        void* temporary = 0;
        if (first) {
            u8* record = list->records + list->recordCount * 28;
            record[0] = 3;
            u32* fields = reinterpret_cast<u32*>(record);
            fields[5] = (fields[5] & ~7u) | 4;
            fields[1] = fn_0028E240(0x30000) - 0x7fff;
            temporary = dat_003E2654.allocate
                ? dat_003E2654.allocate(0x10000, 0x100, 0, 0x8010) : 0;
            if (!temporary) goto failed;
            u32 address = reinterpret_cast<u32>(temporary);
            fields[2] = address;
            if (address & 15) fields[2] = address + 16 - (address & 15);
            u16* extents = reinterpret_cast<u16*>(record + 12);
            extents[0] = extents[1] = extents[2] = extents[3] = 128;
            fields[5] = (fields[5] & ~0xff8u) | 0x20;
            ++list->recordCount;
        }
        list = dat_0041CFA0.current;
        if (list && !dat_0041CFA0.busy) {
            dat_0041CFA0.active = list;
            dat_0041CFA0.submitting = 1;
            if (list->processedCount >= list->recordCount) goto complete;
            dat_0041CFA0.busy = 1;
            if (list->mode == 0x300) {
                fn_0028A814();
                fn_0028C640();
                nngxlowUnlock();
            }
        }
        while (dat_0041CFA0.busy) nngxlowYieldThread();
complete:
        if (first && dat_003E2654.release)
            dat_003E2654.release(0x10000, 0x100, 0, temporary);
        list = dat_0041CFA0.active;
        if (list) {
            fn_0028A814();
            if (!dat_0041CFA0.busy) dat_0041CFA0.submitting = 0;
            else {
                list->records[list->submittedCount * 28 - 26] = 1;
                dat_0041CFA0.cancelPending = 1;
            }
            nngxlowUnlock();
        }
        fn_0028F6F0(0);
        fn_0028C500(1, &id);
        dat_0041CFA0.initialized = 1;
        return 1;
    }
failed:
    fn_0028C500(1, &id);
    return 0;
}
#endif
