#include <Item/ItemHolder275828.h>
#include <MapObj/ItemHolder.h>
#include <new>

using namespace dot275828;

extern "C" {
// Existing original rows only. NameRef construction is the retail helper
// 0x0027A7B0; its temporary map alias is recorded in the replay recipe.
void fn_0027A81C(ArrayView*, int capacity, void* buffer);
void fn_0027A7C4(FreeList*, void* storage, int stride, int capacity);
}

namespace dot275828
{
inline ArrayView::ArrayView() : size(0), capacity(0), buffer(0) {}
inline FreeList::FreeList() : storage(0), head(0) {}

template<int Count> inline FixedArray<Count>::FixedArray()
{
    fn_0027A81C(&view, Count, storage);
}
inline void PooledRecords::setStorage(void* storage, int capacity)
{
    if (storage)
    {
        fn_0027A7C4(&free, storage, 16, capacity);
        fn_0027A81C(&active, capacity,
                    static_cast<unsigned char*>(storage) + 16 * capacity);
    }
}
inline PooledRecords::PooledRecords()
{
    setStorage(records, 128);
}
template<int Count> inline ItemPool<Count>::ItemPool(const char* label)
    : name(label), state(0)
{
}
}

ItemHolder::ItemHolder(bool input51, bool input52, int input54)
{
    mOption51 = input51;
    mOption54 = input54;
    mOption52 = input52;
    mEnabled = true;
    mLimit = 6;
    mCoins = 0;
    mCountUpCoins = 0;
    mFireFlowers = 0;
    mKinokoOneUps = 0;
    mFastKinokoOneUps = 0;
    mKinokoPoisons = 0;
    mFastKinokoPoisons = 0;
    mKinokos = 0;
    mFastKinokos = 0;
    mBoomerangFlowers = 0;
    mPatapataWings = 0;
    mAssistItems = 0;
    mSuperLeaves = 0;
    mSpecialSuperLeaves = 0;
    mSuperStars = 0;
    mClocks = 0;
    mCollectCoins = 0;
    mKickKouras = 0;
    mCoinCharger = 0;
    if (input52) mLimit = 10;
    mCoins = new ItemPool<20>("\203\122\203\103\203\223");
    mCountUpCoins = new ItemPool<9>("\203\112\203\105\203\223\203\147\203\101\203\142\203\166\227\160\203\122\203\103\203\223");
    mFireFlowers = new ItemPool<4>("\203\164\203\100\203\103\203\101\203\164\203\211\203\217\201\133");
    mKinokoOneUps = new ItemPool<10>("\061\125\120\203\114\203\155\203\122");
    mFastKinokoOneUps = new ItemPool<2>("\215\202\221\254\061\125\120\203\114\203\155\203\122");
    mKinokoPoisons = new ItemPool<4>("\223\305\203\114\203\155\203\122");
    mFastKinokoPoisons = new ItemPool<2>("\215\202\221\254\223\305\203\114\203\155\203\122");
    mKinokos = new ItemPool<4>("\203\130\201\133\203\160\201\133\203\114\203\155\203\122");
    mFastKinokos = new ItemPool<2>("\215\202\221\254\203\130\201\133\203\160\201\133\203\114\203\155\203\122");
    mBoomerangFlowers = new ItemPool<4>("\203\165\201\133\203\201\203\211\203\223\203\164\203\211\203\217\201\133");
    mPatapataWings = new ItemPool<1>("\203\160\203\136\203\160\203\136\202\314\211\110");
    mAssistItems = new ItemPool<1>("\203\101\203\126\203\130\203\147\203\101\203\103\203\145\203\200");
    mSuperLeaves = new ItemPool<4>("\203\130\201\133\203\160\201\133\202\261\202\314\202\315");
    mSpecialSuperLeaves = new ItemPool<4>("\203\130\201\133\203\160\201\133\202\261\202\314\202\315\203\130\203\171\203\126\203\203\203\213");
    mSuperStars = new ItemPool<4>("\203\130\201\133\203\160\201\133\203\130\203\136\201\133");
    mClocks = new ItemPool<2>("\203\112\203\105\203\223\203\147\203\101\203\142\203\166\216\236\214\166");
    mCollectCoins = new ItemPool<3>("\203\122\203\214\203\116\203\147\203\122\203\103\203\223");
    mKickKouras = new ItemPool<3>("\203\114\203\142\203\116\215\142\227\205");
}

static_assert(sizeof(ItemHolder) == 0x5c, "holder size");
static_assert(sizeof(ItemPool<20>) == 0xa7c, "coin pool size");
static_assert(sizeof(ItemPool<1>) == 0xa30, "single-item pool size");
