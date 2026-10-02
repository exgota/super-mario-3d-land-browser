// Clean-room reconstruction from the owner's EU executable.
// Address identity only: these local layouts do not claim an original SDK API.
#ifdef NON_MATCHING
namespace sound_start_22b6e8
{
struct Link { Link* next; Link* previous; };
struct List { unsigned count; Link sentinel; };
struct Sound;
struct SoundVtable
{
        void* reserved[3];
        void (*initialize)(Sound*);
        void (*release)(Sound*);
};
struct Sound
{
        const SoundVtable* vtable;
        unsigned char opaque04[0x10];
        void* actor;
        unsigned char opaque18[0x38];
        int priorityOffset;
        unsigned char opaque54[0x44];
        unsigned char priority;
        unsigned char opaque99[3];
        unsigned id;
        unsigned char opaqueA0[0x34];
        Link node;
};
template<int Kind> struct TypedSound : Sound {};
template<class T> struct Pool { List active; List free; };
struct PlayerSlot { unsigned char opaque[0x48]; };
struct Player
{
        void* opaque00;
        void* archive;
        unsigned char opaque08[0x1c];
        PlayerSlot* slots;
        Pool<TypedSound<1> > sequence;
        Pool<TypedSound<3> > wave;
        Pool<TypedSound<2> > stream;
        unsigned char opaque70[0x10];
        unsigned char streamBuffer[1];
};
struct Handle { Sound* sound; };
struct ActorSlot { unsigned char opaque[0x10]; };
struct Actor { unsigned char opaque[8]; ActorSlot slots[4]; };
struct SoundInfo
{
        unsigned fileId;
        unsigned playerId;
        unsigned actorPlayerId;
        int priority;
        int volume;
        unsigned char parameter20;
        unsigned char parameter21;
        signed char parameter22;
};
struct SequenceOverride { const void* data; const char* label; unsigned banks[4]; };
struct StartOptions
{
        unsigned flags;
        unsigned char offsetType;
        unsigned char padding[3];
        unsigned offset;
        unsigned playerId;
        int priority;
        unsigned actorPlayerId;
        SequenceOverride sequence;
};
struct SequenceInfo
{
        unsigned offset;
        unsigned banks[4];
        unsigned parameter20;
        unsigned char parameter24;
        signed char parameter25;
};
struct WaveInfo
{
        unsigned index;
        unsigned parameter4;
        unsigned char parameter8;
        signed char parameter9;
};
struct StreamInfo { unsigned short parameter0; unsigned short parameter2; };
static_assert_(sizeof(List) == 12);
static_assert_(sizeof(Sound) == 0xdc);
static_assert_(sizeof(SoundInfo) == 0x18);
static_assert_(sizeof(SequenceInfo) == 0x1c);
static_assert_(sizeof(WaveInfo) == 0xc);

inline int clampPriority(int priority)
{
        return priority < 0 ? 0 : priority > 127 ? 127 : priority;
}
inline int combinedPriority(const Sound* sound)
{
        return clampPriority(static_cast<int>(static_cast<unsigned>(sound->priority) +
                                               static_cast<unsigned>(sound->priorityOffset)));
}
}
using namespace sound_start_22b6e8;

extern "C" bool fn_0022AC18(void* archive);
extern "C" void fn_0024C8C0(Handle* handle);
extern "C" bool fn_00340064(void* archive, unsigned id, SoundInfo* info);
extern "C" int fn_002BD8E8(const void* ambient, unsigned id);
extern "C" bool fn_002B9FE4(PlayerSlot* player, int priority);
extern "C" bool fn_002C0F58(ActorSlot* player, int priority);
extern "C" int fn_0033FF54(void* archive, unsigned id);
extern "C" Link* fn_0022C024(List* list, Link* node);
extern "C" void fn_0022B5E4(Sound* sound, int frames);
extern "C" void fn_0022AC0C(Sound* sound, int priority, int offset);
extern "C" Link* fn_002323E8(List* list, Link* before, Link* node);
extern "C" void fn_0022ABC0(Sound* sound, const void* ambient);
extern "C" bool fn_002B9F40(PlayerSlot* player, Sound* sound);
extern "C" void fn_002BD7DC(Sound* sound, int parameter);
extern "C" void* fn_002BA0C4(PlayerSlot* player, Sound* sound);
extern "C" bool fn_003401B8(void* archive, unsigned id, WaveInfo* info);
extern "C" bool fn_0034042C(void* archive, unsigned id, SequenceInfo* info);
extern "C" unsigned fn_002BCAF8(Player* player, Sound* sound, const SoundInfo* info,
                                const SequenceInfo* sequence, unsigned offsetType,
                                unsigned offset, const SequenceOverride* overrideInfo);
extern "C" bool fn_00340278(void* archive, unsigned id, StreamInfo* info);
extern "C" void fn_002BE7AC(Sound* sound, void* buffer, unsigned parameter2, unsigned parameter0);
extern "C" void fn_002BE80C(Sound* sound, bool fromStart, unsigned offset, void* archive, unsigned fileId);
extern "C" void fn_0022AC28(Sound* sound, float volume);
extern "C" void fn_0022B188(Sound* sound, unsigned parameter);
extern "C" void fn_0022AED0(Sound* sound, unsigned parameter);
extern "C" unsigned fn_002BC8C0(Player* player, Sound* sound, const SoundInfo* info,
                                const WaveInfo* wave, unsigned offsetType, unsigned offset);
extern "C" bool fn_002C0E28(ActorSlot* player, Sound* sound);
extern "C" void fn_002BD8B8(Sound* sound, int priority);
extern "C" void fn_002B9E34(Handle* handle, Sound* sound);

namespace sound_start_22b6e8
{
template<class T> inline T* allocateInstance(Pool<T>* pool, int priority, int offset)
{
        const int combined = clampPriority(static_cast<int>(static_cast<unsigned>(priority) +
                                                             static_cast<unsigned>(offset)));
        T* sound = 0;
        do
        {
                if (!(pool->free.count < 1))
                {
                        Link* node = pool->free.sentinel.next;
                        sound = reinterpret_cast<T*>(reinterpret_cast<char*>(node) - 0xd4);
                        fn_0022C024(&pool->free, node);
                }
                else
                {
                        T* first = pool->active.count < 1 ? 0 :
                            reinterpret_cast<T*>(reinterpret_cast<char*>(pool->active.sentinel.next) - 0xd4);
                        if (!first || combined < combinedPriority(first))
                                return 0;
                        fn_0022B5E4(first, 0);
                }
        } while (!sound);
        sound->vtable->initialize(sound);
        fn_0022AC0C(sound, priority, offset);
        Link* before = pool->active.sentinel.next;
        while (before != &pool->active.sentinel)
        {
                Sound* current = reinterpret_cast<Sound*>(reinterpret_cast<char*>(before) - 0xd4);
                if (combinedPriority(current) > combined)
                        break;
                before = before->next;
        }
        fn_002323E8(&pool->active, before, &sound->node);
        return sound;
}
template<class T> inline T* allocate(Pool<T>* pool, int priority, int offset,
                                    unsigned id, const void* ambient)
{
        T* sound = allocateInstance(pool, priority, offset);
        if (!sound)
                return 0;
        sound->id = id;
        if (ambient)
                fn_0022ABC0(sound, ambient);
        return sound;
}
}

inline unsigned prepareStream(Player* player, TypedSound<2>* stream, unsigned id,
                              const SoundInfo& info, unsigned offsetType, unsigned offset)
{
        StreamInfo streamInfo;
        streamInfo.parameter0 = 0;
        streamInfo.parameter2 = 0;
        if (!fn_00340278(player->archive, id, &streamInfo))
        {
                return 3;
        }
        fn_002BE7AC(stream, player->streamBuffer, streamInfo.parameter2, streamInfo.parameter0);
        bool fromStart;
        unsigned streamOffset = offset;
        switch (offsetType)
        {
        case 0: fromStart = true; break;
        case 2: fromStart = false; break;
        default: fromStart = false; streamOffset = 0; break;
        }
        fn_002BE80C(stream, fromStart, streamOffset, player->archive, info.fileId);
        fn_0022AC28(stream, info.volume * (1.0f / 127.0f));
        fn_0022B188(stream, info.parameter20);
        fn_0022AED0(stream, info.parameter21);
        return 0;
}

// NonMatching until the unchanged project checker accepts the complete root.
extern "C" unsigned fn_0022B6E8(Player* player, Handle* handle, unsigned id,
                                const void* ambient, Actor* actor, int hold,
                                const StartOptions* options)
{
        if (!player->archive || !fn_0022AC18(player->archive))
                return 11;
        if (handle->sound)
                fn_0024C8C0(handle);
        SoundInfo info;
        if (!fn_00340064(player->archive, id, &info))
                return 3;
        unsigned offsetType = 0;
        unsigned offset = 0;
        int priority = info.priority;
        unsigned playerId = info.playerId;
        unsigned actorPlayerId = info.actorPlayerId;
        const SequenceOverride* sequenceOverride = 0;
        if (options)
        {
                if (options->flags & 1)
                {
                        offsetType = options->offsetType;
                        offset = options->offset;
                }
                if (options->flags & 4) priority = options->priority;
                if (options->flags & 2) playerId = options->playerId;
                if (options->flags & 8) actorPlayerId = options->actorPlayerId;
                if (options->flags & 16) sequenceOverride = &options->sequence;
        }
        int startPriority = priority;
        if (hold)
                startPriority = static_cast<int>(static_cast<unsigned>(startPriority) - 1);
        int ambientPriority = 0;
        if (ambient)
                ambientPriority = fn_002BD8E8(ambient, id);
        const int combined = clampPriority(static_cast<int>(static_cast<unsigned>(startPriority) +
                                                             static_cast<unsigned>(ambientPriority)));
        ActorSlot* actorSlot = 0;
        if (actor)
        {
                actorSlot = actorPlayerId < 4 ? &actor->slots[actorPlayerId] : 0;
                if (!actorSlot) return 14;
        }
        PlayerSlot* playerSlot = &player->slots[playerId & 0x00ffffff];
        if (!fn_002B9FE4(playerSlot, combined) ||
            (actorSlot && !fn_002C0F58(actorSlot, combined)))
                return 1;
        TypedSound<1>* sequence = 0;
        TypedSound<2>* stream = 0;
        TypedSound<3>* wave = 0;
        Sound* sound = 0;
        switch (fn_0033FF54(player->archive, id))
        {
        case 1:
                sequence = allocate(&player->sequence, startPriority, ambientPriority, id, ambient);
                if (!sequence) return 13;
                sound = sequence;
                break;
        case 2:
                stream = allocate(&player->stream, startPriority, ambientPriority, id, ambient);
                if (!stream) return 13;
                sound = stream;
                break;
        case 3:
                wave = allocate(&player->wave, startPriority, ambientPriority, id, ambient);
                if (!wave) return 13;
                sound = wave;
                break;
        default: return 3;
        }
        if (!fn_002B9F40(playerSlot, sound))
        {
                sound->vtable->release(sound);
                return 255;
        }
        fn_002BD7DC(sound, info.parameter22);
        switch (fn_0033FF54(player->archive, id))
        {
        case 1:
        {
                fn_002BA0C4(playerSlot, sequence);
                SequenceInfo sequenceInfo;
                sequenceInfo.offset = 0;
                sequenceInfo.parameter20 = 0;
                sequenceInfo.parameter24 = 0;
                sequenceInfo.parameter25 = 0;
                for (unsigned i = 0; i < 4; ++i) sequenceInfo.banks[i] = ~0u;
                if (!fn_0034042C(player->archive, id, &sequenceInfo))
                {
                        sequence->vtable->release(sequence);
                        return 3;
                }
                if (sequenceOverride)
                {
                        for (unsigned i = 0; i < 4; ++i)
                                if (sequenceOverride->banks[i] != ~0u)
                                        sequenceInfo.banks[i] = sequenceOverride->banks[i];
                }
                unsigned result = fn_002BCAF8(player, sequence, &info, &sequenceInfo,
                                             offsetType, offset, sequenceOverride) & 0xff;
                if (result)
                {
                        sequence->vtable->release(sequence);
                        return result;
                }
                break;
        }
        case 2:
        {
                unsigned result = prepareStream(player, stream, id, info, offsetType, offset);
                if (result)
                {
                        stream->vtable->release(stream);
                        return result;
                }
                break;
        }
        case 3:
        {
                fn_002BA0C4(playerSlot, wave);
                WaveInfo waveInfo;
                waveInfo.parameter4 = 0;
                waveInfo.parameter8 = 0;
                waveInfo.parameter9 = 0;
                if (!fn_003401B8(player->archive, id, &waveInfo))
                {
                        wave->vtable->release(wave);
                        return 3;
                }
                unsigned result = fn_002BC8C0(player, wave, &info, &waveInfo, offsetType, offset) & 0xff;
                if (result)
                {
                        wave->vtable->release(wave);
                        return result;
                }
                break;
        }
        default:
                sound->vtable->release(sound);
                return 3;
        }
        if (actorSlot && !fn_002C0E28(actorSlot, sound))
        {
                sound->vtable->release(sound);
                return 255;
        }
        if (actor) sound->actor = actor;
        if (hold) fn_002BD8B8(sound, priority);
        fn_002B9E34(handle, sound);
        return 0;
}

#endif // NON_MATCHING
