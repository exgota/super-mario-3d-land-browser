namespace {
struct FieldView {
    unsigned int padding[8];
    void* field;
};
}

extern "C" void* fn_0035D6C8(void* source) {
    FieldView* fields = static_cast<FieldView*>(source);
    return fields->field;
}
