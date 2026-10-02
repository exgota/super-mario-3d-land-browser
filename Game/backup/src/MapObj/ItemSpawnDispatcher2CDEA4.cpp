// NonMatching: whole item-spawn dispatch at 0x002CDEA4..0x002CE700.
// Reconstructed from the owner's EU executable. No public class layout changes.
#include <MapObj/ItemHolder.h>
#include <LiveActor/alLiveActor.h>
#include <LiveActor/alActorPoseKeeper.h>
#include <Scene/alSceneObjHolder.h>

// Preserve the C declaration already used by the pending StreetPassObj family.
// These types are opaque here: the adapter does not redefine their layouts.
namespace dot14414c { struct Actor; struct Quat; }
extern "C" {
al::LiveActor* fn_00225E70(ItemHolder*, const al::LiveActor*);
al::LiveActor* fn_00225F68(ItemHolder*, const al::LiveActor*);
al::LiveActor* fn_00226060(ItemHolder*, const al::LiveActor*);
al::LiveActor* fn_00226158(ItemHolder*, const al::LiveActor*);
al::LiveActor* fn_0022624C(ItemHolder*, const al::LiveActor*);
al::LiveActor* fn_00226344(ItemHolder*, const al::LiveActor*);
al::LiveActor* fn_0022643C(ItemHolder*, const al::LiveActor*);
al::LiveActor* fn_00226534(ItemHolder*, const al::LiveActor*);
al::LiveActor* fn_0022662C(ItemHolder*, const al::LiveActor*);
al::LiveActor* fn_00226724(ItemHolder*, const al::LiveActor*);
al::LiveActor* fn_0022681C(ItemHolder*, const al::LiveActor*);
al::LiveActor* fn_00226910(ItemHolder*, const al::LiveActor*);
void fn_00226A04(const al::LiveActor*, const sead::Vector3f&, const sead::Vector3f&, int, float, float);
al::LiveActor* fn_00227490(ItemHolder*, const al::LiveActor*);
al::LiveActor* fn_00227588(ItemHolder*, const al::LiveActor*);
void fn_0022767C(al::LiveActor*, int);
al::LiveActor* fn_002276D8(ItemHolder*, const al::LiveActor*);
void fn_00227CF0(const al::LiveActor*, const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&, int, const sead::Quatf*, float);
void fn_00271028(dot14414c::Actor*, const dot14414c::Quat*);
void fn_002783BC(sead::Quatf*, const sead::Quatf&, float);
void fn_0027C05C(al::LiveActor*);
bool fn_0027F2C0();
}

namespace dot2cdea4
{
// Only the two observed appearance entries are called. The middle slot is not
// interpreted. The interfaces begin at item+0x60 or item+0x64, depending on pool.
struct Appearance;
typedef void (*Appear)(Appearance*, const sead::Vector3f&, const sead::Quatf&);
struct AppearanceDispatch { Appear appear; void* unobserved04; Appear alternate; };
struct Appearance { const AppearanceDispatch* dispatch; };
inline ItemHolder* holder() { return static_cast<ItemHolder*>(al::getSceneObj(10)); }
inline bool flag(unsigned int offset)
{
    // Character access is an observed view of accepted ItemHolder flags50..52.
    return reinterpret_cast<const unsigned char*>(holder())[offset] != 0;
}
inline Appearance* appearance(al::LiveActor* item, unsigned int offset)
{
    return item ? reinterpret_cast<Appearance*>(reinterpret_cast<unsigned char*>(item) + offset) : 0;
}
inline void appear(al::LiveActor* item, unsigned int offset,
                   const sead::Vector3f& position, const sead::Quatf& orientation)
{
    Appearance* target = appearance(item, offset);
    target->dispatch->appear(target, position, orientation);
}
}

#ifdef NON_MATCHING
extern "C" void fn_002CDEA4(unsigned int kind, const al::LiveActor* source,
                            const sead::Vector3f& position, const sead::Quatf& orientation)
{
    using namespace dot2cdea4;
    switch (kind)
    {
    case 1:
        if (flag(0x50)) {
            al::LiveActor* item = fn_002276D8(holder(), source);
            al::setTrans(item, position);
            fn_0027C05C(item);
            fn_0022767C(item, 0);
        }
        break;
    case 2:
        if (flag(0x50)) {
            al::LiveActor* item = fn_002276D8(holder(), source);
            al::setTrans(item, position);
            fn_0027C05C(item);
            fn_0022767C(item, 0);
        }
        break;
    case 3: {
        const sead::Vector3f& gravity = al::getGravity(source);
        sead::Vector3f up(-gravity.x, -gravity.y, -gravity.z);
        fn_00226A04(source, position, up, 1, 37.5f, 0.9f);
        break;
    }
    case 4:
        if (flag(0x50)) {
            al::LiveActor* item = fn_002276D8(holder(), source);
            al::setTrans(item, position);
            fn_0027C05C(item);
            fn_0022767C(item, 0);
        }
        break;
    case 5: {
        const sead::Vector3f& gravity = al::getGravity(source);
        const sead::Vector3f& origin = al::getTrans(source);
        fn_00227CF0(source, origin, gravity, position, 1, &orientation, 0.7f);
        break;
    }
    case 6: {
        const sead::Vector3f& gravity = al::getGravity(source);
        const sead::Vector3f& origin = al::getTrans(source);
        fn_00227CF0(source, origin, gravity, position, 3, &orientation, 0.7f);
        break;
    }
    case 7: {
        const sead::Vector3f& gravity = al::getGravity(source);
        const sead::Vector3f& origin = al::getTrans(source);
        fn_00227CF0(source, origin, gravity, position, 5, &orientation, 0.7f);
        break;
    }
    case 8: {
        const sead::Vector3f& gravity = al::getGravity(source);
        const sead::Vector3f& origin = al::getTrans(source);
        fn_00227CF0(source, origin, gravity, position, 5, &orientation, 1.0f);
        break;
    }
    case 9: {
        const sead::Vector3f& gravity = al::getGravity(source);
        const sead::Vector3f& origin = al::getTrans(source);
        fn_00227CF0(source, origin, gravity, position, 10, &orientation, 0.7f);
        break;
    }
    case 10: {
        const sead::Vector3f& gravity = al::getGravity(source);
        sead::Vector3f up(-gravity.x, -gravity.y, -gravity.z);
        fn_00226A04(source, position, up, 5, 37.5f, 0.9f);
        break;
    }
    case 11:
        if (flag(0x52)) {
            sead::Quatf rotated = orientation;
            fn_002783BC(&rotated, rotated, -30.0f);
            appear(fn_00227588(holder(), source), 0x64, position, rotated);
            fn_002783BC(&rotated, rotated, 60.0f);
            appear(fn_00227588(holder(), source), 0x64, position, rotated);
        } else {
            appear(fn_00227588(holder(), source), 0x64, position, orientation);
        }
        break;
    case 12: appear(fn_00226910(holder(), source), 0x64, position, orientation); break;
    case 13: appear(fn_00227490(holder(), source), 0x60, position, orientation); break;
    case 14: appear(fn_0022681C(holder(), source), 0x60, position, orientation); break;
    case 15:
        if (fn_0027F2C0()) appear(fn_00227490(holder(), source), 0x60, position, orientation);
        else appear(fn_00226724(holder(), source), 0x60, position, orientation);
        break;
    case 16: appear(fn_00226724(holder(), source), 0x60, position, orientation); break;
    case 17:
        if (fn_0027F2C0()) appear(fn_00227490(holder(), source), 0x60, position, orientation);
        else if (flag(0x51)) appear(fn_0022662C(holder(), source), 0x60, position, orientation);
        else appear(fn_00226534(holder(), source), 0x60, position, orientation);
        break;
    case 18:
        if (flag(0x51)) appear(fn_0022662C(holder(), source), 0x60, position, orientation);
        else appear(fn_00226534(holder(), source), 0x60, position, orientation);
        break;
    case 19: appear(fn_00226534(holder(), source), 0x60, position, orientation); break;
    case 20: appear(fn_0022662C(holder(), source), 0x60, position, orientation); break;
    case 21:
        if (fn_0027F2C0()) appear(fn_00227490(holder(), source), 0x60, position, orientation);
        else appear(fn_0022643C(holder(), source), 0x60, position, orientation);
        break;
    case 22: appear(fn_0022643C(holder(), source), 0x60, position, orientation); break;
    case 23: {
        Appearance* target = appearance(fn_00226344(holder(), source), 0x60);
        target->dispatch->alternate(target, position, orientation);
        break;
    }
    case 24: appear(fn_0022624C(holder(), source), 0x64, position, orientation); break;
    case 25: appear(fn_00226158(holder(), source), 0x64, position, orientation); break;
    case 26: appear(fn_00226060(holder(), source), 0x60, position, orientation); break;
    case 27: appear(fn_00225F68(holder(), source), 0x60, position, orientation); break;
    case 29: {
        al::LiveActor* item = fn_00225E70(holder(), source);
        fn_00271028(reinterpret_cast<dot14414c::Actor*>(item),
                    reinterpret_cast<const dot14414c::Quat*>(&orientation));
        al::setTrans(item, position);
        item->appear();
        break;
    }
    default: break;
    }
}

#endif // NON_MATCHING
