#include "t2t.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

FILE *f_float;

int f2mf(char *va)
{
    float f = atof(va);

    if (f == 0.0) return 1 << (nbmant + nbexpo -1);

    int *ifl = &f;

    // desempacota padrao IEEE ------------------------------------------------

    int s =  (*ifl >> 31) & 0x00000001;
    int e = ((*ifl >> 23) & 0xFF) - 127 - 22;
    int m = ((*ifl & 0x007FFFFF) + 0x00800000) >> 1;

    // sinal ------------------------------------------------------------------

    s = s << (nbmant + nbexpo);

    // expoente ---------------------------------------------------------------

    e = e + (23-nbmant);

    int sh = 0;
    while (e < -pow(2, nbexpo-1))
    {
        e   = e+1;
        sh = sh+1;
    }
    e = e & ((int)(pow(2,nbexpo)-1));
    e = e << nbmant;

    // mantissa ---------------------------------------------------------------

    if (nbmant == 23)
    {
        if (*ifl & 0x00000001) m = m+1; // arredonda
    }
    else
    {
        sh = 23-nbmant+sh;
        int carry = (m >> (sh-1)) & 0x00000001; // carry de arredondamento
        m = m >> sh;
        if (carry) m = m+1; // arredonda
    }
    return s + e + m;
}

void float_init()
{
    fgen = 0;
    i2f  = 0;
    f2i  = 0;
    fadd = 0;
    fmlt = 0;
    fdiv = 0;
}

void float_begin(FILE *f_asm)
{
    char a;

    fprintf(f_asm, "// ---------- Inicializacao da emulacao do ponto flutuante em software ----------\n\n");
    fprintf(f_asm, "LOAD 0\n");
    fprintf(f_asm, "LOAD %d\nSET nbmant\n", nbmant);
    fprintf(f_asm, "LOAD %d\nSET nbexp\n", nbexpo);
    fprintf(f_asm, "LOAD %d // 0.0\nSET float_zero\n", 1 << (nbmant+nbexpo-1));

    f_float = fopen("float_init.asm", "r");
	do {a = fgetc(f_float); if (a != EOF) fputc(a, f_asm);} while (a != EOF);

	fprintf(f_asm, "\n// ---------- Codigo assembly original ----------\n\n");
	fclose(f_float);
}

void float_gen(char *fasm)
{
    FILE *f_aux = fopen("c2aux.asm", "w");
    FILE *f_asm = fopen(fasm       , "r");

    char a;
    do {a = fgetc(f_asm); if (a != EOF) fputc(a, f_aux);} while (a != EOF);
    fclose(f_aux);
    fclose(f_asm);

    f_aux = fopen("c2aux.asm", "r");
    f_asm = fopen(fasm       , "w");

    float_begin(f_asm);
    do {a = fgetc(f_aux); if (a != EOF) fputc(a, f_asm);} while (a != EOF);
    fclose(f_aux);

    f_float = fopen("float_gen.asm", "r");
	do {a = fgetc(f_float); if (a != EOF) fputc(a, f_asm);} while (a != EOF);
	fclose(f_float);

    if (i2f)
    {
    f_float = fopen("float_i2f.asm", "r");
	do {a = fgetc(f_float); if (a != EOF) fputc(a, f_asm);} while (a != EOF);
	fclose(f_float);
    }

	if (f2i)
    {
    f_float = fopen("float_f2i.asm", "r");
	do {a = fgetc(f_float); if (a != EOF) fputc(a, f_asm);} while (a != EOF);
	fclose(f_float);
    }

    if (fadd)
    {
    f_float = fopen("float_soma.asm", "r");
	do {a = fgetc(f_float); if (a != EOF) fputc(a, f_asm);} while (a != EOF);
	fclose(f_float);
    }

    if (fmlt)
    {
    f_float = fopen("float_mult.asm", "r");
	do {a = fgetc(f_float); if (a != EOF) fputc(a, f_asm);} while (a != EOF);
	fclose(f_float);
    }

    if (fdiv)
    {
    f_float = fopen("float_div.asm", "r");
	do {a = fgetc(f_float); if (a != EOF) fputc(a, f_asm);} while (a != EOF);
	fclose(f_float);
    }

    fclose(f_asm);
}
