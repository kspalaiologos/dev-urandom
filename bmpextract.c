// Extract data from a bmp file.
// 01-04-2024

#include <common.h>

int main(int argc, char * argv[]) {
    ASSERT(argc == 2, "Usage: %s <input>\n", argv[0]);
    FILE * input = fopen(argv[1], "rb");
    ASSERT(input != NULL, "Error: Could not open file %s: %s\n", argv[1], strerror(errno));
    
    u8 header[2];
    ASSERT(fread(header, 1, 2, input) == 2, "Error: Could not read header\n");
    ASSERT(header[0] == 'B' && header[1] == 'M', "Error: Not a BMP file\n");
    
    fseek(input, 18, SEEK_SET);
    u32 width, height;
    ASSERT(fread(&width, 4, 1, input) == 1, "Error: Could not read width\n");
    ASSERT(fread(&height, 4, 1, input) == 1, "Error: Could not read height\n");
    fseek(input, 10, SEEK_SET);
    u32 offset;
    ASSERT(fread(&offset, 4, 1, input) == 1, "Error: Could not read offset\n");
    
    fseek(input, 14, SEEK_SET);
    u16 header_size;
    ASSERT(fread(&header_size, 2, 1, input) == 1, "Error: Could not read header size\n");
    ASSERT(header_size == 12 || header_size == 108 || header_size == 124, "Error: Unrecognised file. Header size is %d.\n", header_size);

    fseek(input, 28, SEEK_SET);
    u16 bits_per_pixel;
    ASSERT(fread(&bits_per_pixel, 2, 1, input) == 1, "Error: Could not read bits per pixel\n");

    fprintf(stderr, "bmpextract: Valid BMP file, width=%u, height=%u, offset=%u, bits_per_pixel=%u\n", width, height, offset, bits_per_pixel);
    
    fseek(input, offset, SEEK_SET);
    u32 row_size = (width * bits_per_pixel + 31) / 32 * 4;
    u8 * row = malloc(row_size);
    ASSERT(row != NULL, "Error: Could not allocate row\n");
    Fy (height,
        ASSERT(fread(row, 1, row_size, input) == row_size, "Error: Could not read row\n");
        Fx (width,
            u32 pixel = 0;
            Fi (bits_per_pixel / 8, pixel |= row[x * bits_per_pixel / 8 + i] << (i * 8))
            fwrite(&pixel, bits_per_pixel / 8, 1, stdout);
        )
    )
    free(row);
}

