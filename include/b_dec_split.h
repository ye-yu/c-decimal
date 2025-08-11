#pragma once
#include "b_dec_defines.h"

b_uint last_half(const r_uint comp);
b_uint first_half(const r_uint comp);

/**
 * Splits a b_uint at a specified bit position.
 *
 * Example:
 *
 * Input = 1001 1010
 * At position from last = 4
 *
 * First half = 0000 1001 (shifted right by 4 bits)
 * Last half = 0000 1010
 *
 *
 * Example 2:
 * Input = 1001 1010
 * At position from last = 6
 *
 * First half = 0000 0010 (shifted right by 6 bits)
 * Last half = 0001 1010

 */
int split_at(b_uint a, size_t at, b_uint *first_half, b_uint *last_half);

/**
 * Splits a b_uint at a specified bit position.
 *
 * Example:
 *
 * Input = 1001 1010
 * At position from last = 4
 *
 * First half = 0000 1001 (shifted right by 4 bits)
 * Last half = 0000 1010
 *
 *
 * Example 2:
 * Input = 1001 1010
 * At position from last = 6
 *
 * First half = 0000 0010 (shifted right by 6 bits)
 * Last half = 0001 1010

 */
int split_arr_at(b_uint *a, size_t at_from_behind, b_uint *first_half, b_uint *last_half, size_t size);