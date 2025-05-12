#include <stdio.h>
int main(void) {
int M[3][4] = {{11,12,13,14},
{21,22,23,24},
{31,32,33,34}};
printf("*M[0] %i\n", *M[0]);
printf("*M[1] %i\n", *M[1]);
printf("*M[2] %i\n", *M[2]);
printf("**M %i\n", **M);}