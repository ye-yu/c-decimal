
#include "unity.h"
#include "b_dec.h"

#define STR_TEST_CHAR_SIZE (BITSIZE * CHUNKSIZE)
static const size_t str_size = STR_TEST_CHAR_SIZE;
static char str[STR_TEST_CHAR_SIZE];
static char str_expected[STR_TEST_CHAR_SIZE];

void setUp(void)
{
    for (size_t i = 0; i < STR_TEST_CHAR_SIZE; i++)
    {
        str[i] = '\0';          // Initialize the string buffer
        str_expected[i] = '\0'; // Initialize the expected string buffer
    }
}

void tearDown(void)
{
    // Tear down code here
}

void test_str(void)
{
    b_uint dec = 123456789;
    int result = b_uint_to_str(dec, str, str_size);
    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_EQUAL_STRING("123456789", str);
}

void test_b_dec_str_large(void)
{
    b_dec dec;
    zero(&dec);
    dec.sign = 0;
    dec.prec = 0;
    dec.mag[CHUNKSIZE - 2] = 0x12345678;
    dec.mag[CHUNKSIZE - 1] = 0x12345678; // Large number
    int result = b_dec_to_str(dec, str, str_size);
    TEST_ASSERT_FALSE(result);
    // python says 0x1234567812345678 is 1311768465173141112
    TEST_ASSERT_EQUAL_STRING("1311768465173141112", str);
}

void test_b_dec_str_pos(void)
{
    b_dec dec;
    zero(&dec);
    dec.sign = 0;
    dec.prec = 0;
    dec.mag[CHUNKSIZE - 1] = 123456789;
    int result = b_dec_to_str(dec, str, str_size);
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
    int result = b_dec_to_str(dec, str, str_size);
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
    int result = b_dec_to_str(dec, str, str_size);
    TEST_ASSERT_FALSE(result);
    double_t expected = 123456789.0 / 2 / 2;
    snprintf(str_expected, STR_TEST_CHAR_SIZE, "%lf", expected);
    TEST_ASSERT_EQUAL_STRING(str_expected, str);
}

void test_b_dec_str_neg_prec_in_bound(void)
{
    b_dec dec;
    zero(&dec);
    dec.sign = 1;
    dec.prec = 2;
    dec.mag[CHUNKSIZE - 1] = 123456789;
    int result = b_dec_to_str(dec, str, str_size);
    TEST_ASSERT_FALSE(result);
    double_t expected = -123456789.0 / 2 / 2;
    snprintf(str_expected, STR_TEST_CHAR_SIZE, "%lf", expected);
    TEST_ASSERT_EQUAL_STRING(str_expected, str);
}

void test_b_dec_str_pos_prec_out_bound(void)
{
    b_dec dec;
    zero(&dec);
    dec.sign = 0;
    dec.prec = 9;
    dec.mag[CHUNKSIZE - 1] = 1234;
    int result = b_dec_to_str(dec, str, str_size);
    TEST_ASSERT_FALSE(result);
    double_t expected = 1234.0 / 2 / 2 / 2 / 2 / 2 / 2 / 2 / 2 / 2;
    snprintf(str_expected, STR_TEST_CHAR_SIZE, "%lf", expected);
    TEST_ASSERT_EQUAL_STRING(str_expected, str);
}

void test_b_dec_str_neg_prec_out_bound(void)
{
    b_dec dec;
    zero(&dec);
    dec.sign = 1;
    dec.prec = 9;
    dec.mag[CHUNKSIZE - 1] = 1234;
    int result = b_dec_to_str(dec, str, str_size);
    TEST_ASSERT_FALSE(result);
    double_t expected = -1234.0 / 2 / 2 / 2 / 2 / 2 / 2 / 2 / 2 / 2;
    snprintf(str_expected, STR_TEST_CHAR_SIZE, "%lf", expected);
    TEST_ASSERT_EQUAL_STRING(str_expected, str);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_str);
    RUN_TEST(test_b_dec_str_large);
    RUN_TEST(test_b_dec_str_pos);
    RUN_TEST(test_b_dec_str_pos_prec_in_bound);
    RUN_TEST(test_b_dec_str_pos_prec_out_bound);
    RUN_TEST(test_b_dec_str_neg);
    RUN_TEST(test_b_dec_str_neg_prec_in_bound);
    return UNITY_END();
}
