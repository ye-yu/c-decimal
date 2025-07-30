#include "b_dec_trailing.h"
#include "b_dec_split.h"
#include "b_dec_zero.h"
#include "b_dec_compare.h"

int remove_trailing_zeroes(b_uint *a, b_uint *prec_ref, const size_t size)
{
    b_uint prec = *prec_ref;
    if (prec <= 0)
    {
        return 0;
    }

    // remove complete zeros
    while (a[size - 1] == 0 && prec > (BITSIZE - 1))
    {
        prec -= (BITSIZE - 1);
        for (size_t i = CHUNKSIZE - 1; i > 0; i--)
        {
            a[i] = a[i - 1];
        }
        a[0] = 0;
    }

    // remove trailing zeroes
    size_t bits_to_shift = 0;
    while (prec > 0 && a[size - 1] != 0 && ((a[size - 1] >> bits_to_shift) & ONE) == 0)
    {
        prec--;
        bits_to_shift++;
    }

    b_uint first_half, last_half;
    const long long last = (long long)(size - 1);
    for (long long i = last; i >= 0; i--)
    {
        first_half = 0;
        last_half = 0;
        split_at(a[i], bits_to_shift, &first_half, &last_half);
        a[i] >>= bits_to_shift;
        if (i == last)
        {
            continue;
        }

        a[i + 1] |= (last_half << (BITSIZE - bits_to_shift));
    }

    return 0;
}


int remove_trailing_log10_zeroes(b_dec *num) {
    if (is_zero(*num)) {
        zero(num);
        return 0;
    }

    b_uint last = num->mag[CHUNKSIZE - 1];
    uint64_t quotient = 0;
    uint64_t remainder = 0;

    quotient = last / 10;
    remainder = last - (quotient * 10);

    while(remainder == 0) {
        

    }

}