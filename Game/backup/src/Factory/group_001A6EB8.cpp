namespace {
struct WrapperObject {
    unsigned char padding[0x14];
    void* value;
};

extern "C" void* fn_00268E64(void*);
extern "C" void* fn_002519DC(void*);
extern "C" void* fn_0025BB60(void*);
}

class alProjectInterface {
public:
    void* getSystemKit();
};

namespace {
}

extern "C" void* fn_001A6EB8(void* self) {
    return static_cast<WrapperObject*>(fn_00268E64(self))->value;
}

extern "C" void* fn_00244900(void* self) {
    return static_cast<WrapperObject*>(fn_002519DC(self))->value;
}

extern "C" void* fn_00277D1C(void* self) {
    return static_cast<WrapperObject*>(fn_0025BB60(self))->value;
}

extern "C" void* fn_0027BAAC(void* self) {
    return static_cast<WrapperObject*>(static_cast<alProjectInterface*>(self)->getSystemKit())->value;
}

extern "C" void* fn_00326478(void* self) {
    return static_cast<WrapperObject*>(static_cast<alProjectInterface*>(self)->getSystemKit())->value;
}
