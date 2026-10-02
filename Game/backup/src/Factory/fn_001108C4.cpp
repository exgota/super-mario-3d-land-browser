namespace {
struct RegionRecord {
    int width;
    int unused0;
    int unused1;
    int unused2;
};

struct RegionArray {
    unsigned int count;
    RegionRecord records[1];
};

struct RegionEntry {
    RegionArray *array;
    int unused[3];
};

struct RegionContainer {
    RegionEntry entries[1];
};

struct Layout {
    unsigned char reserved0[0x0c];
    RegionContainer *container;
    unsigned char reserved1[0x24];
    int current;
};
}

extern "C" int LMS_GetRegionWidth(Layout *layout, unsigned int index) {
    if (layout->current == -1)
        return -1;

    RegionArray *array = layout->container->entries[layout->current].array;
    if (array->count <= index)
        return -1;

    unsigned int offset = 4 + (index << 4);
    RegionRecord *record = reinterpret_cast<RegionRecord *>(
        reinterpret_cast<unsigned char *>(array) + offset);
    if (record != 0)
        return record->width;
    return -1;
}
