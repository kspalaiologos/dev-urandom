/*

To Quote "Data Compression in Depth" by Kamila Szewczyk:

>>>

Remark (Divisionless modulo operations). Under the premise that issuing the division or modulo instruction is
costly, modern compilers efficiently optimise division and modulo operations where the divisor is a constant num-
ber. Considering a parallel to how schoolchildren determine divisibility, we explore a variety of tricks for testing
divisibility: for divisibility by ten, we check whether the last digit of the number is 0; for divisibility by five, we
check whether the last digit of the number is either 0 or 5. Divisibility by three can be established by summing the
digits of a number and determining the divisibility of the sum itself. Similar rules allow us to quickly compute the
modulo operation using hardware.

The simplest case is computing a mod 2^X. We easily accomplish this by noticing that it is sufficient to take a
fixed amount of least significant bits of a to obtain the result. Hence, a mod 2^X = a&(2^X − 1). This is similar to
how one would execute a/10^x in base-10 arithmetic: we simply discard the last x decimal digits of the number. In
order to compute a mod 10^x, we discard all decimal digits except the last x.
We can derive methods like this for taking a number modulo a Mersenne number (2^X − 1 for X ∈ N) and further
edge cases, but the method compilers use is more general. We begin by observing that a mod n = a − n floor(a/n).
Suppose that we are interested only in 32-bit division. To give a particular example, we will derive a fast method
for computing a mod 65521, a constant used by the Adler32 checksum algorithm.
We begin this process by finding the smallest N = M + 32 such that 65521 ≤ (2^(M+32) mod 65521) + 2^M . Calcula-
tions suggest that N = 47. The core idea of the algorithm is to approximate the reciprocal of the divisor: knowing
this, we can now compute K = 2^N/65521 = 2147975281. Now, to compute X/65521 is to compute X × K and shift
it N positions to the right. Hence, we have established that:

We can verify that this is the same method that the compiler uses by checking the assembly output, accomplished
on UNIX systems with gcc as follows. Further optimisations may be applied by the compiler to the code when using
different constants, making the algorithm more difficult to discern.

X mod 65521 =_{32} X - 65521 * floor(X * K/2^N) = X - 65521 * floor(X * 2147975281 / 2^{47}))

We can verify that this is the same method that the compiler uses by checking the assembly output, accomplished
on UNIX systems with gcc as follows. Further optimisations may be applied by the compiler to the code when using
different constants, making the algorithm more difficult to discern.

    ~ % echo "int f(unsigned a){return a%65521;}"|gcc -O1 -S -x c - -o -|sed -e'/^\s*\.[^L]/d'
f:
    movl    %edi, %eax
    movl    $2147975281, %edx     ; <--
    imulq   %rdx, %rax
    shrq    $47, %rax             ; <--
    imull   $65521, %eax, %edx    ; <--
    movl    %edi, %eax
    subl    %edx, %eax            ; <--
    ret

<<<

This program prints the values of N and K given a divisor D.

*/

#include <common.h>

int main(int argc, char * argv[]) {
    ASSERT(argc == 2, "Usage: %s [divisor]\n", argv[0]);
    u64 D = strtoul(argv[1], NULL, 0), N = 0, K;
    ASSERT(D > 1, "Error: Divisor must be greater than 1\n");
    for (; D > (1U << (N + 32)) % D + (1U << N); N++);
    N += 32;  K = (1ULL << N) / D + 1;
    printf("N = %u, K = %u is a solution for x mod %u\n", N, K, D);
}