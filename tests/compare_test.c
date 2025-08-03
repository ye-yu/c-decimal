
#include "unity.h"
#include "b_dec.h"

void setUp(void)
{
    srand(1);
    // Set up code here
}

void tearDown(void)
{
    // Tear down code here
}

void test_compare_mag_more_than(void)
{
    b_dec a;
    b_dec b;
    zero(&a);
    zero(&b);

    b.mag[CHUNKSIZE - 1] = rand() + 1; // Ensure b is greater than a
    a.mag[CHUNKSIZE - 1] = b.mag[CHUNKSIZE - 1] - 1;
    TEST_ASSERT_GREATER_THAN(0, compare_b_dec(b, a));
}

void test_compare_mag_equal(void)
{
    b_dec a;
    b_dec b;
    zero(&a);
    zero(&b);

    a.mag[CHUNKSIZE - 1] = 1;
    b.mag[CHUNKSIZE - 1] = 1;
    TEST_ASSERT_EQUAL(0, compare_b_dec(a, b));
}

void test_compare_mag_less_than(void)
{
    b_dec a;
    b_dec b;
    zero(&a);
    zero(&b);

    b.mag[CHUNKSIZE - 1] = rand() + 1; // Ensure b is greater than a
    a.mag[CHUNKSIZE - 1] = b.mag[CHUNKSIZE - 1] - 1;
    TEST_ASSERT_LESS_THAN(0, compare_b_dec(a, b));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_compare_mag_more_than);
    RUN_TEST(test_compare_mag_equal);
    RUN_TEST(test_compare_mag_less_than);
    return UNITY_END();
}