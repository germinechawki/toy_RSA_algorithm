#include <stdint.h>
#include "math.h"

/* Extended Euclidean Algorithm */
int64_t egcd(int64_t a, int64_t b, int64_t *x, int64_t *y)
{
    if (a == 0)
    {
        *x = 0;
        *y = 1;
        return b;
    }
    int64_t x1, y1;
    int64_t g = egcd(b % a, a, &x1, &y1);
    *x = y1 - (b / a) * x1;
    *y = x1;
    return g;
}

/* Modular Inverse */
uint32_t modinv(uint32_t a, uint32_t m)
{
    int64_t x, y;
    egcd(a, m, &x, &y);
    return (uint32_t)((x % m + m) % m);
}

/* Montgomery Reduction */
uint32_t mont_redc(uint64_t x, uint32_t n, uint32_t n_inv)
{
    uint64_t m = (uint32_t)x * n_inv;
    uint64_t t = (x + m * n) >> 32;
    if (t >= n)
        t -= n;
    return (uint32_t)t;
}

/* Montgomery Multiplication */
uint32_t mont_mul(uint32_t a, uint32_t b, uint32_t n, uint32_t n_inv)
{
    return mont_redc((uint64_t)a * b, n, n_inv);
}
