# Original hierarchy dispatcher at 00338D80

This source is a clean-room reconstruction from the owner-supplied EU executable. The function spans 00338D80..00339578 (2040 bytes); 003391A4..003391B4 is an internal four-word literal pool. The existing map is unchanged in the submitted source. The familiar public class and method names are not established, so the implementation uses address-scoped observed views rather than guessed SDK names.

## Calling convention and records

Original callers 00128FFC and 00129080 reach this dispatcher through the existing model calculation chain. The second register argument is a controller. Its +8 points at a skeleton resource, +C at a model/root owner, +10/+14 at before/after callback containers. Vtable offsets +C/+14/+1C supply local records, composed records and output matrices. Record buffers have their data pointer at +4. The output range uses begin/end at +4/+8, with 48-byte matrix stride. Local/composed records have a 48-byte matrix, three scale floats at +30 and flags at +3C, for a 64-byte stride.

The root saves r0/r1/r2, then d8/d9 and 0x68 bytes of frame storage. Its accesses at resulting sp+0x80 load the saved THIRD register argument. That value is forwarded as the first argument to composition helpers. The first root argument is otherwise unused. The fixture deliberately provides different first and third arguments to expose mistakes here.

Skeleton +1C holds a self-relative dictionary pointer. The per-index dictionary entry is another self-relative pointer at dictionary+28+16*index. The resource's joint flag word is +4, and signed parent identifier is +C. The record index advances in dictionary order; parent records are indexed directly by that parent identifier. The original dereferences unresolved/null joint records. The reconstruction retains that behavior rather than inventing validation.

A parent identifier other than -1 selects local/composed buffer records. For -1, skeleton flag bit 0 chooses between the owner records at +4C/+BC and two calls to the real identity-cache accessor at 002164F8. The order is composed first, local second. The callback argument pair is represented as a two-word by-value aggregate; this is an ABI-equivalent source hypothesis, not an established original type name.

## Modes, scale, and flags

Only the low byte of skeleton+24 selects the mode. Mode 1 calls 0033AA94 when resource flag 0x20 is present, otherwise 0025C3E4. Mode 0 always calls 0025C3E4. Every other low-byte mode calls 0033A90C. Before/after callbacks run on every visited output, including records without local flag bit 0. The transform helpers run only when that bit is present.

The returned aggregate scale's squared norm is tested by a signed comparison of its IEEE-754 word against 0x358637BE. This is not a generic epsilon float comparison and must preserve its behavior on unusual IEEE inputs. If the comparison triggers, each component becomes signed epsilon. Original conditional moves select negative only for values below zero; unordered values take positive epsilon. The literal is 0x358637BE (1.0000001111620804e-6), one ULP above the usual 1e-6f encoding 0x358637BD.

After copying aggregate scale to the composed record, 002723E0 forms the output matrix. Modes 0 and 1 pass the local scale. Other modes pass the composed aggregate scale. The composed flags then clear 0x7E0. If mask 0x8 is absent, equal scale components set uniform-scale 0x400; exactly positive one by bit pattern additionally sets 0x200. Existing flags outside those masks remain intact.

## Prior evidence and remaining limits

Main's project/native_mesh_evidence.md already described bounded static mode-zero hierarchy, parent indexing, palette composition and root-owner behavior. This work builds on that evidence, extends the source to all observed mode branches and callbacks, and tests those branches with constructed state. It does not claim original API names, original compiler identity, complete cache construction, asset loading, animation, rendering or gameplay.

The candidate's normal ARMCC 4.1/791 carrier is lib/al, consistent with this checkout's existing build configuration. That is a reproducible compiler choice, not proof that this original unnamed library routine used the same compiler. The only compiled definition is the 1912-byte fn_00338D80. It imports five existing original functions and defines no helper aliases or data identities. The canonical checker rejects its complete section size before exact comparison; the diagnostic original-helper link is not a canonical closure acceptance.
