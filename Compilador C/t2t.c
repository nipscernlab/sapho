// rotinas para conversao entre int e float

#include "t2t.h"
#include <math.h>
#include <stdlib.h>

FILE *f_float;

// ----------------------------------------------------------------------------
// funcoes auxiliares ---------------------------------------------------------
// ----------------------------------------------------------------------------

// inicializa as variaveis de estado para compilacao de ponto flutuante
void float_init()
{
    fgen = 0; // precisa gerar pf em software?
    i2f  = 0; // gera macro int2float
    f2i  = 0; // gera macro float2int
    fadd = 0; // gera macro de soma
    fmlt = 0; // gera macro de multiplicacao
    fdiv = 0; // gera macro de divisao

    // valores otimos achados no trabalho do Manso
    nbmant = 16;
    nbexpo =  6;
}

// converte um numero float (escrito em uma string) em meu float
int f2mf(char *va)
{
    float f = atof(va);

    if (f == 0.0) return 1 << (nbmant + nbexpo -1);

    int *ifl = &f; // isso aqui ta perigoso

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

// ----------------------------------------------------------------------------
// geracao do codigo em assembly para as operacoes encontradas ----------------
// ----------------------------------------------------------------------------

// deve ser incluido no comeco do arquivo asm
void float_begin(FILE *f_asm)
{
    char a;

    fprintf(f_asm, "// Inicializacao da emulacao do ponto flutuante em software -------------------\n\n");
    fprintf(f_asm, "LOAD NULL              // evita problema da primeira instrucao com o reset ascincrono\n");
    fprintf(f_asm, "\nLOAD %d\nSET  nbmant            // guarda num de bits da mantissa\n", nbmant);
    fprintf(f_asm, "\nLOAD %d\nSET  nbexp             // guarda num de bits do expoente\n\n" , nbexpo);
    fprintf(f_asm, "LOAD %d           // 0.0\nSET  float_zero", 1 << (nbmant+nbexpo-1));
    fprintf(f_asm, "        // guarda o num zero\n\n");

        f_float =     fopen  ("float_init.asm", "r");
    if (f_float == 0) fprintf(stderr, "Cad� a macro float_init.asm? Tinha que estar na pasta do projeto do SAPHO!\n");
	do {          a = fgetc  (f_float); if (a != EOF) fputc(a, f_asm);} while (a != EOF);

	fprintf(f_asm, "\n// Codigo assembly original ---------------------------------------------------\n\n");
	fclose (f_float);
}

// gera as instrucoes asm para ponto flutuante no final do arquivo asm
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

        f_float  =    fopen  ("float_gen.asm", "r");
    if (f_float == 0) fprintf(stderr, "Cad� a macro float_gen.asm? Tinha que estar na pasta do projeto do SAPHO!\n");
	do {      a  =    fgetc  (f_float); if (a != EOF) fputc(a, f_asm);} while (a != EOF);
	                  fclose (f_float);

    if (i2f)
    {
        f_float  =    fopen  ("float_i2f.asm", "r");
    if (f_float == 0) fprintf(stderr, "Cad� a macro float_i2f.asm? Tinha que estar na pasta do projeto do SAPHO!\n");
	do {      a  =    fgetc  (f_float); if (a != EOF) fputc(a, f_asm);} while (a != EOF);
	                  fclose (f_float);
    }

	if (f2i)
    {
        f_float  =    fopen  ("float_f2i.asm", "r");
    if (f_float == 0) fprintf(stderr, "Cad� a macro float_f2i.asm? Tinha que estar na pasta do projeto do SAPHO!\n");
	do {      a  =    fgetc  (f_float); if (a != EOF) fputc(a, f_asm);} while (a != EOF);
	                  fclose (f_float);
    }

    if (fadd)
    {
        f_float  =    fopen  ("float_soma.asm", "r");
    if (f_float == 0) fprintf(stderr, "Cad� a macro float_soma.asm? Tinha que estar na pasta do projeto do SAPHO!\n");
	do {      a  =    fgetc  (f_float); if (a != EOF) fputc(a, f_asm);} while (a != EOF);
	                  fclose (f_float);
    }

    if (fmlt)
    {
        f_float  =    fopen  ("float_mult.asm", "r");
    if (f_float == 0) fprintf(stderr, "Cad� a macro float_mult.asm? Tinha que estar na pasta do projeto do SAPHO!\n");
	do {      a  =    fgetc  (f_float); if (a != EOF) fputc(a, f_asm);} while (a != EOF);
	                  fclose (f_float);
    }

    if (fdiv)
    {
        f_float  =    fopen  ("float_div.asm", "r");
    if (f_float == 0) fprintf(stderr, "Cad� a macro float_div.asm? Tinha que estar na pasta do projeto do SAPHO!\n");
	do {      a  =    fgetc  (f_float); if (a != EOF) fputc(a, f_asm);} while (a != EOF);
	                  fclose (f_float);
    }

    fclose(f_asm);
}
