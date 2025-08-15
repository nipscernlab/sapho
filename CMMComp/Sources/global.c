// ----------------------------------------------------------------------------
// funcoes e variaveis globais do compilador ----------------------------------
// ----------------------------------------------------------------------------

// includes globais
#include <stdarg.h>
#include <string.h>
#include  <ctype.h>

// includes locais
#include "..\Headers\t2t.h"
#include "..\Headers\macros.h"
#include "..\Headers\global.h"
#include "..\Headers\diretivas.h"
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
int  sim_arr  = 0;    // diz se simula ou nao array

// ----------------------------------------------------------------------------
// funcoes auxiliares ---------------------------------------------------------
// ----------------------------------------------------------------------------

// funcao para achar a instrucao correta na funcao add_instr
int find_opc(const char *opc, const char *str)
{
    size_t len = strlen(opc);
    const char *p = str;

    while ((p = strstr(p, opc)) != NULL)
    {
        char before = (p == str) ? '\0' : *(p - 1);
        char after = *(p + len);

        int before_ok = (p == str) || before == '\0' || isspace((unsigned char)before);
        int  after_ok =                after == '\0' || isspace((unsigned char) after);

        if (before_ok && after_ok) return 1;

        p += len;
    }

    return 0;
}

// Substitui ⟨ por < e ⟩ por >
// para mostrar no gtkwave
void substituir_braket(char *str)
{
    unsigned char *src = (unsigned char *)str;
    unsigned char *dst = (unsigned char *)str;

    while (*src)
    {
        // ⟨ = 0xE2 0x9F 0xA8
        if (src[0] == 0xE2 && src[1] == 0x9F && src[2] == 0xA8)
        {
            *dst++ = '<';
            src += 3;
        }
        // ⟩ = 0xE2 0x9F 0xA9
        else if (src[0] == 0xE2 && src[1] == 0x9F && src[2] == 0xA9)
        {
            *dst++ = '>';
            src += 3;
        }
        else *dst++ = *src++;
    }
    *dst = '\0';
}

// ----------------------------------------------------------------------------
// funcoes de inicio e termino do parse ---------------------------------------
// ----------------------------------------------------------------------------

void parse_init(char *f_name, char *prname, char *d_proc, char *d_macro, char *d_tmp, char *d_array)
{
    // pega os argumentos -----------------------------------------------------

    char cmm_file[1024]; sprintf(cmm_file, "%s/Software/%s"    , d_proc, f_name); // nome do arquivo .cmm de entrada
    char asm_file[1024]; sprintf(asm_file, "%s/Software/%s.asm", d_proc, prname); // nome do arquivo .asm de saida

    yyin  = fopen(cmm_file, "r"); // abre arquivo .cmm
    f_asm = fopen(asm_file, "w"); // cria arquivo .asm

    sprintf(dir_soft , "%s/Software", d_proc ); // pega o diretorio Software
    strcpy (dir_macro,                d_macro); // pega o diretorio Macro
    strcpy (dir_tmp  ,                d_tmp  ); // pega o diretorio Tmp

    sim_arr = strcmp(d_array, "1") == 0;

    // cria arquivos auxiliares -----------------------------------------------

    char path[1024];

    sprintf(path,   "%s/cmm_log.txt", dir_tmp        ); f_log = fopen(path,"w"); // log com infos pro assembler e gtkwave
    sprintf(path, "%s/pc_%s_mem.txt", dir_tmp, prname); f_lin = fopen(path,"w"); // memoria no pc.v que passa de asm para cmm

    // gera uma instrucao NOP no inicio (tentar tirar isso) -------------------

    add_sinst(-1,"NOP\n");
}

void parse_end(char *prname, char *d_proc)
{
    printf("Info: %d assembly instructions generated.\n", num_ins);
    
    // fecha os arquivos --------------------------------------------------------

    fclose(f_asm); // codigo   assembly
    fclose(f_lin); // tradutor assembly
    
    // checa se precisa adicionar macros no arquivo .asm ------------------------

    char asm_file[1024]; sprintf(asm_file, "%s/Software/%s.asm", d_proc, prname);

	mac_copy(asm_file);

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
        substituir_braket(texto);
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
    va_list  args;
    va_start(args , inst);

    char     str[100];
    vsprintf(str, inst, args);
    va_end  (args);

    // ------------------------------------------------------------------------
    // verifica se instrucao precisa de NOP antes -----------------------------
    // ------------------------------------------------------------------------

    // se nao usa pipeline, nao precisa de NOP
    if (pipeln > 7)
    {
        if (find_opc("LDI", str)) add_instr("NOP\n");
        if (find_opc("ILI", str)) add_instr("NOP\n");
    }

    // ------------------------------------------------------------------------
    // adiciona instrucao -----------------------------------------------------
    // ------------------------------------------------------------------------

    if (mac_using == 0) fprintf(f_asm, "%s", str);

    // tabela para tradutor assembly do gtkwave -------------------------------

    if (mac_using == 0) num_ins++;
    if (mac_using == 0) fprintf(f_lin, "%s\n", itob(line_num+1,20));

    // ------------------------------------------------------------------------
    // verifica se a instrucao precisa de alguma macro especial ---------------
    // ------------------------------------------------------------------------

    if (find_opc("int_div"   , str))  mac_add("idiv" );                                    // divisao inteira
    if (find_opc("int_mod"   , str)) {mac_add("imod" );                 mac_add("idiv" );} // resto da divisao inteira
    if (find_opc("float_inv" , str))  mac_add("finv" );                                    // inverso de float
    if (find_opc("float_sqrt", str)) {mac_add("fsqrt"); if (pipeln > 3) mac_add("finv" );} // raiz quadrada de float
    if (find_opc("float_atan", str)) {mac_add("fatan"); if (pipeln > 3) mac_add("finv" );} // arco tangente de float
    if (find_opc("float_sin" , str)) {mac_add("fsin" ); if (pipeln > 3) mac_add("finv" );} // seno de float

    // ------------------------------------------------------------------------
    // verifica se instrucao precisa de NOP depois ----------------------------
    // ------------------------------------------------------------------------
    
    // coloca mais um clock internamente na de-normalizacao em ponto flutuante
    if (pipeln > 7 && find_opc( "F_ADD"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc( "F_ADD_V", str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc("SF_ADD"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc( "F_GRE"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc("SF_GRE"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc( "F_LES"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc("SF_LES"  , str)) add_instr("NOP\n");

    // espera mais um clock para terminar o processo de de-normalizacao em ponto flutuante
    if (pipeln > 4 && find_opc( "F_ADD"  , str)) add_instr("NOP\n");
    if (pipeln > 4 && find_opc( "F_ADD_V", str)) add_instr("NOP\n");
    if (pipeln > 4 && find_opc("SF_ADD"  , str)) add_instr("NOP\n");
    if (pipeln > 4 && find_opc( "F_GRE"  , str)) add_instr("NOP\n");
    if (pipeln > 4 && find_opc("SF_GRE"  , str)) add_instr("NOP\n");
    if (pipeln > 4 && find_opc( "F_LES"  , str)) add_instr("NOP\n");
    if (pipeln > 4 && find_opc("SF_LES"  , str)) add_instr("NOP\n");

    // coloca mais um clock no meio da normalizacao em ponto flutuante
    if (pipeln > 5 && find_opc(   "I2F"  , str)) add_instr("NOP\n");
    if (pipeln > 5 && find_opc(   "I2F_M", str)) add_instr("NOP\n");
    if (pipeln > 5 && find_opc( "P_I2F_M", str)) add_instr("NOP\n");
    if (pipeln > 5 && find_opc( "F_ADD"  , str)) add_instr("NOP\n");
    if (pipeln > 5 && find_opc( "F_ADD_V", str)) add_instr("NOP\n");
    if (pipeln > 5 && find_opc("SF_ADD"  , str)) add_instr("NOP\n");
    if (pipeln > 5 && find_opc( "F_MLT"  , str)) add_instr("NOP\n");
    if (pipeln > 5 && find_opc( "F_MLT_V", str)) add_instr("NOP\n");
    if (pipeln > 5 && find_opc("SF_MLT"  , str)) add_instr("NOP\n");
    if (pipeln > 5 && find_opc( "F_DIV"  , str)) add_instr("NOP\n");
    if (pipeln > 5 && find_opc("SF_DIV"  , str)) add_instr("NOP\n");

    // coloca mais um clock no final da normalizacao em ponto flutuante
    if (pipeln > 7 && find_opc(   "I2F"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc(   "I2F_M", str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc( "P_I2F_M", str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc( "F_ADD"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc( "F_ADD_V", str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc("SF_ADD"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc( "F_MLT"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc( "F_MLT_V", str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc("SF_MLT"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc( "F_DIV"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc("SF_DIV"  , str)) add_instr("NOP\n");

    // coloca mais um clock internamente em algumas operacoes aritmeticas
    if (pipeln > 7 && find_opc(   "F2I"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc(   "F2I_M", str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc( "P_F2I_M", str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc(   "LAN"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc( "S_LAN"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc(   "LIN"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc(   "LIN_M", str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc( "P_LIN_M", str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc( "F_MLT"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc( "F_MLT_V", str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc("SF_MLT"  , str)) add_instr("NOP\n");

    // espera mais um clock para terminar algumas operacoes aritmeticas
    if (pipeln > 3 && find_opc( "F_ADD"  , str)) add_instr("NOP\n");
    if (pipeln > 3 && find_opc( "F_ADD_V", str)) add_instr("NOP\n");
    if (pipeln > 3 && find_opc("SF_ADD"  , str)) add_instr("NOP\n");
    if (pipeln > 6 && find_opc(   "MLT"  , str)) add_instr("NOP\n");
    if (pipeln > 6 && find_opc(   "MLT_V", str)) add_instr("NOP\n");
    if (pipeln > 6 && find_opc( "S_MLT"  , str)) add_instr("NOP\n");
    if (pipeln > 4 && find_opc( "F_MLT"  , str)) add_instr("NOP\n");
    if (pipeln > 4 && find_opc( "F_MLT_V", str)) add_instr("NOP\n");
    if (pipeln > 4 && find_opc("SF_MLT"  , str)) add_instr("NOP\n");
    if (pipeln > 5 && find_opc(   "F2I"  , str)) add_instr("NOP\n");
    if (pipeln > 5 && find_opc(   "F2I_M", str)) add_instr("NOP\n");
    if (pipeln > 5 && find_opc( "P_F2I_M", str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc(   "LES"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc( "S_LES"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc(   "EQU"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc( "S_EQU"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc(   "LAN"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc( "S_LAN"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc(   "GRE"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc( "S_GRE"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc(   "SHL"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc( "S_SHL"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc(   "LIN"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc(   "LIN_M", str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc( "P_LIN_M", str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc( "F_LES"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc("SF_LES"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc(   "SRS"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc( "S_SRS"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc(   "SHR"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc( "S_SHR"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc(   "NRM"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc(   "NRM_M", str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc( "P_NRM_M", str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc(   "ADD"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc(   "ADD_V", str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc( "S_ADD"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc( "F_GRE"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc("SF_GRE"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc(   "LOR"  , str)) add_instr("NOP\n");
    if (pipeln > 7 && find_opc( "S_LOR"  , str)) add_instr("NOP\n");
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
    if (mac_using == 0) vfprintf(f_asm, inst, args);
    va_end  (args);

    // tabela para tradutor asselbly do gtkwave -------------------------------

    if (type != 0)
    {
        if (mac_using == 0) num_ins++;
        if (mac_using == 0) fprintf(f_lin, "%s\n", itob(type,20));
    }
}