#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include <stdint.h>

/* Generate random odd number between min and max */
#include <stdlib.h>
#include <stdbool.h>

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


uint32_t discrete_exponent(uint32_t C, uint32_t d, uint32_t m)
{
    uint32_t phi_m=m-1; //since mod is prime
    uint32_t exp=d%phi_m;
    C=C%m;
    return mont_pow(C, exp, m);

}


uint32_t chinese_remainder_theorem(uint32_t C,uint32_t d, uint32_t n, uint32_t p, uint32_t q)
{
    uint32_t m1=p, m2=q;
    uint32_t M1=q, M2=p;

    uint32_t M1_inv=modinv(M1,m1);
    uint32_t M2_inv=modinv(M2,m2);
    //hanle overflow by using uint64_t
    uint64_t c1 = (uint64_t)M1 * M1_inv;
    uint64_t c2 = (uint64_t)M2 * M2_inv;

    uint32_t a1= discrete_exponent(C,d,m1);
    uint32_t a2= discrete_exponent(C,d,m2);

    //handle overflow
    uint64_t t = (uint64_t)a1 * c1 + (uint64_t)a2 * c2;
    uint32_t x = (uint32_t)(t % n);

    return x;


}

/* RSA Encrypt / Decrypt */
uint32_t rsa_encrypt(uint32_t m, uint32_t e, uint32_t n)
{
    m=m%n;
    return mont_pow(m, e, n);
}

uint32_t rsa_decrypt(uint32_t c, uint32_t d, uint32_t n, uint32_t p, uint32_t q)
{
    // return chinese_remainder_theorem(c,d,n,p,q);

    c=c%n;
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


uint32_t change_end(uint32_t x) {
    return ((x & 0x000000FF) << 24) |
           ((x & 0x0000FF00) <<  8) |
           ((x & 0x00FF0000) >>  8) |
           ((x & 0xFF000000) >> 24);
}

void write_file(FILE *f, uint32_t x) {
    uint32_t b = change_end(x);
    fwrite(&b, sizeof(b), 1, f);
}



uint32_t read_file(FILE *f) {
    uint32_t b;
    fread(&b, sizeof(b), 1, f);
    return change_end(b);
}


int main(int argc, char **argv)
{
    int op = -1;
    uint32_t p = 41;
    uint32_t q = 3041;
    uint32_t e = 5;

    uint32_t N   = p * q;
    uint32_t phi = (p - 1) * (q - 1);

    uint32_t d   = modinv(e, phi);
    FILE *fpubkey, *fprivkey, *fplain, *fcipher;
    if(argv[1][0] == 'g'){
        op=2;
        if (argc != 4)
        {
        fprintf(stderr, "Usage: %s public key  private key\n", argv[0]);
        return 1;
        }
        fpubkey = fopen(argv[2], "wb");
        fprivkey = fopen(argv[3], "wb");
        if (!fpubkey || !fprivkey) {
            perror("Cannot open key file");
            return 1;
        }

        srand((unsigned)time(NULL));
        generate_N(&e,&phi,&N,&p,&q); //generate N that fits e and phi
         d   = modinv(e, phi);
        // Write public key (N, e)
//        fwrite(&N, sizeof(N), 1, fpubkey);
//        fwrite(&e, sizeof(e), 1, fpubkey);
        write_file(fpubkey, e);
        write_file(fpubkey, N);



        // Write private key (N, d)
//        fwrite(&N, sizeof(N), 1, fprivkey);
//        fwrite(&d, sizeof(d), 1, fprivkey);
        write_file(fprivkey, d);
        write_file(fprivkey, N);

//        write_file(fprivkey, p);
//        write_file(fprivkey, q);



        fclose(fpubkey);
        fclose(fprivkey);

        printf("Keys generated successfully.\n");
    }
    else if (argv[1][0] == 'e')
    {
        op = 0;
        if (argc != 5)
        {
        fprintf(stderr, "Usage: %s key plaintext ciphertext\n", argv[0]);
        return 1;
        }
        fpubkey = fopen(argv[2], "rb");
        fplain = fopen(argv[3], "rb");
        fcipher = fopen(argv[4], "wb");

        if (!fpubkey || !fplain || !fcipher) {
            perror("Cannot open file");
            return 1;
        }
        // Read public key
        e=read_file( fpubkey);
        N=read_file(  fpubkey);

        fclose(fpubkey);

        // Read plaintext
        uint32_t plaintext;
        plaintext =read_file(fplain);
        fclose(fplain);

        // Encrypt
        uint32_t ciphertext = rsa_encrypt(plaintext, e, N);

        // Write ciphertext
        write_file( fcipher,ciphertext);
        fclose(fcipher);

        printf("Encryption done: plaintext %u -> ciphertext %u\n", plaintext, ciphertext);
    }
    else if (argv[1][0] == 'd')
    {
        // decrypt
        op = 1;
        if (argc != 5)
        {
        fprintf(stderr, "Usage: %s keyfile ciphertext plaintextfile\n", argv[0]);
        return 1;
        }
        fprivkey = fopen(argv[2], "rb");
        fcipher = fopen(argv[3], "rb");
        fplain = fopen(argv[4], "wb");
        if (!fprivkey || !fcipher || !fplain) {
            perror("Cannot open file");
            return 1;
        }

        // Read private key
        d=read_file( fprivkey);
        N=read_file(  fprivkey);
//        p=read_file(  fprivkey);
//        q=read_file(  fprivkey);

        fclose(fprivkey);

        // Read ciphertext
        uint32_t ciphertext;
        ciphertext= read_file( fcipher);
        fclose(fcipher);

        // Decrypt
        uint32_t plaintext = rsa_decrypt(ciphertext, d, N,p,q);

        // Write plaintext
        write_file( fplain,plaintext);
        fclose(fplain);

        printf("Decryption done: ciphertext %u -> plaintext %u\n", ciphertext, plaintext);

    }
     else {
        fprintf(stderr, "Invalid operation. Use g, e, or d.\n");
        return 1;
    }
    // uint32_t plaintext  = 17767;
    // uint32_t ciphertext = rsa_encrypt(plaintext, e, N);
    // uint32_t decrypted  = rsa_decrypt(ciphertext, d, N);

    // printf("Toy RSA Example\n");
    // printf("p = %u, q = %u\n", p, q);
    // printf("N = %u\n", N);
    // printf("e = %u, d = %u\n", e, d);
    // printf("Plaintext  = %u\n", plaintext);
    // printf("Ciphertext = %u\n", ciphertext);
    // printf("Decrypted  = %u\n", decrypted);

    return 0;
}
