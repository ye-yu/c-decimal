#include "b_dec_div_10.h"
#include "b_dec_shift.h"
#include "b_dec_mul.h"
#include "b_dec_add.h"
#include "b_dec_copy.h"
#include "b_dec_subtract.h"
#include "b_dec_compare.h"
#include "b_dec_zero.h"

int div_10(const uint64_t num, uint64_t *quotient, uint64_t *remainder)
{
    uint64_t q, r;
    q = (num >> 1) + (num >> 2);
    q = q + (q >> 4);
    q = q + (q >> 8);
    q = q + (q >> 16);
    q = q + (q >> 32);
    q = q >> 3;
    r = num - q * 10;
    if (r > 9)
    {
        q++;
        r -= 10;
    }

    *quotient = q;
    *remainder = r;
    return 0;
}

int div_10_uint128(const uint128_t num, uint128_t *quotient, uint128_t *remainder)
{
#ifdef UINT128_NOT_SUPPORTED
    return 1; // error: UINT128 is not supported on this platform
#endif
    uint128_t q, r;
    q = (num >> 1) + (num >> 2);
    q = q + (q >> 4);
    q = q + (q >> 8);
    q = q + (q >> 16);
    q = q + (q >> 32);
    q = q + (q >> 64); // correct?
    q = q >> 3;
    r = num - q * 10;
    if (r > 9)
    {
        q++;
        r -= 10;
    }

    *quotient = q;
    *remainder = r;
    return 0;
}

int div_10_b_uint_arr(const b_uint *num, b_uint *result, uint8_t *remainder, const size_t size)
{

    if (size == 0)
    {
        return 1; // error: size cannot be zero
    }

    if (is_zero_b_uint_arr(num, size))
    {
        zero_b_uint_arr(result, size);
        return 0;
    }

    b_uint *shift_buffer_left = (b_uint *)malloc(size * sizeof(b_uint));
    b_uint *shift_buffer_right = (b_uint *)malloc(size * sizeof(b_uint));
    b_uint *b_uint_one = (b_uint *)malloc(size * sizeof(b_uint));
#define MAYBE_FREE(v) \
    do                \
    {                 \
        if (v)        \
        {             \
            free(v);  \
        }             \
    } while (0);

    if (!shift_buffer_left || !shift_buffer_right || !b_uint_one)
    {
        MAYBE_FREE(shift_buffer_left);
        MAYBE_FREE(shift_buffer_right);
        MAYBE_FREE(b_uint_one);
        return 1; // error: memory allocation failed
    }

    zero_b_uint_arr(shift_buffer_left, size);
    zero_b_uint_arr(shift_buffer_right, size);
    zero_b_uint_arr(b_uint_one, size);
    b_uint_one[size - 1] = 1;

#define RETURN_AFTER_FREE(v)      \
    do                            \
    { /* free here */             \
        free(shift_buffer_left);  \
        free(shift_buffer_right); \
        free(b_uint_one);         \
        return v;                 \
    } while (0);

#define SUM_LEFT_RIGHT_AND_MAYBE_PANIC                                                              \
    do                                                                                              \
    {                                                                                               \
        int carry = add_b_uint_arr(shift_buffer_left, shift_buffer_right, shift_buffer_left, size); \
        if (carry)                                                                                  \
        {                                                                                           \
            RETURN_AFTER_FREE(1); /* should not happen? but if it does, i don't know what to do  */ \
        }                                                                                           \
    } while (0);

    // prepare for (num >> 1) and (num >> 2)
    shift_arr_right(num, shift_buffer_left, size, 1);
    shift_arr_right(num, shift_buffer_right, size, 2);

    // put sum to left buffer
    SUM_LEFT_RIGHT_AND_MAYBE_PANIC

    // following 4, 8, 16, 32, etc...
    // if bitsize is 64, target is 32
    // if bitsize is 128, target is 64
    // if bitsize is 256, target is 128, etc...
    const size_t target_bit_to_shift = (BITSIZE * size) / 2;
    for (size_t i = 0b100; i <= target_bit_to_shift; i <<= 1)
    {
        // prepare for (num >> i)
        copy_b_uint_arr(shift_buffer_left, shift_buffer_right, size);
        shift_arr_right(shift_buffer_right, shift_buffer_right, size, i);
        // put sum to left buffer
        SUM_LEFT_RIGHT_AND_MAYBE_PANIC
    }

    // now prepare for final (num >> 3)
    shift_arr_right(shift_buffer_left, shift_buffer_left, size, 3);
    // q stayed at shift_buffer_left, must not change!

    int overflow = mul_10_b_uint_arr(shift_buffer_left, shift_buffer_right, size);
    if (overflow)
    {
        return 1; // should not happen? but if it does, i don't know what to do
    }

    // put remainder to right buffer
    overflow = subtract_b_uint_arr(num, shift_buffer_right, shift_buffer_right, size);
    if (overflow)
    {
        return 1; // should not happen? but if it does, i don't know what to do
    }
    uint8_t rem = (uint8_t)shift_buffer_right[size - 1];
    if (rem > 9)
    {

        // we need to add 1 to the result
        int carry = add_b_uint_arr(shift_buffer_left, b_uint_one, shift_buffer_left, size);
        if (carry)
        {
            return 1; // should not happen? but if it does, i don't know what to do
        }
        rem -= 10;
    }

    copy_b_uint_arr(shift_buffer_left, result, size);
    *remainder = rem;
    RETURN_AFTER_FREE(0);
}

int div_10_b_dec(const b_dec num, b_dec *result, uint8_t *remainder)
{
    if (is_zero_b_dec(num))
    {
        zero(result);
        return 0;
    }

    b_uint r[CHUNKSIZE];
    const int error = div_10_b_uint_arr(num.mag, r, remainder, CHUNKSIZE);
    if (error)
    {
        return error; // error in division
    }

    result->sign = num.sign;
    result->prec = num.prec + 1;
    copy_b_uint_arr(r, result->mag, CHUNKSIZE);
    return 0;
}