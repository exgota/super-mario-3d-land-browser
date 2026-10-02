# Complete original root and pools

Verified EU executable SHA256e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64. Existing interval0014414C..00144B3C is unchanged. The map's Pool field00144500 is the first of multiple internal pools; it is not the end of executable instructions.

```text
0014414C  push       {r0, r1, r4, r5, r6, r7, r8, sb, sl, fp, lr}
00144150  sub        sp, sp, #0xb4
00144154  mov        r6, r0
00144158  add        r0, pc, #0x3a0
0014415C  add        r2, sp, #0x40
00144160  mov        r3, #0
00144164  ldr        fp, [pc, #0x3a4]
00144168  str        r0, [sp, #0x44]
0014416C  mov        r0, r6
00144170  str        fp, [sp, #0x40]
00144174  ldr        r1, [sp, #0xb8]
00144178  bl         #0x2801e0
0014417C  mov        r7, #0
00144180  mov        r1, r6
00144184  mov        r0, #0xc
00144188  bl         #0x2932b0
0014418C  cmp        r0, #0
00144190  beq        #0x1441a0
00144194  str        r7, [r0]
00144198  str        r7, [r0, #4]
0014419C  str        r7, [r0, #8]
001441A0  mov        r3, #4
001441A4  mov        r2, #0
001441A8  mov        r1, #0xa
001441AC  str        r0, [r6, #0x64]
001441B0  bl         #0x26ac60
001441B4  nop        
001441B8  nop        
001441BC  bl         #0x32aefc
001441C0  ldrsb      r0, [r0, #0x60]
001441C4  add        r1, pc, #0x348
001441C8  str        r0, [r6, #0x68]
001441CC  add        r0, sp, #0x40
001441D0  str        r1, [sp, #0x44]
001441D4  str        fp, [sp, #0x40]
001441D8  bl         #0x243260
001441DC  add        r2, pc, #0x34c
001441E0  add        r1, sp, #0x40
001441E4  str        r2, [sp, #0x44]
001441E8  str        fp, [sp, #0x40]
001441EC  bl         #0x290640
001441F0  mov        r1, r0
001441F4  add        r0, sp, #0x50
001441F8  bl         #0x2905f0
001441FC  add        r0, sp, #0x70
00144200  nop        
00144204  bl         #0x26902c
00144208  mov        r1, r6
0014420C  mov        r0, #0x14
00144210  bl         #0x2932b0
00144214  cmp        r0, #0
00144218  nop        
0014421C  beq        #0x144234
00144220  str        r7, [r0]
00144224  str        r7, [r0, #4]
00144228  str        r7, [r0, #8]
0014422C  str        r7, [r0, #0xc]
00144230  str        r7, [r0, #0x10]
00144234  mov        r4, r0
00144238  str        r0, [r6, #0x60]
0014423C  add        r0, sp, #0x50
00144240  bl         #0x2910f8
00144244  cmp        r0, #0
00144248  mov        r1, #0
0014424C  mov        r2, #4
00144250  ble        #0x1442c4
00144254  mov        r5, r0
00144258  add        r0, r0, r5, lsl #1
0014425C  lsl        r0, r0, #3
00144260  bl         #0x2933d0
00144264  cmp        r0, #0
00144268  nop        
0014426C  beq        #0x1442c4
00144270  sub        sb, r5, #1
00144274  cmp        sb, #0
00144278  mov        r2, #5
0014427C  movgt      r1, #0
00144280  subgt      r3, r5, #1
00144284  str        r0, [r4, #0xc]
00144288  ble        #0x1442a4
0014428C  add        r8, r2, r1
00144290  subs       r3, r3, #1
00144294  add        ip, r0, r8, lsl #2
00144298  str        ip, [r0, r1, lsl #2]
0014429C  add        r1, r1, r2
001442A0  bne        #0x14428c
001442A4  mul        r2, r2, sb
001442A8  add        r1, r5, r5, lsl #2
001442AC  str        r7, [r0, r2, lsl #2]
001442B0  add        r2, r0, r1, lsl #2
001442B4  str        r0, [r4, #0x10]
001442B8  mov        r1, r5
001442BC  mov        r0, r4
001442C0  bl         #0x27a81c
001442C4  mov        r8, #0
001442C8  add        r0, sp, #0x50
001442CC  bl         #0x2910f8
001442D0  cmp        r0, #0
001442D4  nop        
001442D8  ble        #0x1444b8
001442DC  add        r0, sp, #0x38
001442E0  bl         #0x2910e8
001442E4  mov        r2, r8
001442E8  add        r1, sp, #0x38
001442EC  add        r0, sp, #0x50
001442F0  bl         #0x29107c
001442F4  ldr        r0, [r6, #0x60]
001442F8  ldm        r0, {r1, r2}
001442FC  cmp        r1, r2
00144300  movge      r7, #0
00144304  bge        #0x144354
00144308  ldr        r7, [r0, #0xc]
0014430C  cmp        r7, #0
00144310  beq        #0x144334
00144314  ldr        r1, [r7]
00144318  str        r1, [r0, #0xc]
0014431C  mov        r1, #0
00144320  str        r1, [r7]
00144324  str        r1, [r7, #4]
00144328  str        r1, [r7, #8]
0014432C  str        r1, [r7, #0xc]
00144330  str        r1, [r7, #0x10]
00144334  ldm        r0, {r1, r2}
00144338  cmp        r1, r2
0014433C  bge        #0x144354
00144340  ldr        r3, [r0, #8]
00144344  str        r7, [r3, r1, lsl #2]
00144348  ldr        r1, [r0]
0014434C  add        r1, r1, #1
00144350  str        r1, [r0]
00144354  add        r0, sp, #0x38
00144358  bl         #0x2910f8
0014435C  cmp        r0, #0
00144360  mov        r1, #0
00144364  mov        r2, #4
00144368  ble        #0x1443e0
0014436C  mov        r4, r0
00144370  mov        r5, r7
00144374  add        r0, r0, r4, lsl #1
00144378  lsl        r0, r0, #2
0014437C  bl         #0x2933d0
00144380  movs       sl, r0
00144384  nop        
00144388  beq        #0x1443e0
0014438C  sub        sb, r4, #1
00144390  cmp        sb, #0
00144394  mov        lr, #0
00144398  mov        r2, #2
0014439C  movgt      r1, #0
001443A0  subgt      r3, r4, #1
001443A4  str        sl, [r5, #0xc]
001443A8  ble        #0x1443c4
001443AC  add        ip, r2, r1
001443B0  subs       r3, r3, #1
001443B4  add        ip, r0, ip, lsl #2
001443B8  str        ip, [r0, r1, lsl #2]
001443BC  add        r1, r1, r2
001443C0  bne        #0x1443ac
001443C4  mul        r1, r2, sb
001443C8  add        r2, sl, r4, lsl #3
001443CC  str        lr, [r0, r1, lsl #2]
001443D0  mov        r1, r4
001443D4  mov        r0, r5
001443D8  str        sl, [r5, #0x10]
001443DC  bl         #0x27a81c
001443E0  mov        r5, #0
001443E4  add        r0, sp, #0x38
001443E8  bl         #0x2910f8
001443EC  cmp        r0, #0
001443F0  movgt      sb, #0
001443F4  ble        #0x1444a0
001443F8  add        r0, sp, #8
001443FC  bl         #0x2910e8
00144400  mov        r2, r5
00144404  add        r1, sp, #8
00144408  add        r0, sp, #0x38
0014440C  bl         #0x29107c
00144410  ldrd       r0, r1, [r7]
00144414  cmp        r0, r1
00144418  movge      r4, #0
0014441C  bge        #0x144460
00144420  ldr        r4, [r7, #0xc]
00144424  mov        r0, r7
00144428  cmp        r4, #0
0014442C  beq        #0x144440
00144430  ldr        r1, [r4]
00144434  str        r1, [r7, #0xc]
00144438  str        sb, [r4]
0014443C  str        sb, [r4, #4]
00144440  ldm        r0, {r1, r2}
00144444  cmp        r1, r2
00144448  bge        #0x144460
0014444C  ldr        r3, [r0, #8]
00144450  str        r4, [r3, r1, lsl #2]
00144454  ldr        r1, [r0]
00144458  add        r1, r1, #1
0014445C  str        r1, [r7]
00144460  str        sb, [r4]
00144464  add        r2, pc, #0xd0
00144468  mov        r1, r4
0014446C  add        r0, sp, #8
00144470  str        sb, [r4, #4]
00144474  bl         #0x29101c
00144478  add        r2, pc, #0xc4
0014447C  add        r1, r4, #4
00144480  add        r0, sp, #8
00144484  bl         #0x27e068
00144488  add        r5, r5, #1
0014448C  add        r0, sp, #0x38
00144490  bl         #0x2910f8
00144494  cmp        r0, r5
00144498  nop        
0014449C  bgt        #0x1443f8
001444A0  add        r8, r8, #1
001444A4  add        r0, sp, #0x50
001444A8  bl         #0x2910f8
001444AC  cmp        r0, r8
001444B0  nop        
001444B4  bgt        #0x1442dc
001444B8  ldr        r0, [sp, #0xb8]
001444BC  mov        r8, #0
001444C0  bl         #0x27ab64
001444C4  cmp        r0, #0
001444C8  movgt      sl, #2
001444CC  ble        #0x144a20
001444D0  add        r0, sp, #0x68
001444D4  bl         #0x2910e8
001444D8  ldr        r1, [sp, #0xb8]
001444DC  mov        r2, r8
001444E0  add        r0, sp, #0x68
001444E4  bl         #0x267088
001444E8  mov        r0, sp
001444EC  nop        
001444F0  bl         #0x276c3c
001444F4  nop        
001444F8  nop        
001444FC  b          #0x14454c
00144500  .word      0x65727453  ; b'Stre'
00144504  .word      0x61507465  ; b'etPa'
00144508  .word      0x624F7373  ; b'ssOb'
0014450C  .word      0x0000006A  ; b'j\x00\x00\x00'
00144510  .word      0x003D9C3C  ; b'<\x9c=\x00'
00144514  .word      0x656A624F  ; b'Obje'
00144518  .word      0x61447463  ; b'ctDa'
0014451C  .word      0x532F6174  ; b'ta/S'
00144520  .word      0x65657274  ; b'tree'
00144524  .word      0x73615074  ; b'tPas'
00144528  .word      0x6A624F73  ; b'sObj'
0014452C  .word      0x00000000  ; b'\x00\x00\x00\x00'
00144530  .word      0x656A624F  ; b'Obje'
00144534  .word      0x694C7463  ; b'ctLi'
00144538  .word      0x00007473  ; b'st\x00\x00'
0014453C  .word      0x65707954  ; b'Type'
00144540  .word      0x00000000  ; b'\x00\x00\x00\x00'
00144544  .word      0x61776552  ; b'Rewa'
00144548  .word      0x00006472  ; b'rd\x00\x00'
0014454C  ldr        r2, [sp, #0xb8]
00144550  add        r1, sp, #0x68
00144554  mov        r0, sp
00144558  bl         #0x276c08
0014455C  add        r1, pc, #0x3a8
00144560  add        r0, sp, #0x68
00144564  bl         #0x267050
00144568  cmp        r0, #0
0014456C  movne      r1, #0
00144570  beq        #0x1445f4
00144574  ldr        r2, [r6, #0x60]
00144578  ldr        r0, [r6, #0x68]
0014457C  ldr        ip, [r2]
00144580  cmp        ip, r0
00144584  ldrhi      r4, [r2, #8]
00144588  movls      r3, #0
0014458C  ldrhi      r3, [r4, r0, lsl #2]
00144590  ldr        r4, [r3]
00144594  cmp        r4, r1
00144598  ldrhi      r4, [r3, #8]
0014459C  movls      r3, #0
001445A0  ldrhi      r3, [r4, r1, lsl #2]
001445A4  cmp        ip, r0
001445A8  ldrhi      r2, [r2, #8]
001445AC  ldr        r7, [r3]
001445B0  ldrhi      r0, [r2, r0, lsl #2]
001445B4  movls      r0, #0
001445B8  ldr        r2, [r0]
001445BC  cmp        r2, r1
001445C0  ldrhi      r0, [r0, #8]
001445C4  ldrhi      r0, [r0, r1, lsl #2]
001445C8  movls      r0, #0
001445CC  cmp        r7, #0
001445D0  ldr        sb, [r0, #4]
001445D4  beq        #0x144a08
001445D8  sub        r1, pc, #0xb4
001445DC  mov        r0, r7
001445E0  bl         #0x292308
001445E4  cmp        r0, #0
001445E8  nop        
001445EC  beq        #0x144640
001445F0  b          #0x144a08
001445F4  add        r1, pc, #0x320
001445F8  add        r0, sp, #0x68
001445FC  bl         #0x267050
00144600  cmp        r0, #0
00144604  movne      r1, #1
00144608  bne        #0x144574
0014460C  add        r1, pc, #0x318
00144610  add        r0, sp, #0x68
00144614  bl         #0x267050
00144618  cmp        r0, #0
0014461C  movne      r1, #2
00144620  bne        #0x144574
00144624  add        r1, pc, #0x310
00144628  add        r0, sp, #0x68
0014462C  bl         #0x267050
00144630  cmp        r0, #0
00144634  movne      r1, #3
00144638  bne        #0x144574
0014463C  b          #0x144a08
00144640  add        r1, pc, #0x304
00144644  mov        r0, r7
00144648  bl         #0x292308
0014464C  cmp        r0, #0
00144650  nop        
00144654  beq        #0x144704
00144658  mov        r0, #0xa4
0014465C  bl         #0x2932b0
00144660  cmp        r0, #0
00144664  nop        
00144668  beq        #0x144680
0014466C  add        r1, pc, #0x2d8
00144670  str        r1, [sp, #0x84]
00144674  add        r1, sp, #0x80
00144678  str        fp, [sp, #0x80]
0014467C  bl         #0x12f204
00144680  mov        r7, r0
00144684  mov        r1, sp
00144688  str        sl, [r0, #0x68]
0014468C  bl         #0x277d94
00144690  ldr        r0, [r7, #0x64]
00144694  mov        r4, #0
00144698  cmp        r0, #0
0014469C  beq        #0x144a08
001446A0  ldr        r0, [r0, #0x28]
001446A4  add        r0, r0, #1
001446A8  cmp        r0, #0
001446AC  ble        #0x144a08
001446B0  ldr        r5, [r6, #0x64]
001446B4  mov        r1, r4
001446B8  mov        r0, r7
001446BC  bl         #0x326a04
001446C0  ldr        r1, [r5, #4]
001446C4  ldr        r2, [r5]
001446C8  cmp        r1, r2
001446CC  ble        #0x1446e4
001446D0  ldr        r1, [r5, #8]
001446D4  str        r0, [r1, r2, lsl #2]
001446D8  ldr        r0, [r5]
001446DC  add        r0, r0, #1
001446E0  str        r0, [r5]
001446E4  ldr        r0, [r7, #0x64]
001446E8  add        r4, r4, #1
001446EC  cmp        r0, #0
001446F0  ldrne      r0, [r0, #0x28]
001446F4  addne      r0, r0, #1
001446F8  cmp        r0, r4
001446FC  bgt        #0x1446b0
00144700  b          #0x144a08
00144704  add        r1, pc, #0x24c
00144708  mov        r0, r7
0014470C  bl         #0x292308
00144710  cmp        r0, #0
00144714  nop        
00144718  beq        #0x1447f8
0014471C  mov        r0, #0x78
00144720  bl         #0x2932b0
00144724  cmp        r0, #0
00144728  nop        
0014472C  beq        #0x144744
00144730  add        r1, pc, #0x220
00144734  str        r1, [sp, #0x8c]
00144738  add        r1, sp, #0x88
0014473C  str        fp, [sp, #0x88]
00144740  bl         #0x26c290
00144744  mov        r4, r0
00144748  mov        r1, sp
0014474C  bl         #0x277d94
00144750  mov        r0, r4
00144754  nop        
00144758  bl         #0x16f8a4
0014475C  mov        r0, r4
00144760  nop        
00144764  bl         #0x27d588
00144768  vldmia     r0, {s0, s1, s2}
0014476C  add        r1, sp, #0x34
00144770  mov        r0, r4
00144774  vneg.f32   s0, s0
00144778  vneg.f32   s2, s2
0014477C  vneg.f32   s1, s1
00144780  vstmia     r1, {s0, s1, s2}
00144784  bl         #0x28028c
00144788  mov        r5, r0
0014478C  nop        
00144790  bl         #0x26ccd0
00144794  mov        r2, r0
00144798  mov        r1, r5
0014479C  add        r0, sp, #0x90
001447A0  bl         #0x27cb64
001447A4  add        r3, sp, #0x90
001447A8  ldm        r3, {r0, r1, r2}
001447AC  add        r3, sp, #0x7c
001447B0  stm        r3, {r0, r1, r2}
001447B4  add        r2, sp, #0x7c
001447B8  add        r1, sp, #0x34
001447BC  add        r0, sp, #0xa0
001447C0  bl         #0x2702ec
001447C4  add        r1, sp, #0xa0
001447C8  mov        r0, r4
001447CC  bl         #0x271028
001447D0  ldr        r0, [r6, #0x64]
001447D4  ldm        r0, {r1, r2}
001447D8  cmp        r1, r2
001447DC  bge        #0x144a08
001447E0  ldr        r2, [r0, #8]
001447E4  str        r4, [r2, r1, lsl #2]
001447E8  ldr        r1, [r0]
001447EC  add        r1, r1, #1
001447F0  str        r1, [r0]
001447F4  b          #0x144a08
001447F8  mov        r1, r7
001447FC  add        r0, sp, #0x70
00144800  bl         #0x268eb0
00144804  cmp        r0, #0
00144808  str        r0, [sp, #0x38]
0014480C  beq        #0x144a08
00144810  mov        r4, #0
00144814  mov        r0, sp
00144818  bl         #0x27ab64
0014481C  cmp        r0, #0
00144820  nop        
00144824  ble        #0x1449c4
00144828  mov        r1, r4
0014482C  mov        r0, sp
00144830  bl         #0x27aaec
00144834  mov        r1, r0
00144838  mov        r0, r7
0014483C  bl         #0x292308
00144840  cmp        r0, #0
00144844  nop        
00144848  beq        #0x1449ac
0014484C  add        r1, pc, #0x118
00144850  mov        r0, r7
00144854  bl         #0x292308
00144858  cmp        r0, #0
0014485C  nop        
00144860  beq        #0x144984
00144864  mov        r0, #0x84
00144868  bl         #0x2932b0
0014486C  movs       r2, r0
00144870  streq      r2, [sp, #0x34]
00144874  beq        #0x144894
00144878  add        r0, pc, #0xec
0014487C  str        r0, [sp, #0x84]
00144880  add        r1, sp, #0x80
00144884  mov        r0, r2
00144888  str        fp, [sp, #0x80]
0014488C  bl         #0x16743c
00144890  str        r0, [sp, #0x34]
00144894  mov        r2, r4
00144898  mov        r1, sp
0014489C  bl         #0x27a9d4
001448A0  ldr        r0, [sp, #0x34]
001448A4  mov        r4, #0
001448A8  bl         #0x166f3c
001448AC  cmp        r0, #0
001448B0  nop        
001448B4  ble        #0x144904
001448B8  ldr        r5, [r6, #0x64]
001448BC  ldr        r0, [sp, #0x34]
001448C0  mov        r1, r4
001448C4  bl         #0x16742c
001448C8  ldr        r1, [r5, #4]
001448CC  ldr        r2, [r5]
001448D0  cmp        r2, r1
001448D4  bge        #0x1448ec
001448D8  ldr        r1, [r5, #8]
001448DC  str        r0, [r1, r2, lsl #2]
001448E0  ldr        r0, [r5]
001448E4  add        r0, r0, #1
001448E8  str        r0, [r5]
001448EC  ldr        r0, [sp, #0x34]
001448F0  add        r4, r4, #1
001448F4  bl         #0x166f3c
001448F8  cmp        r0, r4
001448FC  nop        
00144900  bgt        #0x1448b8
00144904  nop        
00144908  b          #0x14497c
0014490C  .word      0x65727453  ; b'Stre'
00144910  .word      0x61507465  ; b'etPa'
00144914  .word      0x624F7373  ; b'ssOb'
00144918  .word      0x0000316A  ; b'j1\x00\x00'
0014491C  .word      0x65727453  ; b'Stre'
00144920  .word      0x61507465  ; b'etPa'
00144924  .word      0x624F7373  ; b'ssOb'
00144928  .word      0x0000326A  ; b'j2\x00\x00'
0014492C  .word      0x65727453  ; b'Stre'
00144930  .word      0x61507465  ; b'etPa'
00144934  .word      0x624F7373  ; b'ssOb'
00144938  .word      0x0000336A  ; b'j3\x00\x00'
0014493C  .word      0x65727453  ; b'Stre'
00144940  .word      0x61507465  ; b'etPa'
00144944  .word      0x624F7373  ; b'ssOb'
00144948  .word      0x0000346A  ; b'j4\x00\x00'
0014494C  .word      0x6972754B  ; b'Kuri'
00144950  .word      0x6F546F62  ; b'boTo'
00144954  .word      0x00726577  ; b'wer\x00'
00144958  .word      0x6972754B  ; b'Kuri'
0014495C  .word      0x61546F62  ; b'boTa'
00144960  .word      0x65536C69  ; b'ilSe'
00144964  .word      0x68637261  ; b'arch'
00144968  .word      0x00000000  ; b'\x00\x00\x00\x00'
0014496C  .word      0x746E6554  ; b'Tent'
00144970  .word      0x65476E65  ; b'enGe'
00144974  .word      0x6172656E  ; b'nera'
00144978  .word      0x00726F74  ; b'tor\x00'
0014497C  ldr        r5, [sp, #0x34]
00144980  b          #0x1449a0
00144984  ldr        r1, [sp, #0x38]
00144988  mov        r0, r7
0014498C  blx        r1
00144990  mov        r5, r0
00144994  mov        r2, r4
00144998  mov        r1, sp
0014499C  bl         #0x27a9d4
001449A0  cmp        r5, #0
001449A4  bne        #0x1449dc
001449A8  b          #0x1449c4
001449AC  add        r4, r4, #1
001449B0  mov        r0, sp
001449B4  bl         #0x27ab64
001449B8  cmp        r0, r4
001449BC  nop        
001449C0  bgt        #0x144828
001449C4  ldr        r1, [sp, #0x38]
001449C8  mov        r0, r7
001449CC  blx        r1
001449D0  mov        r5, r0
001449D4  mov        r1, sp
001449D8  bl         #0x277d94
001449DC  cmp        sb, #0
001449E0  beq        #0x144a08
001449E4  ldr        r0, [r6, #0x64]
001449E8  ldm        r0, {r1, r2}
001449EC  cmp        r2, r1
001449F0  ble        #0x144a08
001449F4  ldr        r2, [r0, #8]
001449F8  str        r5, [r2, r1, lsl #2]
001449FC  ldr        r1, [r0]
00144A00  add        r1, r1, #1
00144A04  str        r1, [r0]
00144A08  ldr        r0, [sp, #0xb8]
00144A0C  add        r8, r8, #1
00144A10  bl         #0x27ab64
00144A14  cmp        r0, r8
00144A18  nop        
00144A1C  bgt        #0x1444d0
00144A20  ldr        r0, [r6, #0x64]
00144A24  ldr        r0, [r0]
00144A28  cmp        r0, #0
00144A2C  beq        #0x144b04
00144A30  bl         #0x327e90
00144A34  strb       r0, [r6, #0x6c]
00144A38  ldr        r1, [sp, #0xb8]
00144A3C  mov        r2, #0xf
00144A40  mov        r0, r6
00144A44  bl         #0x27b51c
00144A48  ldr        r1, [sp, #0xb8]
00144A4C  mov        r0, r6
00144A50  bl         #0x11c224
00144A54  mov        r1, r6
00144A58  mov        r0, #0x78
00144A5C  bl         #0x2932b0
00144A60  cmp        r0, #0
00144A64  movne      r1, #0xf
00144A68  blne       #0x1b88b8
00144A6C  str        r0, [r6, #0x70]
00144A70  ldr        r1, [r0]
00144A74  ldr        r2, [r1, #4]
00144A78  ldr        r1, [sp, #0xb8]
00144A7C  blx        r2
00144A80  ldrb       r0, [r6, #0x6c]
00144A84  cmp        r0, #0
00144A88  movne      r4, #0
00144A8C  beq        #0x144ac0
00144A90  b          #0x144ab0
00144A94  ldrhi      r1, [r0, #8]
00144A98  movls      r0, #0
00144A9C  mov        r2, #1
00144AA0  ldrhi      r0, [r1, r4, lsl #2]
00144AA4  ldr        r1, [sp, #0xb8]
00144AA8  bl         #0x277e5c
00144AAC  add        r4, r4, #1
00144AB0  ldr        r0, [r6, #0x64]
00144AB4  ldr        r1, [r0]
00144AB8  cmp        r1, r4
00144ABC  bgt        #0x144a94
00144AC0  ldr        r3, [pc, #0x6c]
00144AC4  ldr        r2, [pc, #0x6c]
00144AC8  ldrd       r0, r1, [r3, #8]
00144ACC  strd       r0, r1, [sp, #0x28]
00144AD0  add        r0, sp, #0x20
00144AD4  stm        r0, {r2, r6}
00144AD8  ldrd       r0, r1, [r3]
00144ADC  add        r3, sp, #0x58
00144AE0  stm        r3, {r2, r6}
00144AE4  add        r2, sp, #0x20
00144AE8  strd       r0, r1, [sp, #0x60]
00144AEC  add        r1, sp, #0x58
00144AF0  add        r0, r6, #0xc
00144AF4  bl         #0x280474
00144AF8  cmp        r0, #0
00144AFC  nop        
00144B00  beq        #0x144b1c
00144B04  ldr        r0, [r6]
00144B08  ldr        r1, [r0, #0x18]
00144B0C  mov        r0, r6
00144B10  blx        r1
00144B14  add        sp, sp, #0xbc
00144B18  pop        {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00144B1C  ldr        r0, [r6]
00144B20  ldr        r1, [r0, #0x10]
00144B24  mov        r0, r6
00144B28  blx        r1
00144B2C  add        sp, sp, #0xbc
00144B30  pop        {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00144B34  .word      0x003BF430  ; b'0\xf4;\x00'
00144B38  .word      0x003D5B9C  ; b'\x9c[=\x00'
```
