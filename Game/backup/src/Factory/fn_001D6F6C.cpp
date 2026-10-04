namespace {
struct StringField {
    int unknown_0;
    const char* value;
};
}

extern "C" int strcmp(const char*, const char*);

extern "C" int fn_001D6F6C(const StringField* lhs, const StringField* rhs) {
    return strcmp(lhs->value, rhs->value);
}
