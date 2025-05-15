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
// redeclaracao de variaveis globais ------------------------------------------
// ----------------------------------------------------------------------------

int using_macro = 0; // se estiver lendo uma macro, nao deve escrever o assembler durante o parse

int fatan  = 0;      // se vai precisar de macro pra arco tangente
int fsqrt  = 0;      // se vai precisar de macro pra raiz quadrada
int fsin   = 0;      // se vai precisar de macro pra seno

// ----------------------------------------------------------------------------
// gerenciamento de macros criadas pelo usuario -------------------------------
// ----------------------------------------------------------------------------

// nao deixa o parser escrever no arquivo assembler
// ao inves disso, copia o codigo de uma macro
void dire_macro(int ids, int global, int id_num)
{
    if (using_macro == 1)
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

    using_macro = 1;
}

// libera o parser pra salvar no arquivo assembler
void dire_maend()
{
    if (using_macro == 0) fprintf(stderr, "Erro na linha %d: não estou achando o começo da macro\n", line_num+1);
        using_macro  = 0;
}

// ----------------------------------------------------------------------------
// funcoes auxiliares para geracao de macros ----------------------------------
// ----------------------------------------------------------------------------

// valor a ser usado na convergencia dos funcoes aritmeticas iterativas
// o inum pega uma string com o numero inteiro equivalente
// o fnum pega uma string com o proprio numero float
void epsilon_taylor(char *fnum)
{
    // acha o dobro do menor valor possivel em float
    double numf = 2.0*pow(2, nbmant-1)*pow(2,-pow(2,nbexpo-1));
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

// deve ser incluido no comeco do arquivo asm (para proc ponto fixo)
void header_int(char *fasm, char *pc_sim_mem)
{
    f_asm = fopen(fasm      , "w");
    f_lin = fopen(pc_sim_mem, "w");

    add_sinst(0, "// Gera variaveis auxiliares --------------------------------------------------\n\n");

    // LOD NULL deve ser a primeira instrucao sempre, pra evitar problemas de reset
    add_sinst(-1, "LOD NULL              // evita problema da primeira instrucao com o reset ascincrono\n");

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
// geracao do codigo em assembly para as operacoes encontradas ----------------
// ----------------------------------------------------------------------------

// gera macros pra ponto fixo
void mac_geni(char *fasm)
{
    // se nao tiver nada pra fazer, sai!
    if (!(fsqrt || fatan || fsin)) return;

    char tasm[1024]; // arquivo temporario para o asm
    char tmem[1024]; // arquivo temporario para a tabela de memoria
    char fmem[1024]; // arquivo final para a tabela de memoria

    sprintf(tasm, "%s/%s", dir_tmp, "tasm.txt");
    sprintf(tmem, "%s/%s", dir_tmp, "tmem.txt");
    sprintf(fmem, "%s/pc_%s_mem.txt", dir_tmp, prname);

    // cria os cabecalhos -----------------------------------------------------

    header_int(tasm,tmem);

    // coloca os cabecalhos no inicio dos arquivos ----------------------------

    fcat2begin(fasm,tasm);
    fcat2begin(fmem,tmem);

    // coloca o resto que precisa no final do asm -----------------------------

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