#include "b_dec_split.h"
#include "b_dec_shift.h"

b_uint last_half(const r_uint comp)
{
    return (b_uint)(comp & ALL_ONES);
}

b_uint first_half(const r_uint comp)
{
    return last_half(comp >> BITSIZE);
}

int split_at(const b_uint a, size_t at, b_uint *first_half, b_uint *last_half)
{
    if (at >= BITSIZE)
    {
        return 1; // Invalid position
    }
    *first_half = a >> at;
    *last_half = a & ~((ZERO - 1) << at);
    return 0;
}

int split_arr_at(const b_uint *a, const size_t at_from_behind, b_uint *first_half_arr, b_uint *last_half_arr, size_t size)
{
    if (at_from_behind >= BITSIZE * size)
    {
        // not possible
        return 1;
    }
    long long at_from_behind_copy = (long long)at_from_behind;

    // copy for last half first
    for (size_t i = 0; i < size; at_from_behind_copy -= BITSIZE, i++)
    {
        size_t last = size - i - 1;
        // at_from_behind_copy is 32, 33, ...
        if (at_from_behind_copy >= BITSIZE)
        {
            last_half_arr[last] = a[last];
            continue;
        }
        else if (at_from_behind_copy < 0)
        {
            last_half_arr[last] = 0;
        }
        // at_from_behind_copy is 0, 1, 2, ..., 30, 31
        else
        {
            b_uint first_half, last_half;
            int error = split_at(a[last], at_from_behind_copy, &first_half, &last_half);
            if (error)
            {
                return 1;
            }
            last_half_arr[last] = last_half;
        }
    }

    // do first half by shifting
    return shift_arr_right(a, first_half_arr, size, at_from_behind);
}
