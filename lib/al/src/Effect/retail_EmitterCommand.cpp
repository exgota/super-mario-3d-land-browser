// Retail emitter command update 002EEB70..002EF5C8.
// NonMatching clean-room reconstruction from the owner's EU executable.
// Neutral names describe observed fields, not recovered original API identities.
// lib/al is the existing effect build carrier; original library/compiler is unproven.
#ifdef NON_MATCHING

namespace emitter2eeb70 {
struct Vec3 { float x, y, z; };
struct Vec4 { float x, y, z, w; };
struct Matrix34 { float m[12]; };
struct Resource {
    unsigned char unknown000[0x4];
    unsigned flags; // +0x4
    unsigned seed; // +0x8
    unsigned char unknown00C[0x23];
    unsigned char rotateVelocity; // +0x2F
    unsigned char unknown030[0x3];
    unsigned char captureStop; // +0x33
    unsigned char unknown034[0x18];
    float speed; // +0x4C
    Vec3 direction; // +0x50
    float directionScale; // +0x5C
    float directionW; // +0x60
    Vec3 constantVector; // +0x64
    int start; // +0x70
    int duration; // +0x74
    int spacing; // +0x78
    unsigned char unknown07C[0x4];
    int batch; // +0x80
    int life; // +0x84
    int ratio; // +0x88
    float constantW; // +0x8C
    unsigned char unknown090[0xc];
    Vec3 velocity; // +0x9C
    Vec3 color; // +0xA8
    unsigned char unknown0B4[0x4];
    Vec3 middleColor; // +0xB8
    unsigned char unknown0C4[0x4];
    Vec3 endColor; // +0xC8
    unsigned char unknown0D4[0x10];
    int colorStart; // +0xE4
    int colorMiddle; // +0xE8
    int colorEnd; // +0xEC
    int colorCycles; // +0xF0
    float alpha; // +0xF4
    float alphaStart; // +0xF8
    float alphaEnd; // +0xFC
    int alphaStartTime; // +0x100
    int alphaEndTime; // +0x104
    float scaleX; // +0x108
    float scaleY; // +0x10C
    float scaleBegin; // +0x110
    unsigned char unknown114[0x4];
    float scaleEnd; // +0x118
    unsigned char unknown11C[0x4];
    int scaleBeginTime; // +0x120
    int scaleEndTime; // +0x124
    float scaleZ; // +0x128
    int shape; // +0x12C
    unsigned char unknown130[0x10];
    unsigned angle0; // +0x140
    unsigned char unknown144[0x8];
    unsigned angle1; // +0x14C
    unsigned char unknown150[0x8];
    unsigned angle2; // +0x158
    unsigned char unknown15C[0x8];
    unsigned angle3; // +0x164
    unsigned char unknown168[0x68];
    float fadeStep; // +0x1D0
};
struct Owner {
    unsigned char unknown000[0xfc];
    Matrix34 baseMatrix; // +0xFC
    Matrix34 worldMatrix; // +0x12C
    unsigned char unknown15C[0x14];
    float scaleX; // +0x170
    float scaleY; // +0x174
    float sizeX; // +0x178
    float sizeY; // +0x17C
    unsigned char unknown180[0x1c];
    float speed; // +0x19C
    float directionScale; // +0x1A0
    float directionW; // +0x1A4
    Vec3 wind; // +0x1A8
    Vec3 direction; // +0x1B4
    unsigned char unknown1C0[0xc];
    unsigned char fading; // +0x1CC
    unsigned char overrideDirection; // +0x1CD
};
struct State {
    int frame; // +0x0
    unsigned char unknown004[0x8];
    Owner* owner; // +0xC
    unsigned char unknown010[0x8];
    Matrix34 rotation; // +0x18
    Matrix34 matrix; // +0x48
    unsigned char unknown078[0x4];
    unsigned random; // +0x7C
    unsigned char unknown080[0x4];
    float fade; // +0x84
    int stop; // +0x88
    int phase; // +0x8C
    int fadeFrames; // +0x90
    int time; // +0x94
    unsigned randomIndex; // +0x98
    unsigned char unknown09C[0xc];
    Resource* resource; // +0xA8
};
union FloatBits { float value; int bits; };
inline int signedBits(float value) { FloatBits b; b.value = value; return b.bits; }
}

extern "C" void* dat_003EF914;
extern "C" void* dat_003EF918;
extern "C" float dat_003EF9C4;
extern "C" void fn_002201C4(void*, emitter2eeb70::State*);
extern "C" unsigned* fn_002EE878(void*, unsigned*, emitter2eeb70::State*);
extern "C" unsigned* fn_002F0018(void*, unsigned*);
extern "C" unsigned* fn_002F00DC(void*, unsigned*);
extern "C" unsigned* fn_002EFC44(void*, unsigned*, const emitter2eeb70::Matrix34*,
    const emitter2eeb70::Vec4*, const emitter2eeb70::Vec4*, const emitter2eeb70::Vec4*,
    const emitter2eeb70::Vec4*, const emitter2eeb70::Vec4*, const emitter2eeb70::Vec4*,
    const emitter2eeb70::Vec4*, const emitter2eeb70::Vec4*, const emitter2eeb70::Vec4*,
    const emitter2eeb70::Vec4*, const emitter2eeb70::Vec4*, const emitter2eeb70::Vec4*,
    int, int, unsigned);

extern "C" unsigned* fn_002EEB70(void* receiver, unsigned* commands,
    emitter2eeb70::State* state, int* countOut)
{
    using namespace emitter2eeb70;
    Owner* owner = state->owner;
    Resource* resource = state->resource;
    if (state->frame == 0) {
        unsigned seed = resource->seed;
        state->randomIndex = seed;
        if (seed == 0) {
            seed = state->random;
            state->randomIndex = seed;
            state->random = seed * 0x41c64e6dU + 12345U;
        }
        state->randomIndex = seed % 3;
    }
    if (owner->fading) {
        if (state->stop == -1 && resource->captureStop) {
            state->stop = resource->duration == 0x7fffffff ? state->time : state->frame;
            state->fadeFrames = resource->life;
        }
        state->fade -= resource->fadeStep;
        --state->fadeFrames;
        if (state->fade <= 0.0f || (resource->fadeStep == 0.0f && state->fadeFrames < 0)) {
            fn_002201C4(dat_003EF914, state);
            *countOut = 0;
            return commands;
        }
    } else {
        state->fade += resource->fadeStep;
        if (signedBits(state->fade) > 0x3f800000)
            state->fade = 1.0f;
    }
    ++state->time;
    ++state->frame;
    commands = fn_002EE878(receiver, commands, state);
    int count = 0;
    int duration = resource->duration;
    int limit;
    int half;
    int cycle;
    if (duration != 0x7fffffff) {
        half = duration;
        limit = duration;
        if (state->stop != -1 && limit > state->stop)
            limit = state->stop;
        cycle = resource->start + duration + resource->life;
        if (state->frame > cycle) {
            fn_002201C4(dat_003EF914, state);
            *countOut = 0;
            return commands;
        }
    } else {
        int period = resource->spacing;
        int life = resource->life;
        int raw = period * ((2 * life) / period + 2);
        if (raw < life)
            raw = life;
        limit = raw - 1;
        half = limit;
        cycle = 2 * raw - 2;
        if ((state->phase == 1 || state->phase == 2) && limit > state->stop)
            limit = state->stop;
    }
    int step = resource->spacing + 1;
    if (step > limit)
        step = limit;
    int relative;
    int active;
    if (duration != 0x7fffffff) {
        relative = state->time - resource->start;
        active = relative;
        if (active > limit - resource->start)
            active = limit - resource->start;
    } else {
        relative = state->time;
        active = relative;
        if (active > limit)
            active = limit;
    }
    int groups = active / step;
    if (active >= 0)
        ++groups;
    int end = resource->batch * groups;
    int expired = active - resource->life;
    if (expired < 0)
        expired = 0;
    int begin = resource->batch * (expired / step);
    Vec4 timing = { (float)relative, (float)resource->life, (float)step, 1.0f / (float)resource->batch };
    Vec4 size = {
        (resource->scaleX * owner->sizeX) * (owner->scaleX * 288.0f),
        (resource->scaleY * owner->sizeY) * (owner->scaleY * 288.0f),
        resource->scaleZ,
        resource->speed * state->owner->speed
    };
    Vec4 sizeChange = {
        (float)resource->scaleBeginTime * 0.01f,
        (float)resource->scaleEndTime * 0.01f,
        (resource->scaleBegin * owner->scaleX) * (owner->sizeX * 288.0f),
        (resource->scaleEnd * owner->scaleX) * (owner->sizeX * 288.0f)
    };
    float directionScale = resource->directionScale * owner->directionScale;
    Vec4 direction;
    if (owner->overrideDirection) {
        direction.x = owner->direction.x * directionScale;
        direction.y = owner->direction.y * directionScale;
        direction.z = owner->direction.z * directionScale;
        direction.w = resource->directionW * owner->directionW;
    } else {
        direction.x = resource->direction.x * directionScale;
        direction.y = resource->direction.y * directionScale;
        direction.z = resource->direction.z * directionScale;
        direction.w = resource->directionW * owner->directionW;
    }
    direction.x = (direction.x + owner->wind.x * owner->worldMatrix.m[0]) +
        (owner->wind.y * owner->worldMatrix.m[4] + owner->wind.z * owner->worldMatrix.m[8]);
    direction.y = (direction.y + owner->wind.x * owner->worldMatrix.m[1]) +
        (owner->wind.y * owner->worldMatrix.m[5] + owner->wind.z * owner->worldMatrix.m[9]);
    direction.z = (direction.z + owner->wind.x * owner->worldMatrix.m[2]) +
        (owner->wind.y * owner->worldMatrix.m[6] + owner->wind.z * owner->worldMatrix.m[10]);
    Vec4 constant = { resource->constantVector.x, resource->constantVector.y, resource->constantVector.z, resource->constantW };
    Vec4 velocity;
    if (resource->rotateVelocity) {
        velocity.x = ((resource->velocity.x * state->rotation.m[0] + resource->velocity.y * state->rotation.m[4]) +
            resource->velocity.z * state->rotation.m[8]) * dat_003EF9C4;
        velocity.y = ((resource->velocity.x * state->rotation.m[1] + resource->velocity.y * state->rotation.m[5]) +
            resource->velocity.z * state->rotation.m[9]) * dat_003EF9C4;
        velocity.z = ((resource->velocity.x * state->rotation.m[2] + resource->velocity.y * state->rotation.m[6]) +
            resource->velocity.z * state->rotation.m[10]) * dat_003EF9C4;
    } else {
        velocity.x = resource->velocity.x * dat_003EF9C4;
        velocity.y = resource->velocity.y * dat_003EF9C4;
        velocity.z = resource->velocity.z * dat_003EF9C4;
    }
    velocity.w = (float)resource->ratio / (float)resource->life;
    Vec4 color = { resource->color.x, resource->color.y, resource->color.z, resource->alpha * state->fade };
    Vec4 middleColor;
    Vec4 endColor;
    if (resource->flags & 0x100) {
        middleColor.x = resource->middleColor.x;
        middleColor.y = resource->middleColor.y;
        middleColor.z = resource->middleColor.z;
        middleColor.w = (float)resource->colorMiddle * 0.01f;
        endColor.x = resource->endColor.x;
        endColor.y = resource->endColor.y;
        endColor.z = resource->endColor.z;
        endColor.w = 1.0f / ((float)(resource->colorEnd - resource->colorMiddle) * 0.01f);
    } else {
        middleColor.x = resource->color.x;
        middleColor.y = resource->color.y;
        middleColor.z = resource->color.z;
        middleColor.w = (float)resource->colorMiddle * 0.01f;
        endColor.x = resource->color.x;
        endColor.y = resource->color.y;
        endColor.z = resource->color.z;
        endColor.w = 1.0f / ((float)(resource->colorEnd - resource->colorMiddle) * 0.01f);
    }
    Vec4 colorTiming = {
        (float)resource->colorStart * 0.01f,
        1.0f / ((float)(resource->colorMiddle - resource->colorStart) * 0.01f),
        (float)resource->colorCycles, 0.0f
    };
    Vec4 alphaTiming = {
        (float)resource->alphaStartTime * 0.01f,
        (float)resource->alphaEndTime * 0.01f,
        resource->alphaStart * state->fade,
        resource->alphaEnd * state->fade
    };
    if (signedBits(alphaTiming.x) < 0x3ca3d70a) {
        color.w += alphaTiming.z;
        alphaTiming.x = 0.0f;
        alphaTiming.z = 0.0f;
    }
    Vec4 angles;
    if (resource->shape == 3 || resource->shape == 8 || resource->shape == 13) {
        // Materialize the positive factor: fast-mode constant folding of its
        // negation changes VNMUL rounding under directed FPSCR modes.
        volatile float scale = 1.462918119976564e-9f;
        angles.x = -((float)resource->angle0 * scale);
        angles.y = ((float)resource->angle1 * scale) * -255.0f;
        angles.z = -((float)resource->angle2 * scale);
        angles.w = ((float)resource->angle3 * scale) * -255.0f;
    } else {
        angles.x = 0.0f;
        angles.y = 0.0f;
        angles.z = 0.0f;
        angles.w = 0.0f;
    }
    if (resource->duration == 0x7fffffff && state->stop != -1) {
        switch (state->phase) {
            case 0: state->phase = state->stop < half ? 1 : 3; break;
            case 1: if (state->time > half) state->phase = 2; break;
            case 3: if (state->time < half) state->phase = 4; break;
        }
    }
    if (end - begin > 0) {
        count = end - begin;
        commands = fn_002F0018(dat_003EF918, commands);
        if (state->phase != 4)
            commands = fn_002EFC44(dat_003EF918, commands, &state->matrix, &timing,
                &size, &sizeChange, &direction, &constant, &velocity, &color,
                &middleColor, &endColor, &colorTiming, &alphaTiming, &angles,
                end, begin, state->randomIndex);
        if (resource->duration == 0x7fffffff && state->frame > half && state->phase != 2) {
            if (timing.x <= (float)half)
                timing.x += (float)half;
            else
                timing.x -= (float)half;
            if (state->phase == 3 || state->phase == 4)
                half = state->stop - half;
            int relative2 = (int)timing.x;
            if (relative2 > half)
                relative2 = half;
            int groups2 = relative2 / step;
            if (relative2 >= 0)
                ++groups2;
            int end2 = resource->batch * groups2;
            count = (resource->batch * (half / step)) / 2;
            if (end2 >= 1)
                commands = fn_002EFC44(dat_003EF918, commands, &state->matrix, &timing,
                    &size, &sizeChange, &direction, &constant, &velocity, &color,
                    &middleColor, &endColor, &colorTiming, &alphaTiming, &angles,
                    end2, 0, state->randomIndex);
            if (state->time > cycle)
                state->time -= cycle;
        }
        commands = fn_002F00DC(dat_003EF918, commands);
    }
    *countOut = count;
    return commands;
}
#endif
