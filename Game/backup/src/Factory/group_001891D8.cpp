namespace {
struct ApplicationPart {
    unsigned char padding[0x1B0];
    void* value;
};
}

namespace al {
extern void* getApplication();
}

extern "C" void* fn_0028E678(void*);
extern "C" void* fn_0011A710(void*);
extern "C" void* fn_001EC46C(void*);
extern "C" void* fn_001EC430(void*);

extern "C" void* fn_001891D8() {
    return fn_0011A710(static_cast<ApplicationPart*>(fn_0028E678(al::getApplication()))->value);
}

extern "C" void* fn_00274480() {
    return fn_001EC46C(static_cast<ApplicationPart*>(fn_0028E678(al::getApplication()))->value);
}

extern "C" void* fn_00276858() {
    return fn_001EC430(static_cast<ApplicationPart*>(fn_0028E678(al::getApplication()))->value);
}
