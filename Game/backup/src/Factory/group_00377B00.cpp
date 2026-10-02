namespace {
typedef unsigned int Word;
extern "C" void fn_002805B8(void *, Word, Word, Word);
extern "C" unsigned char dat_003F16D8;
extern "C" unsigned char dat_003F16DC;
extern "C" unsigned char dat_003F1708;
extern "C" unsigned char dat_003F170C;
extern "C" unsigned char dat_003F1710;
}

extern "C" void fn_00377B00(void *self, Word a1, Word a2) { fn_002805B8((char *)self - 0x60, a1, a2, (Word)&dat_003F16D8); }
extern "C" void fn_00377B10(void *self, Word a1, Word a2) { fn_002805B8((char *)self - 0x60, a1, a2, (Word)&dat_003F16DC); }
extern "C" void fn_00377B20(void *self, Word a1, Word a2) { fn_002805B8((char *)self - 0x60, a1, a2, (Word)&dat_003F1708); }
extern "C" void fn_00377B40(void *self, Word a1, Word a2) { fn_002805B8((char *)self - 0x60, a1, a2, (Word)&dat_003F170C); }
extern "C" void fn_00377B50(void *self, Word a1, Word a2) { fn_002805B8((char *)self - 0x60, a1, a2, (Word)&dat_003F1710); }
