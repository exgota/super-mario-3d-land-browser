// NonMatching complete reconstruction of EU 002ECBD0..002ED470.
// lib/al/791 is a provisional effect-family carrier, not an original module claim.
#ifdef NON_MATCHING
#include <Effect/retail_EmitterChildUpdate.h>
#include <math/seadVector.h>

extern "C" void fn_002ECBD0(void*, emitter2ecbd0::State* state, emitter2ecbd0::Particle* parent)
{
    using namespace emitter2ecbd0;
    int age = parent->frame - 1;
    Resource* resource = state->resource;
    ChildConfig* child = &resource->child;
    if (resource->shape == 5) {
        TrailConfig* config = reinterpret_cast<TrailConfig*>(reinterpret_cast<Byte*>(resource) + resource->trailOffset);
        if (age == 0)
            parent->trail = fn_002E5984(dat_003EF914, state, parent);
        Trail* trail = parent->trail;
        if (trail) {
            TrailSample* current = &trail->samples[trail->end];
            if (resource->trailFlags & 1) {
                current->position = parent->position;
                current->matrix.copy(dat_00430A88);
            } else {
                current->position = parent->localPosition;
                current->matrix.copy(state->matrix);
            }
            current->width = parent->scale.x * state->owner->sizeX;
            if (trail->end != trail->begin) {
                int previousIndex = trail->end - 1;
                if (previousIndex < 0)
                    previousIndex = config->capacity - 1;
                TrailSample* previous = &trail->samples[previousIndex];
                if (age < 2) {
                    trail->direction = current->position - previous->position;
                    trail->direction.normalize();
                } else {
                    Vec3 movement = current->position - previous->position;
                    movement.normalize();
                    trail->direction.add((movement - trail->direction) * config->smoothing);
                    trail->direction.normalize();
                }
                if (config->orientation == 2) {
                    current->direction.x = current->matrix.m[0][1] * previous->width;
                    current->direction.y = current->matrix.m[1][1] * previous->width;
                    current->direction.z = current->matrix.m[2][1] * previous->width;
                } else if (config->orientation == 1) {
                    Vec3 up;
                    up.x = current->matrix.m[0][1];
                    up.y = current->matrix.m[1][1];
                    up.z = current->matrix.m[2][1];
                    Vec3 direction = trail->direction;
                    Vec3 cross;
                    _ZN4sead14Vector3CalcCtrIfE5crossERN2nn4math4VEC3ERKS4_S7_(cross, up, direction);
                    cross.normalize();
                    _ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f(cross, cross, previous->width);
                    current->direction = cross;
                } else {
                    current->direction = trail->direction;
                }
            }
            if (++trail->end >= config->capacity)
                trail->end = 0;
            if (trail->end == trail->begin && ++trail->begin >= config->capacity)
                trail->begin = 0;
            ++trail->count;
            if (trail->count > config->capacity)
                trail->count = config->capacity;
            trail->texture = parent->texture;
            trail->color = state->owner->color;
            trail->alpha = (parent->alpha * state->owner->alpha) * state->fade;
            trail->matrix.copy(state->matrix);
        }
    }
    if (!(resource->childFlags & 1))
        return;
    if (age == 0)
        parent->childTimer = 0x7FFFFFFF;
    if (age < ((parent->life - 1) * child->startPercent) / 100)
        return;
    if (parent->childTimer < child->spacing) {
        ++parent->childTimer;
        return;
    }
    parent->childTimer = 0;
    for (int i = 0; i < child->count; ++i) {
        Particle* particle = fn_0021FFCC(dat_003EF914);
        if (!particle)
            continue;
        particle->resource = resource;
        unsigned flags = resource->childFlags;
        int life = child->life;
        particle->life = life;
        particle->trail = 0;
        if (flags & 0x20) {
            particle->velocity.x = parent->emissionVelocity.x * child->velocityScale;
            particle->velocity.y = parent->emissionVelocity.y * child->velocityScale;
            particle->velocity.z = parent->emissionVelocity.z * child->velocityScale;
        } else {
            particle->velocity.x = sead::Vector3f::zero.x;
            particle->velocity.y = sead::Vector3f::zero.y;
            particle->velocity.z = sead::Vector3f::zero.z;
        }
        float alpha = flags & 4 ? parent->alpha * parent->alphaScale : child->alpha;
        if (flags & 8) {
            particle->scale.x = (parent->scale.x * child->inheritScale) * parent->sizeScale;
            particle->scale.y = (parent->scale.y * child->inheritScale) * parent->sizeScale;
        } else {
            particle->scale = child->scale;
        }
        if (flags & 0x10) {
            particle->angles = parent->angles;
        } else {
            particle->angles.x = child->angles.x + randomByte(state) * child->angleRange.x;
            particle->angles.y = child->angles.y + randomByte(state) * child->angleRange.y;
            particle->angles.z = child->angles.z + randomByte(state) * child->angleRange.z;
        }
        particle->color = flags & 2 ? parent->color : child->color;
        particle->angularVelocity.x = child->angularVelocity.x + randomByte(state) * child->angularRange.x;
        particle->angularVelocity.y = child->angularVelocity.y + randomByte(state) * child->angularRange.y;
        particle->angularVelocity.z = child->angularVelocity.z + randomByte(state) * child->angularRange.z;
        if (child->alphaStartTime == 0) {
            particle->alpha = alpha;
            particle->alphaBeginStep = 0.0f;
        } else {
            particle->alpha = child->startAlpha;
            particle->alphaBeginStep = (alpha - child->startAlpha) / (float)child->alphaStartTime;
        }
        particle->alphaEndStep = (child->endAlpha - alpha) * (1.0f / (float)(life - child->alphaEndTime));
        float scaleTime = 1.0f / (float)(life - child->scaleEndTime);
        particle->scaleStep.x = particle->scale.x * ((child->endScale.x - 1.0f) * scaleTime);
        particle->scaleStep.y = particle->scale.y * ((child->endScale.y - 1.0f) * scaleTime);
        particle->texture.x = 0.0f;
        particle->texture.y = 0.0f;
        particle->texture.z = child->textureStep.x;
        particle->texture.w = child->textureStep.y;
        const nn::math::VEC3* randomDirection = &dat_003EFA18[state->randomIndexB++ & 0x1FF];
        const nn::math::VEC3* randomOffset = &dat_003EFA14[state->randomIndexA++ & 0x1FF];
        particle->velocity.x = (randomDirection->x * child->randomVelocity + randomOffset->x * child->randomX) + particle->velocity.x;
        particle->velocity.y = (randomDirection->y * child->randomVelocity + randomOffset->y * child->randomY) + particle->velocity.y;
        particle->velocity.z = (randomDirection->z * child->randomVelocity + randomOffset->z * child->randomZ) + particle->velocity.z;
        particle->frame = 0;
        particle->position.x = parent->position.x + randomDirection->x * child->randomPosition;
        particle->position.y = parent->position.y + randomDirection->y * child->randomPosition;
        particle->position.z = parent->position.z + randomDirection->z * child->randomPosition;
        particle->seed = state->random;
        state->random = state->random * 0x41C64E6DU + 12345U;
        if (!(resource->childFlags & 0x40)) {
            if (resource->useEmitterMatrices) {
                particle->rotation.copy(state->rotation);
                particle->matrix.copy(state->matrix);
            } else {
                particle->rotation.copy(parent->rotation);
                particle->matrix.copy(parent->matrix);
            }
        }
        if (!state->children) {
            state->children = particle;
            particle->previous = 0;
        } else {
            state->children->next = particle;
            particle->previous = state->children;
            state->children = particle;
        }
        particle->next = 0;
    }
}
#endif
