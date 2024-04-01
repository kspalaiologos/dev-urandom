// Extract the data from .WAV files to a .RAW file.
// E.g. wavfunnel FALL3new.wav 16 44100 2 $(wc -c<FALL3.dat) < FALL3.dat

#include <common.h>

int main(int argc, char * argv[]) {
    ASSERT(argc == 6, "Usage: %s <output> <bits_per_sample> <sample_rate> <channels> <input_size>\n", argv[0]);
    FILE * output = fopen(argv[1], "wb");
    ASSERT(output != NULL, "Error: Could not open file %s: %s\n", argv[1], strerror(errno));
    u16 bits_per_sample = atoi(argv[2]);
    u32 sample_rate = atoi(argv[3]);
    u16 channels = atoi(argv[4]);
    u32 bytes_per_sample = bits_per_sample / 8;
    u32 bytes_per_frame = bytes_per_sample * channels;
    u32 data_size = atoi(argv[5]);
    u32 file_size = 36 + data_size;
    u8 header[] = {
        'R', 'I', 'F', 'F', // RIFF
        file_size & 0xFF, (file_size >> 8) & 0xFF, (file_size >> 16) & 0xFF, (file_size >> 24) & 0xFF, // File size
        'W', 'A', 'V', 'E', // WAVE
        'f', 'm', 't', ' ', // fmt
        16, 0, 0, 0, // fmt size
        1, 0, // PCM
        channels & 0xFF, (channels >> 8) & 0xFF, // Channels
        sample_rate & 0xFF, (sample_rate >> 8) & 0xFF, (sample_rate >> 16) & 0xFF, (sample_rate >> 24) & 0xFF, // Sample rate
        sample_rate * bytes_per_frame & 0xFF, (sample_rate * bytes_per_frame >> 8) & 0xFF, (sample_rate * bytes_per_frame >> 16) & 0xFF, (sample_rate * bytes_per_frame >> 24) & 0xFF, // Byte rate
        bytes_per_frame & 0xFF, (bytes_per_frame >> 8) & 0xFF, // Block align
        bits_per_sample & 0xFF, (bits_per_sample >> 8) & 0xFF, // Bits per sample
        'd', 'a', 't', 'a', // data
        data_size & 0xFF, (data_size >> 8) & 0xFF, (data_size >> 16) & 0xFF, (data_size >> 24) & 0xFF, // Data size
    };

    fwrite(header, 1, 44, output);
    u8 sample[4]; Fi(data_size / bytes_per_sample,
        ASSERT(fread(sample, 1, bytes_per_sample, stdin) == bytes_per_sample, "Error: Could not read sample: %s\n", strerror(errno))
        ASSERT(fwrite(sample, 1, bytes_per_sample, output) == bytes_per_sample, "Error: Could not write sample: %s\n", strerror(errno))
    )
}

