
#include "unity.h"
#include "b_dec.h"

#define STR_TEST_CHAR_SIZE 20
static const size_t str_size = STR_TEST_CHAR_SIZE;
static char str[STR_TEST_CHAR_SIZE];

void setUp(void)
{
    for (size_t i = 0; i < 20; i++)
    {
        str[i] = '\0'; // Initialize the string buffer
    }
}

void tearDown(void)
{
    // Tear down code here
}

void test_str(void)
{
    b_uint dec = 123456789;
    int result = b_uint_to_str(dec, str, 20);
    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_EQUAL_STRING("123456789", str);
}

void test_b_dec_str_pos(void)
{
    b_dec dec;
    zero(&dec);
    dec.sign = 0;
    dec.prec = 0;
    dec.mag[CHUNKSIZE - 1] = 123456789;
    int result = b_dec_to_str(dec, str, 20);
    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_EQUAL_STRING("123456789", str);
}

void test_b_dec_str_neg(void)
{
    b_dec dec;
    zero(&dec);
    dec.sign = 1;
    dec.prec = 0;
    dec.mag[CHUNKSIZE - 1] = 123456789;
    int result = b_dec_to_str(dec, str, 20);
    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_EQUAL_STRING("-123456789", str);
}

void test_b_dec_str_pos_prec_in_bound(void)
{
    b_dec dec;
    zero(&dec);
    dec.sign = 0;
    dec.prec = 2;
    dec.mag[CHUNKSIZE - 1] = 123456789;
    int result = b_dec_to_str(dec, str, 20);
    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_EQUAL_STRING("1234567.89", str);
}

void test_b_dec_str_neg_prec_in_bound(void)
{
    b_dec dec;
    zero(&dec);
    dec.sign = 1;
    dec.prec = 2;
    dec.mag[CHUNKSIZE - 1] = 123456789;
    int result = b_dec_to_str(dec, str, 20);
    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_EQUAL_STRING("-1234567.89", str);
}

void test_b_dec_str_pos_prec_out_bound(void)
{
    b_dec dec;
    zero(&dec);
    dec.sign = 0;
    dec.prec = 9;
    dec.mag[CHUNKSIZE - 1] = 1234;
    int result = b_dec_to_str(dec, str, 20);
    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_EQUAL_STRING("0.000001234", str);
}

void test_b_dec_str_neg_prec_out_bound(void)
{
    b_dec dec;
    zero(&dec);
    dec.sign = 1;
    dec.prec = 9;
    dec.mag[CHUNKSIZE - 1] = 1234;
    int result = b_dec_to_str(dec, str, 20);
    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_EQUAL_STRING("-0.000001234", str);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_str);
    RUN_TEST(test_b_dec_str_pos);
    RUN_TEST(test_b_dec_str_pos_prec_in_bound);
    RUN_TEST(test_b_dec_str_pos_prec_out_bound);
    RUN_TEST(test_b_dec_str_neg);
    RUN_TEST(test_b_dec_str_neg_prec_in_bound);
    return UNITY_END();
}
