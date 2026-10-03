#include <Layout/alLayoutActor.h>

namespace observed_staff_roll {
template<class T> struct Ring {
    T* data;
    int capacity;
    int start;
    int count;
    T& at(unsigned index) {
        if (index >= static_cast<unsigned>(count)) return data[0];
        int position = start + static_cast<int>(index);
        if (position >= capacity) position -= capacity;
        return data[position];
    }
    void append(const T& value) {
        if (count >= capacity) return;
        int position = count++ + start;
        if (position >= capacity) position -= capacity;
        data[position] = value;
    }
};
struct ActiveEntry { al::LayoutActor* actor; int line; };
struct Owner {
    u8 unknown00[0x60];
    void* lineMetadata;
    int lineCount;
    Ring<al::LayoutActor*>* special;
    Ring<al::LayoutActor*>* ordinary;
    u8 unknown70[8];
    Ring<ActiveEntry>* active;
};
static al::LayoutActor* findInactive(Ring<al::LayoutActor*>* pool) {
    for (int i = 0; i < pool->count; ++i) {
        al::LayoutActor* actor = pool->at(i);
        if (!actor->isAlive()) return actor;
    }
    return 0;
}
static_assert(sizeof(wchar_t) == 2, "observed UTF-16 code units");
static_assert(sizeof(Ring<ActiveEntry>) == 16, "observed ring header");
static_assert(sizeof(ActiveEntry) == 8, "active actor/index stride");
static_assert(offsetof(Owner, lineCount) == 0x64, "line count");
static_assert(offsetof(Owner, active) == 0x78, "active ring");
}
extern "C" float fn_00252E00(observed_staff_roll::Owner*, int);
extern "C" const wchar_t* fn_00252DB4(const char*, const char*);
extern "C" wchar_t* fn_00252D34(wchar_t*, unsigned, const wchar_t*, int);
extern "C" void fn_0013E5F8(al::LayoutActor*, const wchar_t*);
extern "C" void fn_00144014(al::LayoutActor*, const wchar_t*);

extern "C" al::LayoutActor* fn_001A05CC(observed_staff_roll::Owner* owner, int line) {
    using namespace observed_staff_roll;
    if (line < 0 || line >= owner->lineCount) return 0;
    float position = fn_00252E00(owner, line);
    if (!(position >= -129.0f && position <= 129.0f)) return 0;
    Ring<ActiveEntry>* active = owner->active;
    for (int i = 0; i < active->count; ++i)
        if (active->at(i).line == line) return 0;
    const wchar_t* message = fn_00252DB4("StaffRoll", "StaffRollString");
    wchar_t text[128];
    fn_00252D34(text, 128, message, line);
    al::LayoutActor* selected;
    if (text[0] == L'@') {
        selected = findInactive(owner->special);
        if (!selected) return 0;
        fn_0013E5F8(selected, text + 1);
        ActiveEntry entry = {selected, line};
        owner->active->append(entry);
    } else if (text[0] == L'\n' || text[0] == 0 || text[0] == L' ') {
        selected = findInactive(owner->ordinary);
        if (!selected) return 0;
        fn_00144014(selected, L" ");
        ActiveEntry entry = {selected, line};
        owner->active->append(entry);
    } else {
        selected = findInactive(owner->ordinary);
        if (!selected) return 0;
        fn_00144014(selected, text);
        ActiveEntry entry = {selected, line};
        owner->active->append(entry);
    }
    selected->appear();
    return selected;
}
