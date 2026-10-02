#include <retail/GraphicsIntegerQuery.h>

#ifdef NON_MATCHING
// NonMatching: complete clean-room proposal for 0038F9F0..00390204.
using namespace retail_graphics_integer_query;
extern "C" {
extern retail_graphics::RenderControl* dat_003E3154;
void fn_00377DAC(unsigned query, int* output);
}
static_assert_(offsetof(StateView, word01c) == 0x1c);
static_assert_(offsetof(StateView, vector020) == 0x20);
static_assert_(offsetof(StateView, word034) == 0x34);
static_assert_(offsetof(StateView, word038) == 0x38);
static_assert_(offsetof(StateView, flag03c) == 0x3c);
static_assert_(offsetof(StateView, float040) == 0x40);
static_assert_(offsetof(StateView, float044) == 0x44);
static_assert_(offsetof(StateView, range04c) == 0x4c);
static_assert_(offsetof(StateView, flag054) == 0x54);
static_assert_(offsetof(StateView, activeUnit) == 0x58);
static_assert_(offsetof(StateView, texture2D) == 0x5c);
static_assert_(offsetof(StateView, textureCube) == 0x68);
static_assert_(offsetof(StateView, auxiliary) == 0x74);
static_assert_(offsetof(StateView, bindingSet) == 0xf4);
static_assert_(offsetof(StateView, word508) == 0x508);
static_assert_(offsetof(StateView, word50c) == 0x50c);
static_assert_(offsetof(StateView, word510) == 0x510);
static_assert_(offsetof(StateView, vector514) == 0x514);
static_assert_(offsetof(StateView, word52c) == 0x52c);
static_assert_(offsetof(StateView, word530) == 0x530);
static_assert_(offsetof(StateView, word534) == 0x534);
static_assert_(offsetof(StateView, word538) == 0x538);
static_assert_(offsetof(StateView, word53c) == 0x53c);
static_assert_(offsetof(StateView, word540) == 0x540);
static_assert_(offsetof(StateView, word544) == 0x544);
static_assert_(offsetof(StateView, word548) == 0x548);
static_assert_(offsetof(StateView, word54c) == 0x54c);
static_assert_(offsetof(StateView, word550) == 0x550);
static_assert_(offsetof(StateView, word554) == 0x554);
static_assert_(offsetof(StateView, word558) == 0x558);
static_assert_(offsetof(StateView, word55c) == 0x55c);
static_assert_(offsetof(StateView, normalized560) == 0x560);
static_assert_(offsetof(StateView, word570) == 0x570);
static_assert_(offsetof(StateView, flag578) == 0x578);
static_assert_(offsetof(StateView, flag57a) == 0x57a);
static_assert_(offsetof(StateView, flag57b) == 0x57b);
static_assert_(offsetof(StateView, flag57c) == 0x57c);
static_assert_(offsetof(StateView, flag57d) == 0x57d);
static_assert_(offsetof(StateView, word580) == 0x580);
static_assert_(offsetof(StateView, flags584) == 0x584);
static_assert_(offsetof(StateView, flag588) == 0x588);
static_assert_(offsetof(StateView, word58c) == 0x58c);
static_assert_(offsetof(StateView, normalized590) == 0x590);
static_assert_(offsetof(StateView, float5a0) == 0x5a0);
static_assert_(offsetof(StateView, word5a4) == 0x5a4);
static_assert_(offsetof(StateView, word5a8) == 0x5a8);
static_assert_(offsetof(StateView, flag5c3) == 0x5c3);

// The retail VFP path multiplies, subtracts and halves before truncating.
static inline int normalizedInteger(float value)
{
    return static_cast<int>((value * 4294967296.0f - 1.0f) * 0.5f);
}

extern "C" void fn_0038F9F0(unsigned query, int* output)
{
    StateView* state = reinterpret_cast<StateView*>(dat_003E3154);
    switch (query) {
    case 0x0b44:
        *output = state->flag03c == 1;
        return;
    case 0x0b45:
        *output = state->word038;
        return;
    case 0x0b46:
        *output = state->word034;
        return;
    case 0x0b70:
        output[0] = static_cast<int>(state->range04c[0]);
        output[1] = static_cast<int>(state->range04c[1]);
        return;
    case 0x0b71:
        *output = state->flag57b == 1;
        return;
    case 0x0b72:
        *output = state->flag588 == 1;
        return;
    case 0x0b73:
        *output = static_cast<int>(state->float5a0);
        return;
    case 0x0b74:
        *output = state->word544;
        return;
    case 0x0b90:
        *output = state->flag57a == 1;
        return;
    case 0x0b91:
        *output = state->word5a8;
        return;
    case 0x0b92:
        *output = state->word52c;
        return;
    case 0x0b93:
        *output = state->word534;
        return;
    case 0x0b94:
        *output = state->word538;
        return;
    case 0x0b95:
        *output = state->word53c;
        return;
    case 0x0b96:
        *output = state->word540;
        return;
    case 0x0b97:
        *output = state->word530;
        return;
    case 0x0b98:
        *output = state->word58c;
        return;
    case 0x0ba2:
        output[0] = state->vector020[0];
        output[1] = state->vector020[1];
        output[2] = state->vector020[2];
        output[3] = state->vector020[3];
        return;
    case 0x0be2:
        *output = state->flag57c == 1;
        return;
    case 0x0bf0:
        *output = state->word570;
        return;
    case 0x0bf2:
        *output = state->flag57d == 1;
        return;
    case 0x0c10:
        output[0] = state->vector514[0];
        output[1] = state->vector514[1];
        output[2] = state->vector514[2];
        output[3] = state->vector514[3];
        return;
    case 0x0c11:
        *output = state->flag578 == 1;
        return;
    case 0x0c22:
        output[0] = normalizedInteger(state->normalized590[0]);
        output[1] = normalizedInteger(state->normalized590[1]);
        output[2] = normalizedInteger(state->normalized590[2]);
        output[3] = normalizedInteger(state->normalized590[3]);
        return;
    case 0x0c23:
        output[0] = state->flags584[0] == 1;
        output[1] = state->flags584[1] == 1;
        output[2] = state->flags584[2] == 1;
        output[3] = state->flags584[3] == 1;
        return;
    case 0x0d33:
        *output = 0x400;
        return;
    case 0x0d3a:
        output[0] = 0x400;
        output[1] = 0x400;
        return;
    case 0x0d50:
        *output = 0;
        return;
    case 0x2a00:
        *output = static_cast<int>(state->float044);
        return;
    case 0x6601:
        *output = state->bindingSet;
        return;
    case 0x660e:
        *output = 32;
        return;
    case 0x660f:
        *output = 512;
        return;
    case 0x6781:
        *output = state->word5a4;
        return;
    case 0x6782:
        *output = state->word580;
        return;
    case 0x6788:
        *output = state->flag5c3 == 0 ? 0x6789 : 0x678a;
        return;
    case 0x6801:
        *output = state->word510;
        return;
    case 0x8005:
        output[0] = normalizedInteger(state->normalized560[0]);
        output[1] = normalizedInteger(state->normalized560[1]);
        output[2] = normalizedInteger(state->normalized560[2]);
        output[3] = normalizedInteger(state->normalized560[3]);
        return;
    case 0x8009:
        *output = state->word558;
        return;
    case 0x8037:
        *output = state->flag054 == 1;
        return;
    case 0x8038:
        *output = static_cast<int>(state->float040);
        return;
    case 0x8069:
        *output = state->texture2D[state->activeUnit];
        return;
    case 0x80c8:
        *output = state->word550;
        return;
    case 0x80c9:
        *output = state->word548;
        return;
    case 0x80ca:
        *output = state->word554;
        return;
    case 0x80cb:
        *output = state->word54c;
        return;
    case 0x84e0:
        *output = state->activeUnit + 0x84c0;
        return;
    case 0x84e8:
        *output = 0x400;
        return;
    case 0x8514:
        *output = state->textureCube[state->activeUnit];
        return;
    case 0x851c:
        *output = 0x400;
        return;
    case 0x86a2:
        *output = 1;
        return;
    case 0x86a3:
        *output = 0x675a;
        return;
    case 0x883d:
        *output = state->word55c;
        return;
    case 0x8869:
        *output = 12;
        return;
    case 0x8894:
        *output = state->word50c;
        return;
    case 0x8895:
        *output = state->word508;
        return;
    case 0x8b4d:
        *output = 4;
        return;
    case 0x8b8d:
        *output = state->word01c;
        return;
    case 0x8b9a:
        *output = 0x1401;
        return;
    case 0x8b9b:
        *output = 0x1908;
        return;
    case 0x8df8:
        *output = 0x6000;
        return;
    case 0x8df9:
        *output = 1;
        return;
    case 0x8dfa:
        *output = 0;
        return;
    case 0x6680:
    case 0x6681:
    case 0x6682:
    case 0x6683:
    case 0x6684:
    case 0x6685:
    case 0x6686:
    case 0x6687:
    case 0x6688:
    case 0x6689:
    case 0x668a:
    case 0x668b:
    case 0x668c:
    case 0x668d:
    case 0x668e:
    case 0x668f:
    case 0x6690:
    case 0x6691:
    case 0x6692:
    case 0x6693:
    case 0x6694:
    case 0x6695:
    case 0x6696:
    case 0x6697:
    case 0x6698:
    case 0x6699:
    case 0x669a:
    case 0x669b:
    case 0x669c:
    case 0x669d:
    case 0x669e:
    case 0x669f:
        *output = state->auxiliary[query - 0x6680];
        return;
    case 0x0d52:
    case 0x0d53:
    case 0x0d54:
    case 0x0d55:
    case 0x0d56:
    case 0x0d57:
    case 0x8ca6:
    case 0x8ca7:
        fn_00377DAC(query, output);
        return;
    default:
        return;
    }
}
#endif
