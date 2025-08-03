#include "b_dec_mul.h"
#include "b_dec_split.h"
#include "b_dec_zero.h"
#include "b_dec_carry.h"
#include "b_dec_compare.h"
#include "b_dec_copy.h"

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

    copy(result, r);
    return overflow;
}

int mul_10_b_dec(const b_dec num, b_dec *result)
{
    static const b_dec _10 = {.mag = {10}, .sign = 0, .prec = 0};
    return mul_b_dec(num, _10, result);
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
    if (is_zero_b_uint_arr(arr_a, size) || is_zero_b_uint_arr(arr_b, size))
    {
        zero_b_uint_arr(r, size);
        return 0; // nothing to multiply
    }

    int overflow = 0;
    for (size_t index_b = 0; index_b < size; index_b++)
    {
        const size_t index_b_from_last = BITSIZE - 1 - index_b;
        r_uint operand_b = (r_uint)arr_b[index_b_from_last];

        for (size_t index_a = 0; index_a < size; index_a++)
        {
            const size_t index_a_from_last = BITSIZE - 1 - index_a;
            const r_uint operand_a = (r_uint)arr_a[index_a_from_last];
            const r_uint product = operand_a * operand_b;
            const b_uint first_half_product = first_half(product);
            const b_uint last_half_product = last_half(product);

            const size_t index_r_last = index_b_from_last - index_a;
            const b_uint r_last_sum = r[index_r_last] + last_half_product;
            r[index_r_last] = r_last_sum;
            const b_uint carry_to_r_first = r_last_sum < r[index_r_last] || r_last_sum < last_half_product ? 1 : 0;

            if (carry_to_r_first)
            {
                if (index_r_last == 0)
                {
                    overflow = carry_to_r_first;
                }
                else
                {
                    r[index_r_last - 1] += carry_to_r_first;
                }
            }

            if (index_r_last == 0)
            {
                continue;
            }

            const size_t index_r_first = index_r_last - 1;
            const b_uint r_first_sum = r[index_r_first] + first_half_product + carry_to_r_first;
            r[index_r_first] = r_first_sum;
            const b_uint carry_to_r_first_before = r_first_sum < r[index_r_first] || r_first_sum < first_half_product ? 1 : 0;
            if (carry_to_r_first_before)
            {
                if (index_r_first == 0)
                {
                    overflow = carry_to_r_first_before;
                }
                else
                {
                    r[index_r_first - 1] += carry_to_r_first_before;
                }
            }
        }
    }

    return overflow;
}