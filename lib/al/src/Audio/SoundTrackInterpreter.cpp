// Clean-room reconstruction from the owner's verified EU executable.
// Neutral field identities; the original library class and API names are unknown.
#ifdef NON_MATCHING
namespace sound_track_interpreter
{
struct ControlFrame
{
        unsigned char kind;
        unsigned char remaining;
        unsigned char opaque02[2];
        const unsigned char* position;
};
template<class T> struct Ramp
{
        T start;
        T target;
        short duration;
        short elapsed;

        void set(T value, short frames)
        {
                T current = target;
                if (elapsed < duration)
                        current = static_cast<T>(start + (target - start) * elapsed / duration);
                start = current;
                target = value;
                duration = frames;
                elapsed = 0;
        }
};
struct State
{
        const unsigned char* base;
        const unsigned char* position;
        unsigned char condition;
        unsigned char parameter09;
        unsigned char parameter0A;
        unsigned char parameter0B;
        ControlFrame frames[3];
        unsigned char depth;
        unsigned char parameter25;
        unsigned char opaque26[9];
        unsigned char parameter2F;
        unsigned char parameter30;
        unsigned char parameter31;
        unsigned char opaque32[2];
        unsigned program;
        float parameter38;
        float parameter3C;
        int parameter40;
        unsigned char parameter44;
        unsigned char opaque45[3];
        unsigned char parameter48;
        unsigned char opaque49[3];
        float parameter4C;
        Ramp<unsigned char> parameter50;
        Ramp<signed char> parameter56;
        Ramp<signed char> parameter5C;
        Ramp<signed char> parameter62;
        unsigned char parameter68;
        unsigned char parameter69;
        unsigned char parameter6A;
        signed char parameter6B;
        unsigned char parameter6C;
        unsigned char parameter6D;
        unsigned char parameter6E;
        unsigned char parameter6F;
        unsigned char parameter70;
        unsigned char parameter71;
        unsigned char parameter72;
        unsigned char parameter73;
        unsigned short parameter74;
        unsigned char parameter76;
        unsigned char parameter77;
        unsigned char parameter78;
        unsigned char parameter79;
        unsigned char opaque7A[2];
        float parameter7C;
        float parameter80;
};
struct Track
{
        unsigned char opaque00[0x1c];
        State state;
        short variables[16];
        void* owner;
};
struct OwnerParameters
{
        unsigned char parameter00;
        unsigned char opaque01;
        unsigned char parameter02;
        unsigned char opaque03;
        unsigned short parameter04;
};
static_assert_(sizeof(ControlFrame) == 8);
static_assert_(sizeof(Ramp<unsigned char>) == 6);
static_assert_(sizeof(Ramp<signed char>) == 6);
static_assert_(sizeof(State) == 0x84);
static_assert_(sizeof(Track) == 0xc4);
}

// Import declarations agree with the separate SoundCommandDispatch proposal.
extern "C" void* fn_002C6348(void*, int);
extern "C" void fn_002C2F20(void*, unsigned char);
extern "C" void fn_002C2AA0(void*, int);
extern "C" void fn_002C29E8(void*);
extern "C" short* fn_00214928(void*, int);
extern "C" short* fn_00228C94(void*, int);
extern "C" void fn_00228D28(void*);
extern "C" void fn_002C2810(void*, const unsigned char*, unsigned);
extern "C" void fn_002C2BE4(void*);
extern "C" void fn_002C6690(void*, unsigned short, void*);
extern "C" unsigned fn_0024B5C4();
extern "C" unsigned char dat_003F152D;

using namespace sound_track_interpreter;

// NonMatching: the checker must accept the complete root, including inline data.
extern "C" void fn_003409FC(void*, Track* track, unsigned command, int argument, int extra)
{
        State& state = track->state;
        void* owner = track->owner;
        OwnerParameters& parameters = *reinterpret_cast<OwnerParameters*>(
            static_cast<unsigned char*>(owner) + 0x70);
        if (command <= 0xff)
        {
                switch (command)
                {
                case 0x81:
                        if (argument < 0x10000) state.program = static_cast<unsigned short>(argument);
                        break;
                case 0x88:
                {
                        void* other = fn_002C6348(owner, argument);
                        if (other && other != track)
                        {
                                fn_00228D28(other);
                                fn_002C2810(other, state.base, extra);
                                fn_002C2BE4(other);
                        }
                        break;
                }
                case 0x89: state.position = state.base + argument; break;
                case 0x8a:
                        if (state.depth < 3)
                        {
                                state.frames[state.depth].position = state.position;
                                state.frames[state.depth].kind = 0;
                                ++state.depth;
                                state.position = state.base + argument;
                        }
                        break;
                case 0xb0: parameters.parameter02 = argument; break;
                case 0xb1: state.parameter74 = static_cast<unsigned char>(argument); break;
                case 0xb2:
                        state.parameter0B = argument != 0;
                        if (state.parameter0B)
                        {
                                fn_002C2AA0(track, -1);
                                fn_002C29E8(track);
                        }
                        break;
                case 0xb3: state.parameter69 = argument; break;
                case 0xb4: state.parameter79 = argument; break;
                case 0xb5: state.parameter80 = argument * (1.0f / 127.0f); break;
                case 0xb6: state.parameter31 = argument; break;
                case 0xbf: state.parameter25 = argument != 0; break;
                case 0xc0: state.parameter56.set(static_cast<signed char>(argument - 64), static_cast<short>(extra)); break;
                case 0xc1: state.parameter50.set(static_cast<unsigned char>(argument), static_cast<short>(extra)); break;
                case 0xc2: parameters.parameter00 = argument; break;
                case 0xc3: state.parameter6C = argument; break;
                case 0xc4: state.parameter62.set(static_cast<signed char>(argument), static_cast<short>(extra)); break;
                case 0xc5: state.parameter6A = argument; break;
                case 0xc6: state.parameter6D = argument; break;
                case 0xc7: state.parameter09 = argument != 0; break;
                case 0xc8:
                        state.parameter0A = argument != 0;
                        fn_002C2AA0(track, -1);
                        fn_002C29E8(track);
                        break;
                case 0xc9:
                        state.parameter6E = state.parameter6C + argument;
                        state.parameter2F = 1;
                        break;
                case 0xca: state.parameter38 = static_cast<unsigned char>(argument) * (1.0f / 128.0f); break;
                case 0xcb: state.parameter3C = static_cast<unsigned char>(argument) * 0.390625f; break;
                case 0xcc: state.parameter48 = argument; break;
                case 0xcd: state.parameter44 = argument; break;
                case 0xce: state.parameter2F = argument != 0; break;
                case 0xcf: state.parameter6F = argument; break;
                case 0xd0: state.parameter70 = argument; break;
                case 0xd1: state.parameter71 = argument; break;
                case 0xd2: state.parameter72 = argument; break;
                case 0xd3: state.parameter73 = argument; break;
                case 0xd4:
                        if (state.depth < 3)
                        {
                                state.frames[state.depth].position = state.position;
                                state.frames[state.depth].remaining = argument;
                                state.frames[state.depth].kind = 1;
                                ++state.depth;
                        }
                        break;
                case 0xd5: state.parameter68 = argument; break;
                case 0xd6:
                        if (dat_003F152D)
                        {
                                if (argument < 32) fn_00214928(owner, argument);
                                else if (argument < 48) fn_00228C94(track, argument - 32);
                        }
                        break;
                case 0xd7: state.parameter5C.set(static_cast<signed char>(argument), static_cast<short>(extra)); break;
                case 0xd8: state.parameter7C = (argument - 64) * (1.0f / 64.0f); break;
                case 0xd9: state.parameter77 = argument; break;
                case 0xda: state.parameter78 = argument; break;
                case 0xdb: state.parameter76 = argument; break;
                case 0xdc: state.parameter6B = argument - 64; break;
                case 0xdd: fn_002C2F20(track, static_cast<unsigned char>(argument)); break;
                case 0xdf: state.parameter30 = static_cast<unsigned char>(argument) >= 64; break;
                case 0xe0: state.parameter40 = argument * 5; break;
                case 0xe1:
                        if (argument > 1023) argument = 1023;
                        else if (argument < 0) argument = 0;
                        parameters.parameter04 = argument;
                        break;
                case 0xe3: state.parameter4C = argument * (1.0f / 64.0f); break;
                case 0xfb:
                        state.parameter70 = 255;
                        state.parameter71 = 255;
                        state.parameter72 = 255;
                        state.parameter73 = 255;
                        state.parameter74 = 255;
                        break;
                case 0xfc:
                        if (state.depth)
                        {
                                ControlFrame& frame = state.frames[state.depth - 1];
                                if (frame.kind)
                                {
                                        unsigned char remaining = frame.remaining;
                                        if (remaining && !--remaining) --state.depth;
                                        else
                                        {
                                                frame.remaining = remaining;
                                                state.position = frame.position;
                                        }
                                }
                        }
                        break;
                case 0xfd:
                        if (state.depth)
                        {
                                ControlFrame* frame = 0;
                                do
                                {
                                        --state.depth;
                                        if (!state.frames[state.depth].kind)
                                        {
                                                frame = &state.frames[state.depth];
                                                break;
                                        }
                                } while (state.depth);
                                if (frame) state.position = frame->position;
                        }
                        break;
                default: break;
                }
        }
        else if (command < 0x10000)
        {
                unsigned operation = command & 0xff;
                short* variable = 0;
                if ((operation & 0xf0) == 0x80 || (operation & 0xf0) == 0x90)
                {
                        if (argument < 32) variable = fn_00214928(owner, argument);
                        else if (argument < 48) variable = fn_00228C94(track, argument - 32);
                        if (!variable) return;
                }
                switch (operation)
                {
                case 0x80: *variable = static_cast<short>(extra); break;
                case 0x81: *variable += static_cast<short>(extra); break;
                case 0x82: *variable -= static_cast<short>(extra); break;
                case 0x83: *variable *= static_cast<short>(extra); break;
                case 0x84:
                        if (extra) *variable /= static_cast<short>(extra);
                        break;
                case 0x85:
                        // The original register shift uses the low eight bits of
                        // its count. Keep the full signed-16 caller domain defined.
                        if (extra < 0)
                        {
                                unsigned amount = (0u - static_cast<unsigned>(extra)) & 255;
                                *variable = amount < 32 ? *variable >> amount : (*variable < 0 ? -1 : 0);
                        }
                        else
                        {
                                unsigned amount = static_cast<unsigned>(extra) & 255;
                                *variable = amount < 32 ? static_cast<unsigned>(static_cast<unsigned short>(*variable)) << amount : 0;
                        }
                        break;
                case 0x86:
                {
                        bool negative = extra < 0;
                        if (negative) extra = static_cast<short>(-extra);
                        int value = static_cast<int>(fn_0024B5C4() * static_cast<unsigned>(extra + 1)) >> 16;
                        *variable = negative ? -value : value;
                        break;
                }
                case 0x87: *variable &= extra; break;
                case 0x88: *variable |= extra; break;
                case 0x89: *variable ^= extra; break;
                case 0x8a: *variable = ~extra; break;
                case 0x8b: if (extra) *variable %= extra; break;
                case 0x90: state.condition = *variable == extra; break;
                case 0x91: state.condition = *variable >= extra; break;
                case 0x92: state.condition = *variable > extra; break;
                case 0x93: state.condition = *variable <= extra; break;
                case 0x94: state.condition = *variable < extra; break;
                case 0x95: state.condition = *variable != extra; break;
                case 0xe0: fn_002C6690(owner, static_cast<unsigned short>(argument), track); break;
                default: break;
                }
        }
}
#endif // NON_MATCHING
