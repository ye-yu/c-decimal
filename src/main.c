#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define CHUNKSIZE 4
#define CHUNKSIZE_DOUBLE 8
#define BITSIZE 32
#define b_uint uint32_t
#define r_uint uint64_t
#define ONE ((b_uint)1)
#define ZERO ((b_uint)0)
#define MASK 0x7fffffff
#define MASK_WORD 0xffffffff

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

typedef struct
{
    // sign: 0 for positive, 1 for negative
    uint8_t sign;
    b_uint prec;
    // most significant bit is carry
    b_uint mag[CHUNKSIZE];
} b_dec;

int zero_uint(b_uint *a, const size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        a[i] = 0;
    }
    return 0;
}

int zero(b_dec *a)
{
    a->sign = 0;
    a->prec = 0;
    zero_uint(a->mag, CHUNKSIZE);
    return 0;
}

int overflown(const b_uint comp)
{
    return ((comp >> (BITSIZE - 1)) & (0b1)) != 0;
}

int normalize(b_uint *a)
{
    (*a) = (*a) & MASK;
    return 0;
}

int carry(b_uint *a, const size_t size)
{
    for (size_t i = size - 1; i > 0; i--)
    {
        if (overflown(a[i]))
        {
            normalize(&a[i]);
            a[i - 1] += 1;
        }
    }
    if (overflown(a[0]))
    {
        return 1; // Indicate overflow
    }
    return 0;
}

b_uint last_half(const r_uint comp)
{
    return (b_uint)(comp & MASK_WORD);
}

b_uint first_half(const r_uint comp)
{
    return last_half(comp >> BITSIZE);
}

int split_at(b_uint a, size_t at, b_uint *first_half, b_uint *last_half)
{
    if (at >= BITSIZE)
    {
        return 1; // Invalid position
    }
    *first_half = a >> at;
    *last_half = a & ~((ZERO - 1) << at);
    return 0;
}

int copy(const b_dec src, b_dec *dest)
{
    dest->sign = src.sign;
    dest->prec = src.prec;
    for (int i = 0; i < CHUNKSIZE; i++)
    {
        dest->mag[i] = src.mag[i];
    }
    return 0;
}

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

int remove_trailing_zeroes(b_uint *a, b_uint *prec_ref, const size_t size)
{
    b_uint prec = *prec_ref;
    if (prec <= 0)
    {
        return 0;
    }

    // remove complete zeros
    while (a[size - 1] == 0 && prec > (BITSIZE - 1))
    {
        prec -= (BITSIZE - 1);
        for (size_t i = CHUNKSIZE - 1; i > 0; i--)
        {
            a[i] = a[i - 1];
        }
        a[0] = 0;
    }

    // remove trailing zeroes
    size_t bits_to_shift = 0;
    while (prec > 0 && a[size - 1] != 0 && ((a[size - 1] >> bits_to_shift) & ONE) == 0)
    {
        prec--;
        bits_to_shift++;
    }

    b_uint first_half, last_half;
    for (size_t i = size - 1; i >= 0; i--)
    {
        first_half = 0;
        last_half = 0;
        split_at(a[i], bits_to_shift, &first_half, &last_half);
        a[i] >>= bits_to_shift;
        if (i == size - 1)
        {
            continue;
        }

        a[i + 1] |= (last_half << (BITSIZE - bits_to_shift));
        if (i == 0)
        {
            break;
        }
    }

    return 0;
}

// i got it from here lol https://math.stackexchange.com/a/3001889/704406
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

b_uint rand_b_uint()
{
    b_uint r = 0;
    const size_t iterations = BITSIZE / (sizeof(int));
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

int main()
{
    srand(1);
    b_uint array[2] = {0, 0};
    for (size_t i = 0; i < 100; i++)
    {
        printf("iteration %02d:                   ", (int)i);
        b_uint a = rand_b_uint();
        b_uint r = rand_b_uint();
        size_t z = ((size_t)rand()) % BITSIZE;
        for (size_t j = 0; j < z; j++)
        {
            r = make_zero_at_bit(r, j);
        }
        print_b_uint_binary(a);
        printf(" ");
        print_b_uint_binary(r);
        printf("\n");

        uint32_t prec = (uint32_t)z;
        array[0] = a;
        array[1] = r;

        remove_trailing_zeroes(array, &prec, 2);

        printf("After removing trailing zeroes: ");
        print_b_uint_binary(array[0]);
        printf(" ");
        print_b_uint_binary(array[1]);
        printf("\n");
        printf("Number of trailing zeroes:      %d\n", (int)z);
        printf("\n");
    }
}