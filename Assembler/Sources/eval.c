// ----------------------------------------------------------------------------
// rotinas para gerar os arquivos .mif das memorias ... -----------------------
// a medida que o lex vai escaneando o .asm -----------------------------------
// ----------------------------------------------------------------------------

// includes locais
#include "..\Headers\eval.h"
#include "..\Headers\t2t.h"
#include "..\Headers\variaveis.h"
#include "..\Headers\labels.h"
#include "..\Headers\veri_comp.h"
#include "..\Headers\mnemonicos.h"
#include "..\Headers\simulacao.h"

// includes globais
#include <stdlib.h>
#include  <stdio.h>
#include   <math.h>
#include <string.h>

// ----------------------------------------------------------------------------
// redeclaracao de variaveis globais ------------------------------------------
// ----------------------------------------------------------------------------

// variaveis de estado --------------------------------------------------------

int  pp       = 1;         // pre-processing
char opcd[64];             // guarda opcode atual

// informacoes atualizadas durante o pp ---------------------------------------

int  n_ins	  = 0;        // numero de instrucoes adicionadas
int  n_dat    = 0;        // numero de variaveis  adicionadas
int  fftsiz   = 8;        // tamanho da fft (em bits)
int  isrf     = 0;        // diz se achou uma instrucao pra fazer FFT
int  itr_addr = 0;        // endereco de interrupcao

// guarda os valores das diretivas
char prname [128];        // nome do processador
int  nubits  = 23;        // tamanho da palavra da ula
int  nbmant  = 16;        // numero de bits da mantissa
int  nbexpo  =  6;        // numero de bits do expoente
int  ndstac  = 10;        // tamanho da pilha de dados
int  sdepth  = 10;        // tamanho da pilha de subrotinas
int  nuioin  =  1;        // numero de portas de entrada
int  nuioou  =  1;        // numero de portas de saida
int  nugain  = 64;        // constante de divisao

// informacoes atualizadas depois do pp ---------------------------------------

int  nbopr;               // num de bits de operando

int  fim_addr;            // endereco do fim do programa
int  v_tipo[1000];        // tipo da variavel encontrada
int  v_add [1000];        // endereco da variavel encontrada
char v_namo[1000][64];    // nome da variavel encontrada

int  m_count;             // contador de macros
char m_name[NMNEMAX][64]; // nome das macros

// ----------------------------------------------------------------------------
// variaveis locais -----------------------------------------------------------
// ----------------------------------------------------------------------------

#define NBITS_OPC 7     // tem que mudar no verilog de acordo (em proc.v)

FILE *f_data, *f_instr; // .mif das memorias de dado e instrucao

// variaveis de estado --------------------------------------------------------

int state = 0;          // estado do compilador
int c_op;               // guarda opcode atual

int tam_var;            // auxilia no preenchimento de array em memoria (tamanho do array)
int fil_typ;            // auxilia no preenchimento de array em memoria (tipo de dado)

// executada antes de iniciar o lexer
void eval_init(int prep)
{
    pp = prep;          // se estou ou nao na fase de pre-processamento

    var_reset();        // reseta a contagem das variaveis

    if (!pp)            // pp soh conta, nao faz os arquivos ainda
    {
        // num de bits de endereco para o operando (depois do mnemonico)
        // depende de quem eh maior, mem de dado ou de instr
        // esse valor foi achado na fase de pp
        nbopr = (n_ins > n_dat+ndstac) ? ceil(log2(n_ins)) : ceil(log2(n_dat+ndstac));

        // abre os arquivos .mif
        f_data  = fopen(get_dname(), "w");
        f_instr = fopen(get_iname(), "w");

        // inicializa rotinas pra simulacao com o iverilog
        sim_init();
    }
}

// executado quando uma diretiva eh encontrada
void eval_direct(int next_state)
{
    state = next_state; // vai pro estado que pega o argumento especifico da diretiva
}

// executado quando acha a diretiva #ITRAD
void eval_itrad()
{
    if (pp) itr_addr = n_ins;
}

// funcao auxiliar usada em eval_opcode (abaixo)
void add_instr(int opc, int opr)
{
    // se for fase de pp, soh conta o num de instrucoes
    if ( pp)
    {
        n_ins++;

        // vai usar inversao de bits (opc 3 e 7)
        // entao, precisa checar tamanho da memoria no final
        if ((opc == 3) || (opc == 7)) isrf = 1;
    }

    // se nao, escreve a instrucao no .mif em binario
    // tambem escreve a traducao no arquivo de traducao de opcode
    if (!pp)
    {
        fprintf(f_instr, "%s%s\n" , itob(opc,NBITS_OPC), itob(opr,nbopr));
        sim_add(opcd);
    }
}

// executado quando um novo opcode eh encontrado
void eval_opcode(int op, int next_state, char *text, char *nome)
{
    c_op  = op;          // cadastra opcode atual
    strcpy(opcd,text);   // guarda nome do opcode atual

    // proximo estado depende do tipo de opcode:
    // 0: nao tem operando
    // 1: operando eh endereco da memoria de daddos
    // 2: operando eh endereco da memoria de instrucao
    state = next_state;

    // nao tem operando, ja pode escrever a instrucao
    if (state == 0) add_instr(op,0);

    // cadastra mnemonico para alocar recurso em hardware
    if (strcmp(nome,"") != 0) add_mne(nome);
}

void add_data(int val)
{
    // se for pp, soh conta a quantidade de dados
    if ( pp) n_dat++;

    // se nao, escreve o valor no .mif
    if (!pp)
    {
        // escreve o dado em binario no .mif
        fprintf(f_data, "%s\n", itob(val,nubits));
    }
}

// funcao auxiliar que procura no arquivo cmm_log.txt se ...
// char *va aponta para uma variavel declarada no .cmm
// tambem retorna o tipo da variavel encontrada
int is_var(char *va, int *tipo, int *is_global, char *nome)
{
    char texto[1001];
    char funcao[128];
    char variav[128];

    // abre o arquivo de log
    char path[1024];
    sprintf(path, "%s/cmm_log.txt", temp_dir);
    FILE *input = fopen(path, "r");

    // pula as 3 primeiras linhas
    fgets(texto, 1001, input); // nome do processador
    fgets(texto, 1001, input); // numero de bits da mantissa
    fgets(texto, 1001, input); // numero de bits do expoente
  
    int ok = 0;
    // varre as linhas do arquivo
    while(fgets(texto, 1001, input) != NULL)
    {
        // secao de variaveis termina quando encontra um #
        if (strstr(texto, "#") != NULL) break;

        sscanf (texto, "%s %s %d", funcao, variav, tipo);

        // se for variavel global, nao coloca o nome da funcao
        if (strcmp(funcao,"global")==0)
        {
            *is_global = 1;
            sprintf(nome , "%s", variav);
        }
        else
        {
            *is_global = 0;
            sprintf(nome , "%s_%s", funcao, variav);
        }

        if (strcmp(nome,va) == 0)
        {
            sprintf(nome , "%s_v_%s", funcao, variav);
            ok = 1;
            break;
        }
    }
    
    fclose(input);

    return ok;
}

// cadastra instrucao com a ula
// se o operando for uma constante, converte seu valor para binario ...
// e coloca o simbolo na memoria de dados
// por ultimo, coloca a instrucao na memoria
// soh eh executado na fase pp
void oper_ula(char *va, int is_const)
{
    if (find_var(va) == -1)
    {
        int val;
        switch(is_const)
        {
            case 0: val = 0;        break;
            case 1: val = atoi(va); break;
            case 2: val = f2mf(va); break;
        }

        add_var (va, val);
        add_data(    val);

        // procura, no arquivo de log, se eh uma variavel ...
        // declarada no codigo .cmm
        // se sim, cadastra ela para mostrar no simulador
        int tipo;
        int is_global;
        char var_name[128];
        if (pp && is_var(va, &tipo, &is_global, var_name))
        {
            if (is_global)
                sprintf(v_namo[sim_v_cnt], "me%d_f_global_v_%s_e_", tipo, va);
            else
                sprintf(v_namo[sim_v_cnt], "me%d_f_%s_e_", tipo, var_name);
            v_add  [sim_v_cnt] = n_dat-1;
            v_tipo [sim_v_cnt] = tipo;
            sim_v_cnt++;
        }
    }

    strcat(opcd, " "); strcat(opcd, va);
    add_instr(c_op, find_var(va));
}

// cadastra instrucoes de salto
void oper_salto(char *va)
{
    strcat(opcd, " "); strcat(opcd, va); 
    add_instr (c_op, find_label(va));
}

// funcao auxiliar para remover espacos em branco
void rem_space(char *text)
{
    int i = 0, j = 0;
    char temp[256];
    strcpy(temp, text);
    while (temp[i] != '\0')
    {
        while (temp[i] == ' ') i++;
        text[j] = temp[i];
        i++;
        j++;
    }
    text[j] = '\0';
}

// funcao auxiliar para preencher array na memoria de dados
// usado com inicializacao de array (ex: int x[10] "nome do arquivo")
// f_name eh o nome do arquivo a ser lido
// tam eh o tamanho do arquivo
// na fase de pp soh conta as variaveis
void fill_mem(char *f_name, int tam)
{
    FILE* filepointer = NULL;

    // primeiro pega o caminho completo e abre o arquivo ----------------------
    // mudar a sintaxe para nao precisar das aspas

    char addr_tab[2048];
    if(pp == 0)
    {
        int tamanho = strlen(f_name); // tamanho da string do nome do arquivo
        int idxToDel = tamanho-1;     // indice para deletar, nesse caso o ultimo, as aspas.
        memmove(&f_name[idxToDel], &f_name[idxToDel +1], 1); // deletando de fato o indice
        sprintf(addr_tab, "%s/Software/%s", proc_dir, f_name);

        filepointer = fopen(addr_tab, "r");
        if (filepointer == NULL)
        fprintf(stderr, "Erro: Não rolou de abrir/achar o arquivo %s!!\n", addr_tab);
    }

    // agora le o arquivo -----------------------------------------------------

    int  i,val = 0;
    char linha[512];

    for (i = 0; i < tam ; i++)
    {
        if (pp == 0)
        {
            // le linha por linha
            // o que fazer depende ...
            // do tipo de proc e do tipo de dado
            fgets(linha, sizeof(linha), filepointer);

            // com int
            if (fil_typ == 1)
            {
                val = atoi(linha);
            }

            // com float
            if (fil_typ == 2)
            {
                val = f2mf(linha);
            }

            // com real comp
            if (fil_typ == 3)
            {
                char  num[64];
                float real,img;

                rem_space(linha);
                   sscanf(linha,"%f %f",&real,&img);

                   sprintf(num,"%f",real);
                val = f2mf(num);
            }

            // com imag comp
            if (fil_typ == 4)
            {
                char  num[64];
                float real,img;

                rem_space(linha);
                   sscanf(linha,"%f %f",&real,&img);

                   sprintf(num,"%f",img);
                val = f2mf(num);
            }

            add_data(val);
        }
        else
            add_data(0); // no pp soh conta as variaveis
    }

    if (pp == 0) fclose(filepointer);
}

// adiciona array na memoria de dados
// se for array normal, completa com zero
// se for array inicializado, chama fill_mem para preencher
// va eh o tamanho do array
void add_array(int va, char *f_name)
{
    // incrementa o tamanho da memoria de acordo
    inc_vcont(va-1);

    // se nao tem arquivo, preenche com zero
    if (strcmp(f_name, "") == 0)
        for (int i = 0; i < va; i++) add_data(0);
    else
        fill_mem(f_name, va);
}

void eval_opernd(char *va, int is_const)
{
    switch (state)
    {
        case  1: oper_ula  (va, is_const);                       // operacoes com a ULA
                 state = 0;  break;
        case  2: oper_salto(va);                                 // operacoes de salto
                 state = 0;  break;
        case  3: add_var   (va,0);                               // achou um array sem inicializacao
                 state = 4;  break;
        case  4: add_array (atoi(va), "");                       // declara  array sem inicializacao
                 state = 0;  break;
        case  5: if (pp) set_name  (va);                         // nome do processador
                 state = 0;  break;
        case  6: if (pp) set_nbits (atoi(va));                   // numero de bits de dados
                 state = 0;  break;
        case  7: if (pp) set_nbmant(atoi(va));                   // numero de bits de mantissa
                 state = 0;  break;
        case  8: if (pp) set_nbexpo(atoi(va));                   // numero de bits do expoente
                 state = 0;  break;
        case  9: if (pp) set_ndstac(atoi(va));                   // tamanho da pilha de dados
                 state = 0;  break;
        case 10: if (pp) set_sdepth(atoi(va));                   // tamanho da pilha de instrucoes
                 state = 0;  break;
        case 11: if (pp) set_nuioin(atoi(va));                   // numero de enderecoes de io - entrada
                 state = 0;  break;
        case 12: if (pp) set_nuioou(atoi(va));                   // numero de enderecoes de io - saida
                 state = 0;  break;
      //case 13: era usado para setar float ou int
      //case 14: era usado para o nome do diretorio
        case 15: if (pp) set_nugain(atoi(va));                   // valor da normalizacao
                 state = 0;  break;
        case 16: add_var(va,0);                                  // declarando array com arquivo
                 state = 17; break;
        case 17: fil_typ = atoi(va);                             // pega o tipo de array
                 state = 18; break;
        case 18: tam_var = atoi(va);                             // pega o tamanho do array com arquivo
                 state = 19; break;
        case 19: add_array (tam_var,va);                         // preenche memoria com valor do arquivo (zero se nao tem arquivo)
                 state =  0; break;
        case 20: if (pp) set_fftsiz(atoi(va));                   // num de bits pra inverter na fft
                 state =  0; break;
    }

}

void eval_label(char *la)
{
    if (pp)
    {
        add_label(la, n_ins); // cadastra label

        // cadastra endereco da instrucao de fim do programa
        if (strcmp(la,"fim") == 0) fim_addr = n_ins;
    }
}

void eval_finish()
{
    int i, aux;

    // completa memoria de instrucao ------------------------------------------
/*
    aux = n_ins;
    n_ins = (n_ins % (int)pow(2, logb(n_ins))) ? pow(2, logb(n_ins)+1) : pow(2, logb(n_ins));
    for (i=0; i<n_ins-aux; i++)
        fprintf(f_instr, "%s\n", itob(0,NBITS_OPC+nbopr));    // completando com potencia de 2
*/
    fclose(f_instr);

    // completa memoria de dados ----------------------------------------------

    int s = nubits;

    n_dat = ndstac+n_dat;
	for (i=0; i<ndstac; i++)
        fprintf(f_data, "%s\n", itob(0,s));     // completando com a pilha
/*
    aux = n_dat;
    n_dat = (n_dat % (int)pow(2, logb(n_dat))) ? pow(2, logb(n_dat)+1) : pow(2, logb(n_dat));
    for (i=0; i<n_dat-aux; i++)
        fprintf(f_data, "%s\n", itob(0,s));     // completando com potencia de 2
*/
    int fft_siz = 2*pow(2,fftsiz);
    if ((isrf == 1) && (fft_siz > n_dat))
    {
        aux = n_dat;
        n_dat = fft_siz;
        for (i=0; i<n_dat-aux; i++)
            fprintf(f_data, "%s\n", itob(0,s)); // completa com tamanho da fft (se necessario)
    }

    fclose(f_data);

    // checa integridade do tamanho do dado -----------------------------------

    if (nubits != nbmant+nbexpo+1)
        fprintf(stderr, "Erro: NUBITS (%d) tem que ser NBMANT (%d) + NBEXPO (%d) + 1!\n", nubits, nbmant, nbexpo);

    // gera arquivos ----------------------------------------------------------

    build_vv_file();  // arquivo verilog top level do processador   
    build_tb_file();  // arquivo de test bench

    // finaliza traducao ------------------------------------------------------

    fclose(f_tran);
}
