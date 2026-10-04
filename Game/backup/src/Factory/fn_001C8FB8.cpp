namespace {

struct Entry {
    const char* name;
    unsigned short kind;
    short first;
    short second;
    short unused;
};

struct State {
    const char* name;
    int unused;
    Entry* entries;
};

struct EntryList {
    void* unused;
    int count;
};

struct Controller {
    void* object;
    void* unused;
    EntryList* list;
    int stateCount;
    State** states;
    State* current;
    bool active;
};

extern "C" bool fn_0024C7EC(const char*, const char*);
extern "C" void fn_001C8C54(Controller*);
extern "C" void fn_001EBB10(void*, const char*);
extern "C" void fn_001EB94C(void*, const char*);

}

extern "C" void fn_001C8FB8(Controller* self, const char* name) {
    State* state;
    for (int i = 0; i < self->stateCount; ++i) {
        state = self->states[i];
        if (fn_0024C7EC(state->name, name))
            goto found;
    }
    state = 0;
found:
    self->current = state;
    self->active = false;
    if (self->current) {
        fn_001C8C54(self);
        if (self->list) {
            int i = 0;
            goto check;
process:
            {
                Entry* entry = &self->current->entries[i];
                if (entry->first > 0) {
                    self->active = true;
                } else {
                    if (entry->second > 0)
                        self->active = true;
                    if (entry->kind == 3) {
                        fn_001EBB10(self->object, entry->name);
                    } else if (entry->kind == 2) {
                        fn_001EB94C(self->object, entry->name);
                    }
                }
            }
            ++i;
check:
            if (i < self->list->count)
                goto process;
        }
    }
}
