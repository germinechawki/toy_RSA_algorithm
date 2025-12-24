#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "core/rsa.h"
#include "core/math.h"

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
