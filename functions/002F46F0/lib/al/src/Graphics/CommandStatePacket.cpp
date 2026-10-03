namespace observed_command_state {
struct FourWords { unsigned words[4]; };
// Only the observed prefix is described; no total allocation is asserted.
struct RecordPrefix {
    unsigned flags;
    FourWords initialPacket;
    unsigned enable;
    unsigned unknown18[4];
    unsigned modeAndPacket[6];
    FourWords optionalPacket;
};
static_assert(offsetof(RecordPrefix, initialPacket) == 4, "first packet");
static_assert(offsetof(RecordPrefix, enable) == 0x14, "low-byte enable");
static_assert(offsetof(RecordPrefix, modeAndPacket) == 0x28, "six-word packet");
static_assert(offsetof(RecordPrefix, optionalPacket) == 0x40, "optional packet");
}
extern "C" {
extern unsigned char dat_003F1474;
extern unsigned char dat_003F1475;
extern signed char dat_003F1476;
extern signed char dat_003F1477;
extern signed char dat_003F1478;
extern signed char dat_003F1479;
extern unsigned char dat_003F147B;
extern unsigned dat_003F147C;
extern unsigned* dat_003E2E30;
extern unsigned dat_003F148C[10];
unsigned* fn_002542BC(unsigned*, const unsigned*);
void* nnnstdMemCpy(void*, const void*, unsigned int);
void fn_001E104C();
}

extern "C" void fn_002F46F0(const observed_command_state::RecordPrefix* record) {
    using namespace observed_command_state;
    unsigned flags = record->flags;
    bool firstEnabled = (flags & 1) != 0;
    dat_003F1475 = dat_003F1475 || dat_003F1477 != firstEnabled;
    dat_003F1477 = firstEnabled;
    bool secondEnabled = (flags & 2) != 0;
    dat_003F1475 = dat_003F1475 || dat_003F1479 != secondEnabled;
    dat_003F1479 = secondEnabled;
    unsigned maskSnapshot = dat_003F147C;
    unsigned byteSnapshot = dat_003F147B;
    unsigned* output = dat_003E2E30;
    output[0] = (maskSnapshot << 8) | (static_cast<unsigned>(secondEnabled) << 12);
    output[1] = 0x20107;
    output[2] = byteSnapshot << 8;
    output[3] = 0x20105;
    *reinterpret_cast<FourWords*>(output + 4) = record->initialPacket;
    output += 8;
    dat_003E2E30 = output;
    const FourWords* optional = &record->optionalPacket;
    if (optional) {
        bool enabled = (optional->words[0] & 1) != 0;
        dat_003F1475 = dat_003F1475 || dat_003F1478 != enabled;
        dat_003F1478 = enabled;
        *reinterpret_cast<FourWords*>(output) = *optional;
        dat_003E2E30 = output + 4;
    }
    bool enabled = (record->enable & 0xff) != 0;
    dat_003F1475 = dat_003F1475 || dat_003F1476 != enabled;
    dat_003F1476 = enabled;
    output = fn_002542BC(dat_003E2E30, record->modeAndPacket);
    dat_003E2E30 = output;
    unsigned mode = record->modeAndPacket[0] & 3;
    dat_003F1474 = mode;
    dat_003F1475 = 0;
    dat_003F148C[4] = 0;
    dat_003F148C[6] = 0;
    if (mode) {
        dat_003F148C[4] = 15;
        dat_003F148C[6] = 15;
    } else if (dat_003F147C) {
        if (dat_003F1476 || dat_003F147C != 15)
            dat_003F148C[4] = 15;
        dat_003F148C[6] = 15;
    }
    dat_003F148C[7] = 0;
    dat_003F148C[8] = 0;
    if (mode == 1) {
        dat_003F148C[7] = 3;
    } else if (!mode) {
        if (dat_003F1477) {
            if (dat_003F1479) {
                dat_003F148C[7] = 2;
                dat_003F148C[8] = 2;
            } else if (dat_003F147C) {
                dat_003F148C[7] = 2;
            }
        }
        if (dat_003F1478) {
            if (dat_003F147B) {
                dat_003F148C[7] |= 1;
                dat_003F148C[8] |= 1;
            } else if (dat_003F147C) {
                dat_003F148C[7] |= 1;
            }
        }
    }
    nnnstdMemCpy(output, dat_003F148C, 40);
    dat_003E2E30 += 10;
    fn_001E104C();
}
