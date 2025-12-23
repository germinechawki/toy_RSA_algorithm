#include <stdint.h>
#include "rsa.h"
#include "math.h"

/* Montgomery Exponentiation */
uint32_t mont_pow(uint32_t base, uint32_t exp, uint32_t n)
{
    uint32_t n_inv = 1;
    for (int i = 0; i < 32; i++)
        n_inv *= 2 - n * n_inv;
    n_inv = (uint32_t)(0 - n_inv);

    uint64_t R = (uint64_t)1 << 32;
    uint32_t R2 = (uint32_t)((R % n) * (R % n) % n);

    uint32_t a = mont_redc((uint64_t)base * R2, n, n_inv);
    uint32_t result = mont_redc(R2, n, n_inv);

    while (exp)
    {
        if (exp & 1)
            result = mont_mul(result, a, n, n_inv);
        a = mont_mul(a, a, n, n_inv);
        exp >>= 1;
    }

    return mont_redc(result, n, n_inv);
}

/* RSA Encrypt / Decrypt */
uint32_t rsa_encrypt(uint32_t m, uint32_t e, uint32_t n)
{
    return mont_pow(m, e, n);
}

uint32_t rsa_decrypt(uint32_t c, uint32_t d, uint32_t n)
{
    return mont_pow(c, d, n);
}
