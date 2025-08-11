#pragma once
#include "b_dec_defines.h"

int mul_b_dec(const b_dec a, const b_dec b, b_dec *r);
int mul_10_b_dec(const b_dec num, b_dec *result);

/** @returns 1 if overflow, otherwise 0 */
int mul_b_uint(const b_uint a, const b_uint b, b_uint *r);

/** @returns 1 if overflow, otherwise 0 */
int mul_b_uint_arr(const b_uint *arr_a, const b_uint *arr_b, b_uint *r, const size_t size);

// multiply by shift and add
int mul_10_b_uint_arr(const b_uint *arr_a, b_uint *r, const size_t size);
