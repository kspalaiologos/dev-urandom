// Convert a stream of 24-bit pixel values (RGB) to a stream of 24-bit YcoCg values.
// 01-04-2024

#include <common.h>

int main(int argc, char * argv[]) {
    ASSERT(argc == 1, "Usage: %s < in > out\n", argv[0]);
    u8 pixel[3];
    for (;;) {
        ASSERT(fread(pixel, 1, 3, stdin) == 3, "Error: Could not read pixel: %s\n", strerror(errno));
        u8 r = pixel[0];
        u8 g = pixel[1];
        u8 b = pixel[2];
        u8 y = (u8)(0.25 * r + 0.5 * g + 0.25 * b);
        u8 co = (u8)(0.5 * r - 0.5 * b);
        u8 cg = (u8)(-0.25 * r + 0.5 * g - 0.25 * b);
        fputc(y, stdout);  fputc(co, stdout);  fputc(cg, stdout);
    }
}
