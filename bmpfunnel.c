// Turn raw data into a BMP file.
// 01-04-2024

#include <common.h>

int main(int argc, char * argv[]) {
    ASSERT(argc == 5, "Usage: %s <output> <width> <height> <bits_per_pixel>\n", argv[0]);
    FILE * output = fopen(argv[1], "wb");
    ASSERT(output != NULL, "Error: Could not open file %s: %s\n", argv[1], strerror(errno));
    u32 width = atoi(argv[2]);
    u32 height = atoi(argv[3]);
    u16 bits_per_pixel = atoi(argv[4]);
    u32 bytes_per_pixel = bits_per_pixel / 8;
    u32 row_size = width * bytes_per_pixel;
    u32 padding = (4 - (row_size % 4)) % 4;
    u32 row_size_padded = row_size + padding;
    u32 data_size = row_size_padded * height;
    u32 file_size = 54 + data_size;
    u8 header[54] = {
        'B', 'M', // Magic number
        file_size & 0xFF, (file_size >> 8) & 0xFF, (file_size >> 16) & 0xFF, (file_size >> 24) & 0xFF, // File size
        0, 0, 0, 0, // Reserved
        54, 0, 0, 0, // Data offset
        40, 0, 0, 0, // DIB header size
        width & 0xFF, (width >> 8) & 0xFF, (width >> 16) & 0xFF, (width >> 24) & 0xFF, // Width
        height & 0xFF, (height >> 8) & 0xFF, (height >> 16) & 0xFF, (height >> 24) & 0xFF, // Height
        1, 0, // Planes
        bits_per_pixel & 0xFF, (bits_per_pixel >> 8) & 0xFF, // Bits per pixel
        0, 0, 0, 0, // Compression
        data_size & 0xFF, (data_size >> 8) & 0xFF, (data_size >> 16) & 0xFF, (data_size >> 24) & 0xFF, // Data size
        0, 0, 0, 0, // Horizontal resolution
        0, 0, 0, 0, // Vertical resolution
        0, 0, 0, 0, // Colors in palette
        0, 0, 0, 0, // Important colors
    };
    fwrite(header, 1, 54, output);
    u8 pixel[4];
    Fy(height,
        Fx(width,
            ASSERT(fread(pixel, 1, bytes_per_pixel, stdin) == bytes_per_pixel, "Error: Could not read pixel: %s\n", strerror(errno))
            ASSERT(fwrite(pixel, 1, bytes_per_pixel, output) == bytes_per_pixel, "Error: Could not write pixel: %s\n", strerror(errno)))
        Fi(padding, ASSERT(fputc(0, output) != EOF, "Error: Could not write padding: %s\n", strerror(errno)))
    )
}