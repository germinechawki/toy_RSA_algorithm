#include <stdint.h>
#include "rsa.h"
/* Generate random odd number between min and max */
#include <stdlib.h>
#include <stdbool.h>





/* RSA Encrypt / Decrypt */
uint32_t rsa_encrypt(uint32_t m, uint32_t e, uint32_t n)
{
    return mont_pow(m, e, n);
}

uint32_t rsa_decrypt(uint32_t c, uint32_t d, uint32_t n)
{
    return mont_pow(c, d, n);
}


uint32_t random_odd(uint32_t min, uint32_t max) {
    if (min % 2 == 0) min++;
    if (max % 2 == 0) max--;

    uint32_t count = ((max - min) / 2) + 1;
    return min + 2 * (rand() % count);
}
uint32_t miller_rabin_prime()
{
    uint32_t n = random_odd(1000, 5000);

    while(1) {
        //finding q&k
        uint32_t q = n - 1;
        uint32_t k = 0;

        while ((q & 1) == 0) {   // while q is even
            q >>= 1;             // divide by 2
            k++;
        }
        bool all_prime=true;

        for (int r = 0; r < 10; r++) {
            bool is_prime = false;

            uint32_t a = 2 + rand() % (n - 3); //random a in [2,n-2]
            uint32_t x = mont_pow(a, q, n);
            if (x == 1 || x == n - 1) {
                is_prime = true;
                continue;
            }
            for (uint32_t i = 1; i < k; i++) {
                x = mont_pow(x, 2, n);
                if (x == n - 1) {
                    is_prime = true;
                    break;
                } // probably prime


            }
           if(!is_prime){
               all_prime=false;
                break; // composite
           }

        }
       if(all_prime){
           return n;
       }
       n+=2; //try next odd number
    }

}
void generate_N(uint32_t *e, uint32_t *phi, uint32_t *N, uint32_t *p, uint32_t *q)
{
    *p = miller_rabin_prime();
    do {
        *q = miller_rabin_prime();
    } while (*q == *p);

    *N = (*p) * (*q);
    *phi = (*p - 1) * (*q - 1);

    int64_t x, y;
    // Ensure e is coprime to phi
    while (egcd(*e, *phi, &x,&y) != 1) {
        *p = miller_rabin_prime();
        do {
            *q = miller_rabin_prime();
        } while (*q == *p);

        *N = (*p) * (*q);
        *phi = (*p - 1) * (*q - 1);
    }

}
