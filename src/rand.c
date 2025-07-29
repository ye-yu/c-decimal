#include "b_dec_rand.h"

b_uint rand_b_uint()
{
    b_uint r = 0;
    for (size_t i = 0; i < CHUNKSIZE; i++)
    {
        r |= ((b_uint)rand() << (i * BITSIZE));
    }
    normalize(&r);
    return r;
}

b_uint make_zero_at_bit(const b_uint a, const size_t bit)
{
    if (bit >= BITSIZE)
    {
        return a; // Invalid bit position, return original value
    }
    b_uint mask = ~(ONE << bit);
    return a & mask;
}
