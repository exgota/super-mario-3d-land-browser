namespace {
extern "C" {
extern const char dat_003BDF58[];
extern const char dat_003BDF28[];
extern const char dat_003BDF64[];
extern const unsigned char dat_003F282C;
extern const void* _ZTVN4sead14SafeStringBaseIcEE[];
}

struct SafeString {
    const void* vtable;
    const char* text;
    SafeString(const char* value)
        : vtable(_ZTVN4sead14SafeStringBaseIcEE + 2), text(value) {}
};

struct Actor {
    unsigned char pad00[8];
    unsigned char interface08[0x6c];
    float value74;
    float value78;
    unsigned char pad7c[0x40];
    void* actorbc;
    unsigned char padc0[4];
    bool flagc4;
};

extern "C" {
bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const Actor*);
bool _ZN2al6isStepEPNS_9IUseNerveEi(Actor*, int);
void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(Actor*, const void*);
int fn_0025FA40(void*, const char*);
void fn_00271330(void*, const char*);
bool fn_00273ABC(void*, const SafeString&);
void fn_0027109C(void*, const SafeString&);
void fn_00273B1C(void*, const SafeString&, int);
}
}

extern "C" void fn_0019CED8(Actor* self) {
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(self)) {
        if (self->flagc4 && !fn_0025FA40(self->actorbc, dat_003BDF58))
            fn_00271330(self->actorbc, dat_003BDF58);
        bool playing = fn_00273ABC(self->interface08, SafeString(dat_003BDF28));
        playing ^= true;
        if (playing)
            fn_0027109C(self->interface08, SafeString(dat_003BDF64));
        fn_00273B1C(self->interface08, SafeString(dat_003BDF28), 0);
    }
    if (_ZN2al6isStepEPNS_9IUseNerveEi(self, 40)) {
        self->value74 = 0.0f;
        self->value78 = 0.0f;
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(self, &dat_003F282C);
    }
}
