
#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>

#define _(a...) {return({a;});}
#define Fi(n,a...) for(i64 i=0;i<n;i++){a;}
#define Fj(n,a...) for(i64 j=0;j<n;j++){a;}
#define Fx(n,a...) for(i64 x=0;x<n;x++){a;}
#define Fy(n,a...) for(i64 y=0;y<n;y++){a;}
#define ASSERT(a,m...) if(!(a)){fprintf(stderr,m);exit(1);}

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#endif

