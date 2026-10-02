#include <retail/GraphicsBindingState.h>

#ifdef NON_MATCHING
// NonMatching: complete clean-room proposal for 00282D20..00283594.
using namespace retail_graphics_binding;
extern "C" {
extern retail_graphics::RenderControl* dat_003E3154;
extern RegistryGlobal dat_003E3180;
extern Allocate dat_003E2654;
void* fn_0028B290(unsigned name);
void* fn_0028B1E0(unsigned name);
void fn_00210850(Node*, unsigned name);
void fn_0028D1F0(void*, unsigned bytes);
void fn_00282650(int count, unsigned* names);
}
static_assert_(sizeof(Node) == 0x10);
static_assert_(sizeof(BindingSet) == 0x9c);
static_assert_(sizeof(Auxiliary) == 0x820);
static_assert_(offsetof(State, activeTexture) == 0x58);
static_assert_(offsetof(State, bindingSet) == 0xf4);
static_assert_(offsetof(Registry, buckets) == 0x10);
static_assert_(offsetof(Registry, texture2D) == 0x810);
static_assert_(offsetof(Registry, textureCube) == 0x81c);
static_assert_(offsetof(Registry, auxiliary) == 0x828);
static_assert_(offsetof(Registry, bindingSet) == 0x8a8);

static inline Node* findNode(Node* node, unsigned name)
{
    while (node && node->name != name) node = node->next;
    return node;
}

static inline void* allocate(Allocate callback, unsigned bytes)
{
    return callback ? callback(0x10000, 0x100, 0, bytes) : 0;
}

static inline void* makePayload(unsigned name, unsigned kind)
{
    if (kind == 0) return fn_0028B290(name);
    if (kind == 1) return fn_0028B1E0(name);
    if (kind == 2) {
        BindingSet* set = static_cast<BindingSet*>(allocate(dat_003E2654, sizeof(BindingSet)));
        if (set) {
            fn_0028D1F0(set, sizeof(BindingSet));
            set->name = name;
        }
        return set;
    }
    Auxiliary* value = static_cast<Auxiliary*>(allocate(dat_003E2654, sizeof(Auxiliary)));
    if (value) {
        fn_0028D1F0(value, sizeof(Auxiliary));
        value->sentinel = -1;
        value->name = name;
    }
    return value;
}

static inline Node* obtainNode(Node* first, unsigned name, unsigned kind, Allocate callback)
{
    Node* node = findNode(first, name);
    if (!node) {
        node = static_cast<Node*>(allocate(callback, sizeof(Node)));
        node->payload = makePayload(name, kind);
        node->kind = kind;
        node->name = name;
        node->next = 0;
        fn_00210850(node, name);
    } else if (!node->payload) {
        node->payload = makePayload(name, kind);
        node->kind = kind;
    }
    return node;
}

static inline BindingSet* selectedSet(Registry* registry)
{
    return registry->bindingSet ? static_cast<BindingSet*>(registry->bindingSet->payload)
                                : registry->defaults;
}

static inline void markUnit(State* state, unsigned unit)
{
    unsigned bit = unit + 10;
    state->dirty[bit >> 5] |= 1u << (bit & 31);
}

extern "C" void fn_00282D20(unsigned target, unsigned name)
{
    Node* node = 0;
    State* state = reinterpret_cast<State*>(dat_003E3154);
    Node** bucket = &dat_003E3180.current->buckets[name & 511];
    Allocate callback = dat_003E2654;
    switch (target) {
    case 0xde1: {
        if (state->texture2D[state->activeTexture] == name) return;
        if (name) node = obtainNode(*bucket, name, 0, callback);
        Registry* registry = dat_003E3180.current;
        selectedSet(registry)->texture2D[state->activeTexture] = name;
        state->texture2D[state->activeTexture] = name;
        registry->texture2D[state->activeTexture] = node;
        markUnit(state, state->activeTexture);
        break;
    }
    case 0x8513: {
        if (state->textureCube[state->activeTexture] == name) return;
        if (name) node = obtainNode(*bucket, name, 1, callback);
        Registry* registry = dat_003E3180.current;
        selectedSet(registry)->textureCube[state->activeTexture] = name;
        state->textureCube[state->activeTexture] = name;
        registry->textureCube[state->activeTexture] = node;
        markUnit(state, state->activeTexture);
        break;
    }
    case 0x6600: {
        if (state->bindingSet == name) return;
        if (name) node = obtainNode(*bucket, name, 2, callback);
        Registry* registry = dat_003E3180.current;
        bool changed = false;
        for (int i = 0; i < 32; ++i) {
            unsigned value = (name ? static_cast<BindingSet*>(node->payload) : registry->defaults)->auxiliary[i];
            if (state->auxiliary[i] != value) {
                state->auxiliary[i] = value;
                registry->auxiliary[i] = value ? findNode(registry->buckets[value & 511], value) : 0;
                changed = true;
            }
        }
        if (changed) state->dirty[0] |= 0x4000;
        for (int i = 0; i < 3; ++i) {
            BindingSet* set = name ? static_cast<BindingSet*>(node->payload) : registry->defaults;
            unsigned texture = set->texture2D[i];
            unsigned cube = set->textureCube[i];
            if (state->texture2D[i] != texture || state->textureCube[i] != cube) {
                state->texture2D[i] = texture;
                registry->texture2D[i] = texture ? findNode(registry->buckets[texture & 511], texture) : 0;
                state->textureCube[i] = cube;
                registry->textureCube[i] = cube ? findNode(registry->buckets[cube & 511], cube) : 0;
                markUnit(state, i);
            }
        }
        state->bindingSet = name;
        registry->bindingSet = node;
        if (registry->pendingDelete) {
            unsigned old = registry->pendingDelete;
            fn_00282650(1, &old);
            dat_003E3180.current->pendingDelete = 0;
        }
        break;
    }
    case 0x6610: case 0x6611: case 0x6612: case 0x6613:
    case 0x6614: case 0x6615: case 0x6616: case 0x6617:
    case 0x6618: case 0x6619: case 0x661a: case 0x661b:
    case 0x661c: case 0x661d: case 0x661e: case 0x661f:
    case 0x6620: case 0x6621: case 0x6622: case 0x6623:
    case 0x6624: case 0x6625: case 0x6626: case 0x6627:
    case 0x6628: case 0x6629: case 0x662a: case 0x662b:
    case 0x662c: case 0x662d: case 0x662e: case 0x662f: {
        unsigned index = target - 0x6610;
        if (state->auxiliary[index] == name) return;
        if (name) node = obtainNode(*bucket, name, 3, callback);
        Registry* registry = dat_003E3180.current;
        selectedSet(registry)->auxiliary[index] = name;
        state->auxiliary[index] = name;
        registry->auxiliary[index] = node;
        state->dirty[0] |= 0x4000;
        break;
    }
    }
}
#endif
