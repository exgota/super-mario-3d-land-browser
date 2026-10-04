# Integrator queue progress after a slow full check

The recovery full check completed on October 4, 2026 at 03:21:48 Eastern after 2,785 seconds, with zero demotions and a successful push. Because the configured sync interval is 2,700 seconds and the loop immediately continued after success, the next iteration began another full check before processing any saved proposal. Checks exceeding the interval can repeatedly starve the proposal queue.

After a successful sync, the integrator now processes one ordinary queue batch before evaluating whether another periodic check is due. A timeout retains the existing retry path. A completed stop with an empty queue exits without waiting for a proposal. The 2,700-second interval, two-hour stage budgets, worker scheduling, six-slot cap, load guard, canonical checker, and reference acceptance gates are unchanged.

The isolated queue fixture reproduces starvation against the prior production source, verifies queue progress against the candidate, verifies timeout retry precedes acceptance, and verifies an empty completed stop. The existing five timeout recovery tests also pass. Canonical reference-safety verification passed all three gates against this source: 24/25 exact leaves the reference unchanged; 25/25 exact moves it once after the last check; post-check source tampering leaves it unchanged. The source SHA-256 is `5e6cf6f8b9e619dee377224199414173f989ffe4022f64a7aa71fe12b5d52fe3`. No production database or Git reference is changed by these fixtures.

Run the queue fixture with FACTORY_UNDER_TEST pointing to the candidate, FACTORY_BASELINE pointing to the preceding production source, and optionally QUEUE_PROGRESS_RESULTS selecting the JSON receipt path. The negative control must remain the preceding source, because it establishes that the test detects the observed bug.

This is a driver implementation correction needed to complete the owner's recovery order, quoted: "wtf why??? can u restart yes please". The owner required queued proposals, a zero-demotion full check, and a push to be verified. It is not a new matching trial or a change to the worker scheduling policy.
