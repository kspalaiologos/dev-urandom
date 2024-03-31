// Split sequences of M bytes into N files.
// 31-03-2024

#include <common.h>

int main(int argc, char * argv[]) {
    ASSERT(argc == 4, "Usage: %s <input> <n> (streams) <m> (bytes each)\n", argv[0]);

    FILE * input = fopen(argv[1], "rb");
    ASSERT(input != NULL, "Error: Could not open file %s: %s\n", argv[1], strerror(errno));

    int n = atoi(argv[2]),m = atoi(argv[3]);

    FILE ** output = (FILE **) malloc(n * sizeof(FILE *));
    Fi(n,
        char filename[100];
        sprintf(filename, "%s.%d", argv[1], i);
        output[i] = fopen(filename, "wb");
        ASSERT(output[i] != NULL, "Error: Could not open file %s: %s\n", filename, strerror(errno));
    )

    u8 * buffer = malloc(m * n);
    for (;;) {
        size_t bytes_read = fread(buffer, 1, m * n, input);
        if (bytes_read == 0) break;
        Fi(n, ASSERT(fwrite(buffer + i * m, 1, m, output[i]) == m, "Error: Could not write to file %d: %s\n", i, strerror(errno)))
    }

    Fi(n, ASSERT(fclose(output[i]) == 0, "Error: Could not close file %d: %s\n", i, strerror(errno)))
    ASSERT(fclose(input) == 0, "Error: Could not close file %s: %s\n", argv[1], strerror(errno));

    free(buffer);  free(output);
}