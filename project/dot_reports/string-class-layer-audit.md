# Independent string class-layer audit (2026-10-01)

## Conclusion and limits

**The former FixedSafeString<64> owner at 003DA280 is withdrawn as an adoption proposal.** The independently supported replacement hypothesis is `sead::FixedSafeStringBase<char,64>` at that table base, with `sead::FixedSafeString<64>` at 003D9D04. These remain inferred names pending canonical source-closure and main metadata review. No map rows are accepted or changed by this report, and no exact function or byte credit is claimed.

The report [effect-string-table-ownership.md](effect-string-table-ownership.md) preserves byte-bound evidence. Its historical symbolic repair row must not be applied as written.

## Allowed API-shape reference

Reviewed the [open-ead/sead README](https://github.com/open-ead/sead/blob/master/README.md) before consulting the [SafeString API](https://github.com/open-ead/sead/blob/master/include/prim/seadSafeString.h). The README describes reconstruction of later titles, debug-symbol-derived names and guessed inline/template names. Accordingly, this establishes only plausible class/API shape, not a 3DS implementation. No implementation was copied. The missing layer is the generic `FixedSafeStringBase<Character, N>` below char-only `FixedSafeString<N>`.

## Independent retail constructors

| Constructor | Capacity | Buffered address point | Intermediate address point | Final installed address point |
|---|---:|---|---|---|
| 0021F264..0021F2AC | 64 | 003DA510 | 003DA288 | 003D9D0C |
| 0021F21C..0021F264 | 96 | 003DA510 | 003DA29C | 003D9D20 |
| 0027AD3C..0027ADA8 StringTmp | 64 | 003DA510 | 003DA288 | 003D7AF8 |
| 0027BEC8..0027BF34 StringTmp | 32 | 003DA510 | 003DA260 | 003D7AE4 |

The first two references are retained literal loads, not live vptr stores, in these optimized constructors. The last reference is stored. The same 64-byte member construction pattern independently appears at 0028C30C..0028C380. Actual Buffered storage also appears at 002E9CA8 in a separate constructor; this is stronger than dead literals alone.

The proposed generic-base table bases are 003DA258 (32), 003DA280 (64), 003DA294 (96). Proposed char-wrapper table bases observed directly are 003D9D04 (64) and 003D9D18 (96). Address points are base+8. The 64/96 wrapper tables are 20 bytes containing two header zeros, zero destructor slots and char termination at0039E0E4. The two wrappers are not aliases of the generic-base tables.

## Controlled compiler experiment

Compiled the minimal independently reconstructed hierarchy below using approved ARMCC 4.1/791 (SHA-256 d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d) and the unchanged project flags from the existing source-build provenance. Scratch only: this is not a canonical project acceptance check.

Five placement constructors force observable use. All fixed32/64/96 constructors emitted the relocation sequence BufferedSafeStringBase<char>, FixedSafeStringBase<char,N>, FixedSafeString<N>. Both StringTmp32/64 emitted BufferedSafeStringBase<char>, FixedSafeStringBase<char,N>, StringTmp<N>. Crucially, the empty char-wrapper intermediate vptr reference disappears automatically inside StringTmp. This independently explains why the retail temporary constructor has only three table literals despite four class layers.

A diagnostic with only StringTmp constructors marked noinline emitted 104-byte out-of-line bodies versus retail108. Their three relocation targets/order agree with this hypothesis; instruction bytes do not yet match. Adding the attribute was solely to expose the constructor body, not a proposed production matching technique. A separate out-of-line fixed-wrapper diagnostic emitted68 versusretail72. No source-closure checker was used for these scratch probes, so no match is claimed. Pointer-expression grouping and devirtualized termination-call checks did not alter the size difference; further cosmetic variants are not justified.

This compiler experiment supports the hierarchy, but cannot alone prove historical class spelling. Main must independently review identities and table boundaries, then use unchanged canonical checker and regression controls after any header change. Existing production proposal header remains unrevised pending that integration.

## Reproducible minimal diagnostic source

The source below is the initial hierarchy experiment, not game source for intake. Standard placement-new supplies the caller wrappers; no assembly, absolute game address or oracle substitution is used.

```cpp
#include <stdarg.h>
namespace sead {
template<class T> class SafeStringBase {protected: const T* mStringTop; public: SafeStringBase(const T* s):mStringTop(s){} virtual ~SafeStringBase(){} virtual void assureTermination()const{} };
template<class T> class BufferedSafeStringBase:public SafeStringBase<T> {protected:int mBufferSize; public: BufferedSafeStringBase(T*b,int n):SafeStringBase<T>(b),mBufferSize(n){const_cast<T*>(this->mStringTop)[mBufferSize-1]=0;} virtual void assureTermination()const{const_cast<T*>(this->mStringTop)[mBufferSize-1]=0;} int formatV(const T*,va_list); };
template<class T,int N>class FixedSafeStringBase:public BufferedSafeStringBase<T>{T mBuffer[N];public:FixedSafeStringBase():BufferedSafeStringBase<T>(mBuffer,N){const_cast<T*>(this->mStringTop)[0]=0;}};
template<int N>class FixedSafeString:public FixedSafeStringBase<char,N>{public:FixedSafeString():FixedSafeStringBase<char,N>(){} };
}
namespace al {template<int N> class StringTmp:public sead::FixedSafeString<N>{public:StringTmp(const char*f,...):sead::FixedSafeString<N>(){va_list a;va_start(a,f);this->formatV(f,a);va_end(a);}};}
template class sead::FixedSafeString<32>;
template class sead::FixedSafeString<64>;
template class sead::FixedSafeString<96>;
template class al::StringTmp<32>;
template class al::StringTmp<64>;
#include <new>
extern "C" void makeFixed32(void*p){new(p)sead::FixedSafeString<32>();}
extern "C" void makeFixed64(void*p){new(p)sead::FixedSafeString<64>();}
extern "C" void makeFixed96(void*p){new(p)sead::FixedSafeString<96>();}
extern "C" void makeTmp32(void*p,const char*s){new(p)al::StringTmp<32>(s);}
extern "C" void makeTmp64(void*p,const char*s){new(p)al::StringTmp<64>(s);}
```
