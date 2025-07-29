#include "b_dec_str.h"
#include "b_dec_div_10.h"

/**
 * Converts a b_uint to a string representation.
 *
 * @param dec The b_uint to convert.
 * @param str The output string buffer.
 * @param size The size of the output buffer.
 * @return 0 on success, 1 if the buffer is too small.
 */
int b_uint_to_str(const b_uint dec, char *str, size_t size)
{
    uint64_t quotient = (uint64_t)dec;
    uint64_t remainder = 0;
    const char char_map[] = "0123456789";

    for (size_t i = 0; i < size; i++)
    {
        str[i] = '\0'; // Initialize the string buffer
    }

    for (size_t i = 0; quotient > 0 && i < size - 1; i++)
    {
        const int result = div_10(quotient, &quotient, &remainder);
        if (result)
        {
            // something is wrong, return error
            printf("Error in div_10: quotient %llu, remainder %llu\n", quotient, remainder);
            return 1;
        }

        str[size - 2 - i] = char_map[remainder];
    }

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

        str[swap_at] = str[i];
        str[i] = '\0';
        swap_at++;
    }

    return 0;
}
