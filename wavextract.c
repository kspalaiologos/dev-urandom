// Extract the data from .WAV files to a .RAW file.

#include <common.h>

int main(int argc, char * argv[]) {
    ASSERT(argc == 2, "Usage: %s <input> > output\n", argv[0]);
    FILE * input = fopen(argv[1], "rb");
    ASSERT(input != NULL, "Error: Could not open file %s: %s\n", argv[1], strerror(errno));

    u8 header[4];  u16 bits_per_sample, channels;  u32 sample_rate, data_size;  
    ASSERT(fread(header, 1, 4, input) == 4, "Error: Could not read header\n");
    ASSERT(header[0] == 'R' && header[1] == 'I' && header[2] == 'F' && header[3] == 'F', "Error: Not a WAV file\n");
    fseek(input, 22, SEEK_SET);
    ASSERT(fread(&channels, 2, 1, input) == 1, "Error: Could not read channels\n");
    ASSERT(fread(&sample_rate, 4, 1, input) == 1, "Error: Could not read sample rate\n");
    fseek(input, 34, SEEK_SET);
    ASSERT(fread(&bits_per_sample, 2, 1, input) == 1, "Error: Could not read bits per sample\n");
    fseek(input, 40, SEEK_SET);
    ASSERT(fread(&data_size, 4, 1, input) == 1, "Error: Could not read data size\n");
    
    fprintf(stderr, "wavextract: Valid WAV file, bits_per_sample=%u, sample_rate=%u, channels=%u\n", bits_per_sample, sample_rate, channels);

    Fi(data_size,
        u8 sample;
        ASSERT(fread(&sample, 1, 1, input) == 1, "Error: Could not read sample\n");
        ASSERT(fwrite(&sample, 1, 1, stdout) == 1, "Error: Could not write sample\n"));
}

