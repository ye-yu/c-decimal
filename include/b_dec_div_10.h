#pragma once
#include "b_dec_defines.h"

// i got it from here lol https://math.stackexchange.com/a/3001889/704406
int div_10(const uint64_t num, uint64_t *quotient, uint64_t *remainder);

int div_10_b_dec(const b_dec num, b_dec *result, uint8_t *remainder);

int div_10_b_uint_arr(const b_uint *num, b_uint *result, uint8_t *remainder, const size_t size);