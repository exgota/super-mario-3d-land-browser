# fn_001DBEFC

Written by factory job 2869 (gpt-6.1-sol high, run finished).

Address: 0x001DBEFC

## Class guess

al actor-system utility; medium confidence. No inferred member class. Evidence: al::getLiveActorKit() and its LiveActorKit* return type; the entry argument is forwarded as the second argument of fn_001DBF1C.

LiveActorKit and ActorService are provisional type names; the service's concrete class is unknown.

## Struct offsets

- LiveActorKit +0x24: ActorService* (4 bytes); medium confidence, evidence: the result of al::getLiveActorKit() supplies this pointer.
- ActorService +0x08: void* context (4 bytes); low confidence in pointer type, evidence: first argument of fn_001DBF1C. A 32-bit scalar type remains possible.

## Inferred signatures

- 0x001DBEFC: extern "C" void* fn_001DBEFC(void* argument); argument and return types have low confidence.
- 0x00277250: LiveActorKit* al::getLiveActorKit(); high confidence in name and argument list, medium confidence in return type; LiveActorKit is declared locally in an anonymous namespace.
- 0x001DBF1C: extern "C" void* fn_001DBF1C(void* context, void* argument); argument and return types have low confidence.

## Vtable slots

None.

## Data references

None.
