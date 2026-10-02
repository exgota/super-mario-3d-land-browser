# Integer parsing: accepted wrapper prerequisite

- Frozen main: `3d69bf00a769676c77f07465163def56a559b48f`; branch `dot/root-39a364`.
- Target `0x0039A364..0x0039A594`, 560 bytes, remains U.
- The target preserves incoming r0 as an output pointer and stores two words through it on successful parsing. Incoming r1 is an input string-interface object: the decimal path follows its virtual slot +0x8 and buffer field +0x4. Incoming r2 selects the numerical base, including decimal 10.
- The target returns a success indicator. Exact public class and member names are not claimed from this access evidence.
- Accepted `Game/backup/src/Factory/group_002F1AC0.cpp` declares `void fn_0039A364()` and implements the adjacent wrapper `fn_0039A360()` with that zero-argument contract.
- A coherent C++ parser definition needs the real output/input/base contract, conflicting with that accepted declaration. Shared wrapper repair remains integrator-owned.
- No alternate export, dummy arguments or source attempt. No canonical check or regression claim.
- No Factory, map, rank or tool change is included.
