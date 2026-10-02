# Submission commit receipts

Branch root/integrator-commit-receipts fixes two Git bookkeeping failures in the repository copy of tools/factory/factory.py. It does not modify the adjacent deployed factory, any oracle, checker, matching source, rank cell, ledger, or main. Operator review/deployment to the adjacent factory remains necessary.

The result root-heap-layout-e6d8485e8 rejects at the final acceptance commit with 0 exact and 0 non-matching. A header family can pass its normal build/preservation path without creating new rank or ledger changes. Its source already belongs to the merge commit, so a new receipt commit is empty. Git rejects it by default.

The result root-calendar-constructor-placement-a671ac00c-retry rejects at the candidate-rank commit. The claim's row is already M; assigning it M again leaves the index unchanged. Git rejects this audit commit before the actual checker can run.

Add --allow-empty only to those two submission commit invocations. An empty candidate explicitly remains unverified. An empty acceptance receipt still follows the unchanged build, full-map regression/claim check, checked-source-tree equality check and atomic main move. Genuine bookkeeping changes remain staged and committed. This does not permit a false exact claim or bypass any test.

Validation: the patched Python source parses. A new synthetic Git repository reproduces the default empty-commit refusal, then verifies that the explicit candidate receipt preserves its entire tree; a source-only final receipt retains the checked layout tree; and a real synthetic rank/ledger delta survives the same receipt operation. No real project rank or ledger is changed. Local report: build/root_integrator_receipts_validation/report.json. Source SHA-256 c6aefa4acb2cefef7bb23cd3d85e0222dfe600771da933a21442f94e74643e83.

The deployed evaluator and its eventual retries are unverified. Accepting this repository-copy patch does not deploy or restart the factory. Both target families remain without integrator acceptance until their result files say accepted.
