#include "b_dec_copy.h"

int copy(const b_dec src, b_dec *dest)
{
    dest->sign = src.sign;
    dest->prec = src.prec;
    for (int i = 0; i < CHUNKSIZE; i++)
    {
        dest->mag[i] = src.mag[i];
    }
    return 0;
}
