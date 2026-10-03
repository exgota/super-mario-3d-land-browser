namespace {
struct First {
    int padding;
    void* next;
};

struct Second {
    int padding[3];
    int* values;
};
}

extern "C" int* fn_0032EADC(First* object, int index) {
    return &static_cast<Second*>(object->next)->values[index * 3];
}

extern "C" int* fn_0032ED54(First* object, int index) {
    return &static_cast<Second*>(object->next)->values[index * 3];
}
