#ifndef GAME_UTIL_ALLOCATOR_STORAGE_H
#define GAME_UTIL_ALLOCATOR_STORAGE_H

// Neutral identity for the allocator pointers stored in these cells.
// The original class name and complete interface are not yet recovered.
namespace observed_allocator {
class Allocator;
}

extern "C" {
extern observed_allocator::Allocator* dat_003F0380;
extern observed_allocator::Allocator* dat_003F0384;

// The accepted APIs return the address of a pointer cell.
void* fn_0021DFB8();
void* fn_0021CDE4();
}

#endif
