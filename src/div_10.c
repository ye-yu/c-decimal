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

int div_10_b_dec(const b_dec *num, b_dec *result)
{
    if (is_zero_b_dec(*num))
    {
        zero(result);
        return 0;
    }

    b_uint mag[CHUNKSIZE];
    copy_b_uint_arr(num->mag, mag, CHUNKSIZE);

    b_uint shift_buffer_left[CHUNKSIZE];
    b_uint shift_buffer_right[CHUNKSIZE];

#define SUM_LEFT_RIGHT_AND_MAYBE_PANIC                                                                   \
    do                                                                                                   \
    {                                                                                                    \
        int carry = add_b_uint_arr(shift_buffer_left, shift_buffer_right, shift_buffer_left, CHUNKSIZE); \
        if (carry)                                                                                       \
        {                                                                                                \
            return 1; /* should not happen? but if it does, i don't know what to do  */                  \
        }                                                                                                \
    } while (0);

    // prepare for (num >> 1) and (num >> 2)
    shift_arr_right(mag, shift_buffer_left, CHUNKSIZE, 1);
    shift_arr_right(mag, shift_buffer_right, CHUNKSIZE, 2);

    // put sum to left buffer
    SUM_LEFT_RIGHT_AND_MAYBE_PANIC

    // following 4, 8, 16, 32, etc...
    // if bitsize is 64, target is 32
    // if bitsize is 128, target is 64
    // if bitsize is 256, target is 128, etc...
    const size_t target_bit_to_shift = (BITSIZE * CHUNKSIZE) / 2;
    for (size_t i = 0b100; i <= target_bit_to_shift; i <<= 1)
    {
        // prepare for (num >> i)
        copy_b_uint_arr(shift_buffer_left, shift_buffer_right, CHUNKSIZE);
        shift_arr_right(shift_buffer_left, shift_buffer_right, CHUNKSIZE, i);
        // put sum to left buffer
        SUM_LEFT_RIGHT_AND_MAYBE_PANIC
    }

    // now prepare for final (num >> 3)
    // prepare for (num >> i)
    copy_b_uint_arr(shift_buffer_left, shift_buffer_right, CHUNKSIZE);
    shift_arr_right(shift_buffer_left, shift_buffer_right, CHUNKSIZE, 3);
    // put sum to left buffer
    SUM_LEFT_RIGHT_AND_MAYBE_PANIC

    // mul_10_b_dec(shift_buffer_left, shift_buffer_left);
    return 1; // not implemented yet
}