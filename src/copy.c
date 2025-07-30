#include "b_dec_copy.h"

int copy(const b_dec src, b_dec *dest)
{
    dest->sign = src.sign;
    dest->prec = src.prec;
    for (int i = 0; i < CHUNKSIZE; i++)
    {
        dest->mag[i] = src.mag[i];
    }
    return 0;
}

int copy_b_uint_arr(const b_uint *src, b_uint *dest, const size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        dest[i] = src[i];
    }
    return 0;
}