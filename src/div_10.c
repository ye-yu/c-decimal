#include "b_dec_div_10.h"

int div_10(const uint64_t num, uint64_t *quotient, uint64_t *remainder) {
    uint64_t q, r;
    q = (num >> 1) + (num >> 2);
    q = q + (q >> 4);
    q = q + (q >> 8);
    q = q + (q >> 16);
    q = q + (q >> 32);
    q = q >> 3;
    r = num - q * 10;
    if (r > 9)
    {
        q++;
        r -= 10;
    }

    *quotient = q;
    *remainder = r;
    return 0;
}
