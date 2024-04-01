
/* asm2bf    Copyright (C) Kamila Szewczyk 2017-2020. */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

char * src;
long lof, ip = -1;

void match(int dir) {
    int level = 1;
    char ipvalue;
    do {
        ip += dir;
        if (ip < 0 || ip >= lof) {
            fputs("Mismatched brackets", stderr);
            exit(EXIT_FAILURE);
        }
        ipvalue = src[ip];
        if (ipvalue == 91)
            level += dir;
        else if (ipvalue == 93)
            level -= dir;
    } while (level > 0);
}

int main(int argc, char * argv[]) {
    FILE * infile = NULL;
    long mp = 0, maxmp = 1023;
    int n, ic, nc = 0;
    bool xflag = false, dflag = false, cycles = false;
    unsigned short int * mem;

    if (argc < 2) {
        fprintf(stderr, "Error: no input.\n");
        return EXIT_FAILURE;
    }

    for (n = 1; n < argc; n++) {
        if (argv[n][0] == '-') {
            switch (argv[n][1]) {
                case 'x':
                    xflag = true;
                    break;
                
                case 'd':
                    dflag = true;
                    break;

                case 'c':
                    cycles = true;
                    break;
                
                default:
                    fprintf(stderr, "Error: unrecognized command line option '-%c'\n", argv[n][1]);
                    return EXIT_FAILURE;
            }
        } else {
            if (infile == NULL) {
                char *fn = argv[n];
                infile = fopen(fn, "rb");
                if (infile == NULL) {
                    fprintf(stderr, "Error: unable to open file %s\n", fn);
                    return EXIT_FAILURE;
                }
            } else {
                fprintf(stderr, "Error: multiple input files.\n");
                return EXIT_FAILURE;
            }
        }
    }

    if (infile == NULL) {
        fputs("Error opening input file", stderr);
        return EXIT_FAILURE;
    }
    
    if (fseek(infile, 0L, SEEK_END) != 0) {
        fputs("Error determining length of input file", stderr);
        return EXIT_FAILURE;
    }
    lof = ftell(infile);
    if (lof == -1) {
        fputs("Error determining length of input file", stderr);
        return EXIT_FAILURE;
    }
    if (fseek(infile, 0L, SEEK_SET) != 0) {
        fputs("Error determining length of input file", stderr);
        return EXIT_FAILURE;
    }
    if (lof == 0)
        return EXIT_SUCCESS;
    src = (char *) calloc(lof + 2, sizeof(char));
    if (src == NULL) {
        fputs("Program too big to fit in memory", stderr);
        return EXIT_FAILURE;
    }
    if (fread(src, sizeof(char), lof, infile) < (unsigned) lof) {
        fputs("Error reading input file", stderr);
        return EXIT_FAILURE;
    }
    if (fclose(infile) == -1) {
        fputs("Error closing input file", stderr);
        return EXIT_FAILURE;
    }
    mem = calloc(1024, sizeof(unsigned short int));
    if (mem == NULL) {
        fputs("Out of memory", stderr);
        return EXIT_FAILURE;
    }
    while (++ip < lof) {
        if(cycles) nc++;
        switch (src[ip]) {
            case '>':
                if (mp >= maxmp) {
                    mem = realloc(mem, (maxmp + 1024) * sizeof(unsigned short int));
                    if (mem == NULL) {
                        fputs("Out of memory", stderr);
                        return EXIT_FAILURE;
                    }
                    for (n = 1; n < 1024; n++)
                        mem[maxmp + n] = 0;
                    maxmp += 1024;
                }
                mp++;
                break;
            case '<':
                if (mp <= 0) {
                    fprintf(stderr, "Access Violation, ip=%ld", ip);
                    return EXIT_FAILURE;
                }
                mp--;
                break;
            case '+':
                mem[mp]++;
                break;
            case '-':
                mem[mp]--;
                break;
            case '.':
                if(dflag) printf("%u ", mem[mp]);
                else      putchar(mem[mp]);
                
                fflush(stdout);
                break;
            case ',':
                n = getchar();
                if (n < 0) n = 0;
                mem[mp] = n;
                break;
            case '[':
                if (src[ip + 1] == '-' && src[ip + 2] == ']') {
                    mem[mp] = 0;
                    ip += 2;
                } else if (mem[mp] == 0)
                    match(1);
                break;
            case ']':
                match(-1);
                ip--;
                break;
            case '*':
                if (xflag) {
                    for (ic = 0; ic < 64; ic += 16) {
                        for (n = 0; n < 16; n++) {
                            fprintf(stderr, "%4X", mem[ic+n]);
                        }
                        fputc('\n', stderr);
                    }
                }
                break;
        }
    }
    free(mem);
    if(cycles) fprintf(stderr, "Done, %d cycles.", nc);
    return EXIT_SUCCESS;
}
