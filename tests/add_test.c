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

void test_add_b_uint_add(void)
{
    const b_uint a[ARRAYSIZE] = {0, 12};
    const b_uint b[ARRAYSIZE] = {0, 34};

    b_uint result[ARRAYSIZE];
    const int overflow = add_b_uint_arr(a, b, result, ARRAYSIZE);
    TEST_ASSERT_EQUAL(0, overflow);
    TEST_ASSERT_EQUAL(0, result[0]);
    TEST_ASSERT_EQUAL(12 + 34, result[1]);
}

void test_add_b_uint_add_zero(void)
{
    const b_uint a[ARRAYSIZE] = {0, 12};
    const b_uint b[ARRAYSIZE] = {0, 0};

    b_uint result[ARRAYSIZE];
    const int overflow = add_b_uint_arr(a, b, result, ARRAYSIZE);
    TEST_ASSERT_EQUAL(0, overflow);
    TEST_ASSERT_EQUAL(0, result[0]);
    TEST_ASSERT_EQUAL(12, result[1]);
}

void test_add_b_uint_carry(void)
{
    const b_uint a[ARRAYSIZE] = {0, ALL_ONES};
    const b_uint b[ARRAYSIZE] = {0, 1};

    b_uint result[ARRAYSIZE];
    const int overflow = add_b_uint_arr(a, b, result, ARRAYSIZE);
    TEST_ASSERT_EQUAL(0, overflow);
    TEST_ASSERT_EQUAL(1, result[0]);
    TEST_ASSERT_EQUAL(0, result[1]);
}

void test_add_b_uint_carry_add(void)
{
    const b_uint a[ARRAYSIZE] = {3, ALL_ONES};
    const b_uint b[ARRAYSIZE] = {0, 3};

    b_uint result[ARRAYSIZE];
    const int overflow = add_b_uint_arr(a, b, result, ARRAYSIZE);
    TEST_ASSERT_EQUAL(0, overflow);
    TEST_ASSERT_EQUAL(4, result[0]); // 3 + 1 (carry) = 4
    TEST_ASSERT_EQUAL(2, result[1]); // ALL_ONES + 3 = 2 with carry
}

void test_add_b_uint_overflow(void)
{
    const b_uint a[ARRAYSIZE] = {ALL_ONES, ALL_ONES};
    const b_uint b[ARRAYSIZE] = {0, 1};

    b_uint result[ARRAYSIZE];
    const int overflow = add_b_uint_arr(a, b, result, ARRAYSIZE);
    TEST_ASSERT_EQUAL(1, overflow);
    TEST_ASSERT_EQUAL(0, result[0]);
    TEST_ASSERT_EQUAL(0, result[1]);
}

void test_add_b_uint_add_double(void)
{
    const b_uint a[ARRAYSIZE_DOUBLE] = {0, 0, 0b1000'0000'0000'0000'0000'0000'0000'0000, 0b1000'0000'0000'0000'0000'0000'0110'1110};
    const b_uint b[ARRAYSIZE_DOUBLE] = {0, 0, 0b0100'0000'0000'0000'0000'0000'0000'0000, 0b0100'0000'0000'0000'0000'0000'0011'0111};

    b_uint result[ARRAYSIZE_DOUBLE];
    const int overflow = add_b_uint_arr(a, b, result, ARRAYSIZE_DOUBLE);
    TEST_ASSERT_EQUAL(0, overflow);
    TEST_ASSERT_EQUAL(0, result[0]);
    TEST_ASSERT_EQUAL(0, result[1]);
    TEST_ASSERT_EQUAL(0b1100'0000'0000'0000'0000'0000'0000'0000, result[2]);
    TEST_ASSERT_EQUAL(0b1100'0000'0000'0000'0000'0000'1010'0101, result[3]);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_add_b_uint_add);
    RUN_TEST(test_add_b_uint_add_zero);
    RUN_TEST(test_add_b_uint_carry);
    RUN_TEST(test_add_b_uint_carry_add);
    RUN_TEST(test_add_b_uint_overflow);
    RUN_TEST(test_add_b_uint_add_double);
    return UNITY_END();
}