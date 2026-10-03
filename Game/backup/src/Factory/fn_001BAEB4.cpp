namespace {
struct Link;
struct Target {
    virtual void slot00() = 0;
    virtual void slot04() = 0;
    virtual void slot08() = 0;
    virtual void slot0c() = 0;
    virtual void slot10() = 0;
    virtual void slot14() = 0;
    virtual void slot18() = 0;
    virtual void slot1c() = 0;
    virtual void slot20() = 0;
    virtual void slot24() = 0;
    virtual void slot28() = 0;
    virtual void slot2c() = 0;
    virtual void slot30() = 0;
    virtual bool dispatch(int, void*, Link*) = 0;
};
struct Link {
    char padding[0x28];
    Target* target;
    __attribute__((weak)) Target* getTarget();
};
struct Context {
    char padding[0x120];
    Link* link;
};
struct Sender {
    void* unknown;
    void* actor;
};
extern "C" bool fn_0026EA74(Link*);
}

extern "C" bool fn_001BAEB4(Sender* self, Context* context, unsigned kind) {
    int message;
    switch (kind) {
    default:
    case 0: message = 4; break;
    case 1: message = 27; break;
    case 2: message = 28; break;
    case 3: message = 29; break;
    case 4: message = 30; break;
    case 5: message = 5; break;
    case 6: message = 8; break;
    case 7: message = 2; break;
    case 8: message = 14; break;
    case 9: message = 13; break;
    }
    Link* link = context->link;
    if (!link) return false;
    if (!link->getTarget()) return false;
    if (!fn_0026EA74(link)) return false;
    return link->getTarget()->dispatch(message, self->actor, link);
}

namespace {
Target* Link::getTarget() { return target; }
}
