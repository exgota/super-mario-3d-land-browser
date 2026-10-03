# BalanceTruck movement power, refreshed delivery

Claims: `_ZN12BalanceTruck15updateMovePowerEv`

This supersedes the old-format `dot/root-26d6c0` proposal for the same root.
It adds no new matched root or byte credit to that proposal's historical count.
The source and header are byte-identical to the earlier exact reconstruction.

Fresh base: `96773182c9a4b23005dc39b9a661580619c841f2`.
Target: 0x0026D6C0–0x0026D8D8, 536 complete bytes including its literal pool.
Normal project ARMCC 4.1/791 compile, link and export succeeded.
`python tools/check.py _ZN12BalanceTruck15updateMovePowerEv --object build/eu/obj/Game/backup/src/MapObj/BalanceTruck.o`
returned `M -> O: The complete source-generated function interval matches byte for byte.`
The checker is unchanged from the named base. The restored GNU objdump/asm-differ
full-input-section comparison was also run after that normal linked build.
The new object is byte-identical to the historical exact object:
`0d2a7d48d6b270b8cdc96cb9ca5e1ec1fc621962a15630ebc76f892f253dc013`.

The class identity is grounded by the BalanceTruck factory and constructor
0x00135420, its MapObjActor base and vtable 0x003C6BDC. Creator 0x003966DC
allocates 0xB4 at 0x003966E8 and calls the constructor at 0x00396704.
Callers 0x00134364, 0x00134E0C and 0x001351C0 establish the receiver and
movement fields at +0x88/+0x8C, direction fields +0x90/+0x94 and counter +0xB0.
`updateMovePower` remains a descriptive reconstruction name, not recovered
original C++ spelling. Unknown fields remain explicitly qualified in the header.

The method accelerates/brakes movement power, clamps held duration to +/-40,
and selects Front/Back/Wait actions. It reuses the already mapped ordinary
startAction and const-LiveActor reaction helper identities. No shared header,
Factory source, data boundary, helper identity or compiler flag is changed.
The separate map/evidence commit names only the existing target row, retains U,
and records the evidence in project/decisions.md. All scratch ranks are restored.
Two historical source forms were consumed; this refresh changes no body form.
Independent integrator acceptance remains pending.
