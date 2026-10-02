// Course-result curtain object and child construction, recovered from EU retail.
// This ABI view describes only fields touched by this family, not a C++ vtable owner.
namespace al { class LayoutInitInfo; class NerveKeeper; struct Nerve; }

namespace {
struct LayoutActor { unsigned char storage[0x30]; };
struct ResultCurtainTop { unsigned char storage[0x38]; };
struct WipeSimple { unsigned char storage[0x34]; };
struct CourseResultCurtain;
struct CurtainDispatch {
    al::NerveKeeper* (*getNerveKeeper)(const CourseResultCurtain*);
};
struct CourseResultCurtain {
    const CurtainDispatch* dispatch;
    al::NerveKeeper* keeper;
    ResultCurtainTop* top;
    WipeSimple* bottom;
    LayoutActor* coinCounter;
    LayoutActor* timeCounter;
    LayoutActor* oneUp;
    int field1c;
    void* field20;
    void* field24;
    bool field28;
    void* field2c;
};
}

extern "C" {
extern const CurtainDispatch dat_003CE674;
extern const al::Nerve dat_003F1114;
CourseResultCurtain* _ZN2al13NerveExecutorC1EPKc(CourseResultCurtain*, const char*);
void _ZN2al13NerveExecutor9initNerveEPKNS_5NerveEi(CourseResultCurtain*, const al::Nerve*, int);
ResultCurtainTop* fn_0018665C(ResultCurtainTop*, const char*, const al::LayoutInitInfo&);
WipeSimple* _ZN2al10WipeSimpleC1EPKcS2_RKNS_14LayoutInitInfoES2_(WipeSimple*, const char*, const char*, const al::LayoutInitInfo&, const char*);
LayoutActor* _ZN2al11LayoutActorC1EPKc(LayoutActor*, const char*);
void fn_0027E328(LayoutActor*, const al::LayoutInitInfo&, const char*, const char*);
LayoutActor* fn_002756F4(LayoutActor*, const char*, const char*, const al::LayoutInitInfo&, const char*);
}

extern "C" CourseResultCurtain* fn_0018CC38(CourseResultCurtain* self, const al::LayoutInitInfo& info)
{
    self = _ZN2al13NerveExecutorC1EPKc(self, "コースリザルトカーテン");
    self->dispatch = &dat_003CE674;
    self->top = 0;
    self->bottom = 0;
    self->coinCounter = 0;
    self->timeCounter = 0;
    self->oneUp = 0;
    self->field1c = 0;
    self->field20 = 0;
    self->field24 = 0;
    self->field28 = false;
    self->field2c = 0;

    ResultCurtainTop* top = new ResultCurtainTop;
    if (top)
        top = fn_0018665C(top, "リザルトカーテン(上画面)", info);
    self->top = top;

    WipeSimple* bottom = new WipeSimple;
    if (bottom)
        bottom = _ZN2al10WipeSimpleC1EPKcS2_RKNS_14LayoutInitInfoES2_(bottom, "リザルトカーテン(下画面)", "WipeCurtainD", info, 0);
    self->bottom = bottom;

    LayoutActor* coin = new LayoutActor;
    if (coin)
        coin = _ZN2al11LayoutActorC1EPKc(coin, "コインカウンタ");
    self->coinCounter = coin;
    fn_0027E328(coin, info, "CounterCoin", "OverCurtain");

    LayoutActor* time = new LayoutActor;
    if (time)
        time = _ZN2al11LayoutActorC1EPKc(time, "タイムカウンタ");
    self->timeCounter = time;
    fn_0027E328(time, info, "CounterTimeLimit", 0);

    LayoutActor* oneUp = new LayoutActor;
    if (oneUp)
        oneUp = fn_002756F4(oneUp, "１ＵＰ", "GameOneUp", info, "OverCurtain");
    self->oneUp = oneUp;
    _ZN2al13NerveExecutor9initNerveEPKNS_5NerveEi(self, &dat_003F1114, 0);
    return self;
}
