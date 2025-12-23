#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void write_binary_file(const char *filename, uint32_t value) {
    FILE *f = fopen(filename, "wb");  // "wb" = write binary
    if (!f) {
        perror("Cannot open file");
        exit(1);
    }
    fwrite(&value, sizeof(value), 1, f);  // write the raw bytes
    fclose(f);
}

int main() {
    uint32_t key = 0x4567;  // your number in hex
    write_binary_file("plain.hex", key);
    return 0;
}

