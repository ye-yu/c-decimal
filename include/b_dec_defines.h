#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define CHUNKSIZE 4
#define CHUNKSIZE_DOUBLE 8
#define BITSIZE 32
#define b_uint uint32_t
#define r_uint uint64_t
#define ONE ((b_uint)1)
#define ZERO ((b_uint)0)
#define MASK 0x7fffffff
#define ALL_ONES (ZERO - 1)

typedef struct _b_dec
{
    // sign: 0 for positive, 1 for negative
    uint8_t sign;
    b_uint prec;
    b_uint mag[CHUNKSIZE];
} b_dec;

#ifdef __int128
typedef unsigned __int128 uint128_t;
#else
#define UINT128_NOT_SUPPORTED
typedef char uint128_t;
#endif
