# StringTmp<128> constructor identity

The sole name change is the existing U/f interval [0028CB38,0028CB70), pool 0028CB6C: fn_0028CB38 becomes _ZN2al9StringTmpILi128EEC1EPKcz. Rank, boundaries, type and all other rows remain unchanged.

The target 0031B45C EffectObj helper calls 0028CB38 at 0031B478. Pinned authorized RedPepper 6bd828b729f2442d18e7a586b6858a5fc6aeb6cd, Source/al/Npc/EffectObj.cpp, identifies that expression as StringTmp<128>. The existing project template has the same public family. Target 0028CB38 calls 0028AEA8 (capacity 128, inline storage +0C, size 0x8C), installs derived table address point 003D7ABC from row [003D7ABC,003D7AD0), calls formatting helper 0028AE64 with its varargs, and returns the same receiver under the C++ constructor ABI. The ordinary source constructor has no return type. No private C facade remains in either accepted owner.

The RedPepper reference states GPL version 3 in LICENSE and credits open-ead/sead in README. It was inspected to corroborate the constructor identity; no reference implementation body or setup script was copied or executed. Current project source supplies the ordinary template. This commit provides identity evidence only, with no new matching claim or ledger change. The table is not named unless actual emitted/reference closure requires it.
