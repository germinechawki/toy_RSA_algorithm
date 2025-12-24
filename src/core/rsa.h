#ifndef RSA_H
#define RSA_H

#include <stdint.h>
#include"math.h"


uint32_t rsa_encrypt(uint32_t m, uint32_t e, uint32_t n);
uint32_t rsa_decrypt(uint32_t c, uint32_t d, uint32_t n);
void generate_N(uint32_t *e, uint32_t *phi, uint32_t *N, uint32_t *p, uint32_t *q);

#endif
