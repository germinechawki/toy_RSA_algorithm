#include <stdint.h>

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

/* Modular inverse */
uint32_t modinv(uint32_t a, uint32_t m)
{
    int64_t x, y;
    int64_t g = egcd(a, m, &x, &y);
    if (g != 1)
        return 0;

    return (uint32_t)((x % m + m) % m);
}

/* Modular exponentiation */
uint32_t modpow(uint32_t base, uint32_t exp, uint32_t mod)
{
    uint64_t result = 1;
    uint64_t b = base;

    while (exp)
    {
        if (exp & 1)
            result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return (uint32_t)result;
}
