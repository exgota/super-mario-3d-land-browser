// Complete EU root 002EDFE8..002EE878, including both sides of its literal pool.
// NonMatching. Actor/791 is a provisional carrier, not an original-module claim.
#ifdef NON_MATCHING
#include <Effect/retail_ParticleEditorParameters.h>

namespace editor2edfe8 {
inline void defaultColor(Color& color) {
    color.r = dat_003A211C;
    color.g = dat_003A211C;
    color.b = dat_003A211C;
    color.a = dat_003A2118;
}
inline Color clampedColor(float r, float g, float b, float a) {
    Color color = {r, g, b, a};
    fn_00292D20(&color);
    return color;
}
}

extern "C" editor2edfe8::Parameters* fn_002EDFE8(editor2edfe8::Parameters* p, void* owner) {
    using namespace editor2edfe8;
    p->word034 = 0;
    fn_0021F264(&p->range0);
    fn_0021F264(&p->positive0);
    fn_0021F264(&p->range2);
    fn_0021F264(&p->positive2);
    fn_0021F264(&p->smallPositive2);
    fn_0021F21C(&p->autoPositive2);
    fn_0021F21C(&p->autoRange1);
    fn_0021F21C(&p->autoPositive1);
    fn_0021F21C(&p->autoRange3);
    fn_0021F21C(&p->autoPositive3);
    fn_0021F264(&p->integer0);
    fn_0021F264(&p->integer1);
    fn_0021F264(&p->disabledRange0);
    fn_0021F264(&p->disabledPositive0);
    fn_0021F264(&p->disabledPositive2);
    fn_0021F21C(&p->disabledAutoRange1);
    fn_0021F21C(&p->disabledAutoPositive1);
    fn_0021F264(&p->disabledAutoRange3);
    fn_0021F21C(&p->disabledAutoPositive3);
    fn_0021F264(&p->disabledInteger0);
    fn_0021F264(&p->disabledInteger1);
    fn_0021F264(&p->largeInteger0);
    fn_0021F264(&p->largeInteger1);
    fn_0021F264(&p->disabledLargeInteger0);
    fn_0021F264(&p->disabledLargeInteger1);

    p->limit0 = 100.0f;
    p->limit2 = 1.0f;
    p->limit1 = 10.0f;
    p->limit3 = 10.0f;
    p->integerLimit = 1000;
    fn_0028E1E4(p->range0.string(), "Min=%5.1f,Max=%5.1f", -100.0, 100.0);
    fn_0028E1E4(p->positive0.string(), "Min=0,Max=%5.1f", p->limit0);
    fn_0028E1E4(p->range2.string(), "Min=%5.1f,Max=%5.1f", -p->limit2, p->limit2);
    fn_0028E1E4(p->positive2.string(), "Min=0,Max=%5.1f", p->limit2);
    fn_0028E1E4(p->smallPositive2.string(), "Min=0,Max=%5.1f", p->limit2 * 0.1f);
    fn_0028E1E4(p->autoRange1.string(), "Min=%5.1f,Max=%5.1f,Menu=True,MenuDefault=Auto", -p->limit1, p->limit1);
    fn_0028E1E4(p->autoPositive1.string(), "Min=0,Max=%5.1f,Menu=True,MenuDefault=Auto", p->limit1);
    fn_0028E1E4(p->autoRange3.string(), "Min=%5.1f,Max=%5.1f,Menu=True,MenuDefault=Auto", -p->limit3, p->limit3);
    fn_0028E1E4(p->autoPositive3.string(), "Min=0,Max=%5.1f,Menu=True,MenuDefault=Auto", p->limit3);
    fn_0028E1E4(p->integer0.string(), "Min=0,Max=%d", p->integerLimit);
    fn_0028E1E4(p->integer1.string(), "Min=1,Max=%d", p->integerLimit);
    fn_0028E1E4(p->disabledRange0.string(), "%s,IsEnable=False", p->range0.cstr());
    fn_0028E1E4(p->disabledPositive0.string(), "%s,IsEnable=False", p->positive0.cstr());
    fn_0028E1E4(p->disabledPositive2.string(), "%s,IsEnable=False", p->positive2.cstr());
    fn_0028E1E4(p->disabledAutoRange1.string(), "%s,IsEnable=False", p->autoRange1.cstr());
    fn_0028E1E4(p->disabledAutoRange3.string(), "%s,IsEnable=False", p->autoRange3.cstr());
    fn_0028E1E4(p->disabledAutoPositive3.string(), "%s,IsEnable=False", p->autoPositive3.cstr());
    fn_0028E1E4(p->disabledInteger0.string(), "%s,IsEnable=False", p->integer0.cstr());
    fn_0028E1E4(p->disabledAutoPositive1.string(), "%s,IsEnable=False", p->autoPositive1.cstr());
    fn_0028E1E4(p->disabledInteger1.string(), "%s,IsEnable=False", p->integer1.cstr());
    fn_0028E1E4(p->autoPositive2.string(), "%s,Menu=True,MenuDefault=Auto", p->positive2.cstr());
    fn_0028E1E4(p->largeInteger0.string(), "Min=0,Max=%d", p->integerLimit * 10);
    fn_0028E1E4(p->largeInteger1.string(), "Min=1,Max=%d", p->integerLimit * 10);
    fn_0028E1E4(p->disabledLargeInteger0.string(), "%s,IsEnable=False", p->largeInteger0.cstr());
    fn_0028E1E4(p->disabledLargeInteger1.string(), "%s,IsEnable=False", p->largeInteger1.cstr());

    Defaults& d = p->defaults;
    defaultColor(d.color0);
    defaultColor(d.color1);
    defaultColor(d.color2);
    defaultColor(d.color3);
    defaultColor(d.color4);
    defaultColor(d.color5);
    defaultColor(d.color6);
    d.direction.x = -0.5f;
    d.direction.y = -1.0f;
    d.direction.z = -0.5f;
    _ZN4sead14Vector3CalcCtrIfE9normalizeERN2nn4math4VEC3E(d.direction);
    d.color0 = clampedColor(1.0f, 1.0f, 1.0f, 1.0f);
    d.color1 = clampedColor(0.2f, 0.2f, 0.2f, 1.0f);
    d.color2 = clampedColor(1.0f, 1.0f, 1.0f, 1.0f);
    d.color3 = clampedColor(1.0f, 1.0f, 1.0f, 1.0f);
    d.scale.x = 0.0f;
    d.scale.y = 1.0f;
    d.scale.z = 0.0f;
    d.color4 = clampedColor(0.9f, 1.0f, 1.0f, 1.0f);
    d.color5 = clampedColor(0.1f, 0.0f, 0.0f, 1.0f);
    d.scalar78 = 0.5f;
    d.color6 = clampedColor(1.0f, 1.0f, 1.0f, 1.0f);
    d.scalar8c = 0.0f;
    d.scalar90 = 200.0f;
    d.scalar94 = 2.0f;
    d.word98 = 0;
    __aeabi_vec_ctor_nocookie_nodtor(p->indexed, fn_0021F264, sizeof(String64), 16);
    __aeabi_vec_ctor_nocookie_nodtor(p->numeric, fn_0021F264, sizeof(String64), 2);
    p->owner = owner;
    _ZN4sead15Matrix34CalcCtrIfE4copyERN2nn4math5MTX34ERKS4_(p->matrix, dat_00430A88);
    p->scalarEe0 = 1.0f;
    for (int i = 0; i < 16; ++i)
        fn_0028E1E4(p->indexed[i].string(), "[%d]", i);
    // The labels are Japanese "numeric value 1/2" in the executable's Shift-JIS.
    StringView name = {_ZTVN4sead14SafeStringBaseIcEE + 2, "\x90\x94\x92\x6C\x82\x50"};
    fn_002499F4(p->numeric[0].string(), name.string(), -1);
    name.data = "\x90\x94\x92\x6C\x82\x51";
    fn_002499F4(p->numeric[1].string(), name.string(), -1);
    return p;
}
#endif
