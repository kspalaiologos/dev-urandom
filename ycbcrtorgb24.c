// Convert a stream of 24-bit YCbCr values to a stream of 24-bit RGB values.
// 01-04-2024

#include <common.h>

int main(int argc, char * argv[]) {
    ASSERT(argc == 1, "Usage: %s < in > out\n", argv[0]);
    u8 pixel[3];
    for (;;) {
        ASSERT(fread(pixel, 1, 3, stdin) == 3, "Error: Could not read pixel: %s\n", strerror(errno));
        u8 y = pixel[0];
        u8 cb = pixel[1];
        u8 cr = pixel[2];
        u8 r = (u8)(y + cr - 128);
        u8 g = (u8)(y - 0.344136 * (cb - 128) - 0.714136 * (cr - 128));
        u8 b = (u8)(y + cb - 128);
        fputc(r, stdout);  fputc(g, stdout);  fputc(b, stdout);
    }
}
