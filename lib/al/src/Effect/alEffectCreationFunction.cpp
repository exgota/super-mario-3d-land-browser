#ifdef NON_MATCHING
#include <Effect/alEffectCreationFields.h>

using namespace effect_creation_detail;

// First semantic form: preserve the shared allocation-probe counter and the
// descending emitter traversal seen in the retail routine. No compile yet.
extern "C" bool fn_002E54AC(Context* context, Reference* reference,
    const sead::Matrix34f* matrix, int resourceIndex, int bankIndex,
    unsigned char group, unsigned int selectionMask)
{
    sead::Random* random = dat_003E26DC.mInstance;
    int emitterCount = context->mBanks[bankIndex]->mSets[resourceIndex].mEmitterCount;
    if (context->mFreeEmitterCount < emitterCount)
    {
        fn_002200FC(context, "\x83\x47\x83\x7e\x83\x62\x83\x5e\x82\xaa\x8c\xcd\x8a\x89\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n");
        return false;
    }

    int probes = 0;
    Set* set = 0;
    for (;;)
    {
        context->mSetCursor = (context->mSetCursor + 1) & context->mSetMask;
        if (context->mSets[context->mSetCursor].mLiveEmitterCount == 0)
        {
            set = &context->mSets[context->mSetCursor];
            break;
        }
        if (++probes >= context->mSetCapacity)
            break;
    }
    reference->mSet = set;
    if (!set)
    {
        fn_002200FC(context, "\x83\x47\x83\x7e\x83\x62\x83\x5e\x83\x5a\x83\x62\x83\x67\x82\xaa\x8c\xcd\x8a\x89\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n");
        return false;
    }

    sead::Matrix34CalcCtr<float>::copy(
        reinterpret_cast<nn::math::MTX34&>(set->mMatrixFC),
        reinterpret_cast<const nn::math::MTX34&>(*matrix));
    sead::Matrix34CalcCtr<float>::copy(
        reinterpret_cast<nn::math::MTX34&>(set->mMatrix12C),
        reinterpret_cast<const nn::math::MTX34&>(*matrix));
    set->mVector15C.x = sead::Vector3f::ones.x;
    set->mVector15C.y = sead::Vector3f::ones.y;
    set->mVector15C.z = sead::Vector3f::ones.z;
    set->mVector180.x = sead::Vector3f::ones.x;
    set->mVector180.y = sead::Vector3f::ones.y;
    set->mVector180.z = sead::Vector3f::ones.z;
    set->mVector170.x = sead::Vector3f::ones.x;
    set->mVector170.y = sead::Vector3f::ones.y;
    set->mVector178.x = 1.0f;
    set->mVector178.y = 1.0f;
    set->mValue1C0 = 0;
    set->mVector168.y = 1.0f;
    set->mVector168.x = 1.0f;
    set->mValues18C[3] = 1.0f;
    set->mValues18C[2] = 1.0f;
    set->mValues18C[1] = 1.0f;
    set->mValues18C[0] = 1.0f;
    set->mVector19C.z = 1.0f;
    set->mVector19C.y = 1.0f;
    set->mVector19C.x = 1.0f;
    set->mVector1A8.z = 0.0f;
    set->mVector1A8.y = 0.0f;
    set->mVector1A8.x = 0.0f;
    set->mFlag1CD = 0;
    set->mFlag1CC = 0;
    set->mFlag1CE = 0;
    set->mFlag1CF = 0;
    set->mValue1C4 = 0;
    set->mIdentifier = context->mNextIdentifier;
    set->mBankIndex = bankIndex;
    set->mResourceIndex = resourceIndex;
    reference->mIdentifier = context->mNextIdentifier;

    for (int index = emitterCount - 1; index >= 0; --index)
    {
        if (!(selectionMask & (1U << index)))
            continue;
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
        parameters->mValue0 = 0x100;
        parameters->mValue4 = 0x100;
        parameters->mValue8 = 0x100;
        parameters->mValueC = 0;
        parameters->mValue10 = 0;
        parameters->mValue14 = 0;
        emitter->mIdentifier = context->mNextIdentifier;
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
    ++context->mNextIdentifier;
    return true;
}

#endif
