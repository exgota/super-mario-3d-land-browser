# fn_002CCEAC

Written by factory job 6637 (gpt-6.1-sol high, run finished).

Address 0x002CCEAC, 248 bytes.

## Class guess

Object with multiple polymorphic bases: medium confidence; Interface base at +0x64, virtual slot +0x0. Scene: medium confidence; al::getSceneObj(int), flag at +0x52. Value: low confidence; four float fields, sizeof(Value) = 0x10.

## Struct offsets touched (this is r0 on entry)

- Interface +0x0, 4 bytes: virtual table pointer; slot +0x0: void Interface::apply(const void*, const Value*).
- Scene +0x52, 1 byte: bool.
- Object +0x64: Interface base subobject.
- Value +0x0, +0x4, +0x8, +0xC: float, low confidence.

## Callees

- fn_00227588 (0x00227588): Object* fn_00227588(Scene*, const void*).
- _ZN2al11getSceneObjEi (0x002775C0): Scene* al::getSceneObj(int).
- fn_002783BC (0x002783BC): void fn_002783BC(Value*, const Value*, float).
- fn_002CCFA4 (0x002CCFA4): void fn_002CCFA4(const void*, const void*, const Value*), medium confidence.

## Vtable slots

- Interface slot +0x0: void Interface::apply(const void*, const Value*).

## Data references

- 0x002CCF9C: float literal.
- 0x002CCFA0: float literal.
- No external data symbols.
