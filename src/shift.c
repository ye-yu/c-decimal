#include "b_dec_shift.h"
#include "b_dec_copy.h"
#include "b_dec_split.h"

int shift_arr_right(const b_uint *operand, b_uint *result, size_t size, size_t bits_to_shift)
{
    const long long last = (long long)(size - 1);
    if (bits_to_shift == BITSIZE)
    {
        for (long long i = last; i >= 0; i--)
        {
            result[i] = 0; // Shift by BITSIZE means all bits are shifted out
        }
        return 0;
    }
    copy_b_uint_arr(operand, result, size);

    if (bits_to_shift == 0)
    {
        return 0; // No shift needed
    }

    b_uint first_half, last_half;
    for (long long i = last; i >= 0; i--)
    {
        first_half = 0;
        last_half = 0;
        split_at(result[i], bits_to_shift, &first_half, &last_half);
        result[i] = first_half;
        if (i == last)
        {
            continue;
        }

        result[i + 1] |= (last_half << (BITSIZE - bits_to_shift));
    }
    return 0;
}

int shift_arr_left(const b_uint *operand, b_uint *result, size_t size, size_t bits_to_shift)
{
    const long long last = (long long)(size - 1);
    if (bits_to_shift == BITSIZE)
    {
        for (long long i = last; i >= 0; i--)
        {
            result[i] = 0; // Shift by BITSIZE means all bits are shifted out
        }
        return 0;
    }
    copy_b_uint_arr(operand, result, size);

    if (bits_to_shift == 0)
    {
        return 0; // No shift needed
    }

    b_uint first_half, last_half;
    for (size_t i = 0; i < size; i++)
    {
        first_half = 0;
        last_half = 0;
        split_at(result[i], bits_to_shift, &first_half, &last_half);
        result[i] <<= bits_to_shift;
        if (i == 0)
        {
            continue;
        }

        result[i - 1] |= (first_half >> (BITSIZE - bits_to_shift));
    }
    return 0;
}