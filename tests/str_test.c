
#include "unity.h"
#include "b_dec.h"
#include "b_dec_str.h"

void setUp(void)
{
    // Set up code here
}

void tearDown(void)
{
    // Tear down code here
}

void test_str()
{
    b_uint dec = 123456789;
    char str[20];
    for (size_t i = 0; i < 20; i++)
    {
        str[i] = '\0'; // Initialize the string buffer
    }
    int result = b_uint_to_str(dec, str, 20);
    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_EQUAL_STRING("123456789", str);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_str);
    return UNITY_END();
}
