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

void test_b_uint_arr_shift_right(void)
{
    b_uint num[ARRAYSIZE] = {0, 220};
    b_uint result[ARRAYSIZE];
    uint8_t shift_amount = 1; // Shift right by 1

    // Perform the right shift operation
    int error = shift_arr_right(num, result, ARRAYSIZE, shift_amount);
    TEST_ASSERT_EQUAL(0, error);

    // Check the result
    TEST_ASSERT_EQUAL(0, result[0]);   // First element should be 0
    TEST_ASSERT_EQUAL(110, result[1]); // Second element should be 110 after right shift by 1
}

void test_b_uint_arr_shift_right_by_1(void)
{
    b_uint num[ARRAYSIZE] = {0b1, 0};
    b_uint result[ARRAYSIZE];
    uint8_t shift_amount = 1;

    // Perform the right shift operation
    int error = shift_arr_right(num, result, ARRAYSIZE, shift_amount);
    TEST_ASSERT_EQUAL(0, error);

    // Check the result
    TEST_ASSERT_EQUAL(0, result[0]);
    TEST_ASSERT_EQUAL(~(b_uint)(ALL_ONES << 1 >> 1), result[1]);
}

void test_b_uint_arr_shift_right_by_33(void)
{
    b_uint num[ARRAYSIZE] = {0b1, 0};
    b_uint result[ARRAYSIZE];
    uint8_t shift_amount = 33;

    // Perform the right shift operation
    int error = shift_arr_right(num, result, ARRAYSIZE, shift_amount);
    TEST_ASSERT_EQUAL(0, error);

    // Check the result
    TEST_ASSERT_EQUAL(0, result[0]);
    TEST_ASSERT_EQUAL(0, result[1]);
}


void test_b_uint_arr_double_shift_right_by_33(void)
{
    b_uint num[ARRAYSIZE_DOUBLE] = {0b1, 0, 0, 0};
    b_uint result[ARRAYSIZE_DOUBLE];
    uint8_t shift_amount = 33;

    // Perform the right shift operation
    int error = shift_arr_right(num, result, ARRAYSIZE_DOUBLE, shift_amount);
    TEST_ASSERT_EQUAL(0, error);

    // Check the result
    TEST_ASSERT_EQUAL(0, result[0]);
    TEST_ASSERT_EQUAL(0, result[1]);
    TEST_ASSERT_EQUAL(~(b_uint)(ALL_ONES << 1 >> 1), result[2]);
    TEST_ASSERT_EQUAL(0, result[3]);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_b_uint_arr_shift_right);
    RUN_TEST(test_b_uint_arr_shift_right_by_1);
    RUN_TEST(test_b_uint_arr_shift_right_by_33);
    RUN_TEST(test_b_uint_arr_double_shift_right_by_33);
    return UNITY_END();
}