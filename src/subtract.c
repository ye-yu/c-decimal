#include "b_dec_subtract.h"

int subtract(b_uint a, b_uint b, b_uint *result)
{
    uint64_t underflow = 0;
    if (a < b)
    {
        underflow = 1;
        b_uint temp = a;
        a = b;
        b = temp;
    }

    *result = a - b;
    return underflow;
}