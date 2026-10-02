namespace {
struct Entry {
    unsigned short group;
    unsigned short index;
};
struct Message {
    const Entry* entry;
};
struct State {
    unsigned int unknown0;
    void* lookup;
    unsigned int unknown8;
    unsigned int unknownC;
    int value;
    unsigned int unknown14;
    signed char flag;
};
}

namespace al {
extern bool isEqualString(const char*, const char*);
}

extern "C" {
extern const char dat_003AEB44[];
extern const char dat_003AEB4C[];
extern const char dat_003AEB54[];
extern const char* fn_00244B70(void*, unsigned int, unsigned int);
extern bool fn_001DAFA8(void*, const Message*, void*, const int*, int);
extern bool fn_001E5564(State*, void*, void*, int);
extern bool fn_001D884C(State*, void*, const Message*, void*);
extern bool fn_001DB08C(void*, const Message*, void*);

bool fn_001D8750(State* self, void* receiver, const Message* message, void* context) {
    const char* name = fn_00244B70(self->lookup, message->entry->group, message->entry->index);
    if (!name)
        return fn_001E5564(self, receiver, context, 0);
    if (al::isEqualString(name, dat_003AEB44)) {
        int value = self->value;
        return fn_001DAFA8(receiver, message, context, &value, self->flag);
    }
    if (al::isEqualString(name, dat_003AEB4C))
        return fn_001D884C(self, receiver, message, context);
    if (al::isEqualString(name, dat_003AEB54))
        return fn_001DB08C(receiver, message, context);
    return true;
}
}
