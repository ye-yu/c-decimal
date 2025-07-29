#include "unity.h"
#include "b_dec.h"

void setUp(void)
{
    printf("Setting up tests...\n");
}

void tearDown(void)
{
    printf("Tearing down tests...\n");
}

int main()
{
    UNITY_BEGIN();
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
    return UNITY_END();
}