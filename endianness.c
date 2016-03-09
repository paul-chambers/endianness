#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef unsigned char byte;
typedef unsigned long size;
static struct {
    unsigned char pre[16];
    union {
        unsigned short      kShort;
        unsigned long       kLong;
        unsigned long long  kLongLong;
        float               kFloat;
        double              kDouble;
        long double         kLongDouble;
    } data;
    unsigned char post[16];
} dataBlk, maskBlk;

/*
kShort      = { -1, 0x0123,             -1};
kLong       = { -1, 0x01234567,         -1};
kLongLong   = { -1, 0x0123456789abcdef, -1};
kFloat      = { -1, (1.0 / 9.0 + 0.46), -1};
kDouble     = { -1, (2.0 / 9.0 + 0.28), -1};
kLongDouble = { -1, (1.0 / 9.0),        -1};
*/

void prepBlocks(void)
{
    memset(&dataBlk, 0x55, sizeof(dataBlk));
    memset(&maskBlk, 0xAA, sizeof(maskBlk));
}

void processBlocks(void)
{
    byte *d = (byte *)&dataBlk;
    byte *m = (byte *)&maskBlk;
    size i, l = sizeof(dataBlk);

    for (i = 0; i < l; ++i)
    {
        *m = (*m ^ 0xAA) | (*d ^ 0x55);
        ++m; ++d;
    }

    d = (byte *)&dataBlk;
    m = (byte *)&maskBlk;

    printf("[");
    for (i = 0; i < l; ++i)
    {
        if (*m) { printf(" %02x", *d); }
        ++d; ++m;
    }
    printf(" ]\n");
}

int main( int argc, char *argv[] )
{
    prepBlocks();
    dataBlk.data.kShort = 0x1234;
    maskBlk.data.kShort = 0x1234;
    printf("      short: "); processBlocks();

    prepBlocks();
    dataBlk.data.kLong = 0x12345678;
    maskBlk.data.kLong = 0x12345678;
    printf("       long: "); processBlocks();

    prepBlocks();
    dataBlk.data.kLongLong = 0x0123456789abcdef;
    maskBlk.data.kLongLong = 0x0123456789abcdef;
    printf("  long long: "); processBlocks();

    /* IEEE single: 32 bits, as 1 bit sign, 8 bit exponent, 23 bit mantessa */
    /* |s.eeeeeee|e.mmmmmmm|mmmmmmmm|mmmmmmmm| */
    prepBlocks();
    dataBlk.data.kFloat = (1.0 / 9.0 + 0.46);
    maskBlk.data.kFloat = (1.0 / 9.0 + 0.46);
    printf("      float: "); processBlocks();

    /* IEEE double: 64 bits, as 1 bit sign, 11 bit exponent, 52 bit mantessa */
    /* |s.eeeeeee|eeee.mmmm|mmmmmmmm|mmmmmmmm|mmmmmmmm|mmmmmmmm|mmmmmmmm|mmmmmmmm| */
    prepBlocks();
    dataBlk.data.kDouble = (2.0 / 9.0 + 0.28);
    maskBlk.data.kDouble = (2.0 / 9.0 + 0.28);
    printf("     double: "); processBlocks();

    /* Extended precision: 80 bits, as 1 bit sign, 15 bit exponent, 1 bit integer, 63 bit mantessa */
    /* |s.eeeeeee|eeeeeeee|i.mmmmmmm|mmmmmmmm|mmmmmmmm|mmmmmmmm|mmmmmmmm|mmmmmmmm|mmmmmmmm|mmmmmmmm| */
    prepBlocks();
    dataBlk.data.kLongDouble = (2.0 / 9.0 + 0.28);
    maskBlk.data.kLongDouble = (2.0 / 9.0 + 0.28);
    printf("long double: "); processBlocks();
}
