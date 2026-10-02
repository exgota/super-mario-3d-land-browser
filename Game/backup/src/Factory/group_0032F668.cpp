namespace {
struct ThreeWords {
    unsigned int first;
    unsigned int second;
    unsigned int third;
};
}

extern "C" void fn_0032F668(const ThreeWords* source, ThreeWords* destination)
{
    destination->first = source->first;
    destination->second = source->second;
    destination->third = source->third;
}

extern "C" void fn_0032FB38(const ThreeWords* source, ThreeWords* destination)
{
    destination->first = source->first;
    destination->second = source->second;
    destination->third = source->third;
}
