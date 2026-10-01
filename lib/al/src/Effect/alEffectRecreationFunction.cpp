#ifdef NON_MATCHING
#include <Effect/alEffectCreationFields.h>

using namespace effect_creation_detail;
extern "C" void fn_0024DD4C(void*);

// Address-only identity; original class/member spelling remains unknown.
extern "C" void fn_002E4E18(Context* context, Set* set,
    int bankIndex, int resourceIndex, unsigned char group)
{
    sead::Random* random = dat_003E26DC.mInstance;
    int firstUnused = set->mEmitterCount;
    int fillCount = 8 - firstUnused;
    for (int index = 0; index < fillCount; ++index)
        set->mEmitterParameters[firstUnused + index] = set->mEmitterParameters[0];

    Set saved(*set);
    fn_0024DD4C(set);
    *set = saved;
    set->mLiveEmitterCount = 0;
    set->mFlag1CF = 0;
    int emitterCount = context->mBanks[bankIndex]->mSets[resourceIndex].mEmitterCount;
    if (emitterCount > context->mFreeEmitterCount)
        return;

    for (int index = emitterCount - 1; index >= 0; --index)
    {
        int probes = 0;
        Emitter* emitter = 0;
        for (;;)
        {
            context->mEmitterCursor = (context->mEmitterCursor + 1) & context->mEmitterMask;
            if (!context->mEmitters[context->mEmitterCursor].mImplementation)
            {
                emitter = &context->mEmitters[context->mEmitterCursor];
                break;
            }
            if (++probes >= context->mEmitterCapacity)
                break;
        }
        if (!emitter)
        {
            fn_002200FC(context, "\x83\x47\x83\x7e\x83\x62\x83\x5e\x82\xaa\x8c\xcd\x8a\x89\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n");
            break;
        }
        set->mEmitters[set->mLiveEmitterCount++] = emitter;
        EmitterParameters* parameters = &set->mEmitterParameters[index];
        emitter->mParameters = parameters;
        emitter->mSet = set;
        emitter->mIdentifier = set->mIdentifier;
        emitter->mValue84 = 1.0f;
        if (context->mGroupHeads[group])
        {
            context->mGroupHeads[group]->mPrevious = emitter;
            emitter->mNext = context->mGroupHeads[group];
            context->mGroupHeads[group] = emitter;
        }
        else
        {
            context->mGroupHeads[group] = emitter;
            emitter->mNext = 0;
        }
        emitter->mPrevious = 0;
        --context->mFreeEmitterCount;
        emitter->mResource = context->mBanks[bankIndex]->mSets[resourceIndex].mMembers[index].mResource;
        emitter->mValue00 = 0;
        emitter->mValue88 = -1;
        emitter->mValue8C = 0;
        emitter->mValue90 = 0;
        emitter->mValue94 = 0;
        unsigned int seed = emitter->mResource->mRandomSeed;
        if (seed != 0)
        {
            emitter->mRandom.mValue0 = static_cast<unsigned short>(seed);
            emitter->mRandom.mValue2 = static_cast<unsigned short>(seed >> 16);
            emitter->mRandom.mState = seed;
        }
        else
        {
            RandomState* state = &emitter->mRandom;
            unsigned int generated = random->getU32();
            state->mValue0 = static_cast<unsigned short>(generated);
            state->mState = generated;
            state->mValue2 = static_cast<unsigned short>(generated >> 16);
        }
        if (emitter->mResource->mDeletionFilter == 0x7fffffffU)
            set->mFlag1CF = 1;
        emitter->mListAC = 0;
        emitter->mListB0 = 0;
        emitter->mGroup = group;
        emitter->mImplementation = context->mImplementations[emitter->mResource->mImplementationIndex];
        unsigned int state = emitter->mRandom.mState;
        int range = emitter->mResource->mRandomRange;
        emitter->mRandom.mState = state * 0x41c64e6dU + 0x3039U;
        int scaled = static_cast<int>((static_cast<long long>(state) * range) >> 32);
        emitter->mValue04 = 0x7fffffff;
        emitter->mValue80 = emitter->mResource->mValue78 - scaled;
    }
    set->mEmitterCount = set->mLiveEmitterCount;
    ++context->mLiveSetCount;
}
#endif
