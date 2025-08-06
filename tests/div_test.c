#include "unity.h"
#include "b_dec.h"

#define ARRAYSIZE 2
#define ARRAYSIZE_DOUBLE 4

void setUp(void)
{
}

void tearDown(void)
{
}

void test_div_b_uint_arr_simple_no_remainder(void)
{
    b_uint num[ARRAYSIZE] = {0, 220};
    b_uint result[ARRAYSIZE];
    uint8_t remainder = 0;

    // Test division by 10
    int error = div_10_b_uint_arr(num, result, &remainder, ARRAYSIZE);
    TEST_ASSERT_EQUAL(0, error);

    // Check the result
    TEST_ASSERT_EQUAL(0, result[0]);  // First element should be 0
    TEST_ASSERT_EQUAL(22, result[1]); // Second element should be 22

    // Check the remainder
    TEST_ASSERT_EQUAL_UINT8(0, remainder);
}

void test_div_b_uint_arr_simple_with_remainder(void)
{
    b_uint num[ARRAYSIZE] = {0, 227};
    b_uint result[ARRAYSIZE];
    uint8_t remainder = 0;

    // Test division by 10
    int error = div_10_b_uint_arr(num, result, &remainder, ARRAYSIZE);
    TEST_ASSERT_EQUAL(0, error);

    // Check the result
    TEST_ASSERT_EQUAL(0, result[0]);  // First element should be 0
    TEST_ASSERT_EQUAL(22, result[1]); // Second element should be 22

    // Check the remainder
    TEST_ASSERT_EQUAL_UINT8(7, remainder);
}

void test_div_b_uint_arr_full_with_remainder(void)
{
    b_uint num[ARRAYSIZE] = {1, 227};
    b_uint result[ARRAYSIZE];
    uint8_t remainder = 0;

    // r_uint is double size of b_uint, so we test it using direct division
    r_uint num_full = ((r_uint)num[0] << BITSIZE) + (r_uint)num[1];
    r_uint result_full = num_full / 10;
    r_uint expected_remainder = num_full % 10;
    b_uint first_half_division = first_half(result_full);
    b_uint last_half_division = last_half(result_full);

    // Test division by 10
    int error = div_10_b_uint_arr(num, result, &remainder, ARRAYSIZE);
    TEST_ASSERT_EQUAL(0, error);

    // Check the result
    TEST_ASSERT_EQUAL(first_half_division, result[0]);
    TEST_ASSERT_EQUAL(last_half_division, result[1]);

    // Check the remainder
    TEST_ASSERT_EQUAL_UINT8((uint8_t)expected_remainder, remainder);
}

void test_div_10_uint128(void)
{
#ifdef UINT128_NOT_SUPPORTED
    TEST_IGNORE_MESSAGE("UINT128 is not supported on this platform.");
    return;
#endif

    uint128_t num2 = (((uint128_t)1) << (BITSIZE * 2)) + (((uint128_t)1) << (BITSIZE * 1)) + 220; // 0x1_0000_0001_0000_0000 + 220
    uint128_t quotient = 0;
    uint128_t remainder = 0;

    // Test division by 10
    int error = div_10_uint128(num2, &quotient, &remainder);
    TEST_ASSERT_EQUAL(0, error);
}

void test_div_b_uint_arr_double_with_remainder(void)
{
    // according to python0x1_0000_0001_0000_0000 + 220, it is 18446744078004519132
    // remainder is 2
    b_uint num[ARRAYSIZE_DOUBLE] = {0, 1, 1, 220};
    b_uint result[ARRAYSIZE_DOUBLE];
    uint8_t remainder = 0;

    // Test division by 10
    int error = div_10_b_uint_arr(num, result, &remainder, ARRAYSIZE_DOUBLE);
    TEST_ASSERT_EQUAL(0, error);

    // according to python once more, the result in hex is 1999_9999_B333_3349

    // Check the result
    TEST_ASSERT_EQUAL(0, result[0]);
    TEST_ASSERT_EQUAL(0, result[1]);
    TEST_ASSERT_EQUAL((b_uint)0x19999999, result[2]);
    TEST_ASSERT_EQUAL((b_uint)0xB3333349, result[3]);

    // Check the remainder
    TEST_ASSERT_EQUAL_UINT8((uint8_t)2, remainder);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_div_b_uint_arr_simple_no_remainder);
    RUN_TEST(test_div_b_uint_arr_simple_with_remainder);
    RUN_TEST(test_div_b_uint_arr_full_with_remainder);
    RUN_TEST(test_div_10_uint128);
    RUN_TEST(test_div_b_uint_arr_double_with_remainder);
    return UNITY_END();
}