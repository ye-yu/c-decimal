#include "b_dec_compare.h"

int compare_b_dec(const b_dec a, const b_dec b)
{
    if (is_zero_b_dec(a) && is_zero_b_dec(b))
    {
        return 0; // Both are zero
    }

    if (a.sign != b.sign)
    {
        return a.sign ? -1 : 1; // If signs differ, return based on sign
    }

    if (a.prec != b.prec)
    {
        return a.prec < b.prec ? -1 : 1; // Compare precision
    }

    return compare_b_uint_arr(a.mag, b.mag, CHUNKSIZE); // Compare magnitudes
}

int compare_b_uint_arr(const b_uint *a, const b_uint *b, const size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        if (a[i] != b[i])
        {
            return a[i] < b[i] ? -1 : 1; // Compare elements
        }
    }
    return 0; // They are equal
}

int is_zero_b_dec(const b_dec a)
{
    for (size_t i = 0; i < CHUNKSIZE; i++)
    {
        if (a.mag[i] != 0)
        {
            return 0; // Non-zero magnitude indicates non-zero value
        }
    }

    return 1; // All checks passed, it is zero
}

int is_zero_b_uint_arr(const b_uint *a, const size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        if (a[i] != 0)
        {
            return 0; // Non-zero element found
        }
    }
    return 1; // All elements are zero
}