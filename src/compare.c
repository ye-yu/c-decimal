#include "b_dec_compare.h"

int compare(const b_dec a, const b_dec b)
{
    if (is_zero(a) && is_zero(b))
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

    for (size_t i = 0; i < CHUNKSIZE; i++)
    {
        if (a.mag[i] != b.mag[i])
        {
            return a.mag[i] < b.mag[i] ? -1 : 1; // Compare magnitudes
        }
    }

    return 0; // They are equal
}

int is_zero(const b_dec a)
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