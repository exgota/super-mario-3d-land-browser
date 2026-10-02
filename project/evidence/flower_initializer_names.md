# Approved provisional flower initializer names

On 2026-10-02 the owner approved the six provisional FlowerInit constructor names and submission of the reviewed three-initializer family. This separate evidence commit names nine existing map rows. It changes no rank, boundary, pool, section, type, compiler option, source body or ledger entry.

These names identify the reconstruction. They do not assert recovery of the original namespace, class spelling, constructor alias spelling, direct inheritance chain or bool-versus-integer formal types. The source and metadata retain bool,int for Forward and Vertical, which agrees on the independently audited zero-or-one inputs. The owner approved that explicitly limited proposal.

| Address | Proposed identity | Evidence |
| --- | --- | --- |
| `0x0028016C` | `_ZN10FlowerInit5ParamC1Ev` | Constructor role, 32-byte allocation |
| `0x00280018` | `_ZN10FlowerInit6AppearC1EPN2al9LiveActorE` | Constructor role, 16-byte allocation |
| `0x00280068` | `_ZN10FlowerInit7ForwardC1EPN2al9LiveActorEPNS_5ParamEbi` | Constructor role, 36-byte allocation |
| `0x002800F8` | `_ZN10FlowerInit8VerticalC1EPN2al9LiveActorEPNS_5ParamEbi` | Constructor role, 24-byte allocation |
| `0x0027B860` | `_ZN10FlowerInit7ReleaseC1EPN2al9LiveActorEPNS_5ParamE` | Constructor role, 36-byte allocation |
| `0x0027FF7C` | `_ZN10FlowerInit6GroundC1EPN2al9LiveActorEPNS_5ParamE` | Constructor role, 40-byte allocation |
| `0x001579F0` | `_ZN15BoomerangFlower4initERKN2al13ActorInitInfoE` | Actor construction, primary init slot and archive identity |
| `0x0011A174` | `_ZN10FireFlower4initERKN2al13ActorInitInfoE` | Actor construction, primary init slot and archive identity |
| `0x00177020` | `_ZN16SuperLeafSpecial4initERKN2al13ActorInitInfoE` | Actor construction, primary init slot and archive identity |

Independent review of the owned EU executable, SHA-256 e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64, finds 51 direct calls in ten functions to these six constructor imports. Requested allocations consistently measure 32, 16, 36, 24, 36 and 40 bytes in the order shown. The parameter helper initializes eight float fields. The five state constructors call the established NerveStateBase constructor at the same receiver, store an actor pointer at +0x0C and install distinct state tables. Their appearance slots override the base behavior.

Forward and Vertical store their third and fourth explicit arguments to byte fields. All twenty direct calls provide only zero or one. This supports the retained caller interface but cannot distinguish every original bool, character or integer formal type. The literal constructor names and type encoding remain provisional.

FireFlower, BoomerangFlower and SuperLeafSpecial have independently observed 0x6C actor allocations and corresponding primary initialization entries at 0011A174, 001579F0 and 00177020. The prepared final source family has passed 84 canonical project-object checks, preserving 1,260 existing bytes and proposing 1,596 new bytes. This metadata-only commit does not adopt that source or award bytes. The source branch carries its own final hashes and build/check record, and still requires the integrator full preservation gate.

The checked source also reconciles nine shared nerve imports against eighteen independently inspected startup-installed tables. Those existing neutral data names remain unchanged. No additional helper implementation, nerve definition or vtable is introduced by this metadata decision.
