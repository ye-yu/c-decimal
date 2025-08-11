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

void test_split_at_array_0(void)
{
    b_uint arr[ARRAYSIZE] = {0, 0b1011011};
    b_uint first_half[ARRAYSIZE];
    b_uint last_half[ARRAYSIZE];

    int error = split_arr_at(arr, 0, first_half, last_half, ARRAYSIZE);
    TEST_ASSERT_EQUAL(0, error);

    b_uint first_half_expected[ARRAYSIZE] = {0, 0b1011011};
    b_uint last_half_expected[ARRAYSIZE] = {0, 0};

    TEST_ASSERT_EQUAL_INT_ARRAY(first_half_expected, first_half, ARRAYSIZE);
    TEST_ASSERT_EQUAL_INT_ARRAY(last_half_expected, last_half, ARRAYSIZE);
}

void test_split_at_array_lt_bit_size(void)
{
    b_uint arr[ARRAYSIZE] = {0, 0b1011011};
    b_uint first_half[ARRAYSIZE];
    b_uint last_half[ARRAYSIZE];

    int error = split_arr_at(arr, 3, first_half, last_half, ARRAYSIZE);
    TEST_ASSERT_EQUAL(0, error);

    b_uint first_half_expected[ARRAYSIZE] = {0, 0b1011};
    b_uint last_half_expected[ARRAYSIZE] = {0, 0b11};

    TEST_ASSERT_EQUAL_INT_ARRAY(first_half_expected, first_half, ARRAYSIZE);
    TEST_ASSERT_EQUAL_INT_ARRAY(last_half_expected, last_half, ARRAYSIZE);
}

void test_split_at_array_eq_bit_size(void)
{
    b_uint arr[ARRAYSIZE] = {0b1001, 0b1011011};
    b_uint first_half[ARRAYSIZE];
    b_uint last_half[ARRAYSIZE];

    int error = split_arr_at(arr, BITSIZE, first_half, last_half, ARRAYSIZE);
    TEST_ASSERT_EQUAL(0, error);

    b_uint first_half_expected[ARRAYSIZE] = {0, 0b1001};
    b_uint last_half_expected[ARRAYSIZE] = {0, 0b1011011};

    TEST_ASSERT_EQUAL_INT_ARRAY(first_half_expected, first_half, ARRAYSIZE);
    TEST_ASSERT_EQUAL_INT_ARRAY(last_half_expected, last_half, ARRAYSIZE);
}

void test_split_at_array_gt_bit_size(void)
{
    b_uint arr[ARRAYSIZE] = {0b1001, 0b1011011};
    b_uint first_half[ARRAYSIZE];
    b_uint last_half[ARRAYSIZE];

    int error = split_arr_at(arr, BITSIZE + 1, first_half, last_half, ARRAYSIZE);
    TEST_ASSERT_EQUAL(0, error);

    b_uint first_half_expected[ARRAYSIZE] = {0, 0b100};
    b_uint last_half_expected[ARRAYSIZE] = {0b1, 0b1011011};

    TEST_ASSERT_EQUAL_INT_ARRAY(first_half_expected, first_half, ARRAYSIZE);
    TEST_ASSERT_EQUAL_INT_ARRAY(last_half_expected, last_half, ARRAYSIZE);
}

void test_split_at_array_overflow(void)
{
    b_uint arr[ARRAYSIZE] = {0b1001, 0b1011011};
    b_uint first_half[ARRAYSIZE];
    b_uint last_half[ARRAYSIZE];

    int error = split_arr_at(arr, BITSIZE + BITSIZE + 1, first_half, last_half, ARRAYSIZE);
    TEST_ASSERT_EQUAL(1, error);
}

void test_split_at_array_double_gt_bit_size(void)
{
    b_uint arr[ARRAYSIZE_DOUBLE] = {0, 0b1001, 0, 0b1011011};
    b_uint first_half[ARRAYSIZE_DOUBLE];
    b_uint last_half[ARRAYSIZE_DOUBLE];

    int error = split_arr_at(arr, BITSIZE + 1, first_half, last_half, ARRAYSIZE_DOUBLE);
    TEST_ASSERT_EQUAL(0, error);

    // 0b1001 split into two: 0b100, and the last 1 is at the head of the bit size
    b_uint first_half_expected[ARRAYSIZE_DOUBLE] = {0, 0, 0b100, ~(ALL_ONES >> 1)};
    b_uint last_half_expected[ARRAYSIZE_DOUBLE] = {0, 0, 0, 0b1011011};

    TEST_ASSERT_EQUAL_INT_ARRAY(first_half_expected, first_half, ARRAYSIZE_DOUBLE);
    TEST_ASSERT_EQUAL_INT_ARRAY(last_half_expected, last_half, ARRAYSIZE_DOUBLE);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_split_at_array_0);
    RUN_TEST(test_split_at_array_lt_bit_size);
    RUN_TEST(test_split_at_array_eq_bit_size);
    RUN_TEST(test_split_at_array_gt_bit_size);
    RUN_TEST(test_split_at_array_overflow);

    RUN_TEST(test_split_at_array_double_gt_bit_size);
    return UNITY_END();
}