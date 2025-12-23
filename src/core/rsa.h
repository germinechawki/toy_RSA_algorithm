#ifndef RSA_H
#define RSA_H

#include <stdint.h>

/* RSA operations */
uint32_t rsa_encrypt(uint32_t plaintext, uint32_t e, uint32_t N);
uint32_t rsa_decrypt(uint32_t ciphertext, uint32_t d, uint32_t N);

#endif /* RSA_H */
