#include <limits.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

int main2()
  {
     void *tmpptr;
     int   x;
     union ccc {char klein[4];
                int  gross;
               } chk;


     printf("Integer:\n");
     printf("--------------\n\n");
     printf("unsigned short      - unsigned short int : %2d Bytes [%hu; %hu]\n", sizeof(unsigned short int), 0, USHRT_MAX);
     printf("    USHRT_MAX       : %hu\n", USHRT_MAX);
     printf("\n");
     printf("unsigned int        - unsigned int : %2d Bytes [%u; %u]\n", sizeof(unsigned int), (unsigned int)0, UINT_MAX);
     printf("    UINT_MAX        : %u\n", UINT_MAX);
     printf("\n");
     printf("unsigned long       - unsigned long int : %2d Bytes [%lu; %lu]\n", sizeof(unsigned long int), (unsigned long)0, ULONG_MAX);
     printf("    ULONG_MAX       : %lu\n", ULONG_MAX);
     printf("\n");
     printf("unsigned long long  - unsigned long long int : %2d Bytes [%llu; %llu]\n", sizeof(unsigned long long int), (unsigned long long)0, ULLONG_MAX);
     printf("    ULLONG_MAX       : %llu\n", ULLONG_MAX);
     printf("\n");
     printf("short               - signed short int : %2d Bytes [%hd; %hd]\n", sizeof(signed short int), SHRT_MIN, SHRT_MAX);
     printf("    SHRT_MIN        : %hd\n", SHRT_MIN);
     printf("    SHRT_MAX        : %hd\n", SHRT_MAX);
     printf("\n");
     printf("int                 - signed int : %2d Bytes [%d; %d]\n", sizeof(signed int), INT_MIN, INT_MAX);
     printf("    INT_MIN         : %d\n", INT_MIN);
     printf("    INT_MAX         : %d\n", INT_MAX);
     printf("\n");
     printf("long                - signed long int : %2d Bytes [%ld; %ld]\n", sizeof(signed long int), LONG_MIN, LONG_MAX);
     printf("    LONG_MIN        : %ld\n", LONG_MIN);
     printf("    LONG_MAX        : %ld\n", LONG_MAX);
     printf("\n");
     printf("long long           - signed long long int : %2d Bytes [%lld; %lld]\n", sizeof(signed long long int), LLONG_MIN, LLONG_MAX);
     printf("    LLONG_MIN       : %lld\n", LLONG_MIN);
     printf("    LLONG_MAX       : %lld\n", LLONG_MAX);
     printf("\n\n\n");
     printf("Characters and Pointers:\n");
     printf("------------------------\n\n");
     printf("    CHAR_BIT        : %d\n", CHAR_BIT);
     printf("\n");
     printf("char : %2d Bytes [%d; %d]\n", sizeof(char), CHAR_MIN, CHAR_MAX);
     printf("    CHAR_MIN        : %d\n", CHAR_MIN);
     printf("    CHAR_MAX        : %d\n", CHAR_MAX);
     printf("\n");
     printf("unsigned char  - unsigned char : %2d Bytes [%d; %d]\n", sizeof(unsigned char), 0, UCHAR_MAX);
     printf("    UCHAR_MAX       : %d\n", UCHAR_MAX);
     printf("\n");
     printf("signed char : %2d Bytes [%d; %d]\n", sizeof(signed char), SCHAR_MIN, SCHAR_MAX);
     printf("    SCHAR_MIN       : %d\n", SCHAR_MIN);
     printf("    SCHAR_MAX       : %d\n", SCHAR_MAX);
     printf("\n");
     printf("void *     : %2d Bytes\n", sizeof(tmpptr));
     printf("char [10]  : %2d Bytes\n", sizeof(char [10]));
     printf("size_t     : %2d Bytes [%llu; %llu]\n", sizeof(size_t), (unsigned long long)0, (unsigned long long)SIZE_MAX);
     printf("ptrdiff_t  : %2d Bytes [%lld; %lld]\n", sizeof(ptrdiff_t), (long long)PTRDIFF_MIN, (long long)PTRDIFF_MAX);
     printf("\n\n\n");

     printf("Floating point limits: \n");
     printf("---------------------- \n\n");

     printf("  FLT_RADIX       : %d\n", FLT_RADIX);
     printf("  FLT_ROUNDS      : %d  ", FLT_ROUNDS);
     x = (int)FLT_ROUNDS;
     switch (x) {
	case -1: printf("rounding mode: indeterminable\n"); break;
	case 0:  printf("rounding mode: toward 0\n"); break;
	case 1:  printf("rounding mode: nearest\n"); break;
	case 2:  printf("rounding mode: towards infinity\n"); break;
	case 3:  printf("rounding mode: towards -infinity\n"); break;
	default: printf("rounding mode: unknown\n");
     }
     printf("\n");
     printf("float: %2d Bytes [%g; %g]\n", sizeof(float), FLT_MIN, FLT_MAX);
     printf("    FLT_EPSILON     : %g\n", FLT_EPSILON);
     printf("    FLT_DIG         : %d\n", FLT_DIG);
     printf("    FLT_MANT_DIG    : %d\n", FLT_MANT_DIG);
     printf("    FLT_MIN         : %g\n", FLT_MIN);
     printf("    FLT_MAX         : %g\n", FLT_MAX);
     printf("    FLT_MIN_EXP     : %d\n", FLT_MIN_EXP);
     printf("    FLT_MAX_EXP     : %d\n", FLT_MAX_EXP);
     printf("    FLT_MIN_10_EXP  : %d\n", FLT_MIN_10_EXP);
     printf("    FLT_MAX_10_EXP  : %d\n", FLT_MAX_10_EXP);

     printf("\n");
     printf("double: %2d Bytes [%g; %g]\n", sizeof(double), DBL_MIN, DBL_MAX);
     printf("    DBL_EPSILON     : %g\n", DBL_EPSILON);
     printf("    DBL_DIG         : %d\n", DBL_DIG);
     printf("    DBL_MANT_DIG    : %d\n", DBL_MANT_DIG);
     printf("    DBL_MIN         : %g\n", DBL_MIN);
     printf("    DBL_MAX         : %g\n", DBL_MAX);
     printf("    DBL_MIN_EXP     : %d\n", DBL_MIN_EXP);
     printf("    DBL_MAX_EXP     : %d\n", DBL_MAX_EXP);
     printf("    DBL_MIN_10_EXP  : %d\n", DBL_MIN_10_EXP);
     printf("    DBL_MAX_10_EXP  : %d\n", DBL_MAX_10_EXP);

     printf("\n");
     printf("long double: %2d Bytes [%Lg; %Lg]\n", sizeof(long double), LDBL_MIN, LDBL_MAX);
     printf("    LDBL_EPSILON    : %Lg\n", LDBL_EPSILON);
     printf("    LDBL_DIG        : %d\n", LDBL_DIG);
     printf("    LDBL_MANT_DIG   : %d\n", LDBL_MANT_DIG);
     printf("    LDBL_MIN        : %Lg\n", LDBL_MIN);
     printf("    LDBL_MAX        : %Lg\n", LDBL_MAX);
     printf("    LDBL_MIN_EXP    : %d\n", LDBL_MIN_EXP);
     printf("    LDBL_MAX_EXP    : %d\n", LDBL_MAX_EXP);
     printf("    LDBL_MIN_10_EXP : %d\n", LDBL_MIN_10_EXP);
     printf("    LDBL_MAX_10_EXP : %d\n", LDBL_MAX_10_EXP);

     printf("\n\n\n");

     printf("Byteanordnung im Speicher: \n");
     printf("-------------------------- \n\n");

     chk.gross = 0x01020304;

     switch(chk.klein[0]) {
       case 1 :
           printf("Big Endian Format \n");
           printf("  LSB an kleinster Adresse (v.l.n.r. - LSB,...,MSB). \n");
           printf("  (z.B. 68k, Z8000) \n\n");
           break;
       case 2 :
           printf("Middle Endian Format \n");
           printf("  16-bit = Little Endian; 32-bit = Big Endian,");
           printf(" (LSB(MSB,LSB), MSB(MSB,LSB)\n");
           printf("  (DEC PDP-11) \n\n");
           break;
       case 4 :
           printf("Little Endian Format \n");
           printf("  LSB an hoechster Adresse (v.r.n.l. - MSB,...,LSB). \n");
           printf("  (z.B. DEC VAX, Intel x86) \n\n");
           break;
       }
     printf("  Hex(01 02 03 04) =");
     printf(" Bin(%d) = [%2d,%2d,%2d,%2d] \n\n\n", chk.gross,
                                         chk.klein[0], chk.klein[1],
                                         chk.klein[2], chk.klein[3]);

     return (0);
  }


int main()
{
  main2();
  exit(0);
}

