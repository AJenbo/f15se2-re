// seg000 optimized code (/Ot)
#include "egame.h"
#include "offsets.h"
#include "pointers.h"
#include "debug.h"
#include "slot.h"
#include "const.h"

#include <dos.h>
#include <memory.h>
#include <stdlib.h>

// ==== seg000:0xcf32 ====
int sub_1CF32(unsigned int param)
{
    if (param & 0x100) {
        return buf3d3[param & 0x7f];
    }
    return (int)(&byte_2D6A4[((int *)flt15_buf1)[param]] - byte_228D0);
}

// ==== seg002:0xa ====
void far sub_21A7A() {
    sub_21A86();
}

// ==== seg002:0xe ====
void far sub_21A7E() {
    sub_22411();
}

// ==== seg000:0x21c6 ====
void sub_121C6() {
    load3DAll();
}

// ==== seg000:0x21ca ====
//
void sub_121CA(int arg_0, int arg_2, int arg_4, long arg_6, long arg_a, long arg_e, int arg_12, int arg_14, int arg_16, int arg_18) {
    ((int *)var_134)[7] = arg_14;
    ((int *)var_134)[8] = arg_14 + arg_18 - 1;
    ((int *)var_134)[9] = arg_12;
    ((int *)var_134)[10] = arg_12 + arg_16 - 1;
    *(int *)var_134 = gfx_getDisplayPage() & 0xFF;
    sub_12278(var_595);
    ((int *)var_134)[2] = (unsigned char)((char *)word_3419C)[word_3BE98 & 0xFF];
    sub_13932((char *)var_134, arg_0, arg_2, arg_4, 0, 0, (int)arg_e, 1);
    sub_122B8(arg_0, arg_2, arg_6, arg_a, arg_e);
    sub_1A9F8();
    sub_139AA();
    sub_1B147();
    *(char *)&var_135 ^= 1;
}

// ==== seg000:0x2278 ====
void sub_12278(int arg_0) {
    uint8 var_2;
    if (arg_0 > 0) { // 2284
        var_2 = (uint8)arg_0 + byte_3790C[0];
        while (var_2 != byte_3790C[0]) {}
    }
}

// ==== seg000:0xc8de ====
void load15Flt3d3() {
    int var_A, var_C;
    struct SREGS var_8;
    uint8 FAR *var_10;
    TRACE(("load15Flt3d3: a15flt_xxx=%s", a15flt_xxx));
    strcpyFromDot(a15flt_xxx, a_3d3_0);
    TRACE(("load15Flt3d3: after strcpyFromDot=%s", a15flt_xxx));
    fileHandle = fopen(a15flt_xxx, aRb_4);
    TRACE(("load15Flt3d3: fopen returned %d", (int)fileHandle));
    if (fileHandle == NULL) {
        // c90b
        printError(aOpenErrorOn_3d3_0);
        return;
    }
    // c922
    fread(&flt15_word1, 2, 1, fileHandle);
    fread(&flt15_size, 2, 1, fileHandle);
    fread(flt15_buf1, 2, flt15_size, fileHandle);
    fread(&var_A, 2, 1, fileHandle);
    TRACE(("load15Flt3d3: var_A=%d", var_A));
    segread(&var_8);
    var_10 = byte_2D6A4;
    TRACE(("load15Flt3d3: DS=%04x var_10=%04x:%04x", var_8.ds, FP_SEG(var_10), FP_OFF(var_10)));
    while(var_A > 0) {
        // c98b
        var_C = var_A <= 0x800 ? var_A : 0x800;
        // c99b
        fread(flt15_buf2, 1, var_C, fileHandle);
        movedata(var_8.ds, (uint16)flt15_buf2, FP_SEG(var_10), FP_OFF(var_10), var_C);
        var_A -= 0x800;
        FP_OFF(var_10) += 0x800;
    }
    TRACE(("load15Flt3d3: loop done"));
    // c9ca
    fclose(fileHandle);
    TRACE(("load15Flt3d3: done"));
}

// ==== seg000:0xd1c8 ====
int sub_1D1C8(int arg_0) {
    if (arg_0 == 0) {
        return 0;
    }
    if (arg_0 > 0) {
        return 1;
    }
    return -1;
}

void sub_1D1E8(void) {
    if (word_330BE == 0) {
        var_686 = getTimeOfDay();
    }
    srand(var_686);
}

// ==== seg000:0xdd4c ====
int openFileWrapper(char *path, int mode) {
    TRACE(("openFileWrapper: path=%s mode=%d", path, mode));
    return openFile(path, mode);
}

// ==== seg000:0xdd70 ====
void closeFileWrapper(int arg_0) {
    closeFile(arg_0);
}

// ==== seg000:0x229a loadColorPalette ====
void copySomeMem(int arg_0) {
    memcpy(word_3419C, (char *)word_339B4 + arg_0 * 0x10, 0x10);
}

// ==== seg000:0x3922 ====
int sub_13922(int arg_0) {
    return arg_0 - (arg_0 >> 2);
}

// ==== seg000:0x3932 ====
void sub_13932(char *arg_0, int arg_2, int arg_4, int arg_6, int arg_8, int arg_a, int arg_c, int arg_e) {
    sub_139C0((int)arg_0);
    sub_13A6C(arg_2, arg_4, arg_6);
    sub_13A90(arg_8, arg_a, arg_c);
    if (arg_e != 0) {
        var_315 = 0;
        if (word_38FDC == 0) {
            *(uint8 *)&var_316 = 1;
        }
        if (*(uint8 *)&var_316 == 0) {
            sub_20658();
        }
        while (*(uint8 *)&origCBreakSeg != 0)
            ;
        sub_10334(*(int *)(arg_0 + 4));
    }
    var_255 = 0;
    var_261 -= 0x3000 / word_330C4;
}

// ==== seg000:0x36d2 ====
void sub_136D2(char far *param_1, int param_3, int param_4) {
    *(char far **)&var_200 = param_1;
    var_200++;
    var_216 = 0;
    sub_202DA();
    if (word_3C16C >= 3) {
        if ((**(char far **)&var_200 & 0x40) != var_665)
            return;
    }
    switch ((unsigned)(unsigned char)**(char far **)&var_200 & 0x3f) {
    case 0x3e:
        return;
    case 0x3f:
        sub_1374A();
        return;
    }
    sub_1378E(param_3, param_4);
    sub_13816(param_3, param_4);
    sub_20A46();
    sub_20FDC();
}

// ==== seg000:0x378e ====
void sub_1378E(int param_1, int param_2) {
    long p;
    int b;

    p = 1L;
    var_257 = (int)(unsigned char)(*((*(char far **)&var_200)++)) & 0x1f;
    var_259 = -1;
    var_260 = -1;
    *(char *)&var_258 = (var_257 > 0x10) ? 1 : 0;
    b = 0;
    while (b < var_257) {
        var_200 += 4;
        if (*(*(int far **)&var_200)++ < 0) {
            *(long *)&var_259 ^= p;
        }
        var_200 += 2;
        p <<= 1;
        b++;
    }
}



// ==== seg000:0xcf64 clamp ====
int sub_1CF64(int arg_0, int arg_2, int arg_4) {
    if (arg_0 > arg_4) {
        return arg_4;
    }
    if (arg_0 >= arg_2) {
        return arg_0;
    }
    if (arg_0 <= -0x4000) {
        return arg_4;
    }
    return arg_2;
}

// ==== seg000:0xcf8e ====
int sub_1CF8E(int arg_0, int arg_2, int arg_4) {
    if (arg_0 > arg_4) {
        return arg_4;
    }
    if (arg_0 < arg_2) {
        return arg_2;
    }
    return arg_0;
}

// ==== seg000:0xd178 sinMul ====
int sub_1D178(int arg_0, int arg_2) {
    return sub_13B2F(sub_13B96(arg_0), arg_2);
}

// ==== seg000:0xd190 cosMul ====
int sub_1D190(int arg_0, int arg_2) {
    return sub_1D178(arg_0 + 0x4000, arg_2);
}

// ==== seg000:0xd200 randomRange ====
int sub_1D200(int arg_0) {
    return (int)(((long)rand() * (long)arg_0) >> 15);
}

// ==== seg000:0x3a6c ====
void sub_13A6C(int arg_0, int arg_2, int arg_4) {
    sub_20BAE((uint8*)0x19c6, -arg_0, -arg_2, -arg_4);
}

// ==== seg000:0x3a90 ====
void sub_13A90(int arg_0, int arg_2, int arg_4) {
    var_218 = arg_0;
    var_219 = arg_2;
    var_220 = arg_4;
}

// ==== seg000:0xdd5e ====
int sub_1DD5E(int arg_0, int arg_1) {
    return createFile(arg_0, arg_1);
}

// ==== seg000:0xdd7e ====
int sub_1DD7E(int arg_0, int arg_1, int arg_2) {
    return readFile1(arg_0, arg_1, arg_2);
}

// ==== seg000:0xdd92 ====
int sub_1DD92(int arg_0, int arg_1, int arg_2, int arg_3) {
    return readFile2(arg_0, arg_1, arg_2, arg_3);
}

// ==== seg000:0xddaa ====
int sub_1DDAA(int arg_0, int arg_1, int arg_2, int arg_3, int arg_4) {
    return sub_1DF4F(arg_0, arg_1, arg_2, arg_3, arg_4);
}

// ==== seg000:0x3638 ====
void sub_13638(int *arg_0, int *arg_1, int *arg_2, int *arg_3) {
    sub_13694(0, 0, arg_0, arg_2);
    if (*arg_0 < 0) {
        *arg_0 = 0;
    }
    if (*arg_2 < 0) {
        *arg_2 = 0;
    }
    sub_13694(var_349, var_350, arg_1, arg_3);
    if (*arg_1 >= var_662) {
        *arg_1 = var_662 - 1;
    }
    if (*arg_3 >= var_662) {
        *arg_3 = var_662 - 1;
    }
}

// ==== seg000:0xa740 ====
void sub_1A740(int arg_0, int arg_2, int arg_4) {
    sub_19E44(arg_4);
    sub_19D86(var_279 - 4, var_282 - 3, var_279 + 4, var_282 - 3);
    sub_19D86(var_279 + 4, var_282 - 3, var_279 + 4, var_282 + 3);
    sub_19D86(var_279 + 4, var_282 + 3, var_279 - 4, var_282 + 3);
    sub_19D86(var_279 - 4, var_282 + 3, var_279 - 4, var_282 - 3);
}

// ==== seg000:0x3694 ====
void sub_13694(int arg_0, int arg_1, int *arg_2, int *arg_3) {
    *arg_2 = (arg_0 - word_3298C + var_663) / var_661;
    *arg_3 = ((arg_1 - word_3298E) * 4 / 3 + var_664) / var_661;
}

// ==== seg000:0x66be ====
void sub_166BE(void) {
    int p;
    word_336F0 = word_3B0AC;
    if (word_333DA != 0) {
        word_3B4D8 = word_333D2;
        word_3B4E0 = word_333D4;
    } else {
        word_3B4D8 = word_3BEC0;
        word_3B4E0 = word_3BED0;
    }
    word_3B5D6 = var_547;
    word_38FEC = var_542;
    word_3BED4 = 0xFF;
    for (p = 0; p < word_3C69E; p++) {
        if (stru_3AA5E[p].field_4 != 0) {
            *(int *)&stru_3AA5E[p].field_8 = sub_1CF64(*(int *)&stru_3AA5E[p].field_8, ((word_330B8 + word_330BA) << 4) - 16, 0xFF);
        }
    }
}

// ==== seg000:0x3266 ====
int sub_13266(int p1, int p2, int p3, int p4) {
    for (var_660 = word_38FF8 - 1; var_660 >= 0; var_660--) {
        if (*(&byte_3B4E6 + var_660 * 8) == p1 &&
            *(&byte_3B4E6 + var_660 * 8 + 1) == p2 &&
            *(&byte_3B4E6 + var_660 * 8 + 2) == p3 &&
            *(&byte_3B4E6 + var_660 * 8 + 3) == p4) {
            return *(int *)(&byte_3B4E6 + var_660 * 8 + 4);
        }
    }
    return 0;
}

// ==== seg000:0x4224 ====
void sub_13224(char *a, int b, char c) {
    *(int *)(a + 0x12) = b;
    *(a + 0x14) = c;
    memcpy((char *)&byte_3B4E6 + word_38FF8++ * 8, a + 0x0e, 8);
    *(*(char **)(a + 0x0c) + 6) |= 0x80;
}

// ==== seg000:0x660e routine_324 ====
int sub_1660E(int arg_0, int arg_2, int arg_4, int arg_6, int *arg_8, int *arg_a) {
    int p;
    int a;
    int b;
    int c;
    int d;
    int e;
    unsigned f;

    if (arg_6 == 0 || arg_6 == -1) {
        return 0;
    }
    d = word_3BEC0 - arg_0;
    e = word_3BED0 - arg_2;
    c = (unsigned)sub_1CFA6(d, e) >> 6;
    b = sub_1D008(d, -e);
    f = (f = (*(int16 *)&aNone[arg_6 * 14 + 10] + word_330B8 * 2 + 3) * *(int16 *)&aNone[arg_6 * 14 + 8] / 16) * (((unsigned)var_547 >> 6) + 0x40) >> 7;
    *arg_8 = b;
    *arg_a = c;
    return f;
}

// ==== seg000:0x89aa ====
int sub_189AA(int arg_0) {
    if (planeFlags & (0x4000 >> arg_0)) {
        return 0;
    }
    if (*(&word_3B144 + 9 * arg_0) == 4 || *(&word_3B144 + 9 * arg_0) == 3) {
        sub_11D10((arg_0 != 0 ? 0x40 : 0x80) + 0x0b, 0);
    }
    if (arg_0 != 0) {
        strcpy(strBuf, aSecond_Target);
        waypointIndex = 1;
        var_730 |= 0x20;
    } else {
        strcpy(strBuf, aPrimaryTarget_0);
        waypointIndex = 2;
        var_730 |= 0x40;
    }
    if ((planeFlags & 0x6000) == 0x6000) {
        waypointIndex = 3;
    }
    return 1;
}

// ==== seg000:0x8df4 ====
void sub_18DF4(int param_1, int param_2, int param_3) {
    *(char *)&var_315 = 0;
    sub_132BA((long)param_1 << 5, -((long)param_2 - 0x8000L) << 5, (long)param_3);
}

// ==== seg000:0x85be ====
int sub_185BE(int param_1, int param_2, int param_3, int param_4, int param_5) {
    int p;
    int a;
    int b;
    int c;

    b = param_2 - stru_335C4[param_1].field_0;
    c = param_3 - stru_335C4[param_1].field_2;
    a = sub_1CFA6(b, c);
    var_670 = sub_1D008(b, -c);
    if (stru_335C4[param_1].field_6 * 24 / word_330C4 > a) {
        var_669 = a;
        return 1;
    }
    p = abs(var_670 - stru_335C4[param_1].field_8);
    if (p > 0x1000 && param_5 != 3) {
        if (p > 0x6000 && param_1 < 8) {
            if ((stru_335C4[param_1].field_6 << 4) / word_330C4 < a) {
                stru_335C4[param_1].field_E = sub_1CF64(stru_335C4[param_1].field_E, 0, word_330C4 << 4);
            }
        }
        return 0;
    }
    if (param_5 == 0) {
        if (abs(stru_335C4[param_1].field_8 - var_542) > 0x2000) {
            return 0;
        }
    }
    if (param_5 == 0) {
        var_669 = a;
        return 1;
    }
    p = abs(stru_335C4[param_1].field_8 - var_542);
    if (abs(p - 0x4000) >= 0x2000 - word_330B8 * 2048) {
        var_669 = a;
        return 1;
    }
    return 0;
}

// ==== seg000:0xa872 ====
void sub_1A872(int arg_0, int arg_2, int arg_4, int arg_6) {
    var_569 = gfxBufPtr;
    var_570 = arg_0 * 8 + 1;
    var_571 = arg_2 * 8 + 0x1f;
    var_572 = (byte_3C5A0 != 0);
    var_573 = arg_4 - 3;
    var_574 = arg_6 - 3;
    var_575 = 7;
    var_576 = 7;
    gfx_jump_47(&word_383AE);
}

// ==== seg000:0xa8c8 ====
void sub_1A8C8(int arg_0, int arg_2, int arg_4, int arg_6, int arg_8, int arg_a, int arg_c) {
    var_577 = gfxBufPtr;
    var_578 = arg_4;
    var_579 = arg_6;
    var_580 = (byte_3C5A0 != 0);
    var_581 = arg_0;
    var_582 = arg_2;
    var_583 = arg_8;
    var_584 = arg_a;
    var_586 = (char)arg_c;
    if (arg_c != 0) {
        var_585 = 1;
        gfx_jump_47(&word_383CC);
        return;
    }
    var_585 = 0x10;
    gfx_jump_49(&word_383CC);
}

// ==== seg000:0x6742 ====
int routine_191(void) {
    int p;
    int a;
    int b;

    b = 0;
    for (a = 0; a < word_38FFA; a++) {
        p = stru_3AA5E[a].field_4;
        if (p != 0) {
            b += *(int *)((char *)aNone + p * 14 + 8) * *(int *)((char *)aNone + p * 14 + 10) * (word_330B8 + 2) / 64;
        }
    }
    b /= 100;
    return b;
}

int16 sub_1D21E(int16 arg_0)
{
    int16 p;

    if (word_330BE) {
        p = 0;
    } else {
        p = ((commData->setupUseJoy) ? misc_jump_5d_readJoy(arg_0) : 0) + (&word_38606)[arg_0];
    }
    return p;
}

int sub_1CFA6(int arg_0, int arg_1) {
    long a;
    arg_0 = abs(arg_0);
    arg_1 = abs(arg_1);
    if (arg_0 > arg_1)
        a = (long)(arg_1 >> 1) + (long)arg_0;
    else
        a = (long)(arg_0 >> 1) + (long)arg_1;
    if (a > 0x7FFF) a = 0x7FFF;
    return (int)a;
}

// ==== seg000:0xd008 ====
int sub_1D008(int param_1, int param_2) {
    int p, a;
    long b;
    int d, e, f;

    if (param_1 == 0) {
        if (param_2 > 0) return 0;
        return BEARING_SOUTH;
    }
    if (param_2 == 0) {
        if (param_1 > 0) return BEARING_EAST;
        return BEARING_WEST;
    }
    if (abs(param_1) > abs(param_2)) {
        b = (long)abs(param_2) << 0xe;
        d = abs(param_1);
        e = 1;
    }
    else {
        b = (long)abs(param_1) << 0xe;
        d = abs(param_2);
        e = 0;
    }
    f = b / (long)d;
    p = ((0x2800L - (((long)abs(0x1333 - f) * 0xB00L) >> 0xe)) * (long)f) >> 0xe;
    if (param_1 > 0) {
        if (param_2 > 0)
            a = e ? BEARING_EAST - p : p;
        else
            a = e ? p + BEARING_EAST : BEARING_SOUTH - p;
    }
    else {
        if (param_2 > 0)
            a = e ? p + BEARING_WEST : -p;
        else
            a = e ? BEARING_WEST - p : p + BEARING_SOUTH;
    }
}

// ==== seg000:0x8aa6 ====
void sub_18AA6()
{
    int p;
    int a;
    int b;
    int c;

    if (abs(var_545) > 0x3000) return;
    if (word_33702 != 0) return;
    if (word_3BE3C != 0) return;

    b = missleSpec[missileSpecIndex].field_0;
    p = *(int16 *)&missiles[b].field_A[2];

    if (missleSpec[missileSpecIndex].field_2 == 0) {
        strcpy((char *)strBuf, missiles[b].field_0);
        strcat((char *)strBuf, (char *)a0);
        sub_1A204((char *)strBuf);
        goto end;
    }

    if (p == 0) return;
    if (p == -1) return;

    missleSpec[missileSpecIndex].field_2--;

    if (word_330C2 != 0) {
        sub_19E44(0);
        a = ((int16 *)aA)[missileSpecIndex];
        sub_19E5D(a - 1, 0xbe, a + 2, 0xc2);
        sub_1A183(missleSpec[missileSpecIndex].field_2, a, 0xbe, 0x0c);
        strcpy((char *)strBuf, missiles[b].field_0);
        strcat((char *)strBuf, (char *)0x5947);
        strcat((char *)strBuf, itoa(missleSpec[missileSpecIndex].field_2, (char *)unk_3C030, 0x0a));
        sub_1A204((char *)strBuf);
    }

    c = -1;
    a = 8;
    do {
        if (stru_335C4[a].field_E == 0) {
            c = a;
        }
        a++;
    } while (a < 12);

    if (c == -1) goto check_end;

    stru_335C4[c].field_0 = word_3BEC0;
    stru_335C4[c].field_2 = word_3BED0;
    stru_335C4[c].field_4 = var_547 - 0x14;
    stru_335C4[c].field_6 = (unsigned int)word_3A944 >> 11;
    c = c;
    stru_335C4[c].field_8 = var_542;
    stru_335C4[c].field_A = var_544;
    stru_335C4[c].field_C = var_545;

    stru_335C4[c].field_E = (int)(((long)sams[p].field_8 << (6 - (sams[p].field_C == 6 ? 3 : 2))) * (long)word_330C4 / (long)((sams[p].field_A >> 6) + 1)) + 6;

    c = c;
    if (stru_335C4[c].field_E <= 6) {
        stru_335C4[c].field_E = 999;
    }

    *(int16 *)&stru_335C4[c].field_10[0] = p;
    *(int16 *)&stru_335C4[c].field_10[2] = b;
    *(int16 *)&stru_335C4[c].field_10[4] = -1;

    if (p != 0x1e) {
        stru_335C4[c].field_A -= 0x1000;
    } else {
        *(int16 *)&stru_335C4[c].field_10[6] = sub_1C82D() - 0x400;
        word_3C020 = word_336F4;
    }

    if (word_336F4 >= 0 && sams[p].field_C == 6) {
        *(int16 *)&stru_335C4[c].field_10[4] = word_336F4;
    }

    if (word_336F4 >= 0 && sams[p].field_C == 5 && (stru_3AA5E[word_336F4].field_6 & 8)) {
        *(int16 *)&stru_335C4[c].field_10[4] = word_336F4;
    }

    if (p == 0x1d) {
        stru_335C4[c].field_A = (int16)0xc000;
        stru_335C4[c].field_6 = 1;
    }

    word_3A940 = c;
    strcpy((char *)strBuf, missiles[b].field_0);
    strcat((char *)strBuf, (char *)aFired);
    tempStrcpy((char *)strBuf);

    makeSound(sams[p].field_8 != 0 ? 0x12 : 0x18, 2);

    sub_11BC3(c, 1);

check_end:
    if (word_3C09E == 0x15) {
        sub_1957A(0x15);
    }
end:
    ;
}

// ==== seg000:0xa25c ====
int sub_1A25C(char param_1)
{
    int p;
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
    int g;
    int h;
    int i;
    int j;

    h = var_588 + 1;
    sub_19E44(0);
    sub_21444(param_1 == 0 ? var_564 : var_565, 0x78, 0x68, 0xc7, 0xaf);
    sub_19E44(8);
    j = 1;
    if (var_588 < 2 && word_38FDC != 0) {
        j = (1 << (2 - (unsigned char)var_588)) + 1;
    }
    i = 1 - j;
    e = word_3BEC0 & 0xf800;
    g = word_3BED0 & 0xf800;
    f = i * 2;
    while (j * 2 >= f) {
        sub_1A7C4(f * 0x400 + e, g + 0x1c00);
        p = var_279;
        b = var_282;
        sub_1A7C4(f * 0x400 + e, g - 0x1800);
        sub_19C84(p, b, var_279, var_282, 0x78, 0xc7, 0x68, 0xaf, 0);
        f += 2;
    }
    f = i * 2;
    while (j * 2 >= f) {
        sub_1A7C4(e + 0x1c00, f * 0x400 + g);
        p = var_279;
        b = var_282;
        sub_1A7C4(e - 0x1800, f * 0x400 + g);
        sub_19C84(p, b, var_279, var_282, 0x78, 0xc7, 0x68, 0xaf, 0);
        f += 2;
    }
    for (f = 0; f < word_3C046; f++) {
        if ((stru_3B208[f].field_10[8] & 2) && *(int16 *)&stru_3B208[f].field_10[10] != 0) {
            sub_1A7C4(*(int16 *)((char *)&stru_3B208[f] - 4), *(int16 *)((char *)&stru_3B208[f] - 2));
            if (word_3C016 != -1) {
                if (word_3C45C == 1 && f == word_336F2) {
                    sub_1A740(var_279, var_282, 7);
                }
                if (word_336F8 > 0 && f == 0xffff - word_3BE96) {
                    sub_1A740(var_279, var_282, word_38F72);
                }
                a = *(int16 *)&stru_3B208[f].field_10[0] - var_542 + 0x800;
                d = stru_3B208[f].field_0 - var_547;
                c = 0;
                if (d < -1000) {
                    c = 1;
                }
                if (d > 1000) {
                    c = 2;
                }
                sub_1A872((a >> 12) & 0xf, c, var_279, var_282);
            }
        }
    }
    for (f = 0; f < 12; f++) {
        if (*(int16 *)&stru_335C4[f].field_10[4] != 0) {
            sub_1A7C4(stru_335C4[f].field_0, stru_335C4[f].field_2);
            if (word_3C016 != -1) {
                if (sams[*(int16 *)&stru_335C4[f].field_10[6]].field_C <= 0) {
                    sub_19E44(0x0c);
                } else {
                    sub_19E44(0x0e);
                }
                if (sams[*(int16 *)&stru_335C4[f].field_10[6]].field_C == 3) {
                    sub_19E44(*(char *)&gfxModeUnset != 0 ? 8 : 0x0d);
                }
                if (!(stru_335C4[f].field_4 & 1)) {
                    sub_19E44(7);
                }
                if (f >= 8) {
                    sub_19E44(0x0f);
                }
                a = stru_335C4[f].field_8 - var_542;
                sub_19D86(var_279, var_282, var_279 - sub_1D178(a, h), sub_1D190(a, h) + var_282);
            }
        }
    }
    for (f = 0; f < word_3BED2; f++) {
        if (!(stru_3AA5E[f].field_6 & 0x80)) {
            sub_1A7C4(stru_3AA5E[f].field_0, stru_3AA5E[f].field_2);
            if (word_3C016 != -1) {
                if (word_3C45C == 2 && f == word_336F4) {
                    sub_1A740(var_279, var_282, 7);
                }
                a = 5;
                if (stru_3AA5E[f].field_6 & 0x201) {
                    a = (((-var_542 + 0x1000) >> 13) & 3) + 8;
                }
                if (stru_3AA5E[f].field_4 != 0) {
                    a = 1;
                }
                if (stru_3AA5E[f].field_6 & 8) {
                    a = 7;
                }
                if (f == word_3B146 || f == word_3B158) {
                    a = 6;
                }
                sub_1A872(a, 3, var_279, var_282);
            }
        }
    }
    sub_1A7C4(word_3BEC0, word_3BED0);
    if (word_3C016 != -1) {
        sub_1A872(0, 3, var_279, var_282);
    }
    for (f = 0; f < 4; f++) {
        if ((&word_333DA)[f * 6] != 0) {
            sub_1A7C4((&word_333D2)[f * 6], (&word_333D4)[f * 6]);
            if (word_3C016 != -1) {
                switch ((&word_333D8)[f * 6]) {
                case 1:
                    sub_1A872(2, 3, var_279, var_282);
                    break;
                case 2:
                    sub_1A872(3, 3, var_279, var_282);
                    break;
                }
            }
        }
    }
}

// ==== seg000:0x86f8 ====
void sub_186F8(int param_1)
{
    int p;

    if (!(stru_3B208[param_1].field_10[8] & 0x20)) {
        *(int16 *)&aFlogger[*(int16 *)&stru_3B208[param_1].field_10[6] * 32 + 22] += 1;
        if (*(uint16 *)&stru_3B208[param_1].field_10[8] & 0x800) {
            word_3C044--;
        }
        if (param_1 == word_336FC) {
            word_336FC = -1;
        }
        stru_3B208[param_1].field_10[8] |= 0x20;
        word_336F6 = -1;
        word_3BEC2 = *(int16 *)((char *)&stru_3B208[param_1] - 4);
        word_3BED6 = *(int16 *)((char *)&stru_3B208[param_1] - 2);
        word_3BFA2 = stru_3B208[param_1].field_0;
        word_3B4DC = 0x80;
        p = 3;
        sub_11D10(p, (*(uint16 *)&stru_3B208[param_1].field_10[8] & 0x4000 ? 0x80 : 0) + *(int16 *)&stru_3B208[param_1].field_10[6]);
        if (*(int16 *)&stru_3B208[param_1].field_10[10] != 0) goto done;
        *(uint16 *)&stru_3B208[param_1].field_10[8] &= 0x1c1;
    done:
        ;
    }
    strcpy((char *)strBuf, (char *)(*(int16 *)&stru_3B208[param_1].field_10[6] * 32 + 0x2c8));
    makeSound(2, 2);
    if (word_3C45C == 1 && param_1 == word_336F2) {
        word_39604 = 1;
    }
}

// ==== seg000:0x3816 ====
void sub_13816(int arg_0, int arg_1) {
    int p;
    int a;
    int b;
    int c;
    int d;

    b = (int)(unsigned char)**(char far **)&var_200 & 0x80;
    var_256 = (int)(unsigned char)(*(*(char far **)&var_200)++) & 0x7F;
    for (p = 0; p < var_256; p++) {
        var_200 += (unsigned char)var_258 * 2 + 2;
        if (b != 0) {
            a = (int)(unsigned char)(*(*(char far **)&var_200)++);
            c = (((int16 *)&byte_3B7FC[0x600])[buf3d3_1[a]] >> word_3C042) + arg_0;
            d = (((int16 *)byte_3BE3E)[buf3d3_2[a]] >> word_3C042) + arg_1;
        } else {
            c = (*(*(int far **)&var_200)++ >> word_3C042) + arg_0;
            d = (*(*(int far **)&var_200)++ >> word_3C042) + arg_1;
            var_200 += 2;
        }
        (&word_34684)[p * 2] = 1;
        (&word_34686)[p * 2] = 1;
        *(long *)((char *)&word_34868 + p * 4) = (long)(c + word_3298C);
        *(long *)((char *)&word_34A4C + p * 4) = (long)(-sub_13922(d) + word_3298E);
    }
}

// ==== seg000:0x783A ====
int sub_1783A(int arg_0, int arg_2)
{
    int p;

    p = *(int16 *)&stru_3B208[arg_0].field_10[6];
    *(int16 *)&stru_3B208[arg_0].field_10[0] = (word_3AFA8 == 1) ? 0 : (int16)0x8000;
    if (stru_3AA5E[arg_2].field_6 & 0x200) {
        *(int16 *)((char *)&stru_3B208[arg_0] - 4) = word_3AFA8 * 3 + stru_3AA5E[arg_2].field_0;
        *(int16 *)((char *)&stru_3B208[arg_0] - 2) = stru_3AA5E[arg_2].field_2 - word_3AFA8 * 12;
        stru_3B208[arg_0].field_0 = 0x8c;
        *(int16 *)&stru_3B208[arg_0].field_10[10] = 100;
        *(uint8 *)((char *)&stru_3B208[arg_0].field_10[1]) += 0xfc;
    } else {
        *(int16 *)((char *)&stru_3B208[arg_0] - 4) = stru_3AA5E[arg_2].field_0;
        *(int16 *)((char *)&stru_3B208[arg_0] - 2) = 0x1e * word_3AFA8 + stru_3AA5E[arg_2].field_2;
        stru_3B208[arg_0].field_0 = 0x0c;
        *(int16 *)&stru_3B208[arg_0].field_10[10] = 10;
    }
    stru_3B208[arg_0].field_2 = (long)(uint16)*(int16 *)((char *)&stru_3B208[arg_0] - 4) << 5;
    stru_3B208[arg_0].field_6 = (long)(uint16)*(int16 *)((char *)&stru_3B208[arg_0] - 2) << 5;
    *(int16 *)&stru_3B208[arg_0].field_10[2] = 0;
    *(int16 *)&stru_3B208[arg_0].field_10[4] = 0;
    *(uint16 *)&stru_3B208[arg_0].field_10[8] |= 0x403;
    *(int16 *)((char *)&stru_3B208[arg_0] - 6) = arg_2;
    *(int16 *)&stru_3B208[arg_0].field_10[12] = (int16)(((long)*(int16 *)&aFlogger[p * 32 + 12] << 11) * (long)word_330C4 / (long)*(int16 *)&aFlogger[p * 32 + 10]);
    *(int16 *)&stru_3B208[arg_0].field_10[16] = sub_19A4D(stru_3AA5E[arg_2].field_0, stru_3AA5E[arg_2].field_2);
    if (word_336FC == -1) {
        stru_3B208[arg_0].field_10[9] &= 0xfe;
    }
    placeString(arg_2);
    strcat((char *)strBuf, (char *)0x58bb);
    strcat((char *)strBuf, (char *)(*(int16 *)&stru_3B208[arg_0].field_10[6] * 32 + 0x2c8));
    strcat((char *)strBuf, (char *)aTakingOff);
    if (arg_0 < word_3C046 - 4) {
        tempStrcpy((char *)strBuf);
    }
}

void sub_187EA(int param_1)
{
    int p;
    int a;
    int b;

    placeString(param_1);
    p = 1;
    a = stru_3AA5E[param_1].field_6;
    if (a & 0x80) goto done;

    if (a & 0x1000) {
        word_3C048--;
    }

    word_39808 = sub_12FDA(
        (long)stru_3AA5E[param_1].field_0 << 5,
        (0x8000L - (long)stru_3AA5E[param_1].field_2) << 5);

    if (param_1 != 0) {
        if (stru_3AA5E[param_1].field_4 == 0) {
            p = 0x0c;
        }
        *((uint8 *)&stru_3AA5E[param_1].field_6) |= 0x80;
        stru_3AA5E[param_1].field_4 = 0;
        for (a = 0; a < 2; a++) {
            if ((&word_3B146)[a * 9] == param_1) {
                sub_189AA(a);
                if (a == 0) {
                    p |= 0x80;
                } else {
                    p |= 0x40;
                }
                word_336EE = word_336E8 + word_330C4;
                makeSound(0, 2);
            }
        }
        sub_11D10(p, param_1);
        b = sub_1C864(param_1);
    } else {
        if (sub_1C8A4(param_1) != 0) {
            b = (int)(char)byte_3BEC4[0];
        } else {
            b = (int)(char)byte_3C02A[0];
        }
        a = *word_39808;
        if (b != a) {
            byte_3BED8[a]++;
            sub_11D10(2, *word_39808);
        }
        *(((uint8 *)&b) + 1) |= 1;
        stru_3AA5E[param_1].field_C = b;
    }

    if (word_39808 != 0) {
        sub_13224((char *)word_39808, sub_1CF32(b), (char)b);
    }

done:
    word_336F6 = param_1;
    makeSound(2, 2);
    if (word_3C45C == 2 && param_1 == word_336F4) {
        word_39604 = 1;
    }
    if (word_3C09A == 0) {
        sub_195C9(word_3BEC0, word_3BED0);
    }
    if (word_330B8 < 2) {
        sub_166BE();
    }
}

void sub_16172()
{
    int p, a, b, c, d, e;

    if ((word_336F8 == 0 || word_3C03E != word_336E4) && word_330C2 != 0) {
        if (word_336F8 == 0 && word_3C09A == 0) {
            sub_1A962();
            word_3BF3C = 0;
            word_3BF3E = 0x100;
        }
    }

    for (d = 0; d < word_38FFA; d++) {
        if (stru_3AA5E[d].field_4 != 0 &&
            !(stru_3AA5E[d].field_6 & 0x80) &&
            ((d * (word_336E8 >> 10) * 7 & 7) <= 7 ||
            *(int16 *)&stru_3AA5E[d].field_8 != 0 ||
            (stru_3AA5E[d].field_6 & 0x100) != 0)) {
            *((int16 *)&stru_3AA5E[d].field_8 + 1) -= 1;
            if (*((int16 *)&stru_3AA5E[d].field_8 + 1) < 1) {
                *((int16 *)&stru_3AA5E[d].field_8 + 1) =
                    ((int)(char)word_330C4 << 8) /
                    ((*(int16 *)&stru_3AA5E[d].field_8 >> 3) + 0x20) + d / 2;
            }
            if (*((int16 *)&stru_3AA5E[d].field_8 + 1) == 4 && word_336F8 < 0) {
                sub_16346(d);
                stru_3AA5E[d].field_6 |= 0x02;
            }
        } else {
            stru_3AA5E[d].field_6 &= ~0x02;
        }
    }

    if (commData->gfxModeNum == 0) {
        word_38F72 = 0;
    }

    if (word_3C09A == 0 && word_336F8 > 0 && word_330C2 != 0 && word_3BE7E > 1) {
        if (word_38FDC != 0 && commData->gfxModeNum != 0) {
            sub_1A9BC();
            a = (int)((long)sub_1CF64(word_330C4 - word_336F8, 1, word_330C4) * (long)word_3BE7E / (long)word_330C4) << 6;
        } else {
            a = word_3BE7E << 6;
            word_3BE7E = 0;
        }
        if (word_3BE96 >= 0) {
            sub_19ADB(stru_3AA5E[word_3BE96].field_0,
                      stru_3AA5E[word_3BE96].field_2,
                      a, word_38F72, word_3C6AA,
                      word_3BF3C, word_3BF3E);
        }
    }

    word_336F8--;
}

// ==== seg000:0x7594 ====
void sub_17594(int param_1)
{
    int p, a, b, c, d, e, f, g, h, i, j, k, l, m, n;

    i = *(int16 *)&aFlogger[var_667 * 32 + 16];

    m = sub_1660E(
        *(int16 *)((char *)&word_3B204 + param_1 * 0x24),
        *(int16 *)((char *)&word_3B206 + param_1 * 0x24),
        stru_3B208[param_1].field_0,
        i,
        &d, &g);

    word_336FA = 4;

    if ((unsigned)m > (unsigned)g) {
        /* close enough — increment heat */
        *(int16 *)&stru_3B208[param_1].field_10[18] += ((word_330BA + word_330B8) * 16 + 0x20) >> ((planeFlags & 0x10) != 0);

        if (*(int16 *)&stru_3B208[param_1].field_10[18] > 0xc0) {
            word_3C09C++;
            stru_3B208[param_1].field_10[9] |= 0x40;
            sub_166BE();

            j = param_1 % (word_330B8 + 1);

            if (word_330B8 * 2 < word_3A946) goto end_launch;
            if (stru_335C4[j].field_E != 0) goto end_launch;
            if ((unsigned)g <= 8) goto end_launch;

            if (abs(d - *(int16 *)&stru_3B208[param_1].field_10[0]) >= 0x1800) goto end_launch;

            i = *(int16 *)&stru_3B208[param_1].field_10[14];

            if (sams[i].field_8 <= (unsigned)g >> 1) goto end_launch;

            if ((unsigned)(-(word_330B8 * 3 - 0x10)) >= (unsigned)g) goto end_launch;
            if ((unsigned)g >= 0x1000) goto end_launch;
            if (i == 0) goto end_launch;

            /* launch missile into slot j */
            stru_335C4[j].field_0 = *(int16 *)((char *)&stru_3B208[param_1] - 4);
            stru_335C4[j].field_2 = *(int16 *)((char *)&stru_3B208[param_1] - 2);
            stru_335C4[j].field_4 = stru_3B208[param_1].field_0 - 0x19;
            stru_335C4[j].field_6 = sams[i].field_A >> 6;
            stru_335C4[j].field_8 = *(int16 *)&stru_3B208[param_1].field_10[0];
            stru_335C4[j].field_A = *(int16 *)&stru_3B208[param_1].field_10[2] - 0x400;
            stru_335C4[j].field_C = *(int16 *)&stru_3B208[param_1].field_10[4];

            n = stru_335C4[j].field_6;
            stru_335C4[j].field_E = (int)(((long)sams[i].field_8 * 8L * (long)word_330C4) / (long)n);

            *(int16 *)&stru_335C4[j].field_10[0] = i;
            *(int16 *)&stru_335C4[j].field_10[6] = -param_1;

            strcpy((char *)strBuf, (char *)&sams[i].field_0);
            strcat((char *)strBuf, (char *)aFiredBy);
            strcat((char *)strBuf, (char *)(var_667 * 32 + 0x2c8));
            tempStrcpy((char *)strBuf);

            makeSound(6, 2);
            commData->restartFlag++;
            sub_11BC3(param_1 + 0x20, 2);

            if (sub_1D200(4) == 0) {
                stru_3B208[param_1].field_10[8] |= 4;
            }
        end_launch:;
        }
        stru_3B208[param_1].field_10[8] |= 8;
    } else {
        stru_3B208[param_1].field_10[8] &= 0xf7;
        *(int16 *)&stru_3B208[param_1].field_10[18] -= 0x20;
    }

    *(int16 *)&stru_3B208[param_1].field_10[18] =
        sub_1CF64(*(int16 *)&stru_3B208[param_1].field_10[18], 0, 0xff);
}

void sub_16346(int param_1)
{
    int p[11];
    unsigned int a[4];
    int j;
    int k;
    int l;
    int m;
    int n;
    int o;

    k = stru_3AA5E[param_1].field_4;
    o = sub_1660E(stru_3AA5E[param_1].field_0, stru_3AA5E[param_1].field_2, 0, k, p, a);
    word_336FA = 0;
    if (o > 0) {
        j = o;
        if (j > 99) {
            j = 99;
        }
        word_336FA = 4;
        if (o + word_336E4 > 50) {
            word_336FA = 0x0c;
        }
        if (o + word_336E4 > 100) {
            word_336FA = 0x0e;
        }
        word_3BE7E = o;
        word_336F8 = word_330C4;
        word_3BE96 = param_1;
        word_3C6AA = *(int16 *)&aNone[k * 14 + 12] & 1;
        if (*(int16 *)&stru_3AA5E[param_1].field_8 != 0) {
            word_3BF3C = (p[0] >> 8) - 0x20;
            word_3BF3E = (p[0] >> 8) + 0x20;
        }
        word_38F72 = word_336FA;
        if (!(*(uint8 *)&stru_3AA5E[param_1].field_6 & 4)) {
            *(uint8 *)&stru_3AA5E[param_1].field_6 |= 4;
        }
    }
    if (o > a[0]) {
        *(int16 *)&stru_3AA5E[param_1].field_8 += (word_330BA + word_330B8) * 32 + 32;
        if (*(int16 *)&stru_3AA5E[param_1].field_8 > 255) {
            *(int16 *)&stru_3AA5E[param_1].field_8 = 255;
        }
        if (!(stru_3AA5E[param_1].field_6 & 0x100) && word_333DA == 0 &&
            *(int16 *)&stru_3AA5E[param_1].field_8 > 0x7f) {
            sub_166BE();
        }
        if (word_3A946 > word_330B8) goto end_fire;
        if (*(int16 *)&stru_3AA5E[param_1].field_8 <= 0xc0) goto end_fire;
        if (k == 0x15) goto end_fire;
        if (word_38FEE <= 0x500) goto end_fire;
        if ((unsigned)-(word_330B8 * 3 - 20) >= a[0]) goto end_fire;
        word_3C09C++;
        if (*(int16 *)&stru_3AA5E[param_1].field_8 < 0xfa) goto end_fire;
        if (word_330B8 != 0) {
            l = param_1 % word_330B8;
        } else {
            l = 0;
        }
        if (stru_335C4[l].field_E != 0) goto end_fire;
        if (sams[k].field_8 <= (unsigned)a[0]) goto end_fire;

        stru_335C4[l].field_0 = stru_3AA5E[param_1].field_0 + 8;
        stru_335C4[l].field_2 = stru_3AA5E[param_1].field_2;
        stru_335C4[l].field_4 = 0;
        stru_335C4[l].field_6 = 1;
        stru_335C4[l].field_8 = p[0];
        stru_335C4[l].field_A = 0x4000;
        stru_335C4[l].field_E = (int)((((long)sams[k].field_8 << 3) * (long)word_330C4) / (long)(sams[k].field_A >> 6));
        *(int16 *)&stru_335C4[l].field_10[0] = k;
        *(int16 *)&stru_335C4[l].field_10[6] = param_1;

        placeString(param_1);
        strcat((char *)strBuf, (char *)aFiring);
        strcat((char *)strBuf, (char *)&sams[k]);
        tempStrcpy((char *)strBuf);
        makeSound(6, 2);
        sub_11BC3(param_1 + 0x40, 2);
        commData->restartFlag++;

    end_fire:
        *(uint8 *)&stru_3AA5E[param_1].field_6 |= 0x10;
    } else {
        *(uint8 *)&stru_3AA5E[param_1].field_6 &= 0xEF;
        *(int16 *)&stru_3AA5E[param_1].field_8 -= 0x10;
        if (*(int16 *)&stru_3AA5E[param_1].field_8 < 0) {
            *(int16 *)&stru_3AA5E[param_1].field_8 = 0;
        }
    }
}

void sub_10334(int arg_0)
{
    int p;
    int a;
    int b[17];
    int c[17];
    int d[17];
    int e[17];
    int f[8];
    int g;
    register int i;
    register int j;

    if (*(char *)&word_38FDC < 3) {
        sub_1FEEC(arg_0);
        return;
    }
    a = 0;
    do {
        i = a + a;
        *((int *)((char *)&word_3BE9C + i)) = *((int *)((char *)&word_32990 + i));
        a++;
    } while (a < 16);
    word_38FC6 = -var_226;
    p = (int)(((long)var_224 << 8) / (long)(var_225 < 0x200 ? 0x200 : var_225));
    if (*(char *)&var_594 != 0) {
        p <<= *(char *)&var_594;
    }
    if (*(char *)&var_456 != 0) {
        p >>= 1;
    }
    for (a = 0; a < 17; a++) {
        if (a < 16) {
            g = *(&word_3BE9C + a) + p;
        } else {
            g = 0x5848;
        }
        i = sub_13B2F(-0x5848, var_227);
        j = sub_13B2F(g, word_38FC6);
        b[a] = (word_3298C + i) - j;
        d[a] = (word_3298C - i) - j;
        i = sub_13B2F(g, var_227);
        j = sub_13B2F(-0x5848, word_38FC6);
        c[a] = word_3298E - (j - ((i + j >> 2) - i));
        e[a] = ((i - j >> 2) + word_3298E) - i + j;
    }
    a = 0;
    do {
        f[0] = b[a];
        f[1] = c[a];
        f[2] = d[a];
        f[3] = e[a];
        f[4] = d[a + 1];
        f[5] = e[a + 1];
        f[6] = b[a + 1];
        f[7] = c[a + 1];
        sub_1F882(word_3298A, 4, f, a + 0x60);
        a++;
    } while (a < 16);
    word_3BE9C = var_220 / 0x200;
    a = 1;
    do {
        *(&word_3BE9C + a) = var_220 / ((16 - a) * 0x20) - word_3BE9C;
        a++;
    } while (a < 16);
    word_3BE9C = 0;
    for (a = 0; a < 17; a++) {
        if (a < 16) {
            g = p - *(&word_3BE9C + a);
        } else {
            g = -0x5848;
        }
        i = sub_13B2F(-0x5848, var_227);
        j = sub_13B2F(g, word_38FC6);
        b[a] = (word_3298C + i) - j;
        d[a] = (word_3298C - i) - j;
        i = sub_13B2F(g, var_227);
        j = sub_13B2F(-0x5848, word_38FC6);
        c[a] = word_3298E - (j - ((i + j >> 2) - i));
        e[a] = ((i - j >> 2) + word_3298E) - i + j;
    }
    a = 0;
    do {
        f[0] = b[a];
        f[1] = c[a];
        f[2] = d[a];
        f[3] = e[a];
        f[4] = d[a + 1];
        f[5] = e[a + 1];
        f[6] = b[a + 1];
        f[7] = c[a + 1];
        sub_1F882(word_3298A, 4, f, a + 0x70);
        a++;
    } while (a < 16);
}

int sub_1CB42(param_1, param_2, param_3, param_4, param_5, param_6, param_7, param_8, param_9)
int param_1;
int param_2;
int param_3;
int param_4;
int param_5;
int param_6;
int param_7;
int param_8;
int param_9;
{
    int p;
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
    int g;
    int h;
    int i;
    int j;
    int k;
    int l;
    int m;
    char n;

    word_39402 = 1;
    if (param_8 == 1 && word_38FDC == 0 && *(char*)&gfxModeUnset != 0 && (word_336E8 & 3) != 0) {
        return;
    }

    g = sub_1CF32(param_1);
    if (byte_3C5A0 == 0) {
        *var_568 = 0;
    } else {
        *var_568 = 1;
    }

    if (param_8 < 2) {
        var_685 = 0;
        k = param_2 - word_3BEC0;
        l = param_3 - word_3BED0;
        m = (param_4 - var_547) >> 5;
        b = sub_1D008(k, -l);
        f = sub_1D008(m, sub_1CFA6(k, l));
        c = sub_1CFA6(m, sub_1CFA6(k, l));

        if (param_8 == 1) {
            var_680 = c;
            var_682 = (c >> 4) + 0x190;
            var_683 = (var_682 << 5) / (c + 1);
            c = var_682 << 2;
            var_681 = b;
            var_684 = f;
        } else {
            var_683 = (var_680 << 5) / (c + 1);
            if (var_683 > 0x100) {
                var_683 = 0x100;
            }
            if (var_683 < 4) {
                var_683 = 4;
            }
            j = ((b - var_681) >> 5) * var_683;
            d = ((f - var_684) >> 5) * var_683;
            if (abs(j) > 0x1000) {
                return;
            }
            if (abs(d) > 0x1000) {
                return;
            }
            b = (j << 2) + var_681;
            f = (d << 2) + var_684;
            c = (var_682 << 5) / var_683 << 2;
        }

        i = sub_1D190(f, c);
        var_594 = 2;
        if (param_9 < 0) {
            var_594 = (uint8)(param_9 + 2);
            param_9 = 0;
        }
        k = sub_1D178(b, i) >> (char)param_9;
        l = -(sub_1D190(b, i)) >> (char)param_9;
        m = sub_1D178(f, c) >> (char)param_9;
    } else {
        k = (param_2 - word_3BEC0) << 4;
        l = (param_3 - word_3BED0) << 4;
        m = (param_4 - var_547) >> 1;
        var_681 = var_542;
        var_684 = word_38FCE;
        var_685 = var_545;
        var_683 = 0x20;
        var_594 = 2;
    }
    if (param_8 == 1 || param_8 == 3) {
        a = (int)((long)var_683 * (long)((int)var_684 >> 2) >> 5) + 0x9c;
        if (a < 0x80 || (int)var_684 < (int)0xe800) {
            a = 0x80;
        }
        if (a > 0xb8 || (int)var_684 > 0x1800) {
            a = 0xb8;
        }
        *(((int*)var_568) + 2) = (int)byte_3419A[0];
        if (a != 0x80) {
            sub_21444((int*)var_568, 0xe8, 0x80, 0x130, a);
        }
        h = byte_228D0[0x2f];
        e = (int)(signed char)byte_3BFA4[param_1 & 0x7f];
        if (e & 0x10) {
            h = 8;
        }
        n = (char)(e & 0xf);
        if (n == 0xc || n == 9 || n == 0xb) {
            h = 1;
        }
        *(((int*)var_568) + 2) = (int)byte_3419C[h];
        if (a != 0xb8) {
            sub_21444((int*)var_568, 0xe8, a, 0x130, 0xb8);
        }
    }

    var_316 = 1;
    sub_13932((char*)var_568, -var_681, var_684, var_685, 0, 0, 0, 0);
    sub_20104(byte_228D0 + g, -param_5, param_6, param_7, k, -l, m);
    sub_139AA();
    var_316 = 0;

    if (param_8 == 1) {
        strcpy(strBuf, (char*)aBrg);
        strcat(strBuf, itoa((unsigned int)var_681 / 0xb6, unk_3C030, 10));
        draw2Strings(strBuf, 0xf8, 0xb0, 0xf);
    }
    var_594 = 0;
}
