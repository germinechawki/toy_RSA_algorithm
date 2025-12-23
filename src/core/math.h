#ifndef MATH_H
#define MATH_H

#include <stdint.h>

/* Extended Euclidean Algorithm */
int64_t egcd(int64_t a, int64_t b, int64_t *x, int64_t *y);

/* Modular inverse */
uint32_t modinv(uint32_t a, uint32_t m);

/* Modular exponentiation */
uint32_t modpow(uint32_t base, uint32_t exp, uint32_t mod);

#endif /* MATH_H */
