namespace {
struct AnimationChannel;

struct AnimationChannels {
    char unused[0x20];
    AnimationChannel* primary;
    AnimationChannel* secondary;
    AnimationChannel* channel1;
    AnimationChannel* channel2;
    AnimationChannel* channel3;
    AnimationChannel* channel5;
};

struct AnimationHolder {
    AnimationChannels* channels;
};

struct AnimationOwner {
    char unused[0x28];
    AnimationHolder* holder;
};
}

extern "C" float fn_0026244C(AnimationChannel*, int);
extern "C" float fn_00330790(AnimationChannel*, int);
extern "C" float fn_00262430(AnimationChannel*);
extern "C" float fn_00262548(AnimationChannel*, int);
extern "C" float fn_00330740(AnimationChannel*, int);
extern "C" float fn_0026252C(AnimationChannel*);

extern "C" float fn_00244054(AnimationOwner* self, int selector) {
    if (selector == -1) {
        AnimationChannels* channels = self->holder->channels;
        if (channels->primary)
            return fn_0026244C(channels->primary, 0);
        if (channels->channel1)
            return fn_00262430(channels->channel1);
        if (channels->channel2)
            return fn_00262430(channels->channel2);
        if (channels->channel3)
            return fn_00262430(channels->channel3);
        if (channels->secondary)
            return fn_00330790(channels->secondary, 0);
        if (channels->channel5)
            return fn_00262430(channels->channel5);
    } else if (selector == 0) {
        return fn_0026244C(self->holder->channels->primary, 0);
    } else if (selector == 1) {
        return fn_00262430(self->holder->channels->channel1);
    } else if (selector == 2) {
        return fn_00262430(self->holder->channels->channel2);
    } else if (selector == 3) {
        return fn_00262430(self->holder->channels->channel3);
    } else if (selector == 4) {
        return fn_00330790(self->holder->channels->secondary, 0);
    } else if (selector == 5) {
        return fn_00262430(self->holder->channels->channel5);
    }
    return 1.0f;
}

extern "C" float fn_0024BA6C(AnimationOwner* self, int selector) {
    if (selector == -1) {
        AnimationChannels* channels = self->holder->channels;
        if (channels->primary)
            return fn_00262548(channels->primary, 0);
        if (channels->channel1)
            return fn_0026252C(channels->channel1);
        if (channels->channel2)
            return fn_0026252C(channels->channel2);
        if (channels->channel3)
            return fn_0026252C(channels->channel3);
        if (channels->secondary)
            return fn_00330740(channels->secondary, 0);
        if (channels->channel5)
            return fn_0026252C(channels->channel5);
    } else if (selector == 0) {
        return fn_00262548(self->holder->channels->primary, 0);
    } else if (selector == 1) {
        return fn_0026252C(self->holder->channels->channel1);
    } else if (selector == 2) {
        return fn_0026252C(self->holder->channels->channel2);
    } else if (selector == 3) {
        return fn_0026252C(self->holder->channels->channel3);
    } else if (selector == 4) {
        return fn_00330740(self->holder->channels->secondary, 0);
    } else if (selector == 5) {
        return fn_0026252C(self->holder->channels->channel5);
    }
    return 0.0f;
}
