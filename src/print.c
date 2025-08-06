#include "b_dec_print.h"
#include "b_dec_str.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)         \
    ((byte) & 0x80 ? '1' : '0'),     \
        ((byte) & 0x40 ? '1' : '0'), \
        ((byte) & 0x20 ? '1' : '0'), \
        ((byte) & 0x10 ? '1' : '0'), \
        ((byte) & 0x08 ? '1' : '0'), \
        ((byte) & 0x04 ? '1' : '0'), \
        ((byte) & 0x02 ? '1' : '0'), \
        ((byte) & 0x01 ? '1' : '0')

void print_b_uint_binary(const b_uint m)
{
    printf(BYTE_TO_BINARY_PATTERN
           " " BYTE_TO_BINARY_PATTERN
           " " BYTE_TO_BINARY_PATTERN
           " " BYTE_TO_BINARY_PATTERN,
           BYTE_TO_BINARY(m >> 24),
           BYTE_TO_BINARY(m >> 16),
           BYTE_TO_BINARY(m >> 8),
           BYTE_TO_BINARY(m));
}

#define STR_SIZE (BITSIZE * CHUNKSIZE)
void print_b_dec(const b_dec m)
{
    static char str[STR_SIZE];
    b_dec_to_str(m, str, STR_SIZE);
    printf("%s", str);
}