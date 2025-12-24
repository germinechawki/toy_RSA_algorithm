//#include <stdio.h>
//#include <stdint.h>
//#include <stdlib.h>
//
//
//uint32_t change_end(uint32_t x) {
//    return ((x & 0x000000FF) << 24) |
//           ((x & 0x0000FF00) <<  8) |
//           ((x & 0x00FF0000) >>  8) |
//           ((x & 0xFF000000) >> 24);
//}
//
//void write_file(FILE *f, uint32_t x) {
//    uint32_t b = change_end(x);
//    fwrite(&b, sizeof(b), 1, f);
//}
//
//void write_binary_file(const char *filename, uint32_t value) {
//    FILE *f = fopen(filename, "wb");  // "wb" = write binary
//    if (!f) {
//        perror("Cannot open file");
//        exit(1);
//    }
//    write_file(f, value);
//    fclose(f);
//}
//
//int main() {
//    uint32_t key = 0x4567;  // your number in hex
//    write_binary_file("plain.txt", key);
//    return 0;
//}
//
