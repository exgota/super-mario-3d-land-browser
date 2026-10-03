namespace {
struct Opaque;
}

extern "C" Opaque* fn_002913CC(Opaque*, Opaque*);
extern "C" Opaque* fn_0014C400(Opaque*, Opaque*, Opaque*);
extern "C" Opaque* fn_0028BA28(Opaque*, Opaque*);
extern "C" Opaque* fn_00296250(Opaque*, Opaque*, Opaque*);

extern "C" Opaque* fn_0014C3E0(Opaque* first, Opaque* second) {
    return fn_0014C400(fn_002913CC(first, second), first, second);
}

extern "C" Opaque* fn_00296230(Opaque* first, Opaque* second) {
    return fn_00296250(fn_0028BA28(first, second), first, second);
}
