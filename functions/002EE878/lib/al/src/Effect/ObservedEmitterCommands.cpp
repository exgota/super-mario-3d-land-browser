#include <Effect/ObservedEmitterCommands.h>
#include <string.h>
namespace observed_emitter_commands {
static void writePair(u32*& cursor, u32 value, u32 command) {
    *cursor++ = value;
    *cursor++ = command;
}
}
extern "C" u32* fn_002EE878(void*, u32* cursor, observed_emitter_commands::State* state) {
    using namespace observed_emitter_commands;
    Resource* resource = state->resource;
    Owner* owner = state->owner;
    sead::Matrix34CalcCtr<float>::multiply(state->worldMatrix, owner->worldMatrix, resource->worldMatrix);
    sead::Matrix34CalcCtr<float>::multiply(state->baseMatrix, owner->baseMatrix, resource->baseMatrix);
    writePair(cursor, 1, 0x000f0111);
    writePair(cursor, 1, 0x000f0110);
    float red = resource->color[0] * owner->color[0];
    float green = resource->color[1] * owner->color[1];
    float blue = resource->color[2] * owner->color[2];
    Texture* texture = resource->texture;
    if (texture != dat_003EF91C) {
        writePair(cursor, 0x00011001, 0x000f0080);
        writePair(cursor, texture->parameterAC, 0x000f0082);
        writePair(cursor, texture->parameterA8, 0x000f0083);
        writePair(cursor, texture->parameterB0, 0x000f0084);
        writePair(cursor, texture->address >> 3, 0x000f0085);
        writePair(cursor, texture->format, 0x000f008e);
        dat_003EF91C = resource->texture;
    }
    u32 packed = (static_cast<u32>(red) & 255) |
        ((static_cast<u32>(green) & 255) << 8) |
        ((static_cast<u32>(blue) & 255) << 16);
    u32 selector = resource->selector;
    if (selector != dat_003EF921) {
        writePair(cursor, dat_003EF9C8[selector], 0x000f0101);
        dat_003EF921 = selector;
    }
    if (resource->blend != dat_003EF922[0]) {
        writePair(cursor, dat_003EF9D8[resource->blend], 0x000f0107);
        writePair(cursor, dat_003EF9E4[0][resource->blend], 0x00030104);
        writePair(cursor, dat_003EFA08[resource->blend], 0x00010112);
        writePair(cursor, dat_003EF9E4[1][resource->blend], 0x00010114);
        writePair(cursor, dat_003EF9FC[resource->blend], 0x00010115);
        dat_003EF922[0] = resource->blend;
    }
    writePair(cursor, 1, 0x000f0111);
    writePair(cursor, 1, 0x000f0110);
    u32 selectedTemplate = resource->commandTemplate;
    if (selectedTemplate != dat_003EF920) {
        CommandTemplate packet = dat_003F09B4[selectedTemplate];
        packet.color = packed;
        memcpy(cursor, &packet, sizeof(packet));
        cursor += sizeof(packet) / sizeof(*cursor);
        dat_003EF920 = selectedTemplate;
    } else {
        writePair(cursor, packed, 0x000f00c3);
    }
    writePair(cursor, resource->flags & 0x80 ? dat_003EF8C4 : 0, 0x000500e0);
    return cursor;
}
