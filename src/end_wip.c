/* end_wip.c — Work-in-progress functions for porting to end0.c.
 *
 * Variable names are chosen to produce correct BP offsets via the MSC 5.1
 * hash table allocation (hash = sum of uppercase ASCII % 16).
 * See tools/msc_hash.py and docs/porting_notes.md for details.
 */

#if 0 /* routine_130 — word-wrapping text draw, goes in end0.c (/Gs /Zi)
       *
       * Variable name mapping (name → hash bucket → BP offset):
       *   p  = font        (bucket 0 → bp-0x02)
       *   a  = lineStart   (bucket 1 → bp-0x04)
       *   b  = cur         (bucket 2 → bp-0x06)
       *   c  = charCount   (bucket 3 → bp-0x08)
       *   d  = (unused)    (bucket 4 → bp-0x0a)
       *   e  = savedStart  (bucket 5 → bp-0x0c)
       *   f  = running     (bucket 6 → bp-0x0e)
       *   g  = pixelWidth  (bucket 7 → bp-0x10)
       *   buf              (bucket 13 → bp-0x3f8, 1000 bytes)
       */

void routine_130(int *page, char *str, unsigned int maxWidth, int x, int y, int lineHeight) {
    int p;              /* font */
    char *a;            /* lineStart */
    uint8 *b;           /* cur — uint8* for unsigned char extension (sub ah,ah) */
    int c;              /* charCount */
    int d;              /* unused — fills gap at bp-0x0a */
    char *e;            /* savedStart */
    int8 f;             /* running */
    unsigned int g;     /* pixelWidth */
    char buf[1000];

    e = str;
    a = str;
    b = str;
    p = page[6];
    page[5] = y;
    f = 1;
    do {
        if (f == 0) {
            return;
        }
        g = c = 0;
        while (g < maxWidth && *b != '\0' && *b != '\r' && *b != '\n') {
            g += gfx_jump_2f_charWidth(*b++, p);
            c++;
        }
        if (g >= maxWidth) {
            b--;
            c--;
        }
        while (*b != ' ' && *b != '\0' &&
               *b != '\r' && *b != '\n' && *b != '-' &&
               b > e) {
            b--;
            c--;
        }
        if (*b == '-') {
            c++;
        }
        if (*b == '\0') {
            f = 0;
        }
        if (c != 0) {
            routine_146(buf, a, c);
            buf[c] = 0;
            page[4] = x;
            gfx_jump_05_drawString(page, buf);
            page[5] += lineHeight;
            if (*b == '\r') {
                page[5] += 2;
            }
        }
        b++;
        a = b;
    } while (1);
}

#endif
