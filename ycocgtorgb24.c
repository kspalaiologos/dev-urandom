// Convert a stream of 24-bit YCoCg values to a stream of 24-bit RGB values.
// 01-04-2024

#include <common.h>

int main(int argc, char * argv[]) {
    ASSERT(argc == 1, "Usage: %s < in > out\n", argv[0]);
    u8 pixel[3];
    for (;;) {
        ASSERT(fread(pixel, 1, 3, stdin) == 3, "Error: Could not read pixel: %s\n", strerror(errno));
        u8 y = pixel[0];
        u8 co = pixel[1];
        u8 cg = pixel[2];
        u8 r = (u8)(y + co - cg);
        u8 g = (u8)(y + cg);
        u8 b = (u8)(y - co - cg);
        fputc(r, stdout);  fputc(g, stdout);  fputc(b, stdout);
    }
}
