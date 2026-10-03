# Enemy initialization needs state-parameter rows

Branch: dot/root-138390. Base: 64d1bb9f3c97318481de9baa08de473608b9e042.
Target 00138390..001385D8, 584 bytes, U. No source forms or build/check claims.
Missing addresses 0042F5C0 and 0042F554 have no exact or containing rows on this base.
The root supplies 0042F5C0 to the existing EnemyStateBlowDown constructor 0027B54C at 00138400; that constructor retains it in its parameter-pointer field.
The root loads 0042F554, derives 0042F534 by subtracting 0x20, and passes both to state constructor 0026B9D8 at 00138468; it retains these arguments at +18/+1C. The derived 0042F534 already has a row and is not requested.
These calls establish required object identities, not complete global extents. No fixed minimum readable span or enclosing owner is established by this bounded preflight.
The integrator must establish all justified data rows before source/check work resumes.
No source, game-data, map/rank, Factory, tools, configuration, ledger or STATE changes. No exact credit.
