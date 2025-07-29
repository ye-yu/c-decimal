#pragma once
#include "b_dec_defines.h"

b_uint last_half(const r_uint comp);
b_uint first_half(const r_uint comp);
int split_at(b_uint a, size_t at, b_uint *first_half, b_uint *last_half);