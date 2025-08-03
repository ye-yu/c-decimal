#include "b_dec_zero.h"

int zero_b_uint_arr(b_uint *a, const size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        a[i] = 0;
    }
    return 0;
}

int zero(b_dec *a)
{
    a->sign = 0;
    a->prec = 0;
    zero_b_uint_arr(a->mag, CHUNKSIZE);
    return 0;
}