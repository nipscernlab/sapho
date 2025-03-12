#include "..\Headers\t2t.h"
#include "..\Headers\diretivas.h"
#include "..\Headers\global.h"
#include "..\Headers\data_use.h"
#include "..\Headers\variaveis.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

// ----------------------------------------------------------------------------
// redeclaracao de variaveis globais ------------------------------------------
// ----------------------------------------------------------------------------

char dir_macro[1024]; // diretorio Macros
char dir_tmp  [1024]; // diretorio Temp
char dir_soft [1024]; // diretorio Software

FILE *f_float;

// ----------------------------------------------------------------------------
// conversao de dados ---------------------------------------------------------
// ----------------------------------------------------------------------------

// converte um numero float (escrito em uma string) em meu float
unsigned int f2mf(char *va)
{
    float f = atof(va);

    if (f == 0.0) return 1 << (nbmant + nbexpo -1);

    int *ifl = (int*)&f;

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
    return (unsigned)(s + e + m);
}

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

// deve ser incluido no comeco do arquivo asm (para proc ponto fixo)
void header_int(char *fasm, char *pc_sim_mem)
{
    f_asm = fopen(fasm      , "w");
    f_lin = fopen(pc_sim_mem, "w");

    add_sinst(0, "// Inicializacao da emulacao do ponto flutuante em software -------------------\n\n");

    // LOAD NULL deve ser a primeira instrucao sempre, pra evitar problemas de reset
    add_sinst(-1, "LOAD NULL              // evita problema da primeira instrucao com o reset ascincrono\n\n");

    // numero de bits da mantissa
    add_sinst(-1, "LOAD %d\n", nbmant);
    add_sinst(-1, "SET  nbmant            // guarda num de bits da mantissa\n\n");
    
    // numero de bits do expoente
    add_sinst(-1, "LOAD %d\n", nbexpo);
    add_sinst(-1, "SET  nbexp             // guarda num de bits do expoente\n\n");

    // numero zero
    add_sinst(-1, "LOAD %d           // 0.0\n", 1 << (nbmant+nbexpo-1));
    add_sinst(-1, "SET  float_zero        // guarda o num zero\n\n");

    // epsilon para convergencia de funcoes iterativas
    char numi[64], numf[64];
    epsilon_taylor(numi,numf);
    add_sinst(-1, "LOAD %s           // %s epsilon usado em funcoes aritmeticas iterativas\n", numi, numf);
    add_sinst(-1, "SET  epsilon_taylor\n\n");

    // pi sobre 2
    add_sinst(-1, "LOAD %d           // pi/2 usado em funcoes trigonimetricas\n",f2mf("1.57079632679489661923"));
    add_sinst(-1, "SET  pi_div_2\n\n");

    // 1/2
    add_sinst(-1, "LOAD %d           // 1/2\n", f2mf("0.5"));
    add_sinst(-1, "SET  um_div_2\n\n");

    // tudo 1 menos bit de sinal
    add_sinst(-1, "LOAD %d           // zera o bit de sinal (abs pra float em software)\n", (int)pow(2,nbmant+nbexpo)-1);
    add_sinst(-1, "SET  tudo_um\n\n");

    // numero 1.0
    add_sinst(-1, "LOAD %d           // numero 1.0 em float\n", f2mf("1.0"));
    add_sinst(-1, "SET  num_um\n\n");

    // numero de bits total
    add_sinst(-1, "LOAD nbmant            // guarda numero de bits total\n");
    add_sinst(-1, "ADD  nbexp\n");
    add_sinst(-1, "SET  float_nbits\n\n");

    // nbmant+1
    add_sinst(-1, "LOAD nbmant            // variavel auxiliar\n");
    add_sinst(-1, "ADD  1\n");
    add_sinst(-1, "SET  nbmantp1\n\n");

    // nbexp+1
    add_sinst(-1, "LOAD nbexp             // variavel auxiliar\n");
    add_sinst(-1, "ADD  1\n");
    add_sinst(-1, "SET  nbexpp1\n\n");

    add_sinst(0, "// Codigo assembly original ---------------------------------------------------\n\n");

    fclose(f_asm);
    fclose(f_lin);
}

// deve ser incluido no comeco do arquivo asm (para proc float)
void header_float(char *fasm, char *pc_sim_mem)
{
    f_asm = fopen(fasm      , "w");
    f_lin = fopen(pc_sim_mem, "w");

    add_sinst(0, "// Inicializacao de parametros para operacoes aritmeticas ---------------------\n\n");

    // LOAD NULL deve ser a porimeira instrucao sempre, pra evitar problemas de reset
    add_sinst(-1, "LOAD NULL                // evita problema da primeira instrucao com o reset ascincrono\n\n");

    // epsilon para convergencia de funcoes iterativas
    char numi[64], numf[64];
    epsilon_taylor(numi,numf);
    add_sinst(-1, "LOAD %s           // epsilon usado em funcoes aritmeticas iterativas\n", numf);
    add_sinst(-1, "SET  epsilon_taylor\n\n");

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
    if (!(fadd || fmlt || fdiv || fsqrti || fatani)) return;

    char tasm[1024]; // arquivo temporario para o asm
    char tmem[1024]; // arquivo temporario para a tabela de memoria
    char fmem[1024]; // arquivo final para a tabela de memoria

    sprintf  (tasm, "%s/%s", dir_tmp, "tasm.txt");
    sprintf  (tmem, "%s/%s", dir_tmp, "tmem.txt");
    sprintf  (fmem, "%s/pc_%s_mem.txt", dir_tmp, pr_name);

    // cria os cabecalhos -----------------------------------------------------

    header_int(tasm,tmem);

    // coloca os cabecalhos no inicio dos arquivos ----------------------------

    fcat2begin(fasm,tasm);
    fcat2begin(fmem,tmem);

    // coloca o resto que precisa no final do asm -----------------------------

         sprintf(tasm, "%s/%s", dir_macro, "float_gen.asm");
        fcat2end(tasm,fasm);

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

// gera macros pra ponto flutuante
void mac_genf(char *fasm)
{
    // se nao tem nada pra fazer, sai!
    if (!(fsqrt || fatan)) return;

    char tasm[1024]; // arquivo temporario para o asm
    char tmem[1024]; // arquivo temporario para a tabela de memoria
    char fmem[1024]; // arquivo final para a tabela de memoria

    sprintf  (tasm, "%s/%s", dir_tmp, "tasm.txt");
    sprintf  (tmem, "%s/%s", dir_tmp, "tmem.txt");
    sprintf  (fmem, "%s/pc_%s_mem.txt", dir_tmp, pr_name);

    // cria os cabecalhos -----------------------------------------------------

    header_float(tasm,tmem);

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
}