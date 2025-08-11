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

void test_b_uint_arr_shift_left(void)
{
    b_uint num[ARRAYSIZE] = {0, 220};
    b_uint result[ARRAYSIZE];
    uint8_t shift_amount = 1; // Shift left by 1

    // Perform the left shift operation
    int error = shift_arr_left(num, result, ARRAYSIZE, shift_amount);
    TEST_ASSERT_EQUAL(0, error);

    // Check the result
    TEST_ASSERT_EQUAL(0, result[0]);   // First element should be 0
    TEST_ASSERT_EQUAL(440, result[1]); // Second element should be 440 after left shift by 1
}

void test_b_uint_arr_shift_left_by_1(void)
{
    // 0111 inverted becomes 1000. after shifted left, 1 will be added to the previous element
    b_uint num[ARRAYSIZE] = {0, ~(ALL_ONES >> 1)};
    b_uint result[ARRAYSIZE];
    uint8_t shift_amount = 1; // Shift left by 1

    // Perform the left shift operation
    int error = shift_arr_left(num, result, ARRAYSIZE, shift_amount);
    TEST_ASSERT_EQUAL(0, error);

    // Check the result
    TEST_ASSERT_EQUAL(1, result[0]); // First element should be 1
    TEST_ASSERT_EQUAL(0, result[1]); // Second element 0
}

void test_b_uint_arr_shift_left_by_33(void)
{
    // 0111 inverted becomes 1000. after shifted left, all will be discarded
    b_uint num[ARRAYSIZE] = {0, ~(ALL_ONES >> 1)};
    b_uint result[ARRAYSIZE];
    uint8_t shift_amount = 33; // Shift left by 33

    // Perform the left shift operation
    int error = shift_arr_left(num, result, ARRAYSIZE, shift_amount);
    TEST_ASSERT_EQUAL(0, error);

    // Check the result
    TEST_ASSERT_EQUAL(0, result[0]);
    TEST_ASSERT_EQUAL(0, result[1]);
}

void test_b_uint_arr_double_shift_left_by_33(void)
{
    // 0111 inverted becomes 1000. after shifted left, 1 will be at element 2
    b_uint num[ARRAYSIZE_DOUBLE] = {0, 0, 0, ~(ALL_ONES >> 1)};
    b_uint result[ARRAYSIZE_DOUBLE];
    uint8_t shift_amount = 33; // Shift left by 33

    // Perform the left shift operation
    int error = shift_arr_left(num, result, ARRAYSIZE_DOUBLE, shift_amount);
    TEST_ASSERT_EQUAL(0, error);

    // Check the result
    TEST_ASSERT_EQUAL(0, result[0]);
    TEST_ASSERT_EQUAL(1, result[1]);
    TEST_ASSERT_EQUAL(0, result[2]);
    TEST_ASSERT_EQUAL(0, result[3]);
}

void test_b_uint_arr_shift_left_all_ones(void)
{
    // 0111 inverted becomes 1000. after shifted left, 1 will be added to the previous element
    b_uint num[ARRAYSIZE] = {ALL_ONES, ALL_ONES};
    b_uint result[ARRAYSIZE];
    uint8_t shift_amount = 3; // Shift left by 1

    // Perform the left shift operation
    int error = shift_arr_left(num, result, ARRAYSIZE, shift_amount);
    TEST_ASSERT_EQUAL(0, error);

    // Check the result
    TEST_ASSERT_EQUAL(ALL_ONES, result[0]);      // First element should be 1
    TEST_ASSERT_EQUAL(ALL_ONES << 3, result[1]); // Second element 0
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_b_uint_arr_shift_right);
    RUN_TEST(test_b_uint_arr_shift_right_by_1);
    RUN_TEST(test_b_uint_arr_shift_right_by_33);
    RUN_TEST(test_b_uint_arr_double_shift_right_by_33);
    RUN_TEST(test_b_uint_arr_shift_left);
    RUN_TEST(test_b_uint_arr_shift_left_by_1);
    RUN_TEST(test_b_uint_arr_shift_left_by_33);
    RUN_TEST(test_b_uint_arr_double_shift_left_by_33);
    RUN_TEST(test_b_uint_arr_shift_left_all_ones);
    return UNITY_END();
}