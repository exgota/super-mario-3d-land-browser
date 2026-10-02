#include <stdint.h>

namespace {
typedef uint32_t Word;
struct Object { uint32_t pad[2]; uint32_t value; };
}

extern "C" Word fn_001E8D14(Word);
extern "C" Word fn_001E8DCC(Word);
extern "C" Word fn_001E8E44(Word);
extern "C" Word fn_0023F7A4(Word);
extern "C" Word fn_001E8CF0(Word);
extern "C" Word fn_00250B54(Word);
extern "C" Word fn_0024C8C0(Word);
namespace al { class LiveActor; Word getTrans(const LiveActor *); }
extern "C" Word fn_00216EDC(Word);
extern "C" Word fn_00336C98(Word);

extern "C" Word fn_001EA5C0(const Object *p) { return fn_001E8D14(p->value); }
extern "C" Word fn_001EA618(const Object *p) { return fn_001E8DCC(p->value); }
extern "C" Word fn_001EA67C(const Object *p) { return fn_001E8E44(p->value); }
extern "C" Word fn_001EA730(const Object *p) { return fn_0023F7A4(p->value); }
extern "C" Word fn_00249130(const Object *p) { return fn_001E8CF0(p->value); }
extern "C" Word fn_00260448(const Object *p) { return fn_00250B54(p->value); }
extern "C" Word fn_002BD914(const Object *p) { return fn_0024C8C0(p->value); }
extern "C" Word fn_0032BD64(const Object *p) { return al::getTrans(reinterpret_cast<const al::LiveActor *>(p->value)); }
extern "C" Word fn_0032E770(const Object *p) { return fn_00216EDC(p->value); }
extern "C" Word fn_00337064(const Object *p) { return fn_00336C98(p->value); }
