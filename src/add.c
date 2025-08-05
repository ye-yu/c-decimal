#include "b_dec_add.h"
#include "b_dec_copy.h"
#include "b_dec_zero.h"

int add_b_uint(const b_uint a, const b_uint b, b_uint *result)
{
    b_uint sum = a + b;
    *result = sum;

    if (sum < a || sum < b)
    {
        // overflow, should be handled by the caller
        return 1;
    }
    return 0;
}

int add_b_uint_arr(const b_uint *a, const b_uint *b, b_uint *r, const size_t size)
{
    if (size == 0)
    {
        // nothing to add
        return 0;
    }

    b_uint carry = 0;
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

    int64_t size_int = (int64_t)size;
    for (int64_t i_from_last = size_int - 1; i_from_last >= 0; i_from_last--)
    {
        b_uint sum;
        int overflown = add_b_uint(a[i_from_last], b[i_from_last], &sum);
        overflown += add_b_uint(sum, carry, &sum);
        result[i_from_last] = sum;

        if (overflown)
        {
            // to be forwarded to the next iteration
            carry = 1;
        }
        else
        {
            carry = 0;
        }
    }
    copy_b_uint_arr(result, r, size);

    RETURN_AFTER_FREE(carry); // return carry if there was an overflow
}