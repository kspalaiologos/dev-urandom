// Convert PCM to u-law or back.
#include <common.h>

#define BIAS 0x84

static inline int val_seg(int val) {
    int r = 0;  val >>= 7;
    if (val & 0xf0) val >>= 4, r += 4;
    if (val & 0x0c) val >>= 2, r += 2;
    if (val & 0x02) r++;
    return r;
}

static unsigned char linear2ulaw(int pcm_val) {
    int mask, seg;  unsigned char uval;
    if (pcm_val < 0)
        pcm_val = BIAS - pcm_val, mask = 0x7F;
    else pcm_val += BIAS, mask = 0xFF;
    if (pcm_val > 0x7FFF) pcm_val = 0x7FFF;
    seg = val_seg(pcm_val);
    uval = (seg << 4) | ((pcm_val >> (seg + 3)) & 0xF);
    return uval ^ mask;
}

static int ulaw2linear(unsigned char u_val) {
    int t;  u_val = ~u_val;
    t = ((u_val & 0xf) << 3) + BIAS;
    t <<= ((unsigned)u_val & 0x70) >> 4;
    return ((u_val & 0x80) ? (BIAS - t) : (t - BIAS));
}

int main(int argc, char * argv[]) {
    ASSERT(argc == 2, "Usage: %s [-e/-d] < in > out\n", argv[0])
    if (strcmp(argv[1], "-e") == 0) {
        for (i16 pcm; fread(&pcm, sizeof(pcm), 1, stdin) == 1; ) {
            u8 ulaw = linear2ulaw(pcm);
            ASSERT(fwrite(&ulaw, sizeof(ulaw), 1, stdout) == 1, "Failed to write\n")
        }
    } else if (strcmp(argv[1], "-d") == 0) {
        for (u8 ulaw; fread(&ulaw, sizeof(ulaw), 1, stdin) == 1; ) {
            i16 pcm = ulaw2linear(ulaw);
            ASSERT(fwrite(&pcm, sizeof(pcm), 1, stdout) == 1, "Failed to write\n")
        }
    } else {
        ASSERT(0, "Unknown option: %s\n", argv[1])
    }
}