#include "unity.h"
#include "b_dec.h"

#define ARRAYSIZE 2

void setUp(void)
{
}

void tearDown(void)
{
}

void test_subtract_b_uint_subtract(void)
{
    const b_uint a[ARRAYSIZE] = {0, 34};
    const b_uint b[ARRAYSIZE] = {0, 12};

    b_uint result[ARRAYSIZE];
    const int overflow = subtract_b_uint_arr(a, b, result, ARRAYSIZE);
    TEST_ASSERT_EQUAL(0, overflow);
    TEST_ASSERT_EQUAL(0, result[0]);
    TEST_ASSERT_EQUAL(34 - 12, result[1]);
}

void test_subtract_b_uint_subtract_zero(void)
{
    const b_uint a[ARRAYSIZE] = {0, 12};
    const b_uint b[ARRAYSIZE] = {0, 0};

    b_uint result[ARRAYSIZE];
    const int overflow = subtract_b_uint_arr(a, b, result, ARRAYSIZE);
    TEST_ASSERT_EQUAL(0, overflow);
    TEST_ASSERT_EQUAL(0, result[0]);
    TEST_ASSERT_EQUAL(12, result[1]);
}

void test_subtract_b_uint_borrow(void)
{
    const b_uint a[ARRAYSIZE] = {1, 0};
    const b_uint b[ARRAYSIZE] = {0, 1};

    b_uint result[ARRAYSIZE];
    const int overflow = subtract_b_uint_arr(a, b, result, ARRAYSIZE);
    TEST_ASSERT_EQUAL(0, overflow);
    TEST_ASSERT_EQUAL(0, result[0]);
    TEST_ASSERT_EQUAL(ALL_ONES, result[1]);
}

void test_subtract_b_uint_borrow_subtract(void)
{
    const b_uint a[ARRAYSIZE] = {ALL_ONES, 3};
    const b_uint b[ARRAYSIZE] = {0, 10};

    b_uint result[ARRAYSIZE];
    const int overflow = subtract_b_uint_arr(a, b, result, ARRAYSIZE);
    TEST_ASSERT_EQUAL(0, overflow);
    TEST_ASSERT_EQUAL(ALL_ONES - 1, result[0]);     // ALL_ONES borrow 1
    TEST_ASSERT_EQUAL(~((b_uint)0b110), result[1]); // 3 - 10 = -7, which is represented as ALL_ONES - 6
}

void test_subtract_b_uint_underflow(void)
{
    const b_uint a[ARRAYSIZE] = {0, 0};
    const b_uint b[ARRAYSIZE] = {0, 1};

    b_uint result[ARRAYSIZE];
    const int overflow = subtract_b_uint_arr(a, b, result, ARRAYSIZE);
    TEST_ASSERT_EQUAL(1, overflow);
    TEST_ASSERT_EQUAL(ALL_ONES, result[0]);
    TEST_ASSERT_EQUAL(ALL_ONES, result[1]);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_subtract_b_uint_subtract);
    RUN_TEST(test_subtract_b_uint_subtract_zero);
    RUN_TEST(test_subtract_b_uint_borrow);
    RUN_TEST(test_subtract_b_uint_borrow_subtract);
    RUN_TEST(test_subtract_b_uint_underflow);
    return UNITY_END();
}