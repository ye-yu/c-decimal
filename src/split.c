#include "b_dec_split.h"


b_uint last_half(const r_uint comp)
{
    return (b_uint)(comp & ALL_ONES);
}

b_uint first_half(const r_uint comp)
{
    return last_half(comp >> BITSIZE);
}

int split_at(b_uint a, size_t at, b_uint *first_half, b_uint *last_half)
{
    if (at >= BITSIZE)
    {
        return 1; // Invalid position
    }
    *first_half = a >> at;
    *last_half = a & ~((ZERO - 1) << at);
    return 0;
}
