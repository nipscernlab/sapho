// rotinas para conversao entre int e float

#include "..\Headers\t2t.h"
#include <math.h>
#include <stdlib.h>

FILE *f_float;

// ----------------------------------------------------------------------------
// funcoes auxiliares ---------------------------------------------------------
// ----------------------------------------------------------------------------

// inicializa as variaveis de estado para compilacao de ponto flutuante
void float_init()
{
    fgen   = 0; // precisa gerar pf em software?
    i2f    = 0; // gera macro int2float
    f2i    = 0; // gera macro float2int
    fadd   = 0; // gera macro de soma
    fmlt   = 0; // gera macro de multiplicacao
    fdiv   = 0; // gera macro de divisao

     mgen  = 0; // vai usar funcoes aritmeticas
    fsqrt  = 0; // gera macro pra raiz quadrada (proc float)
    fsqrti = 0; // gera macro pra raiz quadrada (proc int  )
    fatan  = 0; // gera macro pra arco tangente (proc float)
    fatani = 0; // gera macro pra arco tangente (proc int  )

    // valores otimos achados no trabalho do Manso
    nbmant = 16;
    nbexpo =  6;
}

// converte um numero float (escrito em uma string) em meu float
int f2mf(char *va)
{
    float f = atof(va);

    if (f == 0.0) return 1 << (nbmant + nbexpo -1);

    int *ifl = (int*)&f; // isso aqui ta perigoso

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

// valor a ser usado na convergencia dos funcoes aritmeticas iterativas
// o inum pega uma string com o numero inteiro equivalente
// o fnum pega uma string com o proprio numero float
void epsilon_taylor(char *inum, char *fnum)
{
    // acha o dobro do menor valor possivel em float
    double numf = 2.0*pow(2, nbmant-1)*pow(2,-pow(2,nbexpo-1));
    // se  aprecisao for grande, usa o padrao
    if    (numf < 0.0000001) numf = 0.0000001;
    sprintf(fnum, "%.7f",    numf);

    int     numi = f2mf(fnum);
    sprintf(inum, "%d", numi);
}

// ----------------------------------------------------------------------------
// geracao do codigo em assembly para as operacoes encontradas ----------------
// ----------------------------------------------------------------------------

// deve ser incluido no comeco do arquivo asm (para proc ponto fixo)
void float_begin(FILE *f_asm)
{
    char a;

    // inicializacao de dados parametrizados ----------------------------------

    fprintf(f_asm, "// Inicializacao da emulacao do ponto flutuante em software -------------------\n\n");

    // LOAD NULL deve ser a porimeira instrucao sempre, pra evitar problemas de reset
    fprintf(f_asm, "LOAD NULL              // evita problema da primeira instrucao com o reset ascincrono\n");

    // numero de bits da mantissa
    fprintf(f_asm, "\nLOAD %d\nSET  nbmant            // guarda num de bits da mantissa\n"  , nbmant);
    // numero de bits do expoente
    fprintf(f_asm, "\nLOAD %d\nSET  nbexp             // guarda num de bits do expoente\n\n", nbexpo);

    // numero zero
    fprintf(f_asm, "LOAD %d           // 0.0\nSET  float_zero", 1 << (nbmant+nbexpo-1));
    fprintf(f_asm, "        // guarda o num zero\n");

    // epsilon para convergencia de funcoes iterativas
    char numi[64], numf[64];
    epsilon_taylor(numi,numf);
    fprintf(f_asm, "\nLOAD %s           // %s epsilon usado em funcoes aritmeticas iterativas\n", numi, numf);
    fprintf(f_asm, "SET  epsilon_taylor\n");

    // pi sobre 2
    fprintf(f_asm, "\nLOAD %d           // pi/2 usado em funcoes trigonimetricas\n", f2mf("1.57079632679489661923"));
    fprintf(f_asm, "SET  pi_div_2\n\n");

    // inicializacao de dados genericos ---------------------------------------

        f_float =     fopen  ("../../Macros/float_init.asm", "r");
    if (f_float == 0) fprintf(stderr, "Cadê a macro float_init.asm? Tinha que estar na pasta do projeto do SAPHO!\n");
	do {          a = fgetc  (f_float); if (a != EOF) fputc(a, f_asm);} while (a != EOF);

	fprintf(f_asm, "\n// Codigo assembly original ---------------------------------------------------\n\n");
	fclose (f_float);
}

// incluir aqui, as macros de matematica que forem sendo criadas
void math_gen(char *fasm)
{
    FILE *f_asm = fopen(fasm,"a");

    char a;

    if (fsqrt == 1)
    {
        f_float  =    fopen  ("../../Macros/float_sqrt.asm", "r");
    if (f_float == 0) fprintf(stderr, "Cadê a macro float_sqrt.asm? Tinha que estar na pasta do projeto do SAPHO!\n");
	do {      a  =    fgetc  (f_float); if (a != EOF) fputc(a, f_asm);} while (a != EOF);
	                  fclose (f_float);
    }

    if (fsqrti == 1)
    {
        f_float  =    fopen  ("../../Macros/float_sqrt_i.asm", "r");
    if (f_float == 0) fprintf(stderr, "Cadê a macro float_sqrt_i.asm? Tinha que estar na pasta do projeto do SAPHO!\n");
	do {      a  =    fgetc  (f_float); if (a != EOF) fputc(a, f_asm);} while (a != EOF);
	                  fclose (f_float);
    }

    if (fatan == 1)
    {
        f_float  =    fopen  ("../../Macros/float_atan.asm", "r");
    if (f_float == 0) fprintf(stderr, "Cadê a macro float_atan.asm? Tinha que estar na pasta do projeto do SAPHO!\n");
	do {      a  =    fgetc  (f_float); if (a != EOF) fputc(a, f_asm);} while (a != EOF);
	                  fclose (f_float);
    }

    if (fatani == 1)
    {
        f_float  =    fopen  ("../../Macros/float_atan_i.asm", "r");
    if (f_float == 0) fprintf(stderr, "Cadê a macro float_atan_i.asm? Tinha que estar na pasta do projeto do SAPHO!\n");
	do {      a  =    fgetc  (f_float); if (a != EOF) fputc(a, f_asm);} while (a != EOF);
	                  fclose (f_float);
    }

    fclose(f_asm);
}

// gera codigo padrao para rodar float no proc ponto fixo
void float_geni(char *fasm)
{
    FILE *f_aux = fopen("c2aux.asm", "w");
    FILE *f_asm = fopen(fasm       , "r");

    char a;

    // copia todo o codigo original em assembler para o arquivo auxiliar
    do {a = fgetc(f_asm); if (a != EOF) fputc(a, f_aux);} while (a != EOF);
    fclose(f_aux);
    fclose(f_asm);

    f_aux = fopen("c2aux.asm", "r");
    f_asm = fopen(fasm       , "w");

    // copia o cabecalho para geracao de float em ponto fixo
    float_begin(f_asm);
    //agora recoloca o codigo original
    do {a = fgetc(f_aux); if (a != EOF) fputc(a, f_asm);} while (a != EOF);
    fclose(f_aux);

        f_float  =    fopen  ("../../Macros/float_gen.asm", "r");
    if (f_float == 0) fprintf(stderr, "Cadê a macro float_gen.asm? Tinha que estar na pasta do projeto do SAPHO!\n");
	do {      a  =    fgetc  (f_float); if (a != EOF) fputc(a, f_asm);} while (a != EOF);
	                  fclose (f_float);

    if (i2f)
    {
        f_float  =    fopen  ("../../Macros/float_i2f.asm", "r");
    if (f_float == 0) fprintf(stderr, "Cadê a macro float_i2f.asm? Tinha que estar na pasta do projeto do SAPHO!\n");
	do {      a  =    fgetc  (f_float); if (a != EOF) fputc(a, f_asm);} while (a != EOF);
	                  fclose (f_float);
    }

	if (f2i)
    {
        f_float  =    fopen  ("../../Macros/float_f2i.asm", "r");
    if (f_float == 0) fprintf(stderr, "Cadê a macro float_f2i.asm? Tinha que estar na pasta do projeto do SAPHO!\n");
	do {      a  =    fgetc  (f_float); if (a != EOF) fputc(a, f_asm);} while (a != EOF);
	                  fclose (f_float);
    }

    if (fadd)
    {
        f_float  =    fopen  ("../../Macros/float_soma.asm", "r");
    if (f_float == 0) fprintf(stderr, "Cadê a macro float_soma.asm? Tinha que estar na pasta do projeto do SAPHO!\n");
	do {      a  =    fgetc  (f_float); if (a != EOF) fputc(a, f_asm);} while (a != EOF);
	                  fclose (f_float);
    }

    if (fmlt)
    {
        f_float  =    fopen  ("../../Macros/float_mult.asm", "r");
    if (f_float == 0) fprintf(stderr, "Cadê a macro float_mult.asm? Tinha que estar na pasta do projeto do SAPHO!\n");
	do {      a  =    fgetc  (f_float); if (a != EOF) fputc(a, f_asm);} while (a != EOF);
	                  fclose (f_float);
    }

    if (fdiv)
    {
        f_float  =    fopen  ("../../Macros/float_div.asm", "r");
    if (f_float == 0) fprintf(stderr, "Cadê a macro float_div.asm? Tinha que estar na pasta do projeto do SAPHO!\n");
	do {      a  =    fgetc  (f_float); if (a != EOF) fputc(a, f_asm);} while (a != EOF);
	                  fclose (f_float);
    }

    fclose(f_asm);
}

// gera constantes trigonometricas
void float_genf(char *fasm)
{
    FILE *f_aux = fopen("c2aux.asm", "w");
    FILE *f_asm = fopen(fasm       , "r");

    char a;

    // copia todo o codigo original em assembler para o arquivo auxiliar
    do {a = fgetc(f_asm); if (a != EOF) fputc(a, f_aux);} while (a != EOF);
    fclose(f_aux);
    fclose(f_asm);

    f_aux = fopen("c2aux.asm", "r");
    f_asm = fopen(fasm       , "w");

    fprintf(f_asm, "// Inicializacao de parametros para operacoes aritmeticas ---------------------\n\n");

    // LOAD NULL deve ser a porimeira instrucao sempre, pra evitar problemas de reset
    fprintf(f_asm, "LOAD NULL                // evita problema da primeira instrucao com o reset ascincrono\n");

    // epsilon para convergencia de funcoes iterativas
    char numi[64], numf[64];
    epsilon_taylor(numi,numf);
    fprintf(f_asm, "\nLOAD %s           // epsilon usado em funcoes aritmeticas iterativas\n", numf);
    fprintf(f_asm, "SET  epsilon_taylor\n");

    fprintf(f_asm, "\n// Codigo assembly original ---------------------------------------------------\n\n");

    //agora recoloca o codigo original
    do {a = fgetc(f_aux); if (a != EOF) fputc(a, f_asm);} while (a != EOF);

    fclose(f_aux);
    fclose(f_asm);
}
