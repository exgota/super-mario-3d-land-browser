# Shared header enforcement

Owner authorization: 2026-10-03, following the reference-provenance decision.

Workers may add or extend shared headers under Game/lib include directories. Header edits are committed in scratch attempts for build provenance and submitted through the existing class/header preservation route. The integrator checks the target and accepted definitions in dependent objects, including transitive header consumers. Large closures ride the periodic full check alone. Stale shared-header proposals are rejected and reopened. No worker changes the compiler, checker, original data or production policy.

Worker attempts, normal proposal batches, class/header proposals and external submissions reject newly introduced translation-unit-local class/struct/union definitions whose names already occur in shared headers. This is a conservative lexical name check, not proof of semantic equivalence: renamed copies and macro-generated declarations still require source review. Forward declarations and existing accepted local definitions are preserved. Adding a header and a duplicate source-local type in the same proposal also fails.

Operator-approved integrator branches may migrate accepted Factory owner files, as explicitly authorized for the API campaign. That permission is absent by default. Shared-header factory submissions carry solo and preserve_exact flags; no constructor exception accepts a loss of prior exact rows. The existing reference compare-and-swap and final checked-source equality gates remain intact.

Pinned references reach workers through project/compiler_notes.md. Source pins and README provenance are in project/reference_sources.json. No reference repository is implicitly added to the build.

Validation receipts remain in the local factory logs: shared_header_policy_verification.json and shared_header_reference_safety.log. Deployment requires the three reference-safety cases to pass and one owner-authorized graceful STOP drain. No additional restart or scheduler change is authorized by this file.
