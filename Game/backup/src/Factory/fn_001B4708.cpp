namespace {

struct Entry {
    unsigned int first;
    unsigned int second;
};

struct PointerList {
    int count;
    int capacity;
    Entry** entries;

    PointerList() : count(0), capacity(0), entries(0) {}

    int append(Entry* entry);
};

struct Owner {
    PointerList* first;
    PointerList* second;
};

}

extern "C" float dat_003EFAE0;
extern "C" void fn_0026AC60(PointerList*, int, void*, int);

extern "C" Owner* fn_001B4708(Owner* self) {
    self->first = new PointerList;
    self->second = new PointerList;
    fn_0026AC60(self->first, static_cast<int>(dat_003EFAE0), 0, 4);
    fn_0026AC60(self->second, static_cast<int>(dat_003EFAE0), 0, 4);
    for (int i = 0; i < dat_003EFAE0; ++i) {
        self->second->append(new Entry);
    }
    return self;
}

namespace {

int PointerList::append(Entry* entry) {
    int limit = capacity;
    int size = count;
    if (size < limit) {
        entries[size] = entry;
        return ++count;
    }
    return size;
}

}
