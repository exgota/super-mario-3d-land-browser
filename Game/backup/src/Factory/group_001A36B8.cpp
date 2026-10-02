namespace {
struct Entry {
    unsigned char pad[4];
    float value;
    unsigned int state;
};

struct Owner {
    unsigned char pad[8];
    Entry **entries;
};
}

extern "C" void fn_001A36B8(Owner *self, unsigned int index, unsigned int state) {
    Entry *entry = self->entries[index];
    entry->value = 1.0f;
    entry->state = state;
}
