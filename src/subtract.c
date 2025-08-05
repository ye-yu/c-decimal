#include "b_dec_subtract.h"
#include "b_dec_add.h"
#include "b_dec_copy.h"
#include "b_dec_compare.h"
#include "b_dec_zero.h"

int subtract_b_uint(const b_uint a, const b_uint b, b_uint *result)
{
    const b_uint b_comp = ~b;
    b_uint sum = 0;
    int overflow = add_b_uint(a, b_comp, &sum);
    if (overflow)
    {
        sum++;
    }
    *result = sum;

    // underflow, negative number
    return b > a ? 1 : 0;
}

int subtract_b_uint_arr(const b_uint *a, const b_uint *b, b_uint *r, const size_t size)

{

    if (is_zero_b_uint_arr(b, size))
    {
        copy_b_uint_arr(a, r, size);
        return 0; // nothing to subtract
    }

    if (compare_b_uint_arr(a, b, size) == 0)
    {
        zero_b_uint_arr(r, size);
        return 0; // same number
    }

    b_uint *result = (b_uint *)malloc(size * sizeof(b_uint));
    if (!result)
    {
        return 1; // memory allocation failed
    }
    zero_b_uint_arr(result, size);

#define RETURN_AFTER_FREE(v) \
    do                       \
    {                        \
        free(result);        \
        return v;            \
    } while (0);

    for (size_t i = 0; i < size; i++)
    {
        result[i] = ~b[i];
    }

    const int underflow = compare_b_uint_arr(a, b, size) < 0 ? 1 : 0;
    int overflow = add_b_uint_arr(a, result, result, size) + underflow;
    if (overflow)
    {
        for (size_t i = 0; i < size && overflow; i++)
        {
            const size_t i_from_last = size - i - 1;
            overflow = add_b_uint(result[i_from_last], 1, &result[i_from_last]);
        }
    }

    copy_b_uint_arr(result, r, size);
    RETURN_AFTER_FREE(underflow);
}