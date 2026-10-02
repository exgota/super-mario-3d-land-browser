namespace {

struct FirstCondition {
    virtual void slot0() = 0;
    virtual void slot1() = 0;
    virtual bool test() = 0;
};

struct SecondCondition {
    virtual void slot0() = 0;
    virtual void slot1() = 0;
    virtual void slot2() = 0;
    virtual void slot3() = 0;
    virtual void slot4() = 0;
    virtual void slot5() = 0;
    virtual void slot6() = 0;
    virtual void slot7() = 0;
    virtual void slot8() = 0;
    virtual bool test() = 0;
};

struct Owner {
    unsigned char unused[0x10];
    SecondCondition* second;
    FirstCondition* first;
};

struct State {
    int value;
    bool changed;

    void set(int next) {
        if (value != next) {
            value = next;
            changed = true;
        }
    }
};

struct Controller {
    unsigned char unused0[4];
    Owner* owner;
    unsigned char unused8[12];
    State* state;
    int countdown;
};

}

extern "C" void fn_0019F108(Controller* self) {
    switch (self->state->value) {
    case 0:
        if (self->countdown != 0) {
            --self->countdown;
            if (self->countdown == 0)
                self->state->set(1);
        }
        break;
    case 1:
        if (self->owner->first->test() && self->owner->second->test())
            self->state->set(2);
        break;
    case 2:
        if (!self->owner->first->test() || !self->owner->second->test())
            self->state->set(1);
        break;
    }
}
