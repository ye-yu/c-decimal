#pragma once
#include "b_dec_defines.h"

int compare_b_dec(const b_dec a, const b_dec b);
int compare_b_uint_arr(const b_uint *a, const b_uint *b, const size_t size);
int is_zero_b_dec(const b_dec a);
int is_zero_b_uint_arr(const b_uint *a, const size_t size);