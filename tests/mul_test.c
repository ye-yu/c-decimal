#include "unity.h"
#include "b_dec.h"

#define ARRAYSIZE 2
#define ARRAYSIZE_DOUBLE 4
#define STR_SIZE (BITSIZE * CHUNKSIZE)

void setUp(void)
{
}

void tearDown(void)
{
}

void __print_multiply_result(b_dec a, b_dec b, b_dec result, const char *file, int line, const char *func)
{
    printf("%s:%d:%s: ", file, line, func);
    print_b_dec(a);
    printf(" * ");
    print_b_dec(b);
    printf(" = ");
    print_b_dec(result);
    printf("\n");
}

#define print_multiply_result(a, b, result) \
    __print_multiply_result(a, b, result, __FILE__, __LINE__, __func__)

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

void test_mul_b_uint_arr_10(void)
{
    const b_uint a[ARRAYSIZE] = {0, 12};
    b_uint result[ARRAYSIZE];
    zero_b_uint_arr(result, ARRAYSIZE);
    const int overflow = mul_10_b_uint_arr(a, result, ARRAYSIZE);

    TEST_ASSERT_EQUAL(0, overflow);
    TEST_ASSERT_EQUAL(0, result[0]);
    TEST_ASSERT_EQUAL(120, result[1]);
}

void test_mul_b_uint_arr_10_second_element(void)
{
    const b_uint a[ARRAYSIZE] = {1, 0};
    b_uint result[ARRAYSIZE];
    zero_b_uint_arr(result, ARRAYSIZE);
    const int overflow = mul_10_b_uint_arr(a, result, ARRAYSIZE);

    TEST_ASSERT_EQUAL(0, overflow);
    TEST_ASSERT_EQUAL(10, result[0]);
    TEST_ASSERT_EQUAL(0, result[1]);
}

void test_mul_b_uint_arr_10_overflow(void)
{
    const b_uint a[ARRAYSIZE] = {ALL_ONES, ALL_ONES};
    b_uint result[ARRAYSIZE];
    zero_b_uint_arr(result, ARRAYSIZE);
    const int overflow = mul_10_b_uint_arr(a, result, ARRAYSIZE);

    TEST_ASSERT_EQUAL(1, overflow);
    TEST_ASSERT_EQUAL(ALL_ONES, result[0]);
    TEST_ASSERT_EQUAL(ALL_ONES * 10, result[1]);
}

// b_dec tests
void test_mul_b_dec(void)
{
    b_dec a, b, result;
    zero(&a);
    zero(&b);
    zero(&result);
    a.sign = 0;
    a.prec = 0;
    a.mag[CHUNKSIZE - 1] = 12;
    b.sign = 0;
    b.prec = 0;
    b.mag[CHUNKSIZE - 1] = 34;

    const int overflow = mul_b_dec(a, b, &result);

    TEST_ASSERT_EQUAL(0, overflow);
    TEST_ASSERT_EQUAL(0, result.sign);
    TEST_ASSERT_EQUAL(0, result.prec);
    TEST_ASSERT_EQUAL(0, result.mag[0]);
    TEST_ASSERT_EQUAL(0, result.mag[1]);
    TEST_ASSERT_EQUAL(0, result.mag[2]);
    TEST_ASSERT_EQUAL(408, result.mag[3]);
}

void test_mul_b_dec_prec(void)
{
    b_dec a, b, result;
    zero(&a);
    zero(&b);
    zero(&result);
    a.sign = 0;
    a.prec = 0;
    a.mag[CHUNKSIZE - 1] = 0b110101;
    b.sign = 0;
    b.prec = 1;
    b.mag[CHUNKSIZE - 1] = 0b101;

    const int overflow = mul_b_dec(a, b, &result);
    print_multiply_result(a, b, result);

    TEST_ASSERT_EQUAL(0, overflow);
    TEST_ASSERT_EQUAL(0, result.sign);
    TEST_ASSERT_EQUAL(1, result.prec);
    TEST_ASSERT_EQUAL(0, result.mag[0]);
    TEST_ASSERT_EQUAL(0, result.mag[1]);
    TEST_ASSERT_EQUAL(0, result.mag[2]);
    TEST_ASSERT_EQUAL(0b110101 * 0b101, result.mag[3]);
}

void test_mul_b_dec_large(void)
{
    TEST_IGNORE_MESSAGE("Stringify needs to be fixed");
    b_dec a, b, result;
    zero(&a);
    zero(&b);
    zero(&result);
    a.sign = 0;
    a.prec = 0;
    a.mag[CHUNKSIZE - 2] = 0b110;
    a.mag[CHUNKSIZE - 1] = 0b110101;
    b.sign = 0;
    b.prec = 1;
    b.mag[CHUNKSIZE - 1] = 0b101;

    const int overflow = mul_b_dec(a, b, &result);
    print_multiply_result(a, b, result);

    // compare by str, python says 25769803829 * 0.5
    // 12884901914.5
    char str[STR_SIZE];
    b_dec_to_str(result, str, STR_SIZE);

    TEST_ASSERT_EQUAL(0, overflow);
    TEST_ASSERT_EQUAL_STRING("12884901914.5", str);
}

void test_mul_b_dec_trailing(void)
{
    TEST_IGNORE_MESSAGE("Stringify needs to be fixed");
    b_dec a, b, result;
    zero(&a);
    zero(&b);
    zero(&result);
    a.sign = 0;
    a.prec = 5;
    a.mag[CHUNKSIZE - 2] = 0b110;
    a.mag[CHUNKSIZE - 1] = 0b110101;
    b.sign = 0;
    b.prec = 0;
    b.mag[CHUNKSIZE - 1] = 0b10100000000;

    const int overflow = mul_b_dec(a, b, &result);
    print_multiply_result(a, b, result);

    // compare by str, python says 257698.03829 * 1280
    // 329853489.0112
    char str[STR_SIZE];
    b_dec_to_str(result, str, STR_SIZE);

    TEST_ASSERT_EQUAL(0, overflow);
    TEST_ASSERT_EQUAL_STRING("329853489.0112", str);
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

    RUN_TEST(test_mul_b_uint_arr_10);
    RUN_TEST(test_mul_b_uint_arr_10_second_element);
    RUN_TEST(test_mul_b_uint_arr_10_overflow);

    RUN_TEST(test_mul_b_dec);
    RUN_TEST(test_mul_b_dec_prec);
    RUN_TEST(test_mul_b_dec_large);
    RUN_TEST(test_mul_b_dec_trailing);
    return UNITY_END();
}