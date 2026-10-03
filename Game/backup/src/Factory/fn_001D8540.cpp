namespace {

struct SwitchStates {
    int state[4];
};

struct SwitchWatcher {
    unsigned int unknown;
    SwitchStates* current;
    SwitchStates* previous;
};

}

extern "C" void fn_00244C6C(SwitchWatcher*, int, bool, unsigned int);

extern "C" void fn_001D8540(SwitchWatcher* self, unsigned int context)
{
    if (self->current->state[0] != self->previous->state[0])
        fn_00244C6C(self, 0, self->current->state[0] == 0, context);
    if (self->current->state[1] != self->previous->state[1])
        fn_00244C6C(self, 1, self->current->state[1] == 0, context);
    if (self->current->state[2] != self->previous->state[2])
        fn_00244C6C(self, 2, self->current->state[2] == 0, context);
    if (self->current->state[3] != self->previous->state[3])
        fn_00244C6C(self, 3, self->current->state[3] == 0, context);

    self->previous->state[0] = self->current->state[0];
    self->previous->state[1] = self->current->state[1];
    self->previous->state[2] = self->current->state[2];
    self->previous->state[3] = self->current->state[3];
}
