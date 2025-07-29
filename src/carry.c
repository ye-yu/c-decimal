#include "b_dec_carry.h"


int overflown(const b_uint comp)
{
    return ((comp >> (BITSIZE - 1)) & (0b1)) != 0;
}

int normalize(b_uint *a)
{
    (*a) = (*a) & MASK;
    return 0;
}

int carry(b_uint *a, const size_t size)
{
    for (size_t i = size - 1; i > 0; i--)
    {
        if (overflown(a[i]))
        {
            normalize(&a[i]);
            a[i - 1] += 1;
        }
    }
    if (overflown(a[0]))
    {
        return 1; // Indicate overflow
    }
    return 0;
}

