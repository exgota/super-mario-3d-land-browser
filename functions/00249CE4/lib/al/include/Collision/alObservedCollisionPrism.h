#pragma once

namespace al
{
// Reconstructed name for the shared 16-byte collision prism record.
// This is also observed_collision_query::Triangle in the stopped query work.
struct ObservedCollisionPrism
{
    float height;
    u16 vertex;
    u16 normal06;
    u16 normal08;
    u16 normal0A;
    u16 normal0C;
    u16 attribute;
};
static_assert(sizeof(ObservedCollisionPrism) == 16, "collision prism stride");
static_assert(offsetof(ObservedCollisionPrism, vertex) == 4 &&
              offsetof(ObservedCollisionPrism, normal06) == 6 &&
              offsetof(ObservedCollisionPrism, normal08) == 8 &&
              offsetof(ObservedCollisionPrism, normal0A) == 10 &&
              offsetof(ObservedCollisionPrism, normal0C) == 12 &&
              offsetof(ObservedCollisionPrism, attribute) == 14,
              "observed prism member offsets");
}
