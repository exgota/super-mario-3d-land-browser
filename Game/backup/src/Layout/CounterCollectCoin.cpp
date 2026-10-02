#include <Layout/alLayoutActor.h>
#include <Nerve/alNerveFunction.h>

// The derived constructor at 00187CDC allocates three flag bytes after the
// established LayoutActor base. Its archive is CounterCollectCoinD.
class CounterCollectCoin : public al::LayoutActor {
public:
    bool* collected;
    void collect(int index);
};

extern "C" {
void* fn_0027109C(al::IUseAudioKeeper*, const sead::SafeString&);
void fn_00278B98(void*, int, int);
int fn_00256A74();
void fn_0027BEA0(al::IUseEffectKeeper*, const char*, const sead::Vector3f*);
void fn_002CEE58(al::LayoutActor*, const char*, int, int);
struct CollectCoinNerve : al::Nerve {
    virtual void execute(al::NerveKeeper*) const;
};
extern const CollectCoinNerve dat_003F17DC;
extern const char dat_003DF29D[];
extern const char dat_003DF2AB[];
extern const char dat_003DF2A6[];
extern const char dat_003DF2B4[];
extern const char dat_003DF2B9[];
}

void CounterCollectCoin::collect(int index) {
    if (!mIsAlive)
        appear();

    void* sound = fn_0027109C(this, "SeSyGetCollectCoin");
    int count = 0;
    if (collected[0])
        ++count;
    if (collected[1])
        ++count;
    if (collected[2])
        ++count;
    switch (count) {
    case 0:
        fn_00278B98(sound, 0, 0);
        break;
    case 1:
        fn_00278B98(sound, 0, 1);
        break;
    case 2:
        fn_00278B98(sound, 0, 2);
        fn_0027109C(this, "SeSyCollectCoinComplete");
        break;
    }

    collected[index] = true;
    switch (fn_00256A74()) {
    case 1:
        fn_0027BEA0(this, "Get1", 0);
        break;
    case 2: {
        const char* names[] = {dat_003DF29D, dat_003DF2AB};
        fn_0027BEA0(this, names[index], 0);
        break;
    }
    case 3: {
        const char* names[] = {dat_003DF2A6, dat_003DF2B4, dat_003DF2B9};
        fn_0027BEA0(this, names[index], 0);
        break;
    }
    }
    fn_002CEE58(this, "TxtCoin", index, 1);
    al::setNerve(this, &dat_003F17DC);
}
