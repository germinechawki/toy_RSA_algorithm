#include <stdint.h>

/* Forward declarations from math.c */
uint32_t modpow(uint32_t base, uint32_t exp, uint32_t mod);

/* RSA encryption */
uint32_t rsa_encrypt(uint32_t plaintext, uint32_t e, uint32_t N)
{
    return modpow(plaintext, e, N);
}

/* RSA decryption */
uint32_t rsa_decrypt(uint32_t ciphertext, uint32_t d, uint32_t N)
{
    return modpow(ciphertext, d, N);
}
