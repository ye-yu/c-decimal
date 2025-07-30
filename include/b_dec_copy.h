#pragma once
#include "b_dec_defines.h"

int copy(const b_dec src, b_dec *dest);

int copy_b_uint_arr(const b_uint *src, b_uint *dest, const size_t size);
