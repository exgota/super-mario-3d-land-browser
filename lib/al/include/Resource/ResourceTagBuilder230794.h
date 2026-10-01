#pragma once

// Clean reconstruction from EU root 0x00230794 and its callers. The names below
// describe observed roles; no original NintendoWare class identity is asserted.
namespace dot230794
{
typedef unsigned int Word;
struct Resource;
struct Builder
{
    Resource* resource;             // +00, used by caller 0x0029BC48
    unsigned char enabled;         // +04
    unsigned char unknown05[3];
    Word count08;                  // +08
    Word count0c;                  // +0c
    Word value10;                  // +10
    Word value14;                  // +14, optional object pointer bits
    Word value18;                  // +18
    Word count1c;                  // +1c
    Word extra20;                  // +20
    unsigned char includeAuxiliary;// +24
};
struct SizeAccumulator
{
    Word size;
    Word alignment;
    Word maximumAlignment;
};
struct Options
{
    unsigned char enabled;
    unsigned char auxiliary;
    Word count0c;
    Word count08;
    Word count1c;
    Options() : enabled(1), auxiliary(1), count0c(4), count08(8), count1c(1) {}
};
struct ExtendedOptions
{
    Options base;
    Word value10, value14, value18;
    ExtendedOptions() : value10(0), value14(0), value18(0) {}
};
struct ModelOptions
{
    ExtendedOptions common;
    Word extra;
    ModelOptions() : extra(0) {}
};
struct TypeInfo { const TypeInfo* parent; };
struct Object;
struct ObjectVtable
{
    void (*unknown00)();
    void (*unknown04)();
    const TypeInfo* (*type)(Object*);
};
struct Object { ObjectVtable* vtable; };
}

extern "C" dot230794::Object* fn_00230794(
    const dot230794::Builder*, dot230794::SizeAccumulator*,
    dot230794::SizeAccumulator*, void*, const dot230794::Resource*,
    void*, void*, bool, bool);
