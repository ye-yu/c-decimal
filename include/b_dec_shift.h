#pragma once
#include "b_dec_defines.h"

int shift_arr_right(const b_uint *operand, b_uint *result, size_t size, size_t bits_to_shift);
int shift_arr_left(const b_uint *operand, b_uint *result, size_t size, size_t bits_to_shift);