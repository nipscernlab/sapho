// esse rquivo ta ficando confuso
// nao esta claro o que executa nas fases pp e nao pp
// tentar separar rotinas pp e nao pp em arquivos diferentes?
// ou talvez em funcoes diferentes?
// acho q vai ter q separar em dois executaveis

#include "..\Headers\eval.h"
#include "..\Headers\t2t.h"
#include "..\Headers\variaveis.h"
#include "..\Headers\labels.h"
#include "..\Headers\veri_comp.h"
#include "..\Headers\mnemonicos.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

// redeclaracao de variaveis globais
int  fftsiz = 0;          // tamanho da fft
int  nbexpo;              // numero de bits do expoente
int  nbmant;              // numero de bits da mantissa
int  fim_addr;            // endereco do fim do programa
int  v_tipo[1000];        // tipo da variavel encontrada
int  v_add [1000];        // endereco da variavel encontrada
char v_namo[1000][64];    // nome da variavel encontrada
int  nbits;               // numero de bits do processador
char opcd[64];            // guarda opcode atual
int  ndstac;              // numero de destinos de acesso direto
int  isrf;                // diz se achou a instrucao pra fazer FFT
int  n_opc;               // numero de instrucoes no arquivo de traducao
int  v_cont;              // numero de variaveis encontradas
int  n_dat;               // numero de variaveis adicionadas
int  m_count;             // contador de macros
char m_name[NMNEMAX][64]; // nome das macros


#define NBITS_OPC 7     // tem que mudar no verilog de acordo (em proc.v)

FILE *f_data, *f_instr; // .mif das memorias de dado e instrucao
FILE *f_tran;           // arquivo para traducao do opcode

int state = 0;          // estado do compilador
int c_op;               // guarda opcode atual
int nbopr;              // num de bits de operando
int tam_var;            // auxilia no preenchimento de array em memoria (tamanho do array)
int fil_typ;            // auxilia no preenchimento de array em memoria (tipo de dado)

void eval_init(int prep)
{
    pp = prep;          // se estou ou nao na fase de pre-processamento

    var_reset();        // reseta a contagem de mnemonicos que alocam recursos em hardware

    if (pp)             // pp soh conta, nao faz os arquivos ainda
    {
        n_ins    = 0;
        n_dat    = 0;
        v_cont   = 0;
        itr_addr = 0;   // reseta endereco de interrupcao
    }
    else                // segunda fase
    {
        n_opc    = 0;
        isrf     = 0;   // ainda nao usou enderecamento invertido

        // num de bits de endereco para o operando (depois do mnemonico)
        // depende de quem eh maior, mem de dado ou de instr
        // esse valor foi achado na fase de pp
        nbopr = (n_ins > n_dat+ndstac) ? ceil(log2(n_ins)) : ceil(log2(n_dat+ndstac));

        // abre os arquivos .mif
        f_data  = fopen(get_dname(), "w");
        f_instr = fopen(get_iname(), "w");

        // abre arquivo de traducao de opcode na pasta Tmp
        char path[1024];
        sprintf(path, "%s/trad_opcode.txt", temp_dir);
        f_tran  = fopen(path, "w");
    }
}

void add_instr(int opc, int opr)
{
    // se for fase de pp, soh conta o num de instrucoes
    if ( pp) n_ins++;

    // se nao, escreve a instrucao no .mif em binario
    // tambem escreve a traducao no arquivo de traducao de opcode
    if (!pp)
    {
        fprintf(f_instr, "%s%s\n" , itob(opc,NBITS_OPC), itob(opr,nbopr));
        fprintf(f_tran , "%d %s\n", n_opc++, opcd);
    }
}

void add_data(int val)
{
    // se for pp, soh conta a quantidade de dados
    if ( pp) n_dat++;

    // se nao, escreve o valor no .mif
    if (!pp)
    {
        // escreve o dado em binario no .mif
        fprintf(f_data, "%s\n", itob(val,nbits));
    }
}

// funcao auxiliar que procura no arquivo cmm_log.txt se ...
// char *va aponta para uma variavel declarada no .cmm
// tambem retorna o tipo da variavel encontrada
int is_var(char *va, int *tipo, int *is_global)
{
    char texto[1001];
    char funcao[128];
    char variav[128];
    char nome  [128];

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

        if (strcmp(nome,va) == 0) {ok = 1; break;}
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
    if (find_var(va) == -1) // aqui tem q mudar para saber se a constante eh fix ou float
                            // talvez criar mais uma variavel array pra marcar cada constante
                            // dependendo do mnemonico a esquerda

    {   // variavel nao existe ainda
        int val = (is_const) ? atoi(va) : 0; // usar f2mf pra escrever em float no assembley

        add_var (va, val);
        add_data(    val);

        // procura, no arquivo de log, se eh uma variavel ...
        // declarada no codigo .cmm
        // se sim, cadastra ela para mostrar no simulador
        int tipo;
        int is_global;
        if (pp && is_var(va, &tipo, &is_global))
        {
            if (is_global)
                sprintf(v_namo[v_cont], "me%d_global_%s", tipo, va);
            else
                sprintf(v_namo[v_cont], "me%d_%s", tipo, va);
            v_add  [v_cont] = n_dat-1;
            v_tipo [v_cont] = tipo;
            v_cont++;
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

// executado quando uma diretiva eh encontrada
void eval_direct(int next_state)
{
    // vai pro estado que pega o argumento especifico da diretiva
    state = next_state;
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

    // vai usar inversao de bits
    // entao, precisa checar tamanho da memoria no final
    if (strcmp(nome, "FFT") == 0) isrf = 1;
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

    int s = nbits;

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

    if (nbits != nbmant+nbexpo+1)
        fprintf(stderr, "Erro: NUBITS (%d) tem que ser NBMANT (%d) + NBEXPO (%d) + 1!\n", nbits, nbmant, nbexpo);

    // gera arquivos ----------------------------------------------------------

    build_vv_file();  // arquivo verilog top level do processador   
    build_tb_file();  // arquivo de test bench
    build_pc_file();  // arquivo de simulacao do program counter
    build_dt_file();  // arquivo de simulacao das variaveis

    // finaliza traducao ------------------------------------------------------

    fclose(f_tran);
}
