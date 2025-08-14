
#include "unity.h"
#include "b_dec.h"

void setUp(void)
{
    // Set up code here
}

void tearDown(void)
{
    // Tear down code here
}

void test_remove_trailing(void)
{
    b_dec a;
    zero(&a);
    a.prec = 5;
    a.mag[CHUNKSIZE - 1] = 0b10100;
    int result = remove_trailing_zeroes(a.mag, &a.prec, CHUNKSIZE);
    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_EQUAL(0b101, a.mag[CHUNKSIZE - 1]);
    TEST_ASSERT_EQUAL(3, a.prec);
}

void test_remove_trailing_shift(void)
{
    b_dec a;
    zero(&a);
    a.prec = 5;
    a.mag[CHUNKSIZE - 2] = 0b1;
    a.mag[CHUNKSIZE - 1] = 0b10100;
    int result = remove_trailing_zeroes(a.mag, &a.prec, CHUNKSIZE);
    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_EQUAL(0b101 | (~(ALL_ONES >> 1) >> 1), a.mag[CHUNKSIZE - 1]);
    TEST_ASSERT_EQUAL(3, a.prec);
}

void test_remove_trailing_non_neg_prec(void)
{
    b_dec a;
    zero(&a);
    a.prec = 5;
    a.mag[CHUNKSIZE - 1] = 0b101000000;
    int result = remove_trailing_zeroes(a.mag, &a.prec, CHUNKSIZE);
    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_EQUAL(0b1010, a.mag[CHUNKSIZE - 1]);
    TEST_ASSERT_EQUAL(0, a.prec);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_remove_trailing);
    RUN_TEST(test_remove_trailing_shift);
    RUN_TEST(test_remove_trailing_non_neg_prec);
    return UNITY_END();
}