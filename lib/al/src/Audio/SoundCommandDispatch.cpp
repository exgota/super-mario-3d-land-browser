// Clean-room reconstruction from the owner's verified EU executable.
// Neutral address/field identities; no claim about the original SDK classes.
#ifdef NON_MATCHING
namespace sound_command_dispatch
{
struct SequenceCallbackArgs
{
        short* ownerValues;
        short* globalValues;
        short* trackValues;
        unsigned char flag;
        unsigned char opaque0D[3];
};
typedef void (*SequenceCallback)(unsigned, SequenceCallbackArgs*, void*);
static_assert_(sizeof(SequenceCallbackArgs) == 0x10);
struct Command
{
        Command* next;
        unsigned char opcode;
        unsigned char opaque05[11];
};
// Commands have different payload types and sizes. Offsets below describe the
// observed records rather than claiming a common complete object layout.
template<class T> inline T& field(void* object, unsigned offset)
{
        return *reinterpret_cast<T*>(static_cast<unsigned char*>(object) + offset);
}
inline unsigned word(Command* command, unsigned offset)
{
        return field<unsigned>(command, offset);
}
inline void* pointer(Command* command, unsigned offset)
{
        return field<void*>(command, offset);
}
inline float real(Command* command, unsigned offset)
{
        return field<float>(command, offset);
}
inline signed char signedByte(Command* command, unsigned offset)
{
        return field<signed char>(command, offset);
}
inline unsigned char byte(Command* command, unsigned offset)
{
        return field<unsigned char>(command, offset);
}
inline void invoke(void* object, unsigned slot)
{
        typedef void (*Method)(void*);
        Method* table = field<Method*>(object, 0);
        table[slot](object);
}
static_assert_(sizeof(Command) == 0x10);
}
using namespace sound_command_dispatch;

extern "C" void fn_002C42FC(void*, unsigned, float);
extern "C" void fn_002C4308(void*, unsigned char, float);
extern "C" void fn_002C6788(void*, void*, unsigned, void*);
extern "C" void fn_002C68AC(void*, const void*, unsigned, const void* const*, int, signed char);
extern "C" void fn_002C6700(void*, unsigned char, int);
extern "C" void fn_002C6684(void*, SequenceCallback, void*);
extern "C" void fn_002C6414(void*, unsigned, short);
extern "C" void fn_002C6468(unsigned, short);
extern "C" void* fn_002C6348(void*, int);
extern "C" void fn_002C2A84(void*, unsigned, short);
extern "C" void fn_002C62B4(void*, unsigned, unsigned char);
extern "C" void fn_002C6390(void*, unsigned, signed char, unsigned);
extern "C" void fn_002C635C(void*, unsigned, float);
extern "C" void fn_002C6330(void*, unsigned, float);
extern "C" void fn_002C6224(void*, unsigned, float);
extern "C" void fn_002C6590(void*, unsigned, float);
extern "C" void fn_002C6378(void*, unsigned, float);
extern "C" void fn_002C6450(void*, unsigned, float);
extern "C" void fn_002C6420(void*, unsigned, float);
extern "C" void fn_002C6438(void*, unsigned, float);
extern "C" void fn_002C65A8(void*, unsigned, unsigned, float);
extern "C" bool fn_002C647C(void*, unsigned, unsigned);
extern "C" void fn_002C6510(void*, unsigned, signed char);
extern "C" void fn_002C6638(void*, unsigned, float);
extern "C" bool fn_002C41A4(void*, void*, unsigned, unsigned char,
                                unsigned, void*, unsigned, signed char);
extern "C" int fn_002C5850(void*, void*, unsigned, unsigned short);
extern "C" bool fn_002C5C64(void*, unsigned char, unsigned, void*,
                                unsigned, void*, unsigned);
extern "C" bool fn_002C44E0(void*, const void*, const void*, const void*,
                                const void*, unsigned, unsigned, unsigned, unsigned short);
extern "C" bool fn_002C4BA0(void*, unsigned, unsigned, unsigned, signed char, signed char);
extern "C" void fn_002C4E28(void*, unsigned, float);
extern "C" void fn_002C4888(void*, unsigned, float);
extern "C" void fn_002C5348(void*, unsigned, float);
extern "C" void* fn_0022A6E4();
extern "C" void fn_002C6BE0(void*, const void*, unsigned);
extern "C" void fn_0022A6D4(void*, void*);
extern "C" void fn_0022A6CC(void*, void*);
extern "C" unsigned dat_003F38B8;
extern "C" unsigned char dat_00430DB0[];
extern "C" int fn_0028A998(unsigned*);
extern "C" void fn_002585C4(void*);
extern "C" bool fn_002C361C(void*, unsigned char, void*);
extern "C" bool fn_002C3554(void*, unsigned char, void*);
extern "C" bool fn_002C35B8(void*, unsigned char, void*);
extern "C" void fn_002C34E8(void*, unsigned char, unsigned);
extern "C" void* fn_0022B410();
extern "C" void fn_0022A66C(void*, unsigned);
extern "C" void fn_002C6674(void*, void*);
extern "C" void fn_002C410C(void*, void*);
extern "C" void* fn_0022B488();
extern "C" void fn_0022A614(void*, void*, unsigned);

namespace sound_command_dispatch
{
inline void* outputControl()
{
        if (!(dat_003F38B8 & 1) && fn_0028A998(&dat_003F38B8))
                fn_002585C4(dat_00430DB0);
        return dat_00430DB0;
}
}

// NonMatching: the complete root and its inline jump table are not accepted.
extern "C" void fn_002BECC8(Command* command)
{
        while (command)
        {
                switch (command->opcode)
                {
                case 2:
                        *static_cast<unsigned char*>(pointer(command, 0x10)) = 1;
                        break;
                case 3:
                        invoke(pointer(command, 0x10), 2);
                        *static_cast<unsigned char*>(pointer(command, 0x14)) = 1;
                        break;
                case 4: invoke(pointer(command, 0x10), 3); break;
                case 5: invoke(pointer(command, 0x10), 4); break;
                case 6:
                        if (byte(command, 0x14)) field<float>(pointer(command, 0x10), 0x10) = 0.0f;
                        invoke(pointer(command, 0x10), 5);
                        break;
                case 7:
                {
                        typedef void (*Method)(void*, signed char);
                        void* object = pointer(command, 0x10);
                        field<Method*>(object, 0)[6](object, signedByte(command, 0x14));
                        break;
                }
                case 8:
                        field<float>(pointer(command, 0x10), 0x10) = real(command, 0x14);
                        field<float>(pointer(command, 0x10), 0x14) = real(command, 0x18);
                        field<float>(pointer(command, 0x10), 0x18) = real(command, 0x1c);
                        field<float>(pointer(command, 0x10), 0x1c) = real(command, 0x20);
                        field<float>(pointer(command, 0x10), 0x20) = real(command, 0x24);
                        fn_002C42FC(pointer(command, 0x10), word(command, 0x28), real(command, 0x2c));
                        field<float>(pointer(command, 0x10), 0x2c) = real(command, 0x30);
                        for (int i = 0; i < 2; ++i)
                                fn_002C4308(pointer(command, 0x10), static_cast<unsigned char>(i), real(command, 0x34 + 4*i));
                        break;
                case 9: field<unsigned char>(pointer(command, 0x10), 0x30) = byte(command, 0x14); break;
                case 10: field<unsigned char>(pointer(command, 0x10), 0x31) = byte(command, 0x15); break;
                case 11: field<unsigned char>(pointer(command, 0x10), 0x2a) = byte(command, 0x14); break;
                case 12: fn_002C6788(pointer(command, 0x10), pointer(command, 0x14), word(command, 0x18), pointer(command, 0x1c)); break;
                case 13: fn_002C68AC(pointer(command, 0x10), pointer(command, 0x14), word(command, 0x18), reinterpret_cast<const void* const*>(reinterpret_cast<unsigned char*>(command) + 0x1c), 4, signedByte(command, 0x2c)); break;
                case 14: fn_002C6700(pointer(command, 0x10), static_cast<unsigned char>(word(command, 0x14)), static_cast<int>(word(command, 0x18))); break;
                case 15: field<float>(pointer(command, 0x10), 0x60) = real(command, 0x14); break;
                case 16: field<unsigned char>(pointer(command, 0x10), 0x71) = word(command, 0x14); break;
                case 17: field<signed char>(pointer(command, 0x10), 0x58) = signedByte(command, 0x14); break;
                case 18: fn_002C6684(pointer(command, 0x10), field<SequenceCallback>(command, 0x14), pointer(command, 0x18)); break;
                case 19: fn_002C6414(pointer(command, 0x10), word(command, 0x18), static_cast<short>(word(command, 0x1c))); break;
                case 20: fn_002C6468(word(command, 0x18), static_cast<short>(word(command, 0x1c))); break;
                case 21:
                {
                        void* track = fn_002C6348(pointer(command, 0x10), static_cast<int>(word(command, 0x14)));
                        if (track) fn_002C2A84(track, word(command, 0x18), static_cast<short>(word(command, 0x1c)));
                        break;
                }
                case 22: fn_002C62B4(pointer(command, 0x10), word(command, 0x14), static_cast<unsigned char>(word(command, 0x18))); break;
                case 23: fn_002C6390(pointer(command, 0x10), word(command, 0x14), signedByte(command, 0x18), word(command, 0x1c)); break;
                case 24: fn_002C635C(pointer(command, 0x10), word(command, 0x14), real(command, 0x18)); break;
                case 25: fn_002C6330(pointer(command, 0x10), word(command, 0x14), real(command, 0x18)); break;
                case 26: fn_002C6224(pointer(command, 0x10), word(command, 0x14), real(command, 0x18)); break;
                case 27: fn_002C6590(pointer(command, 0x10), word(command, 0x14), real(command, 0x18)); break;
                case 28: fn_002C6378(pointer(command, 0x10), word(command, 0x14), real(command, 0x18)); break;
                case 29: fn_002C6450(pointer(command, 0x10), word(command, 0x14), real(command, 0x18)); break;
                case 30: fn_002C6420(pointer(command, 0x10), word(command, 0x14), real(command, 0x18)); break;
                case 31: fn_002C6438(pointer(command, 0x10), word(command, 0x14), real(command, 0x18)); break;
                case 32: fn_002C65A8(pointer(command, 0x10), word(command, 0x14), word(command, 0x18), real(command, 0x1c)); break;
                case 33: fn_002C647C(pointer(command, 0x10), word(command, 0x14), word(command, 0x18)); break;
                case 34: fn_002C6510(pointer(command, 0x10), word(command, 0x14), signedByte(command, 0x18)); break;
                case 35: fn_002C6638(pointer(command, 0x10), word(command, 0x14), real(command, 0x18)); break;
                case 36: fn_002C41A4(pointer(command, 0x10), pointer(command, 0x14), word(command, 0x18), static_cast<unsigned char>(word(command, 0x1c)), word(command, 0x20), pointer(command, 0x24), word(command, 0x28), signedByte(command, 0x2c)); break;
                case 37: field<unsigned char>(pointer(command, 0x10), 0x5b) = word(command, 0x14); break;
                case 38: field<signed char>(pointer(command, 0x10), 0x59) = signedByte(command, 0x14); break;
                case 39: fn_002C5850(pointer(command, 0x10), pointer(command, 0x14), word(command, 0x18), field<unsigned short>(command, 0x1c)); break;
                case 40: fn_002C5C64(pointer(command, 0x10), static_cast<unsigned char>(word(command, 0x24)), word(command, 0x28), pointer(command, 0x14), word(command, 0x18), pointer(command, 0x1c), word(command, 0x20)); break;
                case 42: fn_002C44E0(pointer(command, 0x10), reinterpret_cast<unsigned char*>(command) + 0x14, reinterpret_cast<unsigned char*>(command) + 0x4c, reinterpret_cast<unsigned char*>(command) + 0x60, reinterpret_cast<unsigned char*>(command) + 0x190, word(command, 0x1c0), word(command, 0x1c4), word(command, 0x1c8), field<unsigned short>(command, 0x1cc)); break;
                case 43: fn_002C4BA0(pointer(command, 0x10), word(command, 0x14), word(command, 0x18), word(command, 0x1c), signedByte(command, 0x20), signedByte(command, 0x21)); break;
                case 44: fn_002C4E28(pointer(command, 0x10), word(command, 0x14), real(command, 0x18)); break;
                case 45: fn_002C4888(pointer(command, 0x10), word(command, 0x14), real(command, 0x18)); break;
                case 46: fn_002C5348(pointer(command, 0x10), word(command, 0x14), real(command, 0x18)); break;
                case 47:
                {
                        void* control = fn_0022A6E4();
                        fn_002C6BE0(control, pointer(command, 0x10), word(command, 0x14)); break;
                }
                case 48:
                {
                        void* control = fn_0022A6E4();
                        fn_0022A6D4(control, pointer(command, 0x10)); break;
                }
                case 49:
                {
                        void* control = fn_0022A6E4();
                        fn_0022A6CC(control, pointer(command, 0x10)); break;
                }
                case 50:
                {
                        void* control = outputControl();
                        fn_002C361C(control, byte(command, 0x10), pointer(command, 0x14)); break;
                }
                case 51:
                {
                        void* control = outputControl();
                        fn_002C3554(control, byte(command, 0x10), pointer(command, 0x14)); break;
                }
                case 52:
                {
                        void* control = outputControl();
                        fn_002C35B8(control, byte(command, 0x10), pointer(command, 0x14)); break;
                }
                case 53:
                {
                        void* control = outputControl();
                        fn_002C34E8(control, byte(command, 0x10), word(command, 0x18)); break;
                }
                case 54:
                {
                        void* control = fn_0022B410();
                        fn_0022A66C(control, word(command, 0x10)); break;
                }
                case 55: fn_002C6674(pointer(command, 0x10), pointer(command, 0x14)); break;
                case 56: fn_002C410C(pointer(command, 0x10), pointer(command, 0x14)); break;
                case 57:
                case 58:
                {
                        void* control = fn_0022B488();
                        fn_0022A614(control, pointer(command, 0x10), 1); break;
                }
                default: break;
                }
                command = command->next;
        }
}
#endif // NON_MATCHING
