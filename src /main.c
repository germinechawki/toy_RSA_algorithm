#include <stdio.h>
#include <stdint.h>

#include "core/rsa.h"
#include "core/math.h"
#include "crypto/padding.h"

int main(void)
{
    uint32_t p = 41;
    uint32_t q = 3041;
    uint32_t e = 257;

    uint32_t N   = p * q;
    uint32_t phi = (p - 1) * (q - 1);
    uint32_t d   = modinv(e, phi);

    uint32_t plaintext  = 17767;
    uint32_t ciphertext = rsa_encrypt(plaintext, e, N);
    uint32_t decrypted  = rsa_decrypt(ciphertext, d, N);

    printf("Toy RSA Example\n");
    printf("p = %u, q = %u\n", p, q);
    printf("N = %u\n", N);
    printf("e = %u, d = %u\n", e, d);
    printf("Plaintext  = %u\n", plaintext);
    printf("Ciphertext = %u\n", ciphertext);
    printf("Decrypted  = %u\n", decrypted);

    return 0;
}
