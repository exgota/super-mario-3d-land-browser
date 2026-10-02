# Actor state update: accepted wrapper prerequisite

- Branch `dot/root-3072b4`; base `3d69bf00a769676c77f07465163def56a559b48f`.
- Target `0x003072B4..0x003074E8`, 564 bytes, remains U.
- The target preserves incoming r0 as its actor and supplies it unchanged to named al::startAction at 3072D4, then to further actor helpers. This is a pointer contract, independently of the target's unproved return type.
- Accepted `Game/backup/src/Factory/group_00305BF0.cpp` declares `unsigned int fn_003072B4(unsigned int, unsigned int)`. Its wrapper 3072AC loads the first word through its second integer argument and forwards both integer values.
- No shared typed declaration exists. A coherent actor-pointer definition requires the accepted wrapper/import contract to be repaired by its owner; no pointer-to-integer adapter or duplicate alias was introduced.
- No source, normal build or canonical check attempt. No existing accepted function is claimed to regress.
- Payload is this short note only; no Factory, map, rank, tool or game-data changes.
