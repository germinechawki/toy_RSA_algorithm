#ifndef MATH_H
#define MATH_H

#include <stdint.h>

int64_t egcd(int64_t a, int64_t b, int64_t *x, int64_t *y);
uint32_t modinv(uint32_t a, uint32_t m);

uint32_t mont_redc(uint64_t x, uint32_t n, uint32_t n_inv);
uint32_t mont_mul(uint32_t a, uint32_t b, uint32_t n, uint32_t n_inv);

#endif
