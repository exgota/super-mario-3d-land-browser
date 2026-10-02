#ifndef RETAIL_GRAPHICS_BINDING_STATE_H
#define RETAIL_GRAPHICS_BINDING_STATE_H

// Clean-room field views from 00282D20 and its independently inspected callees.
// Names describe observed roles; no original SDK class or API name is claimed.
// Keep the shared pointer identity compatible with the pending GraphicsGlobals.h.
namespace retail_graphics { struct RenderControl; }
namespace retail_graphics_binding {
struct Node {
    void* payload;                 // 00
    unsigned kind;                // 04: 0, 1, 2, 3
    unsigned name;                // 08
    Node* next;                   // 0C
};
struct BindingSet {
    unsigned name;
    unsigned texture2D[3];         // 04
    unsigned textureCube[3];       // 10
    unsigned auxiliary[32];        // 1C
};
struct Auxiliary {
    unsigned name;
    unsigned char opaque[0x818];
    int sentinel;                 // 81C
};
struct State {
    unsigned dirty[0x58 / 4];
    unsigned activeTexture;        // 58
    unsigned texture2D[3];         // 5C
    unsigned textureCube[3];       // 68
    unsigned auxiliary[32];        // 74
    unsigned bindingSet;           // F4
};
struct Registry {
    unsigned opaque[2];
    BindingSet* defaults;           // 008
    unsigned pendingDelete;         // 00C
    Node* buckets[512];             // 010
    Node* texture2D[3];             // 810
    Node* textureCube[3];           // 81C
    Node* auxiliary[32];            // 828
    Node* bindingSet;               // 8A8
};
struct RegistryGlobal {
    Registry* current;
    unsigned opaque;
};
typedef void* (*Allocate)(unsigned memoryKind, unsigned category,
                          unsigned owner, unsigned bytes);
}

extern "C" void fn_00282D20(unsigned target, unsigned name);
#endif
