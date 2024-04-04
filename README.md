# /dev/urandom

Random programs that perform data analysis tasks of questionable quality. Most of these supply a note that explains what a given program does.

xor256x86: a Xorshift256++ generator for x86_64 seeded using SplitMix64, outputs native-endian sequence of random data.
mtx86: a Mersenne Twister generator for x86_64. Takes a decimal number, outputs native-endian sequence of random data.

Issues/pull requests unwelcome, this is a personal repository. If something doesn't work the way you expect, it was probably intentional.
The rest of programs are used as a partial product in making other software (e.g. DOS32A) and put here merely for convenience.

Most of this was written primarily out of frustration. Extracting the data from or funneling data to a bitmap file should be obvious. Splitting a file into n chunks of m bytes should have been obvious. Turns out that it's not, and I end up rewriting these programs a hundred times, so this repository houses whatever I needed over time. Most of this was written in half an hour and is trivial to improve.
