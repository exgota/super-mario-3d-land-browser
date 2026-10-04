#pragma once
#include <Collision/alKCollisionServer.h>
#include <Collision/alObservedCollisionPrism.h>
#include <math/seadVector.h>

namespace observed_collision_query {
typedef sead::Vector3<s32> Coordinates;
typedef al::ObservedCollisionPrism Triangle;
struct Contact {
    Triangle* triangle;
    float distance;
    u8 kind;
};
struct Ring {
    Contact* data;
    int capacity;
    int head;
    int count;

    struct Iterator {
        Ring* owner;
        int index;
        bool operator==(const Iterator& other) const {
            return owner == other.owner && index == other.index;
        }
        bool operator!=(const Iterator& other) const { return !(*this == other); }
        Iterator& operator++() { ++index; return *this; }
        Contact& operator*() const {
            int position = owner->head + index;
            if (position >= owner->capacity) position -= owner->capacity;
            return owner->data[position];
        }
    };
    Iterator begin() { Iterator it = {this, 0}; return it; }
    Iterator end() { Iterator it = {this, count}; return it; }
    void push(const Contact& value) {
        if (count >= capacity) return;
        int position = count++;
        position += head;
        if (position >= capacity) position -= capacity;
        data[position] = value;
    }
};
static_assert(sizeof(Triangle) == 16, "observed triangle stride");
static_assert(sizeof(Contact) == 12, "observed contact stride");
static_assert(sizeof(Ring) == 16, "observed ring state");
static_assert(offsetof(al::KCollisionHeader, origin) == 0x14, "query origin");
static_assert(offsetof(al::KCollisionHeader, coordinateMasks) == 0x20, "coordinate masks");
static_assert(offsetof(al::KCollisionHeader, subdivisionShifts) == 0x2c, "cell shifts");
}
extern "C" bool fn_00333A74(al::KCollisionServer*, observed_collision_query::Coordinates*,
    observed_collision_query::Coordinates*, const sead::Vector3f*, const sead::Vector3f*);
extern "C" u16* fn_0024A0E0(al::KCollisionServer*, int*, const observed_collision_query::Coordinates*);
extern "C" bool fn_001CF7F0(al::KCollisionServer*, observed_collision_query::Triangle*,
    const sead::Vector3f*, float*, u8*, float);
extern "C" bool fn_001CFFA4(al::KCollisionServer*, observed_collision_query::Triangle*,
    const sead::Vector3f*, float*, u8*, float);
