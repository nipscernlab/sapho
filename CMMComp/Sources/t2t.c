#include "..\Headers\t2t.h"
#include "..\Headers\diretivas.h"
#include "..\Headers\global.h"
#include "..\Headers\data_use.h"
#include "..\Headers\variaveis.h"
#include "..\Headers\stdlib.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

// ----------------------------------------------------------------------------
// redeclaracao de variaveis globais ------------------------------------------
// ----------------------------------------------------------------------------

FILE *f_float;

// ----------------------------------------------------------------------------
// conversao de dados ---------------------------------------------------------
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

// ----------------------------------------------------------------------------
// funcoes auxiliares de acesso a terminais -----------------------------------
// ----------------------------------------------------------------------------

// pega o tipo da variavel
int get_type(int et)
{
    int t;

         if (et <   OFST) t = 0; // indefinido
    else if (et < 2*OFST) t = 1; // int
    else if (et < 3*OFST) t = 2; // float
    else if (et < 4*OFST) t = 3; // comp real
    else if (et < 5*OFST) t = 4; // comp img
    else if (et < 6*OFST) t = 5; // comp const (ex: 3+7.5i)
    else                  t =-1;

    return t;
}

// pega o id da parte imag de uma var complexa
// a parte real esta no param id
int get_img_id(int id)
{
       char name[1024];
    sprintf(name, "%s_i", v_name[id]);

       if (find_var(name) == -1) add_var(name);
    return find_var(name);
}

// separa a parte real e imaginaria de uma constante complexa
// gerando duas entradas na tabela pra ponto flutuante
void get_cmp_cst(int et, int *et_r, int *et_i)
{
    char  txt[64];
    float real, img;

    sscanf(v_name[et % OFST],"%f %f",&real,&img);

    sprintf(txt,"%f",real);
    *et_r = 2*OFST + exec_num(txt);

    sprintf(txt,"%f",img);
    *et_i = 2*OFST + exec_num(txt);
}

// gera ID estendido float pra parte real e imaginaria
// de um num complexo na memoria
void get_cmp_ets(int et, int *et_r, int *et_i)
{
    *et_r = 2*OFST + (et % OFST);
    *et_i = 2*OFST + get_img_id(et % OFST);
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
    if (!(fsqrti || fatani)) return;

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

    if (fsqrti)
    {
         sprintf(tasm, "%s/%s", dir_macro, "float_sqrt_i.asm");
        fcat2end(tasm,fasm);
    }

    if (fatani)
    {
         sprintf(tasm, "%s/%s", dir_macro, "float_atan_i.asm");
        fcat2end(tasm,fasm);
    }
}