# Original display initializer disassembly

EU root 0x0028F9DC..0x002905F0, 3,092 bytes. The map pool marker is interleaved; the final 164 bytes are executable tail code. This is source-review evidence from the authorized private dump, not a source implementation or a matching claim.

```text
0028F9DC  push      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0028F9E0  sub       sp, sp, #0xc
0028F9E4  movs      r4, r0
0028F9E8  mov       r5, r1
0028F9EC  mov       r6, #1
0028F9F0  cmpne     r5, #0
0028F9F4  str       r6, [sp, #8]
0028F9F8  beq       #0x28fa0c
0028F9FC  ldr       sb, [pc, #0x9c8]
0028FA00  ldrb      r0, [sb, #0x10]
0028FA04  cmp       r0, #0
0028FA08  beq       #0x28fa18
0028FA0C  add       sp, sp, #0xc
0028FA10  mov       r0, #0
0028FA14  pop       {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0028FA18  ldr       r0, [pc, #0x9ac]
0028FA1C  mov       r1, #0x180
0028FA20  bl        #0x28d1f0
0028FA24  mov       r0, #0xf0
0028FA28  mov       r1, #0x190
0028FA2C  str       r0, [sb]
0028FA30  str       r1, [sb, #8]
0028FA34  ldr       r8, [pc, #0x994]
0028FA38  str       r0, [sb, #4]
0028FA3C  mov       r0, #0x140
0028FA40  str       r0, [sb, #0xc]
0028FA44  strd      r4, r5, [r8]
0028FA48  str       r6, [r8, #0xc]
0028FA4C  mov       r0, #0x700
0028FA50  str       r6, [r8, #8]
0028FA54  str       r0, [sb, #0x164]
0028FA58  ldr       r3, [pc, #0x974]
0028FA5C  mov       r2, #0
0028FA60  mov       r1, #0x100
0028FA64  mov       r0, #0x10000
0028FA68  blx       r4
0028FA6C  cmp       r0, #0
0028FA70  str       r0, [r8, #0x10]
0028FA74  beq       #0x2905d8
0028FA78  add       r4, r0, #0x400
0028FA7C  add       r5, r0, #0x400
0028FA80  add       r7, r0, #0xc00
0028FA84  add       fp, r0, #0x2000
0028FA88  add       sl, r0, #0x2c00
0028FA8C  add       r4, r4, #0x358
0028FA90  add       r5, r5, #0x368
0028FA94  add       r7, r7, #0x3ac
0028FA98  add       fp, fp, #0x720
0028FA9C  add       sl, sl, #0x3cc
0028FAA0  bl        #0x106b74
0028FAA4  cmp       r0, #0
0028FAA8  nop       
0028FAAC  blt       #0x2905d8
0028FAB0  mov       r0, r4
0028FAB4  bl        #0x106444
0028FAB8  cmp       r0, #0
0028FABC  nop       
0028FAC0  blt       #0x2905d8
0028FAC4  mov       r0, r5
0028FAC8  bl        #0x107168
0028FACC  cmp       r0, #0
0028FAD0  nop       
0028FAD4  blt       #0x2905d8
0028FAD8  mov       r0, r7
0028FADC  bl        #0x1064e4
0028FAE0  cmp       r0, #0
0028FAE4  nop       
0028FAE8  blt       #0x2905d8
0028FAEC  mov       r0, fp
0028FAF0  bl        #0x106fb4
0028FAF4  cmp       r0, #0
0028FAF8  nop       
0028FAFC  blt       #0x2905d8
0028FB00  bl        #0x28ca18
0028FB04  ldr       r0, [sp, #8]
0028FB08  nop       
0028FB0C  bl        #0x28f6f0
0028FB10  ldr       r5, [sb, #0x9c]
0028FB14  mov       r7, #0x10000
0028FB18  mov       r4, #0
0028FB1C  cmp       r5, #0
0028FB20  beq       #0x28fbc0
0028FB24  ldr       r3, [r5, #4]
0028FB28  ldr       fp, [pc, #0x8a8]
0028FB2C  cmp       r3, #0
0028FB30  ldrne     ip, [r8, #4]
0028FB34  cmpne     ip, #0
0028FB38  beq       #0x28fb4c
0028FB3C  ldr       r2, [r5]
0028FB40  mov       r1, fp
0028FB44  mov       r0, #0x10000
0028FB48  blx       ip
0028FB4C  mov       r0, #0x10
0028FB50  str       r0, [r5, #0x1c]
0028FB54  str       r4, [r5, #0xc]
0028FB58  str       r7, [r5, #8]
0028FB5C  str       r4, [r5, #0x20]
0028FB60  str       r4, [r5, #0x10]
0028FB64  str       r4, [r5, #0x28]
0028FB68  str       r4, [r5, #0x24]
0028FB6C  ldr       ip, [r8]
0028FB70  cmp       ip, #0
0028FB74  moveq     r0, #0
0028FB78  beq       #0x28fb90
0028FB7C  ldr       r3, [pc, #0x858]
0028FB80  ldr       r2, [r5]
0028FB84  mov       r1, fp
0028FB88  mov       r0, #0x10000
0028FB8C  blx       ip
0028FB90  str       r0, [r5, #4]
0028FB94  add       r0, r0, #0x10000
0028FB98  mov       r1, #0x1c0
0028FB9C  str       r0, [r5, #0x18]
0028FBA0  bl        #0x28d1f0
0028FBA4  ldr       r1, [pc, #0x834]
0028FBA8  ldr       r0, [r5, #4]
0028FBAC  str       r0, [r1]
0028FBB0  ldr       r1, [r5, #8]
0028FBB4  add       r0, r0, r1
0028FBB8  ldr       r1, [pc, #0x824]
0028FBBC  str       r0, [r1]
0028FBC0  ldr       r0, [sb, #0x9c]
0028FBC4  cmp       r0, #0
0028FBC8  movne     r1, #0x300
0028FBCC  strne     r1, [r0, #0x2c]
0028FBD0  nop       
0028FBD4  bl        #0x28ca18
0028FBD8  cmp       r0, #0
0028FBDC  nop       
0028FBE0  bne       #0x2905d8
0028FBE4  mov       r0, sl
0028FBE8  bl        #0x1071d8
0028FBEC  cmp       r0, #0
0028FBF0  nop       
0028FBF4  blt       #0x2905d8
0028FBF8  bl        #0x107a24
0028FBFC  nop       
0028FC00  nop       
0028FC04  bl        #0x107b10
0028FC08  mov       r5, r0
0028FC0C  nop       
0028FC10  bl        #0x107c0c
0028FC14  ldr       r1, [pc, #0x7b0]
0028FC18  str       r0, [r1, #0x14]
0028FC1C  ldr       r0, [pc, #0x7c4]
0028FC20  mov       r1, #0
0028FC24  bl        #0x107b24
0028FC28  ldr       r0, [pc, #0x7bc]
0028FC2C  mov       r1, #1
0028FC30  bl        #0x107b24
0028FC34  ldr       r0, [pc, #0x7b4]
0028FC38  mov       r1, #2
0028FC3C  bl        #0x107b24
0028FC40  ldr       r0, [pc, #0x7ac]
0028FC44  mov       r1, #3
0028FC48  bl        #0x107b24
0028FC4C  ldr       r0, [pc, #0x7a4]
0028FC50  mov       r1, #4
0028FC54  bl        #0x107b24
0028FC58  ldr       r0, [pc, #0x79c]
0028FC5C  mov       r1, #5
0028FC60  bl        #0x107b24
0028FC64  ldr       r0, [pc, #0x794]
0028FC68  mov       r1, #6
0028FC6C  bl        #0x107b24
0028FC70  cmp       r5, #0
0028FC74  nop       
0028FC78  beq       #0x290280
0028FC7C  ldr       r0, [pc, #0x780]
0028FC80  mov       r2, #4
0028FC84  add       r1, sp, #4
0028FC88  str       r4, [sp, #4]
0028FC8C  bl        #0x28ca14
0028FC90  ldr       r0, [pc, #0x770]
0028FC94  mov       r2, #4
0028FC98  add       r1, sp, #4
0028FC9C  str       r0, [sp, #4]
0028FCA0  ldr       r0, [pc, #0x764]
0028FCA4  bl        #0x28ca14
0028FCA8  mvn       r0, #0xf
0028FCAC  str       r0, [sp, #4]
0028FCB0  ldr       r0, [pc, #0x758]
0028FCB4  mov       r2, #4
0028FCB8  add       r1, sp, #4
0028FCBC  bl        #0x28ca14
0028FCC0  ldr       r0, [pc, #0x74c]
0028FCC4  mov       r2, #4
0028FCC8  add       r1, sp, #4
0028FCCC  str       r6, [sp, #4]
0028FCD0  bl        #0x28ca14
0028FCD4  ldr       r0, [pc, #0x73c]
0028FCD8  mov       r2, #4
0028FCDC  add       r1, sp, #4
0028FCE0  str       r0, [sp, #4]
0028FCE4  ldr       r0, [pc, #0x730]
0028FCE8  bl        #0x28ca14
0028FCEC  mov       sb, #0xd1
0028FCF0  ldr       r0, [pc, #0x728]
0028FCF4  mov       r2, #4
0028FCF8  add       r1, sp, #4
0028FCFC  str       sb, [sp, #4]
0028FD00  bl        #0x28ca14
0028FD04  add       sl, sb, #0xf0
0028FD08  ldr       r0, [pc, #0x714]
0028FD0C  mov       r2, #4
0028FD10  add       r1, sp, #4
0028FD14  str       sl, [sp, #4]
0028FD18  bl        #0x28ca14
0028FD1C  ldr       r0, [pc, #0x704]
0028FD20  mov       r2, #4
0028FD24  add       r1, sp, #4
0028FD28  str       sl, [sp, #4]
0028FD2C  bl        #0x28ca14
0028FD30  ldr       r0, [pc, #0x6f4]
0028FD34  mov       r2, #4
0028FD38  add       r1, sp, #4
0028FD3C  str       r4, [sp, #4]
0028FD40  bl        #0x28ca14
0028FD44  mov       r0, #0xcf
0028FD48  str       r0, [sp, #4]
0028FD4C  ldr       r0, [pc, #0x6dc]
0028FD50  mov       r2, #4
0028FD54  add       r1, sp, #4
0028FD58  bl        #0x28ca14
0028FD5C  ldr       r0, [pc, #0x6d0]
0028FD60  mov       r2, #4
0028FD64  add       r1, sp, #4
0028FD68  str       sb, [sp, #4]
0028FD6C  bl        #0x28ca14
0028FD70  ldr       r0, [pc, #0x6c0]
0028FD74  mov       r2, #4
0028FD78  add       r1, sp, #4
0028FD7C  str       r0, [sp, #4]
0028FD80  ldr       r0, [pc, #0x6b4]
0028FD84  bl        #0x28ca14
0028FD88  mov       r0, #0x10000
0028FD8C  str       r0, [sp, #4]
0028FD90  ldr       r0, [pc, #0x6a8]
0028FD94  mov       r2, #4
0028FD98  add       r1, sp, #4
0028FD9C  bl        #0x28ca14
0028FDA0  add       r0, sb, #0xcc
0028FDA4  str       r0, [sp, #4]
0028FDA8  ldr       r0, [pc, #0x694]
0028FDAC  mov       r2, #4
0028FDB0  add       r1, sp, #4
0028FDB4  bl        #0x28ca14
0028FDB8  mov       fp, #2
0028FDBC  ldr       r0, [pc, #0x684]
0028FDC0  mov       r2, #4
0028FDC4  add       r1, sp, #4
0028FDC8  str       fp, [sp, #4]
0028FDCC  bl        #0x28ca14
0028FDD0  add       r7, sb, #0xc1
0028FDD4  ldr       r0, [pc, #0x670]
0028FDD8  mov       r2, #4
0028FDDC  add       r1, sp, #4
0028FDE0  str       r7, [sp, #4]
0028FDE4  bl        #0x28ca14
0028FDE8  ldr       r0, [pc, #0x660]
0028FDEC  mov       r2, #4
0028FDF0  add       r1, sp, #4
0028FDF4  str       r7, [sp, #4]
0028FDF8  bl        #0x28ca14
0028FDFC  ldr       r0, [pc, #0x650]
0028FE00  mov       r2, #4
0028FE04  add       r1, sp, #4
0028FE08  str       r7, [sp, #4]
0028FE0C  bl        #0x28ca14
0028FE10  ldr       r0, [pc, #0x640]
0028FE14  mov       r2, #4
0028FE18  add       r1, sp, #4
0028FE1C  str       r6, [sp, #4]
0028FE20  bl        #0x28ca14
0028FE24  ldr       r0, [pc, #0x630]
0028FE28  mov       r2, #4
0028FE2C  add       r1, sp, #4
0028FE30  str       fp, [sp, #4]
0028FE34  bl        #0x28ca14
0028FE38  ldr       r0, [pc, #0x620]
0028FE3C  mov       r2, #4
0028FE40  add       r1, sp, #4
0028FE44  str       r0, [sp, #4]
0028FE48  ldr       r0, [pc, #0x614]
0028FE4C  bl        #0x28ca14
0028FE50  ldr       r0, [pc, #0x610]
0028FE54  mov       r2, #4
0028FE58  add       r1, sp, #4
0028FE5C  str       r4, [sp, #4]
0028FE60  bl        #0x28ca14
0028FE64  ldr       r0, [pc, #0x600]
0028FE68  mov       r2, #4
0028FE6C  add       r1, sp, #4
0028FE70  str       r4, [sp, #4]
0028FE74  bl        #0x28ca14
0028FE78  ldr       r0, [pc, #0x5f0]
0028FE7C  mov       r2, #4
0028FE80  add       r1, sp, #4
0028FE84  str       r0, [sp, #4]
0028FE88  ldr       r0, [pc, #0x5e4]
0028FE8C  bl        #0x28ca14
0028FE90  orr       fp, sb, sl, lsl #16
0028FE94  ldr       r0, [pc, #0x5dc]
0028FE98  mov       r2, #4
0028FE9C  add       r1, sp, #4
0028FEA0  str       fp, [sp, #4]
0028FEA4  bl        #0x28ca14
0028FEA8  ldr       r0, [pc, #0x5cc]
0028FEAC  mov       r2, #4
0028FEB0  add       r1, sp, #4
0028FEB4  str       r0, [sp, #4]
0028FEB8  ldr       r0, [pc, #0x5c0]
0028FEBC  bl        #0x28ca14
0028FEC0  ldr       r0, [pc, #0x5bc]
0028FEC4  mov       r2, #4
0028FEC8  add       r1, sp, #4
0028FECC  str       r0, [sp, #4]
0028FED0  ldr       r0, [pc, #0x5b0]
0028FED4  bl        #0x28ca14
0028FED8  ldr       r0, [pc, #0x5ac]
0028FEDC  mov       r2, #4
0028FEE0  add       r1, sp, #4
0028FEE4  str       r4, [sp, #4]
0028FEE8  bl        #0x28ca14
0028FEEC  add       r0, r7, #0x30
0028FEF0  str       r0, [sp, #4]
0028FEF4  ldr       r0, [pc, #0x594]
0028FEF8  mov       r2, #4
0028FEFC  add       r1, sp, #4
0028FF00  bl        #0x28ca14
0028FF04  ldr       r0, [pc, #0x588]
0028FF08  mov       r2, #4
0028FF0C  add       r1, sp, #4
0028FF10  str       sb, [sp, #4]
0028FF14  bl        #0x28ca14
0028FF18  ldr       r0, [pc, #0x578]
0028FF1C  mov       r2, #4
0028FF20  add       r1, sp, #4
0028FF24  str       sl, [sp, #4]
0028FF28  bl        #0x28ca14
0028FF2C  ldr       r0, [pc, #0x568]
0028FF30  mov       r2, #4
0028FF34  add       r1, sp, #4
0028FF38  str       sl, [sp, #4]
0028FF3C  bl        #0x28ca14
0028FF40  mov       r0, #0xcd
0028FF44  str       r0, [sp, #4]
0028FF48  ldr       r0, [pc, #0x550]
0028FF4C  mov       r2, #4
0028FF50  add       r1, sp, #4
0028FF54  bl        #0x28ca14
0028FF58  mov       r0, #0xcf
0028FF5C  str       r0, [sp, #4]
0028FF60  ldr       r0, [pc, #0x53c]
0028FF64  mov       r2, #4
0028FF68  add       r1, sp, #4
0028FF6C  bl        #0x28ca14
0028FF70  ldr       r0, [pc, #0x530]
0028FF74  mov       r2, #4
0028FF78  add       r1, sp, #4
0028FF7C  str       sb, [sp, #4]
0028FF80  bl        #0x28ca14
0028FF84  ldr       r0, [pc, #0x4ac]
0028FF88  mov       r2, #4
0028FF8C  add       r1, sp, #4
0028FF90  str       r0, [sp, #4]
0028FF94  ldr       r0, [pc, #0x510]
0028FF98  bl        #0x28ca14
0028FF9C  mov       r0, #0x10000
0028FFA0  str       r0, [sp, #4]
0028FFA4  ldr       r0, [pc, #0x504]
0028FFA8  mov       r2, #4
0028FFAC  add       r1, sp, #4
0028FFB0  bl        #0x28ca14
0028FFB4  add       r0, r7, #0xb
0028FFB8  str       r0, [sp, #4]
0028FFBC  ldr       r0, [pc, #0x4f0]
0028FFC0  mov       r2, #4
0028FFC4  add       r1, sp, #4
0028FFC8  bl        #0x28ca14
0028FFCC  mov       sb, #0x52
0028FFD0  ldr       r0, [pc, #0x4e0]
0028FFD4  mov       r2, #4
0028FFD8  add       r1, sp, #4
0028FFDC  str       sb, [sp, #4]
0028FFE0  bl        #0x28ca14
0028FFE4  ldr       r0, [pc, #0x4d0]
0028FFE8  mov       r2, #4
0028FFEC  add       r1, sp, #4
0028FFF0  str       r7, [sp, #4]
0028FFF4  bl        #0x28ca14
0028FFF8  ldr       r0, [pc, #0x4c0]
0028FFFC  mov       r2, #4
00290000  add       r1, sp, #4
00290004  str       r7, [sp, #4]
00290008  bl        #0x28ca14
0029000C  mov       r0, #0x4f
00290010  str       r0, [sp, #4]
00290014  ldr       r0, [pc, #0x4a8]
00290018  mov       r2, #4
0029001C  add       r1, sp, #4
00290020  bl        #0x28ca14
00290024  mov       r0, #0x50
00290028  str       r0, [sp, #4]
0029002C  ldr       r0, [pc, #0x494]
00290030  mov       r2, #4
00290034  add       r1, sp, #4
00290038  bl        #0x28ca14
0029003C  ldr       r0, [pc, #0x488]
00290040  mov       r2, #4
00290044  add       r1, sp, #4
00290048  str       sb, [sp, #4]
0029004C  bl        #0x28ca14
00290050  ldr       r0, [pc, #0x478]
00290054  mov       r2, #4
00290058  add       r1, sp, #4
0029005C  str       r0, [sp, #4]
00290060  ldr       r0, [pc, #0x46c]
00290064  bl        #0x28ca14
00290068  ldr       r0, [pc, #0x468]
0029006C  mov       r2, #4
00290070  add       r1, sp, #4
00290074  str       r4, [sp, #4]
00290078  bl        #0x28ca14
0029007C  mov       r0, #0x11
00290080  str       r0, [sp, #4]
00290084  ldr       r0, [pc, #0x450]
00290088  mov       r2, #4
0029008C  add       r1, sp, #4
00290090  bl        #0x28ca14
00290094  ldr       r0, [pc, #0x444]
00290098  mov       r2, #4
0029009C  add       r1, sp, #4
002900A0  str       r0, [sp, #4]
002900A4  ldr       r0, [pc, #0x438]
002900A8  bl        #0x28ca14
002900AC  ldr       r0, [pc, #0x434]
002900B0  mov       r2, #4
002900B4  add       r1, sp, #4
002900B8  str       fp, [sp, #4]
002900BC  bl        #0x28ca14
002900C0  orr       r0, sb, r7, lsl #16
002900C4  str       r0, [sp, #4]
002900C8  ldr       r0, [pc, #0x41c]
002900CC  mov       r2, #4
002900D0  add       r1, sp, #4
002900D4  bl        #0x28ca14
002900D8  ldr       r0, [pc, #0x410]
002900DC  mov       r2, #4
002900E0  add       r1, sp, #4
002900E4  str       r0, [sp, #4]
002900E8  ldr       r0, [pc, #0x404]
002900EC  bl        #0x28ca14
002900F0  ldr       r0, [pc, #0x400]
002900F4  mov       r2, #4
002900F8  add       r1, sp, #4
002900FC  str       r4, [sp, #4]
00290100  bl        #0x28ca14
00290104  ldr       r7, [pc, #0x3f0]
00290108  ldr       r0, [pc, #0x3f0]
0029010C  mov       r2, #4
00290110  add       r1, sp, #4
00290114  str       r7, [sp, #4]
00290118  bl        #0x28ca14
0029011C  ldr       r0, [pc, #0x3e0]
00290120  mov       r2, #4
00290124  add       r1, sp, #4
00290128  str       r7, [sp, #4]
0029012C  bl        #0x28ca14
00290130  ldr       r0, [pc, #0x3d0]
00290134  mov       r2, #4
00290138  add       r1, sp, #4
0029013C  str       r7, [sp, #4]
00290140  bl        #0x28ca14
00290144  ldr       r0, [pc, #0x3c0]
00290148  mov       r2, #4
0029014C  add       r1, sp, #4
00290150  str       r7, [sp, #4]
00290154  bl        #0x28ca14
00290158  ldr       r0, [pc, #0x3b0]
0029015C  mov       r2, #4
00290160  add       r1, sp, #4
00290164  str       r7, [sp, #4]
00290168  bl        #0x28ca14
0029016C  ldr       r0, [pc, #0x3a0]
00290170  mov       r2, #4
00290174  add       r1, sp, #4
00290178  str       r7, [sp, #4]
0029017C  bl        #0x28ca14
00290180  ldr       r0, [pc, #0x390]
00290184  mov       r2, #4
00290188  add       r1, sp, #4
0029018C  str       r6, [sp, #4]
00290190  bl        #0x28ca14
00290194  ldr       r0, [pc, #0x380]
00290198  mov       r2, #4
0029019C  add       r1, sp, #4
002901A0  str       r6, [sp, #4]
002901A4  bl        #0x28ca14
002901A8  mov       r0, #0xff00
002901AC  stm       sp, {r0, r4}
002901B0  ldr       r0, [pc, #0x368]
002901B4  mov       r3, #4
002901B8  mov       r2, sp
002901BC  add       r1, sp, #4
002901C0  bl        #0x107adc
002901C4  ldr       r0, [pc, #0x358]
002901C8  mov       r2, #4
002901CC  add       r1, sp, #4
002901D0  str       r0, [sp, #4]
002901D4  orr       r0, r2, r2, lsl #20
002901D8  bl        #0x28ca14
002901DC  mov       r7, #0xff
002901E0  ldr       r0, [pc, #0x340]
002901E4  mov       r3, #4
002901E8  mov       r2, sp
002901EC  add       r1, sp, #4
002901F0  str       r4, [sp, #4]
002901F4  str       r7, [sp]
002901F8  bl        #0x107adc
002901FC  ldr       r0, [pc, #0x328]
00290200  mov       r3, #4
00290204  mov       r2, sp
00290208  add       r1, sp, #4
0029020C  str       r4, [sp, #4]
00290210  str       r7, [sp]
00290214  bl        #0x107adc
00290218  ldr       r0, [pc, #0x310]
0029021C  mov       r2, #4
00290220  add       r1, sp, #4
00290224  str       r0, [sp, #4]
00290228  ldr       r0, [pc, #0x304]
0029022C  bl        #0x28ca14
00290230  orr       r0, sb, r7, lsl #4
00290234  str       r0, [sp, #4]
00290238  orr       r0, r7, r0, lsl #4
0029023C  str       r0, [sp]
00290240  ldr       r0, [pc, #0x2f0]
00290244  mov       r3, #4
00290248  mov       r2, sp
0029024C  add       r1, sp, #4
00290250  bl        #0x107adc
00290254  ldr       r7, [pc, #0x2e0]
00290258  ldr       r0, [pc, #0x2e0]
0029025C  mov       r2, #4
00290260  add       r1, sp, #4
00290264  str       r7, [sp, #4]
00290268  bl        #0x28ca14
0029026C  ldr       r0, [pc, #0x2d0]
00290270  mov       r2, #4
00290274  add       r1, sp, #4
00290278  str       r7, [sp, #4]
0029027C  bl        #0x28ca14
00290280  nop       
00290284  bl        #0x28c890
00290288  ldr       sl, [pc, #0x13c]
0029028C  cmp       r5, #0
00290290  ldr       fp, [sl, #0x9c]
00290294  beq       #0x290350
00290298  ldr       r0, [fp, #0x20]
0029029C  ldr       r1, [fp, #0x18]
002902A0  rsb       r0, r0, r0, lsl #3
002902A4  add       r7, r1, r0, lsl #2
002902A8  mov       r0, #3
002902AC  strb      r0, [r7]
002902B0  ldr       r0, [r7, #0x14]
002902B4  bic       r0, r0, #7
002902B8  orr       r0, r0, #4
002902BC  str       r0, [r7, #0x14]
002902C0  mov       r0, #0x30000
002902C4  bl        #0x28e240
002902C8  sub       r0, r0, #0x7f00
002902CC  sub       r0, r0, #0xff
002902D0  str       r0, [r7, #4]
002902D4  ldr       ip, [r8]
002902D8  cmp       ip, #0
002902DC  moveq     r0, #0
002902E0  beq       #0x2902f8
002902E4  ldr       r3, [pc, #0x25c]
002902E8  mov       r2, #0
002902EC  mov       r1, #0x100
002902F0  mov       r0, #0x10000
002902F4  blx       ip
002902F8  movs      sb, r0
002902FC  beq       #0x2905d8
00290300  tst       sb, #0xf
00290304  str       sb, [r7, #8]
00290308  beq       #0x29031c
0029030C  and       r1, sb, #0xf
00290310  rsb       r1, r1, #0x10
00290314  add       r0, sb, r1
00290318  str       r0, [r7, #8]
0029031C  ldr       r1, [r7, #0x14]
00290320  mov       r0, #0x80
00290324  strh      r0, [r7, #0xc]
00290328  strh      r0, [r7, #0xe]
0029032C  strh      r0, [r7, #0x10]
00290330  strh      r0, [r7, #0x12]
00290334  bic       r0, r1, #0xc00
00290338  bic       r0, r0, #0x3f8
0029033C  orr       r0, r0, #0x20
00290340  str       r0, [r7, #0x14]
00290344  ldr       r0, [fp, #0x20]
00290348  add       r0, r0, #1
0029034C  str       r0, [fp, #0x20]
00290350  ldr       r0, [sl, #0x9c]
00290354  cmp       r0, #0
00290358  beq       #0x2903ac
0029035C  ldrb      r1, [sl, #0x11]
00290360  cmp       r1, #0
00290364  bne       #0x2903b8
00290368  str       r0, [sl, #0xa0]
0029036C  strb      r6, [sl, #0x12]
00290370  ldr       r1, [r0, #0x28]
00290374  ldr       r2, [r0, #0x20]
00290378  cmp       r1, r2
0029037C  bge       #0x29054c
00290380  strb      r6, [sl, #0x11]
00290384  ldr       r0, [r0, #0x2c]
00290388  cmp       r0, #0x300
0029038C  bne       #0x2903b8
00290390  bl        #0x28a814
00290394  nop       
00290398  nop       
0029039C  bl        #0x28c640
002903A0  nop       
002903A4  nop       
002903A8  bl        #0x28a7b4
002903AC  ldrb      r0, [sl, #0x11]
002903B0  cmp       r0, #0
002903B4  beq       #0x29054c
002903B8  nop       
002903BC  bl        #0x107acc
002903C0  nop       
002903C4  nop       
002903C8  b         #0x2903ac

LITERALS 0x002903CC..0x0029054C
002903CC  0041CFA0
002903D0  003E2654
002903D4  00002FDC
002903D8  00000105
002903DC  000101C0
002903E0  003E2E30
002903E4  003E2E34
002903E8  001072E8
002903EC  001072EC
002903F0  00107258
002903F4  0010729C
002903F8  001072E4
002903FC  00107218
00290400  0028AEF0
00290404  00401000
00290408  12345678
0029040C  00401080
00290410  004010C0
00290414  004010D0
00290418  000001C2
0029041C  00400400
00290420  00400404
00290424  00400408
00290428  0040040C
0029042C  00400410
00290430  00400414
00290434  00400418
00290438  01C501C1
0029043C  0040041C
00290440  00400420
00290444  00400424
00290448  00400428
0029044C  0040042C
00290450  00400430
00290454  00400434
00290458  00400438
0029045C  0040043C
00290460  01960192
00290464  00400440
00290468  00400444
0029046C  00400448
00290470  019000F0
00290474  0040045C
00290478  00400460
0029047C  01920002
00290480  00400464
00290484  00080340
00290488  00400470
0029048C  0040049C
00290490  00400500
00290494  00400504
00290498  00400508
0029049C  0040050C
002904A0  00400510
002904A4  00400514
002904A8  00400518
002904AC  0040051C
002904B0  00400520
002904B4  00400524
002904B8  00400528
002904BC  0040052C
002904C0  00400530
002904C4  00400534
002904C8  00400538
002904CC  0040053C
002904D0  01980194
002904D4  00400540
002904D8  00400544
002904DC  00400548
002904E0  014000F0
002904E4  0040055C
002904E8  00400560
002904EC  00400564
002904F0  00080300
002904F4  00400570
002904F8  0040059C
002904FC  18300000
00290500  00400468
00290504  0040046C
00290508  00400568
0029050C  0040056C
00290510  00400494
00290514  00400498
00290518  00400478
0029051C  00400578
00290520  00400C18
00290524  00070100
00290528  0040001C
0029052C  0040002C
00290530  22221200
00290534  00400050
00290538  00400054
0029053C  00010501
00290540  00400474
00290544  00400574
00290548  00008010

RESUMED EXECUTABLE TAIL
0029054C  cmp       r5, #0
00290550  ldrne     ip, [r8, #4]
00290554  cmpne     ip, #0
00290558  beq       #0x290570
0029055C  mov       r3, sb
00290560  mov       r2, #0
00290564  mov       r1, #0x100
00290568  mov       r0, #0x10000
0029056C  blx       ip
00290570  ldr       r5, [sl, #0xa0]
00290574  cmp       r5, #0
00290578  beq       #0x2905b4
0029057C  bl        #0x28a814
00290580  ldrb      r0, [sl, #0x11]
00290584  cmp       r0, #0
00290588  strbeq    r4, [sl, #0x12]
0029058C  beq       #0x2905ac
00290590  ldr       r1, [r5, #0x18]!
00290594  mvn       r2, #0x19
00290598  ldr       r0, [r5, #0xc]
0029059C  rsb       r0, r0, r0, lsl #3
002905A0  add       r0, r2, r0, lsl #2
002905A4  strb      r6, [r1, r0]
002905A8  strb      r6, [sl, #0x18]
002905AC  nop       
002905B0  bl        #0x28a7b4
002905B4  mov       r0, #0
002905B8  bl        #0x28f6f0
002905BC  add       r1, sp, #8
002905C0  mov       r0, #1
002905C4  bl        #0x28c500
002905C8  strb      r6, [sl, #0x10]
002905CC  add       sp, sp, #0xc
002905D0  mov       r0, #1
002905D4  pop       {r4, r5, r6, r7, r8, sb, sl, fp, pc}
002905D8  add       r1, sp, #8
002905DC  mov       r0, #1
002905E0  bl        #0x28c500
002905E4  add       sp, sp, #0xc
002905E8  mov       r0, #0
002905EC  pop       {r4, r5, r6, r7, r8, sb, sl, fp, pc}
```
