// rotinas para conversao entre int e float

#include "..\Headers\t2t.h"
#include "..\Headers\diretivas.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

// redeclaracao de variaveis globais
char dir_macro[1024]; // diretorio de macros
char dir_tmp  [1024]; // diretorio temporario

FILE *f_float;

// ----------------------------------------------------------------------------
// funcoes auxiliares ---------------------------------------------------------
// ----------------------------------------------------------------------------

// converte o inteiro x para binario de comprimento w
// tentar mudar para conseguir converter int maior de 32 bits
char *itob(int x, int w)
{
	int z;
    char *b = (char *) malloc(w+1);
    b[0] = '\0';

	int s = (w > 31) ? 31 : w;
	if (w > 31)
    {
        for (z = 0; z < w- 31; z++)
            if (x < 0) strcat(b,"1");
            else       strcat(b,"0");
    }

    for (z = pow(2,s-1); z > 0; z >>= 1)
		strcat(b, ((x & z) == z) ? "1" : "0");

    return b;
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

// cria um novo arquivo e copia o conteudo
void copy_file(char *n_in, char *n_out)
{
    FILE *f_in  = fopen(n_in , "r");
    FILE *f_out = fopen(n_out, "w");

    char a;
    do {a = fgetc(f_in); if (a != EOF) fputc(a, f_out);} while (a != EOF);

    fclose(f_in );
    fclose(f_out);
}

// concatena conteudo do arquivo read no arquivo write
void fcat2end(char *n_read, char *n_write)
{
    FILE *f_in  = fopen(n_read , "r");
    FILE *f_out = fopen(n_write, "a");

    char a;
    do {a = fgetc(f_in); if (a != EOF) fputc(a, f_out);} while (a != EOF);

    fclose(f_in );
    fclose(f_out);
}

// concatena, no inicio, conteudo do arquivo app no arquivo orig
void fcat2begin(char *n_orig, char *n_app)
{
    char swap [1024];
    sprintf(swap , "%s/%s", dir_tmp, "swap.txt");

    copy_file(n_orig,  swap);
    copy_file(n_app ,n_orig);
    fcat2end (swap  ,n_orig);
}

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

// ----------------------------------------------------------------------------
// geracao do codigo em assembly para as operacoes encontradas ----------------
// ----------------------------------------------------------------------------

// deve ser incluido no comeco do arquivo asm (para proc ponto fixo)
void float_begin(char *fasm, char *pc_sim_mem)
{
    char a;
    FILE *f_asm = fopen(fasm, "w");

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

    char path[1024];
    sprintf(path, "%s/%s", dir_macro, "float_init.asm");

        f_float =     fopen  (path, "r");
    if (f_float == 0) fprintf(stderr, "Cad� a macro float_init.asm? Tinha que estar na pasta do projeto do SAPHO!\n");
	do {          a = fgetc  (f_float); if (a != EOF) fputc(a, f_asm);} while (a != EOF);

	fprintf(f_asm, "\n// Codigo assembly original ---------------------------------------------------\n\n");

	fclose (f_float);
    fclose (f_asm);

    // arquivo de memoria do pc_sim.v -----------------------------------------

    int soma_inst = 20; // tem que mudar se acrescentar mais instrucoes acima

    FILE *f_mem = fopen(pc_sim_mem, "w");

    for (int i=0;i<soma_inst;i++) fprintf(f_mem, "%s\n", itob(-1,20));
    num_ins += soma_inst;

    fclose(f_mem);
}

// incluir aqui, as macros de matematica que forem sendo criadas
void math_gen(char *fasm)
{
    FILE *f_asm = fopen(fasm,"a");

    char path[1024];

    char a;

    if (fsqrt == 1)
    {
        sprintf(path, "%s/%s", dir_macro, "float_sqrt.asm");
        f_float  =    fopen  (path, "r");
    if (f_float == 0) fprintf(stderr, "Cad� a macro float_sqrt.asm? Tinha que estar na pasta do projeto do SAPHO!\n");
	do {      a  =    fgetc  (f_float); if (a != EOF) fputc(a, f_asm);} while (a != EOF);
	                  fclose (f_float);
    }

    if (fsqrti == 1)
    {
        sprintf(path, "%s/%s", dir_macro, "float_sqrti.asm");
        f_float  =    fopen  (path, "r");
    if (f_float == 0) fprintf(stderr, "Cad� a macro float_sqrt_i.asm? Tinha que estar na pasta do projeto do SAPHO!\n");
	do {      a  =    fgetc  (f_float); if (a != EOF) fputc(a, f_asm);} while (a != EOF);
	                  fclose (f_float);
    }

    if (fatan == 1)
    {
        sprintf(path, "%s/%s", dir_macro, "float_atan.asm");
        f_float  =    fopen  (path, "r");
    if (f_float == 0) fprintf(stderr, "Cad� a macro float_atan.asm? Tinha que estar na pasta do projeto do SAPHO!\n");
	do {      a  =    fgetc  (f_float); if (a != EOF) fputc(a, f_asm);} while (a != EOF);
	                  fclose (f_float);
    }

    if (fatani == 1)
    {
        sprintf(path, "%s/%s", dir_macro, "float_atan_i.asm");
        f_float  =    fopen  (path, "r");
    if (f_float == 0) fprintf(stderr, "Cad� a macro float_atan_i.asm? Tinha que estar na pasta do projeto do SAPHO!\n");
	do {      a  =    fgetc  (f_float); if (a != EOF) fputc(a, f_asm);} while (a != EOF);
	                  fclose (f_float);
    }

    fclose(f_asm);
}

// gera codigo asm para rodar float no proc ponto fixo
void float_geni(char *fasm)
{
    char tasm[1024];
    char tmem[1024];
    char fmem[1024];

    sprintf  (tasm, "%s/%s", dir_tmp, "tasm.txt");
    sprintf  (tmem, "%s/%s", dir_tmp, "tmem.txt");
    sprintf  (fmem, "%s/pc_%s_mem.txt", dir_tmp, pr_name);

    // cria os cabecalhos -----------------------------------------------------

    float_begin(tasm,tmem);

    // coloca os cabecalhos no inicio dos arquivos ----------------------------

    fcat2begin(fasm,tasm);
    fcat2begin(fmem,tmem);

    // coloca o resto que precisa no final do asm -----------------------------

     sprintf(tasm, "%s/%s", dir_macro, "float_gen.asm");
    fcat2end(tasm,fasm);

    if (i2f)
    {
         sprintf(tasm, "%s/%s", dir_macro, "float_i2f.asm");
        fcat2end(tasm,fasm);
    }

	if (f2i)
    {
         sprintf(tasm, "%s/%s", dir_macro, "float_f2i.asm");
        fcat2end(tasm,fasm);
    }

    if (fadd)
    {
         sprintf(tasm, "%s/%s", dir_macro, "float_soma.asm");
        fcat2end(tasm,fasm);
    }

    if (fmlt)
    {
         sprintf(tasm, "%s/%s", dir_macro, "float_mult.asm");
        fcat2end(tasm,fasm);
    }

    if (fdiv)
    {
         sprintf(tasm, "%s/%s", dir_macro, "float_div.asm");
        fcat2end(tasm,fasm);
    }
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
