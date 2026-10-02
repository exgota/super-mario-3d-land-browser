# Remaining BSS identity questions

Reviewed against the owned EU executable and main `aac68c9528598abf01538ee06f045781babb3936`. These are semantic spans and outstanding questions, not accepted complete data rows.

| Address | Observed role and span | Remaining issue |
| --- | --- | --- |
| 0x00430C68 | Guarded Matrix34 identity, 48 bytes; accessor 0x00254890, guard 0x003F389C | Initialization, independent copies and adjacent guarded matrices support the logical span. The original aggregate/allocation extent remains unresolved. |
| 0x00430CF0 | Guarded transform: 48-byte matrix, three scale floats, one flags word; 64 observed bytes | Accessors 0x002164F8 and 0x00261594 independently establish the record. Its complete original allocation boundary remains unresolved. |
| 0x00430D20 | Scale vector at parent 0x00430CF0 + 0x30, 12 bytes | This is an interior field. Do not add an overlapping standalone row. Update the dependent source to refer to the parent field once the parent identity is settled. |
| 0x004244A8 | CFL program context, observed through its +0x50 word, 84 bytes | The named setter and independent consumers agree; another 128-byte work buffer starts at 0x004244FC. Original aggregate/padding and complete allocation extent remain unresolved. |
| 0x00430DB0 | Guarded sound-command singleton; constructor 0x002585C4 initializes at least 0x2A0 bytes | Eight bytes remain between the initialized extent and the independently established BSS end 0x00431058. No next owner or exact original sizeof proves their ownership. |

Do not turn a candidate relocation, a lowest accessed span, or a one-word placeholder into a canonical data row. Only the independently bounded 0x00430A88 Matrix34 identity is proposed in the companion evidence record. The other dependent source families remain held on their actual unresolved identity questions.
