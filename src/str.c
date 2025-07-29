#include "b_dec_str.h"
#include "b_dec_div_10.h"

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
    uint64_t quotient = (uint64_t)dec;
    uint64_t remainder = 0;

    for (size_t i = 0; i < size; i++)
    {
        str[i] = '\0'; // Initialize the string buffer
    }

    for (size_t i = 0; quotient > 0 && i < size - 1; i++)
    {
        div_10(quotient, &quotient, &remainder);
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

#define CHECK_BUFFER_OR_RETURN \
    do                         \
    {                          \
        if (str_i >= size - 1) \
        {                      \
            return 1;          \
        }                      \
    } while (0)

    for (size_t i = 0; i < size; i++)
    {
        str[i] = '\0'; // Initialize the string buffer
    }

    size_t str_i = 0;
    b_uint prec = dec.prec;
    int precision_reached = dec.prec == 0 ? 1 : 0;

    for (int i = CHUNKSIZE - 1; i >= 0; i--)
    {
        uint64_t quotient = (uint64_t)(dec.mag[i]);
        uint64_t remainder = 0;

        for (; quotient > 0 && str_i < size - 1; str_i++)
        {
            if (prec == 0 && !precision_reached)
            {
                precision_reached = 1;
                put_from_last(str, size, str_i, '.');
                str_i++;
            }
            else if (prec > 0)
            {
                prec--;
            }

            div_10(quotient, &quotient, &remainder);
            put_from_last(str, size, str_i, char_map[remainder]);
        }

        if (quotient > 0)
        {
            return 1; // Error: buffer too small or overflow
        }
    }

    // If precision is not reached, fill the rest with zeros
    while (prec > 0 && str_i < size - 1)
    {
        put_from_last(str, size, str_i, '0');
        str_i++;
        prec--;
    }

    if (!precision_reached)
    {

        CHECK_BUFFER_OR_RETURN;
        put_from_last(str, size, str_i, '.');
        str_i++;

        CHECK_BUFFER_OR_RETURN;
        put_from_last(str, size, str_i, '0');
        str_i++;
    }

    if (dec.sign)
    {
        CHECK_BUFFER_OR_RETURN;
        put_from_last(str, size, str_i, '-');
    }

    bring_forward(str, size);
    return 0;
}
