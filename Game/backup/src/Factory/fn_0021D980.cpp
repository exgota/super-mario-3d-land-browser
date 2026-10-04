namespace {
struct StateView {
    unsigned int pad;
    unsigned int flags;
};
}

extern "C" unsigned int fn_0021D980(const StateView *state)
{
    return state->flags & 3;
}
