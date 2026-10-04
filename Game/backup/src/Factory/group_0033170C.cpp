namespace {
struct Selection {
    unsigned char reserved[0x48];
    int index;
};

struct Wrapper {
    void* target;
    unsigned char reserved[0x10];
    Selection* selection;
};
}

extern "C" void fn_00244054(void*, int);
extern "C" void fn_0024BA6C(void*, int);

extern "C" void fn_0033170C(Wrapper* self) {
    Selection* selection = self->selection;
    void* target = self->target;
    if (selection)
        fn_00244054(target, selection->index);
    else
        fn_00244054(target, -1);
}

extern "C" void fn_00331A84(Wrapper* self) {
    Selection* selection = self->selection;
    void* target = self->target;
    if (selection)
        fn_0024BA6C(target, selection->index);
    else
        fn_0024BA6C(target, -1);
}
