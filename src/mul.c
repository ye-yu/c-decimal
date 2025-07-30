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
int mul(const b_dec a, const b_dec b, b_dec *r)
{
    b_dec result;
    zero(&result);
    b_uint result_mag[CHUNKSIZE_DOUBLE];
    zero_uint(result_mag, CHUNKSIZE_DOUBLE);

    for (size_t index_b = 0; index_b < CHUNKSIZE; index_b++)
    {
        r_uint operand_b = (r_uint)b.mag[BITSIZE - 1 - index_b];
        for (size_t index_a = 0; index_a < CHUNKSIZE; index_a++)
        {
            const r_uint operand_a = (r_uint)a.mag[BITSIZE - 1 - index_a];
            const r_uint product = operand_a * operand_b;
            const b_uint first_half_product = first_half(product);
            const b_uint last_half_product = last_half(product);

            const size_t index_r_1 = BITSIZE - (index_b + index_a) - 1;
            const size_t index_r_0 = index_r_1 - 1;

            result_mag[index_r_1] += first_half_product;
            result_mag[index_r_0] += last_half_product;
            carry(result_mag, CHUNKSIZE_DOUBLE);
        }
    }

    // for the first four CHUNKSIZE elements, if non-zero, we have an overflow
    // note: result_mag is double the size of CHUNKSIZE
    for (size_t i = 0; i < CHUNKSIZE; i++)
    {
        if (result_mag[i] != 0)
        {
            return 1;
        }
    }

    result.sign = a.sign ^ b.sign; // XOR for sign
    result.prec = a.prec + b.prec;

    // Copy the result into the b_dec structure
    for (size_t i = 0; i < CHUNKSIZE; i++)
    {
        result.mag[i] = result_mag[i + CHUNKSIZE];
    }

    r->sign = result.sign;
    r->prec = result.prec;
    for (size_t i = 0; i < CHUNKSIZE; i++)
    {
        r->mag[i] = result.mag[i];
    }

    return 0;
}

int mul_10(const b_dec *num, b_dec *result)
{
    if (is_zero(*num))
    {
        zero(result);
        return 0;
    }

    b_dec temp;
    static const b_dec _10 = {.mag = {10}, .sign = 0, .prec = 0};
    copy(*num, &temp);

    return mul(temp, _10, result);
}