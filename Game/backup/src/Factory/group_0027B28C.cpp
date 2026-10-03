namespace {
struct Director {
    unsigned int pad[2];
    void* value;
};
}

extern "C" void fn_00332958();

extern "C" void fn_0027B28C(Director* self) {
    if (self->value)
        fn_00332958();
}

namespace al {
class ExecuteDirector {
public:
    unsigned int pad[2];
    void* value;
    static void createExecutorListTable();
};

}

extern "C" void fn_0027B2FC(al::ExecuteDirector* self) {
    if (self->value)
        self->createExecutorListTable();
}
