#pragma once
#include "b_dec_defines.h"

int overflown(const b_uint comp);
int normalize(b_uint *a);
int carry(b_uint *a, const size_t size);