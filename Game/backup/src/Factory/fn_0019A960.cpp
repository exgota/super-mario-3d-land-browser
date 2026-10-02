namespace {
struct Entry;
struct Slots {
    Entry* entries[5];

    void insert(Entry* entry) {
        if (!entries[0]) entries[0] = entry;
        else if (!entries[1]) entries[1] = entry;
        else if (!entries[2]) entries[2] = entry;
        else if (!entries[3]) entries[3] = entry;
        else if (!entries[4]) entries[4] = entry;
    }
};
struct Holder {
    Entry* current;
    Slots* slots;
};
}

extern "C" int fn_0025357C(Entry*);
extern "C" void fn_00253560(Entry*);
extern "C" void fn_00148EC4(Entry*);

extern "C" void fn_0019A960(Holder* holder, Entry* entry) {
    if (!holder->current || fn_0025357C(holder->current) <= fn_0025357C(entry)) {
        if (holder->current) {
            holder->slots->insert(holder->current);
            fn_00253560(holder->current);
        }
        fn_00148EC4(entry);
        holder->current = entry;
    } else {
        holder->slots->insert(entry);
    }
}
