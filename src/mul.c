#include "b_dec_mul.h"
#include "b_dec_split.h"
#include "b_dec_zero.h"
#include "b_dec_carry.h"
#include "b_dec_compare.h"
#include "b_dec_copy.h"
#include "b_dec_add.h"
#include "b_dec_trailing.h"
#include "b_dec_shift.h"

/**
 * @param a First operand, normalized
 * @param b Second operand, normalized
 * @param r pointer, result of multiplication
 * @return 0 on success, 1 on overflow
 */
int mul_b_dec(const b_dec a, const b_dec b, b_dec *r)
{
    b_dec result;
    zero(&result);
    result.sign = a.sign ^ b.sign; // XOR for sign
    result.prec = a.prec + b.prec;
    int overflow = mul_b_uint_arr(a.mag, b.mag, result.mag, CHUNKSIZE);

    if (overflow)
    {
        copy(result, r);
        return 1; // overflow occurred
    }

    remove_trailing_zeroes(result.mag, &result.prec, CHUNKSIZE);
    copy(result, r);
    return 0;
}

int mul_10_b_dec(const b_dec num, b_dec *result)
{
    zero(result);
    result->sign = num.sign; // sign remains the same
    result->prec = num.prec; // precision remains
    return mul_10_b_uint_arr(num.mag, result->mag, CHUNKSIZE);
}

int mul_b_uint(const b_uint a, const b_uint b, b_uint *r)
{
    r_uint big_a = (r_uint)a;
    r_uint big_b = (r_uint)b;
    r_uint product = big_a * big_b;
    const b_uint first_half_product = first_half(product);
    const b_uint last_half_product = last_half(product);
    *r = last_half_product;
    return first_half_product > 0 ? 1 : 0; // return 1 if overflow, otherwise 0
}

int mul_b_uint_arr(const b_uint *arr_a, const b_uint *arr_b, b_uint *r, const size_t size)
{
    zero_b_uint_arr(r, size);
    if (is_zero_b_uint_arr(arr_a, size) || is_zero_b_uint_arr(arr_b, size))
    {
        return 0; // nothing to multiply
    }

    int overflow = 0;
    for (size_t index_b = 0; index_b < size; index_b++)
    {
        const size_t index_b_from_last = size - 1 - index_b;
        r_uint operand_b = (r_uint)arr_b[index_b_from_last];
        b_uint carry_next = 0;

        for (size_t index_a = 0; index_a < size; index_a++)
        {
            const size_t index_a_from_last = size - 1 - index_a;
            const r_uint operand_a = (r_uint)arr_a[index_a_from_last];
            const r_uint product = operand_a * operand_b;
            const b_uint first_half_product = first_half(product);
            const b_uint last_half_product = last_half(product);

            const size_t index_r_last = index_b_from_last - index_a;
            const b_uint carry_to_r_first = add_b_uint(r[index_r_last], last_half_product, r + index_r_last) ? 1 : 0;

            if (index_r_last == 0)
            {
                carry_next = carry_to_r_first || first_half_product ? 1 : 0;
                continue;
            }

            const size_t index_r_first = index_r_last - 1;
            b_uint carry_to_r_first_before = add_b_uint(r[index_r_first], carry_to_r_first, r + index_r_first);
            carry_to_r_first_before += add_b_uint(r[index_r_first], first_half_product, r + index_r_first);
            if (index_r_first == 0)
            {
                carry_next = carry_to_r_first_before;
                continue;
            }

            // propagate carry
            for (size_t index_r = 0; index_r < index_r_first && carry_next; index_r++)
            {
                const size_t index_r_from_last = index_r_first - 1 - index_r;
                carry_next = add_b_uint(r[index_r_from_last], carry_next, r + index_r_from_last);
            }
        }

        overflow |= carry_next ? 1 : 0; // accumulate overflow status
    }
    return overflow;
}

int mul_10_b_uint_arr(const b_uint *arr_a, b_uint *r, const size_t size)
{
    b_uint *operand1 = (b_uint *)malloc(size * sizeof(b_uint));
    b_uint *operand2 = (b_uint *)malloc(size * sizeof(b_uint));
#define MAYBE_FREE(v) \
    do                \
    {                 \
        if (v)        \
        {             \
            free(v);  \
        }             \
    } while (0);

    if (!operand1 || !operand2)
    {
        MAYBE_FREE(operand1);
        MAYBE_FREE(operand2);
        return 1; // error: memory allocation failed
    }

#define RETURN_AFTER_FREE(v) \
    do                       \
    { /* free here */        \
        free(operand1);      \
        free(operand2);      \
        return v;            \
    } while (0);

    zero_b_uint_arr(operand1, size);
    zero_b_uint_arr(operand2, size);

    shift_arr_left(arr_a, operand1, size, 1); // Shift left by 1
    shift_arr_left(arr_a, operand2, size, 3); // Shift left by 1

    int add_overflow = add_b_uint_arr(operand1, operand2, r, size);     // Add the two shifted values
    int overflow = add_overflow || (arr_a[0] >> (BITSIZE - 3)) ? 1 : 0; // Check for overflow

    RETURN_AFTER_FREE(overflow);
#undef MAYBE_FRE
#undef RETURN_AFTER_FREE
}
