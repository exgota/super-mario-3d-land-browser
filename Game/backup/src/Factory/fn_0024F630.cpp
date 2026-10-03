namespace {
struct StringRef {
    const char* value;
    ~StringRef() {}
};
bool matches(const char*, StringRef&, const char*);
struct Context { const char* primary; const char* secondary; };
struct Node {
    Node* next;
    char unknown[0xb0];
    char kind[0x11];
    char name[1];
};
struct Tree { char unknown[0x14]; Node* children; };
Tree* enclosingTree(Node*);
struct Formatted {
    struct VTable {
        void (*slot0)(Formatted*);
        void (*slot1)(Formatted*);
        void (*terminate)(Formatted*);
    };
    VTable* vtable;
    const char* value;
    char storage[0x44];
    ~Formatted() {}
};
}

extern "C" {
extern const char dat_003A9790[];
extern const char dat_003A978C[];
extern const char dat_003A9798[];
bool fn_0025799C(const char*, const StringRef&, const char*);
Formatted* fn_0027AD3C(Formatted*, const char*, ...);
void fn_00243010(Tree*, void*, const char*, void*);
void fn_0024F630(Context*, Tree*, void*, void*);
}

extern "C" void fn_0024F630(Context* context, Tree* tree, void* arg2, void* arg3) {
    Node* end = reinterpret_cast<Node*>(&tree->children);
    for (Node* node = tree->children; node != end; node = node->next) {
        bool isKind;
        {
            StringRef key;
            isKind = matches(node->kind, key, dat_003A9790);
        }
        if (isKind) {
            const char* value = 0;
            bool isName;
            {
                StringRef key;
                isName = matches(node->name, key, dat_003A978C);
            }
            {
                Formatted temporary;
                if (!isName) {
                    const char* base = context->secondary;
                    if (!base) base = context->primary;
                    Formatted* formatted = fn_0027AD3C(&temporary, dat_003A9798, base, node->name);
                    formatted->vtable->terminate(formatted);
                    value = formatted->value;
                }
                fn_00243010(enclosingTree(node), arg2, value, arg3);
            }
        } else {
            fn_0024F630(context, enclosingTree(node), arg2, arg3);
        }
    }
}

namespace {
bool matches(const char* text, StringRef& key, const char* value) {
    key.value = value;
    return fn_0025799C(text, key, value);
}
Tree* enclosingTree(Node* node) {
    return reinterpret_cast<Tree*>(reinterpret_cast<char*>(node) - 4);
}
}
