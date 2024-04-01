
// Histogram of bit N-grams
// 01-04-2024

#include <common.h>

int main(int argc, char * argv[]) {
    ASSERT(argc == 3, "Usage: %s <input> <n>\n", argv[0]);
    FILE * input = fopen(argv[1], "rb");
    ASSERT(input != NULL, "Error: Could not open file %s: %s\n", argv[1], strerror(errno));
    int n = atoi(argv[2]);
    ASSERT(n > 0 && n < 56, "Error: N must be positive and less than 56.\n");
    fseek(input, 0, SEEK_END);
    u64 length = ftell(input);
    fseek(input, 0, SEEK_SET);
    u64 bits = 0;  u8 bitlen = 0;
    #define CODE(x) \
        x * histogram = calloc(1 << n, sizeof(x)); \
        ASSERT(histogram != NULL, "Error: Could not allocate histogram\n"); \
        while (!feof(input)) { \
            for (; bitlen <= 56; bitlen += 8) \
                { bits |= ((u64) fgetc(input)) << (56ULL - bitlen); if (feof(input)) break; } \
            histogram[bits >> (64ULL - n)]++, bits <<= n, bitlen -= n; \
        } \
        Fi (1 << n, histogram[i] != 0 && printf("%02x %u\n", i, histogram[i])) \
        free(histogram);
    if (length <= 65535) { CODE(u16) } else if (length <= 4294967295) { CODE(u32) } else { CODE(u64) }
}


