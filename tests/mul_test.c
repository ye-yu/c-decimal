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

void test_mul_b_uint(void)
{
    const b_uint a = 12;
    const b_uint b = 34;
    b_uint result;
    const int overflow = mul_b_uint(a, b, &result);

    TEST_ASSERT_EQUAL(0, overflow);
    TEST_ASSERT_EQUAL(a * b, result);
}

void test_mul_b_uint_zero(void)
{
    const b_uint a = 0;
    const b_uint b = 34;
    b_uint result;
    const int overflow = mul_b_uint(a, b, &result);

    TEST_ASSERT_EQUAL(0, overflow);
    TEST_ASSERT_EQUAL(0, result);

    const int overflow_2 = mul_b_uint(b, a, &result);

    TEST_ASSERT_EQUAL(0, overflow_2);
    TEST_ASSERT_EQUAL(0, result);
}

void test_mul_b_uint_one(void)
{
    const b_uint a = 1;
    const b_uint b = 34;
    b_uint result;
    const int overflow = mul_b_uint(a, b, &result);

    TEST_ASSERT_EQUAL(0, overflow);
    TEST_ASSERT_EQUAL(34, result);

    const int overflow_2 = mul_b_uint(b, a, &result);

    TEST_ASSERT_EQUAL(0, overflow_2);
    TEST_ASSERT_EQUAL(34, result);
}

void test_mul_b_uint_overflow(void)
{
    const b_uint a = ALL_ONES;
    const b_uint b = 2;
    b_uint result;
    const int overflow = mul_b_uint(a, b, &result);

    TEST_ASSERT_EQUAL(1, overflow);
    TEST_ASSERT_EQUAL(ALL_ONES << 1, result);

    const int overflow_2 = mul_b_uint(b, a, &result);

    TEST_ASSERT_EQUAL(1, overflow_2);
    TEST_ASSERT_EQUAL(ALL_ONES << 1, result);
}

// array tests

void test_mul_b_uint_arr_zero(void)
{
    const b_uint a[ARRAYSIZE] = {0, 12};
    const b_uint b[ARRAYSIZE] = {0, 0};
    b_uint result[ARRAYSIZE];
    zero_b_uint_arr(result, ARRAYSIZE);
    const int overflow = mul_b_uint_arr(a, b, result, ARRAYSIZE);

    TEST_ASSERT_EQUAL(0, overflow);
    TEST_ASSERT_EQUAL(0, result[0]);
    TEST_ASSERT_EQUAL(0, result[1]);
}

void test_mul_b_uint_arr_one(void)
{
    const b_uint a[ARRAYSIZE] = {0, 12};
    const b_uint b[ARRAYSIZE] = {0, 1};
    b_uint result[ARRAYSIZE];
    zero_b_uint_arr(result, ARRAYSIZE);
    const int overflow = mul_b_uint_arr(a, b, result, ARRAYSIZE);

    TEST_ASSERT_EQUAL(0, overflow);
    TEST_ASSERT_EQUAL(0, result[0]);
    TEST_ASSERT_EQUAL(12, result[1]);
}

void test_mul_b_uint_arr_only_last(void)
{
    const b_uint a[ARRAYSIZE] = {0, 12};
    const b_uint b[ARRAYSIZE] = {0, 34};
    b_uint result[ARRAYSIZE];
    zero_b_uint_arr(result, ARRAYSIZE);
    const int overflow = mul_b_uint_arr(a, b, result, ARRAYSIZE);

    TEST_ASSERT_EQUAL(0, overflow);
    TEST_ASSERT_EQUAL(0, result[0]);
    TEST_ASSERT_EQUAL(a[1] * b[1], result[1]);
}

void test_mul_b_uint_arr_both_last_first(void)
{
    const b_uint a[ARRAYSIZE] = {2, ALL_ONES - 2};
    const b_uint b[ARRAYSIZE] = {0, 34};
    b_uint result[ARRAYSIZE];
    zero_b_uint_arr(result, ARRAYSIZE);
    const int overflow = mul_b_uint_arr(a, b, result, ARRAYSIZE);

    TEST_ASSERT_EQUAL(0, overflow);

    // r_uint is double size of b_uint, so we test it using direct multiplication
    r_uint buffer = ((((r_uint)a[0]) << BITSIZE) + ((r_uint)a[1])) * (r_uint)b[1];
    const b_uint expected_first_half = first_half(buffer);
    const b_uint expected_last_half = last_half(buffer);
    TEST_ASSERT_EQUAL(expected_first_half, result[0]);
    TEST_ASSERT_EQUAL(expected_last_half, result[1]);
}

void test_mul_b_uint_arr_overflow(void)
{
    const b_uint a[ARRAYSIZE] = {ALL_ONES, ALL_ONES};
    const b_uint b[ARRAYSIZE] = {0, 2};
    b_uint result[ARRAYSIZE];
    zero_b_uint_arr(result, ARRAYSIZE);
    const int overflow = mul_b_uint_arr(a, b, result, ARRAYSIZE);

    TEST_ASSERT_EQUAL(1, overflow);
    TEST_ASSERT_EQUAL(ALL_ONES, result[0]);
    TEST_ASSERT_EQUAL(ALL_ONES << 1, result[1]);
}

void test_mul_b_uint_arr_overflow_double(void)
{
    const b_uint a[ARRAYSIZE_DOUBLE] = {ALL_ONES, ALL_ONES, ALL_ONES, ALL_ONES};
    const b_uint b[ARRAYSIZE_DOUBLE] = {0, 0, 0, 2};
    b_uint result[ARRAYSIZE_DOUBLE];
    zero_b_uint_arr(result, ARRAYSIZE_DOUBLE);
    const int overflow = mul_b_uint_arr(a, b, result, ARRAYSIZE_DOUBLE);

    TEST_ASSERT_EQUAL(1, overflow);
    TEST_ASSERT_EQUAL(ALL_ONES, result[0]);
    TEST_ASSERT_EQUAL(ALL_ONES, result[1]);
    TEST_ASSERT_EQUAL(ALL_ONES, result[2]);
    TEST_ASSERT_EQUAL(ALL_ONES << 1, result[3]);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_mul_b_uint);
    RUN_TEST(test_mul_b_uint_zero);
    RUN_TEST(test_mul_b_uint_one);
    RUN_TEST(test_mul_b_uint_overflow);
    RUN_TEST(test_mul_b_uint_arr_zero);
    RUN_TEST(test_mul_b_uint_arr_one);
    RUN_TEST(test_mul_b_uint_arr_only_last);
    RUN_TEST(test_mul_b_uint_arr_both_last_first);
    RUN_TEST(test_mul_b_uint_arr_overflow);
    RUN_TEST(test_mul_b_uint_arr_overflow_double);
    return UNITY_END();
}