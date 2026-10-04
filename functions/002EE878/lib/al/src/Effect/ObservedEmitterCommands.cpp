#include <Effect/ObservedEmitterCommands.h>
#include <string.h>
extern "C" u32* fn_002EE878(void*, u32* cursor, observed_emitter_commands::State* state) {
    using namespace observed_emitter_commands;
    Resource* resource = state->resource;
    Owner* owner = state->owner;
    sead::Matrix34CalcCtr<float>::multiply(state->worldMatrix, owner->worldMatrix, resource->worldMatrix);
    sead::Matrix34CalcCtr<float>::multiply(state->baseMatrix, owner->baseMatrix, resource->baseMatrix);
    *cursor++ = 1;
    *cursor++ = 0x000f0111;
    *cursor++ = 1;
    *cursor++ = 0x000f0110;
    float red = resource->color[0] * owner->color[0];
    float green = resource->color[1] * owner->color[1];
    float blue = resource->color[2] * owner->color[2];
    Texture* texture = resource->texture;
    if (texture != dat_003EF91C) {
        *cursor++ = 0x00011001;
        *cursor++ = 0x000f0080;
        *cursor++ = texture->parameterAC;
        *cursor++ = 0x000f0082;
        *cursor++ = texture->parameterA8;
        *cursor++ = 0x000f0083;
        *cursor++ = texture->parameterB0;
        *cursor++ = 0x000f0084;
        *cursor++ = texture->address >> 3;
        *cursor++ = 0x000f0085;
        *cursor++ = texture->format;
        *cursor++ = 0x000f008e;
        dat_003EF91C = resource->texture;
    }
    u32 redByte = static_cast<u32>(red) & 255;
    u32 greenByte = static_cast<u32>(green) & 255;
    u32 blueByte = static_cast<u32>(blue) & 255;
    u32 selector = resource->selector;
    if (selector != dat_003EF921) {
        *cursor++ = dat_003EF9C8[selector];
        *cursor++ = 0x000f0101;
        dat_003EF921 = selector;
    }
    u32 packed = redByte | (greenByte << 8) | (blueByte << 16);
    u32 blend = resource->blend;
    if (blend != dat_003EF922[0]) {
        u32 first = dat_003EF9D8[blend];
        *cursor++ = first;
        *cursor++ = 0x000f0107;
        u32 thirdIndex = resource->blend;
        u32 second = dat_003EF9E4[0][blend];
        *cursor++ = second;
        *cursor++ = 0x00030104;
        u32 third = dat_003EFA08[thirdIndex];
        u32 fourthIndex = resource->blend;
        *cursor++ = third;
        *cursor++ = 0x00010112;
        u32 fourth = dat_003EF9E4[1][fourthIndex];
        u32 fifthIndex = resource->blend;
        *cursor++ = fourth;
        *cursor++ = 0x00010114;
        u32 cachedBlend = resource->blend;
        u32 fifth = dat_003EF9FC[fifthIndex];
        *cursor++ = fifth;
        *cursor++ = 0x00010115;
        dat_003EF922[0] = cachedBlend;
    }
    *cursor++ = 1;
    *cursor++ = 0x000f0111;
    *cursor++ = 1;
    *cursor++ = 0x000f0110;
    u32 selectedTemplate = resource->commandTemplate;
    if (selectedTemplate != dat_003EF920) {
        const CommandTemplate& packet = dat_003F09B4[selectedTemplate];
        for (unsigned i = 0; i < 3; ++i) cursor[i] = packet.prefix[i];
        u32 lastPrefix = packet.prefix[3];
        cursor[4] = packed;
        cursor[3] = lastPrefix;
        for (unsigned i = 0; i < 5; ++i) cursor[5 + i] = packet.suffix[i];
        cursor += sizeof(packet) / sizeof(*cursor);
        dat_003EF920 = selectedTemplate;
    } else {
        *cursor++ = packed;
        *cursor++ = 0x000f00c3;
    }
    if (resource->flags & 0x80) {
        *cursor++ = dat_003EF8C4;
        *cursor++ = 0x000500e0;
        return cursor;
    }
    *cursor++ = 0;
    *cursor++ = 0x000500e0;
    return cursor;
}
