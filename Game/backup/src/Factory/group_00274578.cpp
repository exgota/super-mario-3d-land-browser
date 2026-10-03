namespace {
struct Component;

struct Service {
    unsigned char reserved[0x30];
    Component* component;
};
}

extern "C" Service* fn_00277658();
extern "C" void fn_0012CCAC(Component*);
extern "C" void fn_0012CC8C(Component*);
extern "C" void fn_0012CCB8(Component*);

extern "C" void fn_00274578() {
    Component* component = fn_00277658()->component;
    if (component)
        fn_0012CCAC(component);
}

extern "C" void fn_002746DC() {
    Component* component = fn_00277658()->component;
    if (component)
        fn_0012CC8C(component);
}

extern "C" void fn_002CF494() {
    Component* component = fn_00277658()->component;
    if (component)
        fn_0012CCB8(component);
}
