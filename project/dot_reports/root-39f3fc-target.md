# Original light-area introsort, 0x0039F3FC

Derived from the authorized EU binary with the pinned executable SHA-256. No boundary changes. ARM instructions end at 0x0039FE34; the unchanged mapped interval ends at 0x0039FE3C.

```text
0039F3FC: push     {r0, r1, r2, r3, r4, r5, r6, r7, r8, sb, sl, fp, ip, lr}
0039F400: sub      sp, sp, #0x4f0
0039F404: mov      sl, r1
0039F408: mov      fp, r0
0039F40C: ldr      r0, [pc, #0xa20]
0039F410: sub      r1, r1, fp
0039F414: cmp      r1, r0
0039F418: ble      #0x39f9e4
0039F41C: ldr      r0, [sp, #0x4f8]
0039F420: cmp      r0, #0
0039F424: beq      #0x39f48c
0039F428: ldr      r2, [pc, #0xa08]
0039F42C: sub      r0, sl, fp
0039F430: mov      r1, #0
0039F434: mov      r4, fp
0039F438: smlal    r1, r0, r2, r0
0039F43C: ldr      r2, [sp, #0x4fc]
0039F440: mov      r5, sl
0039F444: add      r7, sp, #0x140
0039F448: asr      r1, r0, #9
0039F44C: sub      r0, r1, r0, asr #31
0039F450: add      r1, r0, r0, lsl #2
0039F454: add      r0, r1, r0, lsl #5
0039F458: add      r1, fp, r0, lsl #3
0039F45C: mov      r6, r1
0039F460: mov      r0, r4
0039F464: blx      r2
0039F468: cmp      r0, #0
0039F46C: sub      r1, sl, #0x128
0039F470: beq      #0x39fa08
0039F474: ldr      r2, [sp, #0x4fc]
0039F478: mov      r0, r6
0039F47C: blx      r2
0039F480: cmp      r0, #0
0039F484: beq      #0x39f9ec
0039F488: b        #0x39fa40
0039F48C: ldr      sb, [sp, #0x4fc]
0039F490: sub      r0, sl, fp
0039F494: cmp      r0, #0x250
0039F498: mov      r6, fp
0039F49C: mov      r7, sb
0039F4A0: str      r0, [sp, #0x4b8]
0039F4A4: blt      #0x39f534
0039F4A8: ldr      r2, [pc, #0x988]
0039F4AC: mov      r1, #0
0039F4B0: add      r8, sp, #8
0039F4B4: smlal    r1, r0, r2, r0
0039F4B8: asr      r1, r0, #8
0039F4BC: sub      r0, r1, r0, asr #31
0039F4C0: str      r0, [sp, #0x390]
0039F4C4: sub      r0, r0, #2
0039F4C8: add      r0, r0, r0, lsr #31
0039F4CC: asr      r5, r0, #1
0039F4D0: add      r0, r5, r5, lsl #2
0039F4D4: add      r0, r0, r5, lsl #5
0039F4D8: add      r4, fp, r0, lsl #3
0039F4DC: add      r1, r4, #8
0039F4E0: ldr      r0, [r4]
0039F4E4: str      r0, [sp, #8]
0039F4E8: ldr      r0, [r4, #4]
0039F4EC: str      r0, [sp, #0xc]
0039F4F0: add      r0, r8, #8
0039F4F4: bl       #0x24e054
0039F4F8: add      r1, r4, #0x68
0039F4FC: add      r0, r0, #0x60
0039F500: bl       #0x24e054
0039F504: add      r1, r4, #0xc8
0039F508: add      r0, r0, #0x60
0039F50C: bl       #0x24e054
0039F510: sub      r3, r0, #0xc8
0039F514: ldr      r2, [sp, #0x390]
0039F518: mov      r1, r5
0039F51C: mov      r0, fp
0039F520: str      r7, [sp]
0039F524: bl       #0x204950
0039F528: cmp      r5, #0
0039F52C: subne    r5, r5, #1
0039F530: bne      #0x39f4d0
0039F534: mov      r4, sl
0039F538: cmp      sl, r4
0039F53C: mov      r0, r4
0039F540: bhs      #0x39f78c
0039F544: ldr      r1, [sp, #0x4b8]
0039F548: add      r2, sp, #0x390
0039F54C: mov      r0, #0
0039F550: stm      r2, {r0, r1}
0039F554: add      r7, r6, #0xb8
0039F558: add      r8, r6, #0x118
0039F55C: mov      r1, r6
0039F560: mov      r0, r4
0039F564: blx      sb
0039F568: cmp      r0, #0
0039F56C: beq      #0x39f77c
0039F570: ldr      r1, [r4]
0039F574: add      r0, sp, #0x10
0039F578: str      r1, [sp, #8]
0039F57C: ldr      r1, [r4, #4]
0039F580: str      r1, [sp, #0xc]
0039F584: add      r1, r4, #8
0039F588: bl       #0x24e054
0039F58C: add      r1, r4, #0x68
0039F590: add      r0, r0, #0x60
0039F594: bl       #0x24e054
0039F598: add      r1, r4, #0xc8
0039F59C: add      r0, r0, #0x60
0039F5A0: bl       #0x24e054
0039F5A4: ldr      r1, [r6]
0039F5A8: sub      r5, r0, #0xc8
0039F5AC: add      r0, r6, #8
0039F5B0: str      r1, [r4]
0039F5B4: ldr      r3, [r6, #4]
0039F5B8: add      ip, r4, #8
0039F5BC: add      fp, ip, #0x40
0039F5C0: str      r3, [r4, #4]
0039F5C4: add      r3, r6, #0x18
0039F5C8: vldmia   r0, {s0, s1, s2, s3}
0039F5CC: add      r1, r4, #0xb8
0039F5D0: add      lr, sp, #0x268
0039F5D4: add      r2, r4, #0x118
0039F5D8: vstmia   ip, {s0, s1, s2, s3}
0039F5DC: vldmia   r3, {s0, s1, s2, s3}
0039F5E0: add      r3, r4, #0x18
0039F5E4: vstmia   r3, {s0, s1, s2, s3}
0039F5E8: add      r3, r6, #0x28
0039F5EC: vldmia   r3, {s0, s1, s2, s3}
0039F5F0: add      r3, r4, #0x28
0039F5F4: vstmia   r3, {s0, s1, s2, s3}
0039F5F8: add      r3, r0, #0x30
0039F5FC: vldmia   r3, {s0, s1, s2, s3}
0039F600: add      r3, r4, #0x38
0039F604: vstmia   r3, {s0, s1, s2, s3}
0039F608: add      r3, r0, #0x40
0039F60C: vldmia   r3, {s0, s1, s2, s3}
0039F610: vstmia   fp, {s0, s1, s2, s3}
0039F614: add      fp, r6, #0x78
0039F618: vldr     s0, [r0, #0x50]
0039F61C: vstr     s0, [ip, #0x50]
0039F620: vldr     s0, [r0, #0x54]
0039F624: vstr     s0, [ip, #0x54]
0039F628: ldr      r3, [r0, #0x58]
0039F62C: str      r3, [ip, #0x58]
0039F630: ldrb     r0, [r0, #0x5c]
0039F634: add      r3, r4, #0x68
0039F638: strb     r0, [ip, #0x5c]
0039F63C: add      r0, r6, #0x68
0039F640: vldmia   r0, {s0, s1, s2, s3}
0039F644: vstmia   r3, {s0, s1, s2, s3}
0039F648: vldmia   fp, {s0, s1, s2, s3}
0039F64C: add      fp, r4, #0x78
0039F650: vstmia   fp, {s0, s1, s2, s3}
0039F654: add      fp, r6, #0x88
0039F658: vldmia   fp, {s0, s1, s2, s3}
0039F65C: add      fp, r4, #0x88
0039F660: vstmia   fp, {s0, s1, s2, s3}
0039F664: add      fp, r6, #0x98
0039F668: vldmia   fp, {s0, s1, s2, s3}
0039F66C: add      fp, r4, #0x98
0039F670: vstmia   fp, {s0, s1, s2, s3}
0039F674: add      fp, r6, #0xa8
0039F678: vldmia   fp, {s0, s1, s2, s3}
0039F67C: add      fp, r3, #0x40
0039F680: vstmia   fp, {s0, s1, s2, s3}
0039F684: ldr      ip, [r7]
0039F688: str      ip, [r1]
0039F68C: ldr      ip, [r7, #4]
0039F690: str      ip, [r1, #4]
0039F694: ldr      ip, [r7, #8]
0039F698: str      ip, [r1, #8]
0039F69C: ldrb     r0, [r0, #0x5c]
0039F6A0: add      r1, r4, #0xc8
0039F6A4: strb     r0, [r3, #0x5c]
0039F6A8: add      r0, r6, #0xc8
0039F6AC: add      r3, r6, #0xd8
0039F6B0: vldmia   r0, {s0, s1, s2, s3}
0039F6B4: vstmia   r1, {s0, s1, s2}
0039F6B8: vstr     s3, [r4, #0xd4]
0039F6BC: vldmia   r3, {s0, s1, s2, s3}
0039F6C0: add      r3, r4, #0xd8
0039F6C4: vstmia   r3, {s0, s1, s2, s3}
0039F6C8: add      r3, r6, #0xe8
0039F6CC: vldmia   r3, {s0, s1, s2, s3}
0039F6D0: add      r3, r4, #0xe8
0039F6D4: vstmia   r3, {s0, s1, s2, s3}
0039F6D8: add      r3, r6, #0xf8
0039F6DC: vldmia   r3, {s0, s1, s2, s3}
0039F6E0: add      r3, r4, #0xf8
0039F6E4: vstmia   r3, {s0, s1, s2, s3}
0039F6E8: add      r3, r0, #0x40
0039F6EC: vldmia   r3, {s0, s1, s2, s3}
0039F6F0: add      r3, r1, #0x40
0039F6F4: vstmia   r3, {s0, s1, s2, s3}
0039F6F8: ldr      r3, [r8]
0039F6FC: str      r3, [r2]
0039F700: ldr      r3, [r8, #4]
0039F704: str      r3, [r2, #4]
0039F708: ldr      r3, [r8, #8]
0039F70C: str      r3, [r2, #8]
0039F710: ldrb     r0, [r0, #0x5c]
0039F714: strb     r0, [r1, #0x5c]
0039F718: ldr      r0, [r5]
0039F71C: add      r1, r5, #8
0039F720: str      r0, [sp, #0x268]
0039F724: ldr      r0, [r5, #4]
0039F728: str      r0, [sp, #0x26c]
0039F72C: add      r0, lr, #8
0039F730: bl       #0x24e054
0039F734: add      r1, r5, #0x68
0039F738: add      r0, r0, #0x60
0039F73C: bl       #0x24e054
0039F740: add      r1, r5, #0xc8
0039F744: add      r0, r0, #0x60
0039F748: bl       #0x24e054
0039F74C: add      r5, sp, #0x390
0039F750: sub      r3, r0, #0xc8
0039F754: ldr      r2, [pc, #0x6dc]
0039F758: ldr      ip, [sp, #0x4b8]
0039F75C: ldm      r5, {r0, r1}
0039F760: str      sb, [sp]
0039F764: smlal    r0, r1, r2, ip
0039F768: asr      r0, r1, #8
0039F76C: sub      r2, r0, r1, asr #31
0039F770: mov      r1, #0
0039F774: mov      r0, r6
0039F778: bl       #0x204950
0039F77C: add      r4, r4, #0x128
0039F780: cmp      r4, sl
0039F784: mov      r0, sl
0039F788: blo      #0x39f55c
0039F78C: mov      r4, r0
0039F790: ldr      r0, [sp, #0x4b8]
0039F794: mov      r8, r6
0039F798: cmp      r0, #0x250
0039F79C: addge    sl, r8, #0xb8
0039F7A0: addge    fp, r8, #0x118
0039F7A4: blt      #0x39f9e4
0039F7A8: cmp      r8, r4
0039F7AC: mov      r6, r8
0039F7B0: beq      #0x39f9d4
0039F7B4: ldr      r1, [r4, #-0x128]
0039F7B8: sub      r5, r4, #0x128
0039F7BC: add      r0, sp, #0x20
0039F7C0: str      r1, [sp, #0x18]
0039F7C4: ldr      r1, [r4, #-0x124]
0039F7C8: str      r1, [sp, #0x1c]
0039F7CC: sub      r1, r4, #0x120
0039F7D0: bl       #0x24e054
0039F7D4: add      r1, r5, #0x68
0039F7D8: add      r0, r0, #0x60
0039F7DC: bl       #0x24e054
0039F7E0: add      r1, r5, #0xc8
0039F7E4: add      r0, r0, #0x60
0039F7E8: bl       #0x24e054
0039F7EC: ldr      lr, [r6]
0039F7F0: str      sb, [sp, #0x4c8]
0039F7F4: sub      r7, r0, #0xc8
0039F7F8: str      lr, [r5]
0039F7FC: ldr      r0, [r6, #4]
0039F800: add      r2, r6, #8
0039F804: add      r1, r5, #8
0039F808: str      r0, [r5, #4]
0039F80C: add      r0, r6, #0x18
0039F810: vldmia   r2, {s0, s1, s2, s3}
0039F814: add      r3, r6, #0x68
0039F818: vstmia   r1, {s0, s1, s2, s3}
0039F81C: vldmia   r0, {s0, s1, s2, s3}
0039F820: add      r0, r5, #0x18
0039F824: vstmia   r0, {s0, s1, s2, s3}
0039F828: add      r0, r6, #0x28
0039F82C: vldmia   r0, {s0, s1, s2, s3}
0039F830: add      r0, r5, #0x28
0039F834: vstmia   r0, {s0, s1, s2, s3}
0039F838: add      r0, r2, #0x30
0039F83C: vldmia   r0, {s0, s1, s2, s3}
0039F840: add      r0, r1, #0x30
0039F844: vstmia   r0, {s0, s1, s2, s3}
0039F848: add      r0, r2, #0x40
0039F84C: vldmia   r0, {s0, s1, s2, s3}
0039F850: add      r0, r1, #0x40
0039F854: vstmia   r0, {s0, s1, s2, s3}
0039F858: vldr     s0, [r2, #0x50]
0039F85C: vstr     s0, [r1, #0x50]
0039F860: ldr      r0, [r2, #0x54]
0039F864: str      r0, [r1, #0x54]
0039F868: ldr      r0, [r2, #0x58]
0039F86C: str      r0, [r1, #0x58]
0039F870: ldrb     r2, [r2, #0x5c]
0039F874: add      r0, r5, #0x68
0039F878: strb     r2, [r1, #0x5c]
0039F87C: ldm      r3, {r1, r2, ip, lr}
0039F880: stm      r0, {r1, r2, ip, lr}
0039F884: add      r1, r3, #0x10
0039F888: ldm      r1, {r1, r2, ip, lr}
0039F88C: str      r1, [r5, #0x78]
0039F890: add      r1, r5, #0x7c
0039F894: stm      r1, {r2, ip, lr}
0039F898: add      r1, r3, #0x20
0039F89C: ldm      r1, {r1, r2, ip, lr}
0039F8A0: str      r1, [r5, #0x88]
0039F8A4: add      r1, r5, #0x8c
0039F8A8: stm      r1, {r2, ip, lr}
0039F8AC: add      r1, r3, #0x30
0039F8B0: ldm      r1, {r1, r2, ip, lr}
0039F8B4: str      r1, [r0, #0x30]
0039F8B8: str      r2, [r0, #0x34]
0039F8BC: str      ip, [r0, #0x38]
0039F8C0: add      r1, r3, #0x40
0039F8C4: str      lr, [r0, #0x3c]
0039F8C8: add      r2, r6, #0xc8
0039F8CC: vldmia   r1, {s0, s1, s2, s3}
0039F8D0: add      r1, r0, #0x40
0039F8D4: vstmia   r1, {s0, s1, s2, s3}
0039F8D8: ldr      r1, [sl]
0039F8DC: str      r1, [r0, #0x50]
0039F8E0: ldr      r1, [sl, #4]
0039F8E4: str      r1, [r0, #0x54]
0039F8E8: ldr      lr, [sl, #8]
0039F8EC: add      r1, r5, #0xc8
0039F8F0: str      lr, [r0, #0x58]
0039F8F4: ldrb     r3, [r3, #0x5c]
0039F8F8: strb     r3, [r0, #0x5c]
0039F8FC: ldm      r2, {r0, r3, ip, lr}
0039F900: stm      r1, {r0, r3, ip, lr}
0039F904: add      r0, r6, #0xd8
0039F908: ldm      r0, {r0, r3, ip, lr}
0039F90C: str      r0, [r5, #0xd8]
0039F910: str      lr, [r5, #0xe4]
0039F914: add      lr, r5, #0x118
0039F918: add      r0, r5, #0xdc
0039F91C: stm      r0, {r3, ip}
0039F920: add      r0, r2, #0x20
0039F924: vldmia   r0, {s0, s1, s2, s3}
0039F928: add      r0, r5, #0xe8
0039F92C: vstmia   r0, {s0, s1, s2, s3}
0039F930: add      r0, r2, #0x30
0039F934: vldmia   r0, {s0, s1, s2, s3}
0039F938: add      r0, r1, #0x30
0039F93C: vstmia   r0, {s0, s1, s2, s3}
0039F940: add      r0, r2, #0x40
0039F944: vldmia   r0, {s0, s1, s2, s3}
0039F948: add      r0, r1, #0x40
0039F94C: vstmia   r0, {s0, s1, s2, s3}
0039F950: ldr      r0, [fp]
0039F954: str      r0, [r5, #0x118]
0039F958: ldr      r0, [fp, #4]
0039F95C: str      r0, [lr, #4]
0039F960: ldr      r0, [fp, #8]
0039F964: str      r0, [lr, #8]
0039F968: ldrb     r0, [r2, #0x5c]
0039F96C: strb     r0, [r1, #0x5c]
0039F970: ldr      r0, [r7]
0039F974: add      r1, r7, #8
0039F978: str      r0, [sp, #0x390]
0039F97C: ldr      r0, [r7, #4]
0039F980: str      r0, [sp, #0x394]
0039F984: add      r0, sp, #0x398
0039F988: bl       #0x24e054
0039F98C: add      r1, r7, #0x68
0039F990: add      r0, r0, #0x60
0039F994: bl       #0x24e054
0039F998: add      r1, r7, #0xc8
0039F99C: add      r0, r0, #0x60
0039F9A0: bl       #0x24e054
0039F9A4: ldr      r1, [pc, #0x48c]
0039F9A8: sub      r3, r0, #0xc8
0039F9AC: sub      r2, r5, r6
0039F9B0: mov      r0, #0
0039F9B4: smlal    r0, r2, r1, r2
0039F9B8: ldr      r7, [sp, #0x4c8]
0039F9BC: mov      r1, #0
0039F9C0: asr      r0, r2, #8
0039F9C4: sub      r2, r0, r2, asr #31
0039F9C8: mov      r0, r6
0039F9CC: str      r7, [sp]
0039F9D0: bl       #0x204950
0039F9D4: sub      r4, r4, #0x128
0039F9D8: sub      r0, r4, r8
0039F9DC: cmp      r0, #0x250
0039F9E0: bge      #0x39f7a8
0039F9E4: add      sp, sp, #0x500
0039F9E8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, ip, pc}
0039F9EC: ldr      r2, [sp, #0x4fc]
0039F9F0: sub      r1, sl, #0x128
0039F9F4: mov      r0, fp
0039F9F8: blx      r2
0039F9FC: cmp      r0, #0
0039FA00: bne      #0x39fa3c
0039FA04: b        #0x39fa1c
0039FA08: ldr      r2, [sp, #0x4fc]
0039FA0C: mov      r0, fp
0039FA10: blx      r2
0039FA14: cmp      r0, #0
0039FA18: beq      #0x39fa24
0039FA1C: mov      r6, fp
0039FA20: b        #0x39fa40
0039FA24: ldr      r2, [sp, #0x4fc]
0039FA28: sub      r1, sl, #0x128
0039FA2C: mov      r0, r6
0039FA30: blx      r2
0039FA34: cmp      r0, #0
0039FA38: beq      #0x39fa40
0039FA3C: sub      r6, sl, #0x128
0039FA40: add      r1, r6, #8
0039FA44: ldr      r0, [r6]
0039FA48: str      r0, [sp, #0x140]
0039FA4C: ldr      r0, [r6, #4]
0039FA50: str      r0, [sp, #0x144]
0039FA54: add      r0, r7, #8
0039FA58: bl       #0x24e054
0039FA5C: add      r1, r6, #0x68
0039FA60: add      r0, r0, #0x60
0039FA64: bl       #0x24e054
0039FA68: add      r1, r6, #0xc8
0039FA6C: add      r0, r0, #0x60
0039FA70: bl       #0x24e054
0039FA74: ldr      r6, [sp, #0x4fc]
0039FA78: sub      r0, r0, #0xc8
0039FA7C: str      r0, [sp, #0x274]
0039FA80: ldr      r1, [sp, #0x274]
0039FA84: mov      r0, r4
0039FA88: blx      r6
0039FA8C: cmp      r0, #0
0039FA90: beq      #0x39faac
0039FA94: add      r4, r4, #0x128
0039FA98: ldr      r1, [sp, #0x274]
0039FA9C: mov      r0, r4
0039FAA0: blx      r6
0039FAA4: cmp      r0, #0
0039FAA8: bne      #0x39fa94
0039FAAC: sub      r1, r5, #0x128
0039FAB0: ldr      r0, [sp, #0x274]
0039FAB4: mov      r5, r1
0039FAB8: blx      r6
0039FABC: cmp      r0, #0
0039FAC0: bne      #0x39faac
0039FAC4: cmp      r4, r5
0039FAC8: blo      #0x39fb10
0039FACC: add      r5, sp, #0x400
0039FAD0: add      r5, r5, #0xf8
0039FAD4: mov      r1, sl
0039FAD8: ldm      r5, {r2, r3}
0039FADC: mov      r0, r4
0039FAE0: bl       #0x39f3fc
0039FAE4: ldr      r0, [sp, #0x4f8]
0039FAE8: sub      r1, r4, fp
0039FAEC: mov      sl, r4
0039FAF0: add      r0, r0, r0, lsr #31
0039FAF4: asr      r0, r0, #1
0039FAF8: str      r0, [sp, #0x4f8]
0039FAFC: ldr      r0, [pc, #0x330]
0039FB00: cmp      r1, r0
0039FB04: bgt      #0x39f41c
0039FB08: add      sp, sp, #0x500
0039FB0C: pop      {r4, r5, r6, r7, r8, sb, sl, fp, ip, pc}
0039FB10: ldr      r1, [r4]
0039FB14: add      r0, sp, #0x20
0039FB18: str      r1, [sp, #0x18]
0039FB1C: ldr      r1, [r4, #4]
0039FB20: str      r1, [sp, #0x1c]
0039FB24: add      r1, r4, #8
0039FB28: bl       #0x24e054
0039FB2C: add      r1, r4, #0x68
0039FB30: add      r0, r0, #0x60
0039FB34: bl       #0x24e054
0039FB38: add      r1, r4, #0xc8
0039FB3C: add      r0, r0, #0x60
0039FB40: bl       #0x24e054
0039FB44: ldr      r2, [r5]
0039FB48: add      r1, r5, #8
0039FB4C: add      r3, r4, #8
0039FB50: str      r2, [r4]
0039FB54: ldr      r2, [r5, #4]
0039FB58: add      r8, r3, #0x30
0039FB5C: add      lr, r3, #0x40
0039FB60: str      r2, [r4, #4]
0039FB64: add      r2, r5, #0x18
0039FB68: vldmia   r1, {s0, s1, s2, s3}
0039FB6C: add      r0, r5, #0x68
0039FB70: add      r7, r5, #0xb8
0039FB74: add      ip, r4, #0xb8
0039FB78: add      sb, r4, #0x118
0039FB7C: vstmia   r3, {s0, s1, s2, s3}
0039FB80: vldmia   r2, {s0, s1, s2, s3}
0039FB84: add      r2, r4, #0x18
0039FB88: vstmia   r2, {s0, s1, s2, s3}
0039FB8C: add      r2, r5, #0x28
0039FB90: vldmia   r2, {s0, s1, s2, s3}
0039FB94: add      r2, r4, #0x28
0039FB98: vstmia   r2, {s0, s1, s2, s3}
0039FB9C: add      r2, r1, #0x30
0039FBA0: vldmia   r2, {s0, s1, s2, s3}
0039FBA4: add      r2, r4, #0x68
0039FBA8: vstmia   r8, {s0, s1, s2, s3}
0039FBAC: add      r8, r1, #0x40
0039FBB0: vldmia   r8, {s0, s1, s2, s3}
0039FBB4: vstmia   lr, {s0, s1, s2, s3}
0039FBB8: ldr      lr, [r1, #0x50]
0039FBBC: str      lr, [r3, #0x50]
0039FBC0: ldr      lr, [r1, #0x54]
0039FBC4: str      lr, [r3, #0x54]
0039FBC8: ldr      lr, [r1, #0x58]
0039FBCC: str      lr, [r3, #0x58]
0039FBD0: ldrb     r1, [r1, #0x5c]
0039FBD4: strb     r1, [r3, #0x5c]
0039FBD8: ldm      r0, {r1, r3, r8, lr}
0039FBDC: stm      r2, {r1, r3, r8, lr}
0039FBE0: add      r1, r0, #0x10
0039FBE4: ldm      r1, {r1, r3, r8, lr}
0039FBE8: str      r1, [r4, #0x78]
0039FBEC: add      r1, r4, #0x7c
0039FBF0: stm      r1, {r3, r8, lr}
0039FBF4: add      r1, r0, #0x20
0039FBF8: ldm      r1, {r1, r3, r8, lr}
0039FBFC: str      r1, [r4, #0x88]
0039FC00: str      r3, [r4, #0x8c]
0039FC04: str      r8, [r4, #0x90]
0039FC08: add      r1, r0, #0x30
0039FC0C: str      lr, [r4, #0x94]
0039FC10: add      lr, r2, #0x30
0039FC14: vldmia   r1, {s0, s1, s2, s3}
0039FC18: add      r3, r5, #0xc8
0039FC1C: add      r1, r4, #0xc8
0039FC20: add      r8, r5, #0x118
0039FC24: vstmia   lr, {s0, s1, s2, s3}
0039FC28: add      lr, r0, #0x40
0039FC2C: vldmia   lr, {s0, s1, s2, s3}
0039FC30: add      lr, r2, #0x40
0039FC34: vstmia   lr, {s0, s1, s2, s3}
0039FC38: ldr      lr, [r7]
0039FC3C: str      lr, [ip]
0039FC40: ldr      lr, [r7, #4]
0039FC44: str      lr, [ip, #4]
0039FC48: ldr      lr, [r7, #8]
0039FC4C: str      lr, [ip, #8]
0039FC50: ldrb     r0, [r0, #0x5c]
0039FC54: strb     r0, [r2, #0x5c]
0039FC58: add      r0, r3, #0x10
0039FC5C: vldmia   r3, {s0, s1, s2, s3}
0039FC60: add      r2, r5, #8
0039FC64: vstmia   r1, {s0, s1, s2, s3}
0039FC68: vldmia   r0, {s0, s1, s2, s3}
0039FC6C: add      r0, r1, #0x10
0039FC70: add      lr, sp, #0x20
0039FC74: add      ip, r2, #0x50
0039FC78: vstmia   r0, {s0, s1, s2, s3}
0039FC7C: add      r0, r3, #0x20
0039FC80: vldmia   r0, {s0, s1, s2, s3}
0039FC84: add      r0, r1, #0x20
0039FC88: vstmia   r0, {s0, s1, s2, s3}
0039FC8C: add      r0, r3, #0x30
0039FC90: vldmia   r0, {s0, s1, s2, s3}
0039FC94: add      r0, r1, #0x30
0039FC98: vstmia   r0, {s0, s1, s2, s3}
0039FC9C: add      r0, r3, #0x40
0039FCA0: vldmia   r0, {s0, s1, s2, s3}
0039FCA4: add      r0, r1, #0x40
0039FCA8: vstmia   r0, {s0, s1, s2, s3}
0039FCAC: ldr      r0, [r8]
0039FCB0: str      r0, [sb]
0039FCB4: ldr      r0, [r8, #4]
0039FCB8: str      r0, [sb, #4]
0039FCBC: ldr      r0, [r8, #8]
0039FCC0: str      r0, [sb, #8]
0039FCC4: ldrb     r3, [r3, #0x5c]
0039FCC8: add      r0, r5, #0x68
0039FCCC: strb     r3, [r1, #0x5c]
0039FCD0: ldr      r1, [sp, #0x18]
0039FCD4: str      r1, [r5]
0039FCD8: ldr      r1, [sp, #0x1c]
0039FCDC: str      r1, [r5, #4]
0039FCE0: ldm      lr, {r1, r3, r8, sb}
0039FCE4: add      lr, sp, #0x30
0039FCE8: stm      r2, {r1, r3, r8, sb}
0039FCEC: ldm      lr, {r1, r3, r8, sb}
0039FCF0: add      lr, r2, #0x10
0039FCF4: stm      lr, {r1, r3, r8, sb}
0039FCF8: add      lr, sp, #0x40
0039FCFC: ldm      lr, {r1, r3, r8, sb}
0039FD00: add      lr, r2, #0x20
0039FD04: stm      lr, {r1, r3, r8, sb}
0039FD08: add      lr, sp, #0x50
0039FD0C: ldm      lr, {r1, r3, r8, sb}
0039FD10: add      lr, r2, #0x30
0039FD14: stm      lr, {r1, r3, r8, sb}
0039FD18: add      r1, r5, #0xc8
0039FD1C: ldr      r3, [sp, #0x60]
0039FD20: ldr      lr, [sp, #0x6c]
0039FD24: ldrd     r8, sb, [sp, #0x64]
0039FD28: str      r3, [r2, #0x40]
0039FD2C: add      r3, r2, #0x44
0039FD30: stm      r3, {r8, sb, lr}
0039FD34: add      sb, sp, #0x80
0039FD38: ldr      r3, [sp, #0x70]
0039FD3C: str      r3, [ip]
0039FD40: ldr      r3, [sp, #0x74]
0039FD44: str      r3, [ip, #4]
0039FD48: ldr      r3, [sp, #0x78]
0039FD4C: str      r3, [ip, #8]
0039FD50: ldrb     r3, [sp, #0x7c]
0039FD54: strb     r3, [r2, #0x5c]
0039FD58: ldm      sb, {r2, r3, r8, ip}
0039FD5C: add      sb, sp, #0x90
0039FD60: stm      r0, {r2, r3, r8, ip}
0039FD64: ldm      sb, {r2, r3, r8, ip}
0039FD68: add      sb, r0, #0x10
0039FD6C: stm      sb, {r2, r3, r8, ip}
0039FD70: add      sb, sp, #0xa0
0039FD74: ldm      sb, {r2, r3, r8, ip}
0039FD78: add      sb, r0, #0x20
0039FD7C: stm      sb, {r2, r3, r8, ip}
0039FD80: add      sb, sp, #0xb0
0039FD84: ldm      sb, {r2, r3, r8, ip}
0039FD88: add      sb, r0, #0x30
0039FD8C: stm      sb, {r2, r3, r8, ip}
0039FD90: add      sb, sp, #0xc0
0039FD94: ldm      sb, {r2, r3, r8, ip}
0039FD98: add      sb, r0, #0x40
0039FD9C: add      r4, r4, #0x128
0039FDA0: stm      sb, {r2, r3, r8, ip}
0039FDA4: add      r8, sp, #0x120
0039FDA8: ldr      r2, [sp, #0xd0]
0039FDAC: str      r2, [r7]
0039FDB0: ldr      r2, [sp, #0xd4]
0039FDB4: str      r2, [r7, #4]
0039FDB8: ldr      r2, [sp, #0xd8]
0039FDBC: str      r2, [r7, #8]
0039FDC0: ldrb     r2, [sp, #0xdc]
0039FDC4: add      r7, sp, #0xe0
0039FDC8: strb     r2, [r0, #0x5c]
0039FDCC: ldm      r7, {r0, r2, r3, ip}
0039FDD0: add      r7, sp, #0xf0
0039FDD4: stm      r1, {r0, r2, r3, ip}
0039FDD8: ldm      r7, {r0, r2, r3, ip}
0039FDDC: add      r7, r1, #0x10
0039FDE0: stm      r7, {r0, r2, r3, ip}
0039FDE4: add      r7, sp, #0x100
0039FDE8: ldm      r7, {r0, r2, r3, ip}
0039FDEC: add      r7, r1, #0x20
0039FDF0: stm      r7, {r0, r2, r3, ip}
0039FDF4: add      r7, sp, #0x110
0039FDF8: ldm      r7, {r0, r2, r3, ip}
0039FDFC: add      r7, r1, #0x30
0039FE00: stm      r7, {r0, r2, r3, ip}
0039FE04: ldm      r8, {r2, r3, r7, ip}
0039FE08: add      r8, r1, #0x40
0039FE0C: stm      r8, {r2, r3, r7, ip}
0039FE10: ldr      r2, [sp, #0x130]
0039FE14: str      r2, [r1, #0x50]
0039FE18: ldr      r2, [sp, #0x134]
0039FE1C: str      r2, [r1, #0x54]
0039FE20: ldr      r2, [sp, #0x138]
0039FE24: str      r2, [r1, #0x58]
0039FE28: ldrb     r0, [sp, #0x13c]
0039FE2C: strb     r0, [r1, #0x5c]
0039FE30: b        #0x39fa80
0039FE34: .word 0x000013A7
0039FE38: .word 0xDD67C8A7
```
