namespace {
struct Count {
    int value;
};

struct Settings {
    char pad0[0x188];
    float span;
    char pad18c[0x10];
    float scale;
};

struct Source {
    char pad0[0x40];
    float span;
    char pad44[8];
    float scale;
    char pad50[0x30];
    int count;
};

struct Emitter {
    char pad0[0xc];
    Settings* settings;
    Count* count;
    char pad14[0x94];
    Source* source;
};

struct Particle {
    float x;
    float y;
    float z;
    char padc[0xc];
    float vx;
    float vy;
    float vz;
    char pad24[0xf0];
    Source* source;
    char pad118[8];
    int state;
};

struct Manager;
extern "C" Manager* dat_003EF914;
extern "C" Particle* fn_0021FFCC(Manager*);
extern "C" void fn_0021FA0C(Emitter*, Particle*);
}

extern "C" void fn_002E7468(Emitter* self) {
    Source* source = self->source;
    int count = (source->count * self->count->value) >> 8;
    Settings* settings = self->settings;
    if (count == 0)
        return;

    float scale = source->scale * settings->scale;
    float span = source->span * settings->span;
    float position = 0.0f;
    float step = 1.0f;
    if (count == 1) {
        step = 0.0f;
        position = 0.5f;
    } else {
        step /= (count - 1);
    }
    for (int i = 0; i < count; ++i) {
        Particle* particle = fn_0021FFCC(dat_003EF914);
        if (particle) {
            float z = position * span;
            particle->source = source;
            position += step;
            particle->state = 0;
            particle->vz = scale;
            particle->x = 0.0f;
            particle->y = 0.0f;
            particle->vx = 0.0f;
            particle->vy = 0.0f;
            particle->z = z;
            fn_0021FA0C(self, particle);
        }
    }
}
