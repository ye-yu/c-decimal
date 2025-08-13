#include "b_dec_str.h"
#include "b_dec_div_10.h"
#include "b_dec_copy.h"
#include "b_dec_compare.h"
#include "b_dec_split.h"
#include "b_dec_shift.h"
#include "b_dec_mul.h"
#include "b_dec_zero.h"
#include <string.h>

int bring_forward(char *str, const size_t size)
{
    for (size_t i = 0, swap_at = 0; i < size - 1; i++)
    {
        if (str[i] == '\0')
        {
            if (swap_at != 0)
            {
                break;
            }
            continue;
        }

        if (swap_at == i)
        {
            break;
        }

        str[swap_at] = str[i];
        str[i] = '\0';
        swap_at++;
    }

    return 0;
}

int put_from_last(char *str, const size_t size, const size_t at, const char c)
{
    if (size == 0)
    {
        return 1; // Error: size is zero
    }

    if (at >= size - 1)
    {
        return 1; // Error: at is out of bounds
    }

    str[size - 2 - at] = c;
    return 0;
}

const char char_map[] = "0123456789";

/**
 * Converts a b_uint to a string representation.
 *
 * @param dec The b_uint to convert.
 * @param str The output string buffer.
 * @param size The size of the output buffer.
 * @return 0 on success, 1 if the buffer is too small.
 */
int b_uint_to_str(const b_uint dec, char *str, const size_t size)
{
    b_uint quotient = dec;
    b_uint remainder = 0;

    for (size_t i = 0; i < size; i++)
    {
        str[i] = '\0'; // Initialize the string buffer
    }

    for (size_t i = 0; quotient > 0 && i < size - 1; i++)
    {
        b_uint q = quotient / 10;
        b_uint r = quotient - (q * 10);

        quotient = q;
        remainder = r;
        put_from_last(str, size, i, char_map[remainder]);
    }

    bring_forward(str, size);
    return 0;
}

/**
 * Converts a b_uint to a string representation.
 *
 * @param dec The b_uint to convert.
 * @param str The output string buffer.
 * @param size The size of the output buffer.
 * @return 0 on success, 1 if the buffer is too small.
 */
int b_dec_to_str(const b_dec dec, char *str, const size_t size)
{
    if (dec.prec > MAX_PREC)
    {
        strcpy_s(str, size, MAX_PREC_MSG);
        return 1;
    }
#define STR_SIZE (BITSIZE * CHUNKSIZE)
    char integral_buffer[STR_SIZE] = ""; // Initialize the string buffer
    for (size_t i = 0; i < STR_SIZE; i++)
    {
        integral_buffer[i] = '\0';
    }

    b_uint integral[CHUNKSIZE];
    b_uint fractional[CHUNKSIZE + 1];

    if (dec.prec < BITSIZE * CHUNKSIZE)
    {
        int split_error = split_arr_at(dec.mag, dec.prec, integral, fractional, CHUNKSIZE);
        if (split_error)
        {
            return 1;
        }
    }
    else
    {
        copy_b_uint_arr(dec.mag, fractional, CHUNKSIZE);
    }

    // move fractional one to the right
    for (size_t i = 0; i < CHUNKSIZE; i++)
    {
        size_t last = CHUNKSIZE - i - 1;
        fractional[last + 1] = fractional[last];
    }
    fractional[0] = 0;

    size_t str_i = 0;
#define APPEND_TO_END(c)       \
    do                         \
    {                          \
        if (str_i >= size - 2) \
        {                      \
            break;             \
        }                      \
        str[str_i] = c;        \
        str[str_i + 1] = '\0'; \
        str_i++;               \
    } while (0)

#define RETURN_STR_CHECK                  \
    do                                    \
    {                                     \
        return str_i >= size - 2 ? 1 : 0; \
    } while (0)

    if (dec.sign)
    {
        APPEND_TO_END('-');
    }

    // handle integral part
    size_t integral_str_i = 0;
    while (!is_zero_b_uint_arr(integral, CHUNKSIZE))
    {
        uint8_t remainder = 0;
        div_10_b_uint_arr(integral, integral, &remainder, CHUNKSIZE);
        if (integral_str_i >= STR_SIZE - 2)
            continue;
        size_t integral_str_i_last = STR_SIZE - integral_str_i - 2;
        integral_buffer[integral_str_i_last] = char_map[remainder];
        integral_str_i++;
    }
    bring_forward(integral_buffer, STR_SIZE);
    for (size_t i = 0; i < STR_SIZE - 2 && integral_buffer[i] != '\0'; i++)
    {
        APPEND_TO_END(integral_buffer[i]);
    }

    if (is_zero_b_uint_arr(fractional, CHUNKSIZE + 1))
    {
        RETURN_STR_CHECK;
    }

    APPEND_TO_END('.');

    // handle fractional part
    b_uint prec = dec.prec;
    // shift left the fractional to head of index 1
    size_t bits_to_shift = BITSIZE * CHUNKSIZE - prec;
    shift_arr_left(fractional, fractional, CHUNKSIZE + 1, bits_to_shift);

    // buffers

    b_uint *operand1 = (b_uint *)malloc((CHUNKSIZE + 1) * sizeof(b_uint));
    b_uint *operand2 = (b_uint *)malloc((CHUNKSIZE + 1) * sizeof(b_uint));
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

    zero_b_uint_arr(operand1, CHUNKSIZE + 1);
    zero_b_uint_arr(operand2, CHUNKSIZE + 1);

#define RETURN_AFTER_FREE(v) \
    do                       \
    { /* free here */        \
        free(operand1);      \
        free(operand2);      \
        return v;            \
    } while (0);

    while (!is_zero_b_uint_arr(fractional, CHUNKSIZE + 1))
    {
        int overflow = mul_10_b_uint_arr_no_malloc(fractional, fractional, operand1, operand2, CHUNKSIZE + 1);
        if (overflow)
        {
            return 1;
        }
        uint8_t c = (uint8_t)fractional[0];
        fractional[0] = 0;
        if (c < 10)
        {
            APPEND_TO_END(char_map[c]);
        }
        else
        {
            APPEND_TO_END('?');
        }
    }

    RETURN_STR_CHECK;
}
