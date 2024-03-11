#ifndef T2T_H_INCLUDED
#define T2T_H_INCLUDED

#include <stdio.h>

int fgen, i2f, f2i, fadd, fmlt, fdiv;
int type_tmp;
int nbmant, nbexpo;
int line_num;

int  f2mf       (char *va);
void float_init ();
void float_begin(FILE *f_asm);
void float_gen  (char *fasm);

#endif // T2T_H_INCLUDED
