// ----------------------------------------------------------------------------
// funcoes e variaveis pra criacao e utilizacao de macros ---------------------
// ----------------------------------------------------------------------------

// includes globais
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// includes locais
#include "..\Headers\t2t.h"
#include "..\Headers\global.h"
#include "..\Headers\funcoes.h"
#include "..\Headers\diretivas.h"
#include "..\Headers\variaveis.h"

// ----------------------------------------------------------------------------
// gerenciamento de macros criadas pelo usuario -------------------------------
// ----------------------------------------------------------------------------

// redeclaracao de variaveis globais ------------------------------------------

int mac_using = 0; // se estiver lendo uma macro, nao deve escrever o assembler durante o parse

// nao deixa o parser escrever no arquivo assembler ---------------------------
// ao inves disso, copia o codigo de uma macro
void mac_use(int ids, int global, int id_num)
{
    if (mac_using == 1)
        fprintf(stderr, "Erro na linha %d: tá chamando uma macro dentro da outra. você é uma pessoa confusa!\n", line_num+1);

    // se for global, tem q ver se tem que chamar a funcao main ainda ---------

    if ((mainok == 0) && (global == 1))
    {
        add_sinst(-2, "CAL main\n");
        add_sinst(-3, "@fim JMP fim\n");

        mainok = 2; // funcao main foi chamada no inicio
    }

    // remover as aspas da string (trabalho da porra!) ------------------------
    // mudar para um codigo mais simples como o que ta em array.c -------------

    char f_name[64];
    strcpy(f_name, v_name[ids]);
    char file_name[512];
    int  tamanho = strlen(f_name); // tamanho da string
    int idxToDel = tamanho-1;      // indice para deletar, nesse caso o ultimo, as aspas.
    strcpy ( file_name, "");
    memmove(&f_name[idxToDel], &f_name[idxToDel+1], 1); // deletando de fato as ultimas aspas
    strcat ( file_name, f_name+1); // agora copia, tirando as primeiras aspas

    char    mac_name[1024];
    sprintf(mac_name, "%s/%s", dir_soft, file_name);

    // copia o codigo do arquivo asm ------------------------------------------

    FILE *f_macro;
    char a;
        f_macro  =    fopen  (mac_name, "r");
    if (f_macro == 0) fprintf(stderr, "Erro na linha %d: cadê a macro %s? Tinha que estar na pasta Software!\n", line_num+1, file_name);
	do {      a  =    fgetc  (f_macro); if (a != EOF) fputc(a,f_asm);} while (a != EOF);
                      fputc  ('\n',f_asm);
	                  fclose (f_macro);

    // preenche tabela de codigo cmm com -1 (INTERNO) -------------------------
    
    int n = atoi(v_name[id_num]);
    for (int i = 0; i < n; i++)
    {
        num_ins++;
        fprintf(f_lin, "%s\n", itob(-4,20));
    }

    mac_using = 1;
}

// libera o parser pra salvar no arquivo assembler ----------------------------

void mac_end()
{
    if (mac_using == 0) fprintf(stderr, "Erro na linha %d: não estou achando o começo da macro\n", line_num+1);
        mac_using  = 0;
}

// ----------------------------------------------------------------------------
// funcoes auxiliares para geracao de macros pre-definidas --------------------
// ----------------------------------------------------------------------------

// valor a ser usado na convergencia dos funcoes aritmeticas iterativas
// o inum pega uma string com o numero inteiro equivalente
// o fnum pega uma string com o proprio numero float
void epsilon_taylor()
{
    // acha o dobro do menor valor possivel em float
    double numf = 2.0*pow(2, nbmant-1)*pow(2,-pow(2,nbexpo-1));
    // multiplica o resultado por um fator para garatir a estab. da funcao sin(x)
    // mudar para que isso soh use se a funcao sin(x) estiver presente
    numf = numf * 3.1415926535897932 * 3.1415926535897932 * 3.0;
    // se a precisao for grande, usa o padrao
    if     (numf < 0.0000001) numf = 0.0000001;
    char fnum[64]; sprintf(fnum, "%.7f",  numf);
    // escreve a variavel no arquivo de log
    fprintf(f_log, "epsilon_taylor %s\n", fnum);
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

// ----------------------------------------------------------------------------
// gerenciamento de macros pre-definidas --------------------------------------
// ----------------------------------------------------------------------------

// variaveis locais -----------------------------------------------------------

int idiv  = 0; // se vai precisar de macro de divisao entre inteiros
int imod  = 0; // se vai precisar de macro para resto da divisao
int finv  = 0; // se vai precisar de macro para inerter um float
int fatan = 0; // se vai precisar de macro pra arco tangente
int fsqrt = 0; // se vai precisar de macro pra raiz quadrada
int fsin  = 0; // se vai precisar de macro pra seno

// adiciona flag de uma macro pre-definida ------------------------------------

void mac_add(char *name)
{
         if (strcmp(name, "idiv" ) == 0) idiv  = 1; // divisao inteira
    else if (strcmp(name, "imod" ) == 0) imod  = 1; // resto da divisao inteira
    else if (strcmp(name, "finv" ) == 0) finv  = 1; // inverso de float
    else if (strcmp(name, "fsqrt") == 0) fsqrt = 1; // raiz quadrada de float
    else if (strcmp(name, "fatan") == 0) fatan = 1; // arco tangente de float
    else if (strcmp(name, "fsin" ) == 0) fsin  = 1; // seno de float
}

// copia as macros pre-definidas no final arquivo assembler -------------------

void mac_copy(char *fasm)
{
    // se nao tiver nada pra fazer, sai! --------------------------------------

    if (!(idiv || imod || finv || fsqrt || fatan || fsin)) return;

    // cria constantes especiais (quando necessario ) -------------------------

    if (fsqrt || fatan || fsin) epsilon_taylor();

    // copia o que precisa no final do asm ------------------------------------

    char tasm[1024]; sprintf(tasm, "%s/%s", dir_tmp, "tasm.txt");

    if (idiv)
    {
        printf("Info: adding assembly macro for fixed-point division by software.\n");
        sprintf(tasm, "%s/int_div_pl%d.asm", dir_macro, pipeln);
        fcat2end(tasm,fasm);
    }

    if (imod)
    {
        printf("Info: adding assembly macro for module operation by software.\n");
        sprintf(tasm, "%s/int_mod_pl%d.asm", dir_macro, pipeln);
        fcat2end(tasm,fasm);
    }

    if (finv)
    {
        printf("Info: adding assembly macro for float-point division by software.\n");
        sprintf(tasm, "%s/float_inv_pl%d.asm", dir_macro, pipeln);
        fcat2end(tasm,fasm);
    }

    if (fsqrt)
    {
        printf("Info: adding assembly macro for root square computation.\n");
        sprintf(tasm, "%s/float_sqrt_pl%d.asm", dir_macro, pipeln);
        fcat2end(tasm,fasm);
    }

    if (fatan)
    {
        printf("Info: adding assembly macro for arc-tangent computation.\n");
        sprintf(tasm, "%s/float_atan_pl%d.asm", dir_macro, pipeln);
        fcat2end(tasm,fasm);
    }

    if (fsin)
    {
        printf("Info: adding assembly macro for sin computation.\n");
        sprintf(tasm, "%s/float_sin_pl%d.asm", dir_macro, pipeln);
        fcat2end(tasm,fasm);
    }
}

// ----------------------------------------------------------------------------
// backup do codigo em c+- das macros pre-definidas ---------------------------
// ----------------------------------------------------------------------------

// divisao para int (int_div.asm)
/*int divide(int num, int den)
{
    int sig = sign(num,1)*den;

    num = abs(num);
    den = abs(den);

    int result = 0;

    int shift  = 0;
    int dens   = den;
    while ((dens > 0) && (dens <= num))
    {
        shift++;
        dens = den << shift;
    }

    shift = shift -  1;
    while  (shift >= 0)
    {
        dens = den << shift;
        if (dens <= num)
        {
            num = num - dens;
            result = result + (1 << shift);
        }
        shift = shift-1;
    }

    return sign(sig, result);
}*/

// resto da divisao por inteiros (int_mod.asm)
/*int int_mod(int a, int b)
{
    return a-(a/b)*b;
}*/

// inverso de um float (float_inv.asm)
/*float float_inv(float x)
{
    float s = sign(x, 1.0);
    x = abs(x);

    int k = 0;
    while (x > 1.5)
    {
        x = x * 0.5;
        k++;
    }

    while (x < 0.5)
    {
        x = x * 2.0;
        k = k-1;
    }

    float y = 1.0;

    int m = 0;
    while (m < 6)
    {
        y = y*(2.0 - x*y);
        m++;
    }

    while (k > 0)
    {
        y = y*0.5;
        k = k-1;
    }

    while (k < 0)
    {
        y = y*2.0;
        k++;
    }

    return y*s;
}*/

// raiz quadrada para float (macro float_sqrt.asm)
/*float my_sqrt(float num)
{
    float x = num;
    float epslon = 0.000008;  // menor numero possivel = 2^(m-1)*2^(-(2^(e-1)))
                              // para m = 16 e = 6, o num eh: 0.000007629...
    while (1)
    {
        float raiz = 0.5 * (x+num/x);
        if (fabs(x - raiz) < epslon) break;
        x = raiz;
    }

    return raiz;
}*/

// arco-tg para float (float_atan.asm)
/*float float_atan(float x)
{
    float ax = abs(x);

    if (ax == 0.0) return 0.0;
    if (ax > 1.02) return sign(x, 1.5707963268) - my_atan(1.0/x);
    if (ax > 0.98)
    {
        float xm1 = ax-1;
        return sign(x, 0.7853981634 + xm1*0.5 - xm1*xm1*0.25); 
    }

    float termo      = x;
    float x2         = x*x;
    float resultado  = termo;
    float tolerancia = 0.000008/x2;

    int indiceX = 3;

    while ((abs(termo) > tolerancia) && (indiceX < 100)) {
        termo = termo * (- x2 * (indiceX - 2)) / indiceX;

        resultado = resultado + termo;
        indiceX = indiceX + 2;
    }

    return resultado;
}*/

// seno para float (float_sin.asm)
/*float sin(float x)
{
    if (x == 0) return 0.0;
    
    while (abs(x) > 3.141592654) x = x - sign(x, 6.283185307);

    float termo      = x;
    float x2         = x * x;
    float resultado  = termo;
    float tolerancia = 0.000008/x2;

    int indiceX = 3;

    while (abs(termo) > tolerancia) {
        termo = termo * (- x2) / ((indiceX - 1) * indiceX);
        resultado = resultado + termo;
        indiceX = indiceX + 2;
    }

    return resultado;
}*/