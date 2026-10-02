#ifndef RETAIL_GRAPHICS_INTEGER_QUERY_H
#define RETAIL_GRAPHICS_INTEGER_QUERY_H

// Observed offsets for 0038F9F0. These are a field-access view, not an SDK class.
// Keep the shared C pointer compatible with the separate graphics proposals.
namespace retail_graphics { struct RenderControl; }
namespace retail_graphics_integer_query {
struct StateView {
    unsigned char opaque000[0x1c];
    int word01c; // 01C
    int vector020[4]; // 020
    unsigned char opaque030[0x4];
    int word034; // 034
    int word038; // 038
    unsigned char flag03c; // 03C
    unsigned char opaque03d[0x3];
    float float040; // 040
    float float044; // 044
    unsigned char opaque048[0x4];
    float range04c[2]; // 04C
    unsigned char flag054; // 054
    unsigned char opaque055[0x3];
    unsigned activeUnit; // 058
    int texture2D[3]; // 05C
    int textureCube[3]; // 068
    int auxiliary[32]; // 074
    int bindingSet; // 0F4
    unsigned char opaque0f8[0x410];
    int word508; // 508
    int word50c; // 50C
    int word510; // 510
    int vector514[4]; // 514
    unsigned char opaque524[0x8];
    int word52c; // 52C
    int word530; // 530
    int word534; // 534
    int word538; // 538
    int word53c; // 53C
    int word540; // 540
    int word544; // 544
    int word548; // 548
    int word54c; // 54C
    int word550; // 550
    int word554; // 554
    int word558; // 558
    int word55c; // 55C
    float normalized560[4]; // 560
    int word570; // 570
    unsigned char opaque574[0x4];
    unsigned char flag578; // 578
    unsigned char opaque579[0x1];
    unsigned char flag57a; // 57A
    unsigned char flag57b; // 57B
    unsigned char flag57c; // 57C
    unsigned char flag57d; // 57D
    unsigned char opaque57e[0x2];
    int word580; // 580
    unsigned char flags584[4]; // 584
    unsigned char flag588; // 588
    unsigned char opaque589[0x3];
    int word58c; // 58C
    float normalized590[4]; // 590
    float float5a0; // 5A0
    int word5a4; // 5A4
    int word5a8; // 5A8
    unsigned char opaque5ac[0x17];
    unsigned char flag5c3; // 5C3
};
}
extern "C" void fn_0038F9F0(unsigned query, int* output);
#endif
