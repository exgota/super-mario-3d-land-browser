#pragma clang diagnostic push
// The sealed generated header defines vfp_vector even when this host wrapper
// does not invoke it. Keep strict warnings on the verification implementation.
#pragma clang diagnostic ignored "-Wunused-function"
#include "NativeTiming.h"
#pragma clang diagnostic pop
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern const uint32_t recomp_abi, recomp_entry_count;
extern const Entry recomp_entries[];
extern RECOMP_OVERRIDE(0x0010766C);
_Static_assert(sizeof(void*) == 4, "wasm32 pointers");
_Static_assert(sizeof(Context) == 120, "translated wasm32 context ABI");
_Static_assert(sizeof(Entry) == 8, "translated wasm32 entry ABI");
static Code Lookup(Context* context, uint32_t address) {
    (void)context;
    uint32_t low = 0, high = recomp_entry_count;
    while (low < high) {
        uint32_t middle = low + (high - low) / 2;
        if (recomp_entries[middle].address < address) low = middle + 1;
        else high = middle;
    }
    return low < recomp_entry_count && recomp_entries[low].address == address ? recomp_entries[low].code : NULL;
}
static void RefuseHostCallback(void) { fprintf(stderr, "Unsupported host callback during instruction verification\n"); abort(); }
static uint8_t Read8(Context* c, uint32_t a) { (void)c; (void)a; RefuseHostCallback(); return 0; }
static uint16_t Read16(Context* c, uint32_t a) { (void)c; (void)a; RefuseHostCallback(); return 0; }
static uint32_t Read32(Context* c, uint32_t a) { (void)c; (void)a; RefuseHostCallback(); return 0; }
static void Write8(Context* c, uint32_t a, uint8_t v) { (void)c; (void)a; (void)v; RefuseHostCallback(); }
static void Write16(Context* c, uint32_t a, uint16_t v) { (void)c; (void)a; (void)v; RefuseHostCallback(); }
static void Write32(Context* c, uint32_t a, uint32_t v) { (void)c; (void)a; (void)v; RefuseHostCallback(); }
static void Interpret(Context* c, uint32_t a, uint32_t o) { (void)c; (void)a; (void)o; RefuseHostCallback(); }
static const Host host = {Read8, Read16, Read32, Write8, Write16, Write32, Interpret, Lookup};
static uint32_t ReadUnsigned32(const unsigned char* p) {
    return (uint32_t)p[0] | (uint32_t)p[1] << 8 | (uint32_t)p[2] << 16 | (uint32_t)p[3] << 24;
}
static void WriteUnsigned32(unsigned char* p, uint32_t value) {
    for (unsigned i = 0; i < 4; ++i) p[i] = (unsigned char)(value >> (i * 8));
}
int main(int argc, char** argv) {
    if (argc != 4 || recomp_abi != 4 || native_timing_revision != 2 || !recomp_entry_count || native_block_timing_callback) return 2;
    if (Lookup(NULL, 0x0010766Cu) != override_0x0010766C) return 3;
    FILE* addresses = fopen(argv[3], "wbx");
    if (!addresses) return 4;
    for (uint32_t index = 0; index < recomp_entry_count; ++index) {
        if (!recomp_entries[index].code || (index && recomp_entries[index-1].address >= recomp_entries[index].address)) return 3;
        unsigned char address[4];
        WriteUnsigned32(address, recomp_entries[index].address);
        if (fwrite(address, 1, sizeof(address), addresses) != sizeof(address)) return 9;
    }
    if (fclose(addresses)) return 10;
    FILE* input = fopen(argv[1], "rb");
    FILE* output = fopen(argv[2], "wbx");
    if (!input || !output) return 4;
    uint8_t** pages = calloc(1u << 20, sizeof(uint8_t*));
    if (!pages) return 5;
    unsigned char request[208], result[200];
    unsigned count = 0;
    size_t length;
    while ((length = fread(request, 1, sizeof(request), input)) != 0) {
        if (length != sizeof(request)) return 6;
        Context context = {0};
        uint32_t vfp[32], fpscr = ReadUnsigned32(request + 204);
        for (unsigned index = 0; index < 16; ++index) context.r[index] = ReadUnsigned32(request + 8 + index * 4);
        uint32_t flags = ReadUnsigned32(request + 72);
        context.n = flags >> 31; context.z = (flags >> 30) & 1; context.c = (flags >> 29) & 1;
        context.v = (flags >> 28) & 1; context.q = (flags >> 27) & 1;
        context.ge = (flags >> 16) & 15; context.thumb = (flags >> 5) & 1;
        for (unsigned index = 0; index < 32; ++index) vfp[index] = ReadUnsigned32(request + 76 + index * 4);
        context.budget = (int32_t)ReadUnsigned32(request + 4);
        context.tls = 0x1FF82000u; context.vfp = vfp; context.fpscr = &fpscr;
        context.read_pages = pages; context.write_pages = pages; context.host = &host;
        Code code = Lookup(&context, ReadUnsigned32(request));
        if (!code) return 7;
        code(&context);
        if (context.budget != 0 || context.exit == EXIT_SVC) return 8;
        for (unsigned index = 0; index < 16; ++index) WriteUnsigned32(result + index * 4, context.r[index]);
        flags = (uint32_t)context.n << 31 | (uint32_t)context.z << 30 | (uint32_t)context.c << 29
              | (uint32_t)context.v << 28 | (uint32_t)context.q << 27 | (uint32_t)context.ge << 16 | (uint32_t)context.thumb << 5;
        WriteUnsigned32(result + 64, flags);
        for (unsigned index = 0; index < 32; ++index) WriteUnsigned32(result + 68 + index * 4, vfp[index]);
        WriteUnsigned32(result + 196, fpscr);
        if (fwrite(result, 1, sizeof(result), output) != sizeof(result)) return 9;
        ++count;
    }
    free(pages);
    if (ferror(input) || fclose(input) || fclose(output)) return 10;
    printf("{\"abi_revision\":%u,\"timing_revision\":%u,\"actual_instruction_entries\":%u,\"actual_context_bytes\":%zu,\"completed_cases\":%u,\"source_binding_matches\":1,\"interpreter_calls\":0}\n", recomp_abi,native_timing_revision,recomp_entry_count,sizeof(Context),count);
    return 0;
}
