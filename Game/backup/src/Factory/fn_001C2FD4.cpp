namespace {
struct Event {
    int unknown;
    int frame;
    const char* name;
    bool hasAction;
    unsigned char padding[3];
    int action;
    bool alternate;
    bool enabled;
    bool fired;
    unsigned char tail;
};

struct EventList {
    int unknown;
    int count;
    Event* events;
};
}

extern "C" bool fn_0024E6C8(float, float, float);
extern "C" void* fn_0015C958();
extern "C" void fn_0025FB98(void*, const char*);
extern "C" void fn_0025FB50(void*, const char*);
extern "C" void fn_001D34D8(void*, int);

extern "C" void fn_001C2FD4(EventList* self, float previous, float current) {
    for (int i = 0; i < self->count; ++i) {
        Event* event = &self->events[i];
        if (event->enabled && !event->fired &&
            fn_0024E6C8(previous, current, static_cast<float>(event->frame))) {
            if (event->name) {
                if (event->alternate)
                    fn_0025FB98(fn_0015C958(), event->name);
                else
                    fn_0025FB50(fn_0015C958(), event->name);
            }
            if (event->hasAction)
                fn_001D34D8(fn_0015C958(), event->action);
            event->fired = true;
        }
    }
}
