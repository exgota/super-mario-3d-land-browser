namespace {
struct SystemKitLink {
    void *padding;
    void *value;
};
struct SystemKitRoot {
    SystemKitLink *link;
};
}

extern "C" SystemKitRoot *fn_002913CC();

extern "C" void *fn_0026493C() {
    return fn_002913CC()->link->value;
}

namespace alProjectInterface {
SystemKitRoot *getSystemKit();
}

extern "C" void *fn_00293288() {
    return alProjectInterface::getSystemKit()->link->value;
}

