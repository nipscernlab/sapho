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
void epsilon_taylor(char *fnum)
{
    // acha o dobro do menor valor possivel em float
    double numf = 2.0*pow(2, nbmant-1)*pow(2,-pow(2,nbexpo-1));
    // multiplica o resultado por um fator para garatir a estab. da funcao sin(x)
    // mudar para que isso soh use se a funcao sin(x) estiver presente
    numf = numf * 3.1415926535897932 * 3.1415926535897932 * 3.0;
    // se a precisao for grande, usa o padrao
    if     (numf < 0.0000001) numf = 0.0000001;
    sprintf(fnum, "%.7f",     numf);
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

// deve ser incluido no comeco do arquivo asm (para contas em ponto flutuante)
void header_float(char *fasm, char *pc_sim_mem)
{
    f_asm = fopen(fasm      , "w");
    f_lin = fopen(pc_sim_mem, "w");

    add_sinst(0, "// Gera variaveis auxiliares --------------------------------------------------\n\n");

    // NOP deve ser a primeira instrucao no endereco zero da memoria
    add_sinst(-1,"NOP\n");

    // epsilon para convergencia de funcoes iterativas
    char numf[64];
    epsilon_taylor(numf);
    add_sinst(-1, "LOD %s\n", numf);
    add_sinst(-1, "SET epsilon_taylor\n\n");

    add_sinst(0, "// Codigo assembly original ---------------------------------------------------\n\n");

    fclose(f_asm);
    fclose(f_lin);
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

// adiciona uma macro pre-definida --------------------------------------------

void mac_add (char *name)
{
         if (strcmp(name, "idiv" ) == 0) idiv  = 1; // divisao inteira
    else if (strcmp(name, "imod" ) == 0) imod  = 1; // resto da divisao inteira
    else if (strcmp(name, "finv" ) == 0) finv  = 1; // inverso de float
    else if (strcmp(name, "fsqrt") == 0) fsqrt = 1; // raiz quadrada de float
    else if (strcmp(name, "fatan") == 0) fatan = 1; // arco tangente de float
    else if (strcmp(name, "fsin" ) == 0) fsin  = 1; // seno de float
}

// gera as macros pre-definidas no arquivo assembler --------------------------

void mac_gera(char *fasm)
{
    // se nao tiver nada pra fazer, sai!
    if (!(idiv || imod || finv || fsqrt || fatan || fsin)) return;

    char tasm[1024]; // arquivo temporario para o asm
    char tmem[1024]; // arquivo temporario para a tabela de memoria
    char fmem[1024]; // arquivo final para a tabela de memoria

    sprintf(tasm, "%s/%s", dir_tmp, "tasm.txt");
    sprintf(tmem, "%s/%s", dir_tmp, "tmem.txt");
    sprintf(fmem, "%s/pc_%s_mem.txt", dir_tmp, prname);

    // cria os cabecalhos pra float -------------------------------------------

    if (finv || fsqrt || fatan || fsin) header_float(tasm,tmem);

    // coloca os cabecalhos no inicio dos arquivos ----------------------------

    fcat2begin(fasm,tasm);
    fcat2begin(fmem,tmem);

    // coloca o resto que precisa no final do asm -----------------------------

    if (idiv)
    {
         sprintf(tasm, "%s/%s", dir_macro, "int_div.asm");
        fcat2end(tasm,fasm);
    }

    if (imod)
    {
         sprintf(tasm, "%s/%s", dir_macro, "int_mod.asm");
        fcat2end(tasm,fasm);
    }

    if (finv)
    {
         sprintf(tasm, "%s/%s", dir_macro, "float_inv.asm");
        fcat2end(tasm,fasm);
    }

    if (fsqrt)
    {
         sprintf(tasm, "%s/%s", dir_macro, "float_sqrt.asm");
        fcat2end(tasm,fasm);
    }

    if (fatan)
    {
         sprintf(tasm, "%s/%s", dir_macro, "float_atan.asm");
        fcat2end(tasm,fasm);
    }

    if (fsin)
    {
         sprintf(tasm, "%s/%s", dir_macro, "float_sin.asm");
        fcat2end(tasm,fasm);
    }
}