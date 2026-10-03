namespace {
struct Item;
}

extern "C" void fn_001E8DDC(Item*, int);
extern "C" void fn_002490CC(Item*, int);
extern "C" void fn_00248EBC(Item*, int);
extern "C" void fn_0025030C(Item*, int, int);
extern "C" int fn_002503CC(Item*, const char*);
extern "C" int fn_001EA57C(Item*);

namespace {
struct Item {
    char reserved[0x10];
    bool active;
    void setActive(bool value) { active = value; }
    void setMode(int value) { fn_001E8DDC(this, value); }
    void setFrame(int value) { fn_00248EBC(this, value); }
    void setFrames(int value, int other) { fn_0025030C(this, value, other); }
};

struct Owner {
    char reserved0[0x18];
    Item* first;
    Item* second;
    Item* third;
    Item* fourth;
    Item* fifth;
    char reserved1[0x0d];
    bool reset;
    void setAllModes(int value);
    void setBothFrames(int value);
};
}

extern "C" int fn_00248FD8(Owner* self, const char* name) {
    int mode;
    bool reset = self->reset;
    if (reset)
        mode = 2;
    if (reset)
        self->setAllModes(mode);
    self->first->setActive(true);
    fn_002490CC(self->second, 0);
    fn_002490CC(self->fifth, 0);
    self->setBothFrames(0);
    self->fourth->setActive(false);
    int result;
    if (name)
        result = fn_002503CC(self->first, name);
    else
        result = fn_001EA57C(self->first);
    if (self->third->active)
        fn_002490CC(self->first, 0);
    return result;
}

namespace {
void Owner::setAllModes(int value) {
    first->setMode(value);
    second->setMode(value);
    third->setMode(value);
    fourth->setMode(value);
}

void Owner::setBothFrames(int value) {
    first->setFrame(value);
    fourth->setFrames(value, 0);
}
}
