namespace {
struct State {
    unsigned int reserved;
    unsigned int actionName;
    unsigned int actionGroupName;
};
}

extern "C" void fn_001CD450(void* state) {
    State* fields = static_cast<State*>(state);
    fields->actionName = 0;
    fields->actionGroupName = 0;
}
