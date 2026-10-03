# Static parameter construction needs a global object row

Branch: dot/root-38503c. Base: 64d1bb9f3c97318481de9baa08de473608b9e042.
Target 0038503C..00385290, 596 bytes, U. No source forms or build/check claims.
Missing address 0042F510 has no exact or containing row on this base.
Starting at 00385148, the root directly initializes three groups of three floats through offset +0x20, then invokes additional constructors on following storage.
The direct stores establish a 0x24-byte writable minimum; subsequent construction makes that a lower bound, not the complete allocation or table extent.
The observed 0042F510..0042F534 span contains the earlier root-138128-needs-rows.md references 0042F51C and 0042F528. These are overlapping access bounds, not proposals for overlapping map rows.
The integrator must reconcile coverage with that report. This initializer does not independently prove either one shared object or separate allocation owners for the three vector-sized spans.
The containing owner and final boundary remain unproved. Existing nerve/table rows are not redefined here.
The integrator must establish the justified object row before source/check work resumes.
No source, game-data, map/rank, Factory, tools, configuration, ledger or STATE changes. No exact credit.
