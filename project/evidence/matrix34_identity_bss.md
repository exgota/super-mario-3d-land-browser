# Complete Matrix34 identity storage in BSS

Add the neutral identity `dat_00430A88` for 0x00430A88..0x00430AB8, exactly 48 bytes. The row is non-static BSS data, Type `db`, with no Pool field and rank U. No original linker name is claimed. The existing map contains no covering or overlapping row.

The owned EU image has the BRIEF SHA-256. Its independently decoded matrix initializer at 0x003834C8 loads this base at 0x00383580 and writes all twelve binary32 elements between 0x00383588 and 0x003835B0. The three unit entries are at offsets 0, 20 and 40. Every other element is zero. A separately initialized 48-byte zero matrix at 0x00430A58 ends exactly at the new base; a separately initialized sixteen-double matrix begins exactly at 0x00430AB8. These independent neighbors establish the complete span, rather than merely a lowest-accessed prefix.

The emitter child updater at 0x002ECBD0 passes this global to the established Matrix34 copy helper at 0x0027C18C. The original helper copies twelve four-byte values. This independently confirms the logical type and full 48-byte extent. The driver reran the semantic decoder and verified all twelve stores, their identity values, and the twelve and sixteen elements in the preceding and following matrices.

The matrix initializer's current Pool field precedes later executable initialization instructions. This review followed independently decoded control flow through its return at 0x003837D0 and changes no function boundary or Pool field. That pre-existing metadata question remains separate from this data identity.

The dependent source branches are `particle-editor-parameters` at `cdf4a110069630a933d8f9fefd1e7176909ff141` and `emitter-transform-2ecbd0` at `02736aaec7395f43eb26284e4c145aa12681988c`. The new row enables subsequent linking and canonical checks; it proves neither source branch exact. No function rank or prior row changes in this evidence commit.
