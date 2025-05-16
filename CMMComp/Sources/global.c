// ----------------------------------------------------------------------------
// funcoes e variaveis globais do compilador ----------------------------------
// ----------------------------------------------------------------------------

// includes globais
#include <stdarg.h>
#include <string.h>

// includes locais
#include "..\Headers\t2t.h"
#include "..\Headers\macros.h"
#include "..\Headers\global.h"
#include "..\Headers\variaveis.h"

// ----------------------------------------------------------------------------
// redeclaracao de variaveis globais ------------------------------------------
// ----------------------------------------------------------------------------

FILE *f_asm;          // arquivo de saida
FILE *f_log;          // arquivo de log, guarda informacoes do projeto (nome, nbmant etc)
FILE *f_lin;          // arquivo de linhas do programa em cmm

char dir_macro[1024]; // diretorio Macros
char dir_tmp  [1024]; // diretorio Temp
char dir_soft [1024]; // diretorio Software

// variaveis de estado
int  acc_ok   = 0;    // 0 -> acc vazio (use LOD)  , 1 -> acc carregado (use P_LOD)
int  line_num = 0;    // numero da linha sendo parseada
int  num_ins  = 0;    // numero de instrucoes do parse

// ----------------------------------------------------------------------------
// funcoes de inicio e termino do parse ---------------------------------------
// ----------------------------------------------------------------------------

void parse_init(char *prname, char *d_proc, char *d_macro, char *d_tmp)
{
    // pega os argumentos -----------------------------------------------------

    char cmm_file[1024];
    char asm_file[1024];

    sprintf(cmm_file, "%s/Software/%s.cmm", d_proc, prname);
    sprintf(asm_file, "%s/Software/%s.asm", d_proc, prname);

    yyin  = fopen(cmm_file, "r"); // arquivo .cmm de entrada
    f_asm = fopen(asm_file, "w"); // arquivo .asm de saida

    sprintf(dir_soft , "%s/Software", d_proc ); // pega o diretorio software
    strcpy (dir_macro,                d_macro); // pega o diretorio Macro
    strcpy (dir_tmp  ,                d_tmp  ); // pega o diretorio Tmp

    // cria arquivos auxiliares -------------------------------------------------

    char path[1024];

    sprintf(path,   "%s/cmm_log.txt", dir_tmp        ); f_log = fopen(path,"w"); // log com infos pro assembler e gtkwave
    sprintf(path, "%s/pc_%s_mem.txt", dir_tmp, prname); f_lin = fopen(path,"w"); // memoria no pc.v que passa de asm para cmm

    // gera uma instrucao LOD NULL no inicio (tentar tirar isso) ----------------

    add_sinst(-1,"LOD NULL\n");
}

void parse_end(char *prname, char *d_proc)
{
    // fecha os arquivos --------------------------------------------------------

    fclose(f_asm); // codigo   assembly
    fclose(f_lin); // tradutor assembly
    
    // checa se precisa adicionar macros no arquivo .asm ------------------------

    char asm_file[1024]; sprintf(asm_file, "%s/Software/%s.asm", d_proc, prname);

	mac_geni(asm_file);

	// checa consistencia de todas as variaveis e funcoes -----------------------
  
	check_var(); // (variaveis.c)

    // termina o arquivo de log do cmm ------------------------------------------

    fprintf(f_log, "num_ins %d\n", num_ins); // numero de instrucoes (sem macros finais)
    fclose (f_log);

    // gera o arquivo de traducao pro codigo cmm --------------------------------

    char     path[1024]; sprintf(path    , "%s/%s", dir_tmp,     "trad_cmm.txt");
    char cmm_file[1024]; sprintf(cmm_file, "%s/Software/%s.cmm", d_proc, prname);
  
    FILE *output = fopen(path    , "w");
    FILE *input  = fopen(cmm_file, "r");

    char linha[1001], texto[1001] = "";
    fputs("-1 INTERNO\n"     , output); // codigo para inicio do arquivo
    fputs("-2 void main();\n", output); // codigo pra CAL main
    fputs("-3 FIM\n"         , output); // codigo para @fim JMP fim
    fputs("-4 User Macro\n"  , output); // codigo asm do usuario (#USEMAC)

    int cnt = 1;
    while(fgets(texto, 1001, input) != NULL)
    {
        sprintf(linha, "%d %s", cnt++, texto);
        fputs(linha, output);
        memset(texto, 0, sizeof(char) * 1001);
    }

    fclose(input );
    fclose(output);
}

// ----------------------------------------------------------------------------
// funcoes para cadastro de instrucoes ----------------------------------------
// ----------------------------------------------------------------------------

// adiciona instrucao no arquivo asm
void add_instr(char *inst, ...)
{
    // adiciona instrucao -----------------------------------------------------

    va_list  args;
    va_start(args , inst);
    if (using_macro == 0) vfprintf(f_asm, inst, args);
    va_end  (args);

    // tabela para tradutor assembly do gtkwave -------------------------------

    if (using_macro == 0) num_ins++;
    if (using_macro == 0) fprintf(f_lin, "%s\n", itob(line_num+1,20));

    // verifica se instrucao precisa de NOP -----------------------------------

    char     str[100];
    vsprintf(str, inst, args);

    if (strstr(str,   "I2F") != NULL) add_instr("NOP\n");
    if (strstr(str, "F_ADD") != NULL) add_instr("NOP\n");
    if (strstr(str, "F_MLT") != NULL) add_instr("NOP\n");
    if (strstr(str, "F_DIV") != NULL) add_instr("NOP\n");
}

// adiciona instrucoes especiais
// type =  0 -> comentario
// type = -1 -> INTERNO
// type = -2 -> void main();
// type = -3 -> FIM
void add_sinst(int type, char *inst, ...)
{
    // adiciona instrucao -----------------------------------------------------

    va_list  args;
    va_start(args , inst);
    if (using_macro == 0) vfprintf(f_asm, inst, args);
    va_end  (args);

    // tabela para tradutor asselbly do gtkwave -------------------------------

    if (type != 0)
    {
        if (using_macro == 0) num_ins++;
        if (using_macro == 0) fprintf(f_lin, "%s\n", itob(type,20));
    }
}