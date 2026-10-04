#include <Container/ObservedCountFirstPointerArray.h>

namespace {

struct Entry {
    unsigned int first;
    unsigned int second;
};

typedef observed_containers::CountFirstPointerArray<Entry> PointerList;

struct Owner {
    PointerList* first;
    PointerList* second;
};

}

extern "C" float dat_003EFAE0;

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

