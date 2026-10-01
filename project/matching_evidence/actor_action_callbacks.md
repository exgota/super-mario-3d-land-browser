# Actor action callback import evidence

This proposal is based on committed main75ac2e8fdb34f5c8d307dfeab78171b287bb1584 and the owner's verified EU executable, SHA256e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64. It names two existing whole data rows and changes no data bytes, boundary, pool, type or rank.

Existing whole dc row0x003BCDA8..0x003BCDB0 contains the complete ASCII Appear string, one NUL terminator and one zero alignment byte. Existing whole dc row0x003BCDB0..0x003BCDBC contains the complete ASCII Disappear string, one NUL terminator and two zero alignment bytes. The proposed neutral names are dat_003BCDA8 and dat_003BCDB0. The source declares external char arrays; it defines neither array or its bytes.

The independently mapped whole NrvAppearStep tables at0x003BCDBC..0x003BCDCC and0x003BCDDC..0x003BCDEC have address points0x003BCDC4 and0x003BCDE4. Their execute slots point to0x00350870 and0x003508C4 respectively. The separate original singleton initializer0x0038211C..0x00382164 stores these address points into the four existing AppearStep Nerve objects. This establishes the callback family without naming it from the candidate's successful link.

Original callback350870 loads3BCDA8 from its complete8-byte pool and passes it to the existing action-provider entry27063C. Callback3508C4 similarly loads3BCDB0 and calls the same provider. Both first test the existing isFirstStep provider and later call the existing isActionEnd and setNerve providers. The latter transition uses the already named Wait or End singleton.

The action-provider entry is already mapped as fn_0027063c on its unchanged whole U/f row0x0027063C..0x00270710. The committed FireBall import decision independently establishes its actor/string arguments and boolean result, corroborated by accepted FireBall and FallMapParts source consumers. Keep that neutral identity. This proposal makes no semantic rename, provider implementation or functional-equivalence claim.

Accepted AppearStep constructor117710..11773C and factory395D3C..395D74 independently ground the actor identity and current0x60 base-only layout. The two source changes replace the unbound tryStartAction references with the existing opaque provider and the two verified external string imports. They preserve the current NerveKeeper host access and interface adjustment, all member layouts, flags and headers.

The same family bundle also proposes unchanged-source FlowerPot::init31B6EC..31B754 and NoteObjGenerator Success callback364204..364238. Both already have function names and existing import identities. They need no new data or provider names. Their accepted constructors and current singleton/table evidence remain independent of their source diagnostics.

Four roots total324 complete bytes and pass paired791/894 scratch equality, with16 previously accepted source definitions preserved in both compilers. Scratch output adds no accepted credit. Only committed-source canonical project builds and the unchanged project checker can accept these roots. The separate M enrollment patch is a pending check request, not an O claim. No function boundary changes or original bytes are introduced.
