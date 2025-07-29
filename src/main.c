#include "b_dec.h"

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

    r->sign = a.sign ^ b.sign; // XOR for sign
    r->prec = a.prec + b.prec;

    // Copy the result into the b_dec structure
    for (size_t i = 0; i < CHUNKSIZE; i++)
    {
    }

    return 0;
}


b_uint rand_b_uint()
{
    b_uint r = 0;
    for (size_t i = 0; i < CHUNKSIZE; i++)
    {
        r |= ((b_uint)rand() << (i * BITSIZE));
    }
    normalize(&r);
    return r;
}

b_uint make_zero_at_bit(const b_uint a, const size_t bit)
{
    if (bit >= BITSIZE)
    {
        return a; // Invalid bit position, return original value
    }
    b_uint mask = ~(ONE << bit);
    return a & mask;
}
