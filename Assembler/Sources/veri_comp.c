#include "..\Headers\veri_comp.h"
#include "..\Headers\eval.h"
#include "..\Headers\mnemonicos.h"

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

char name[128]; // nome do processador
char  tmp[512]; // guarda, temporariamente, os nomes de arquivos .v, .mif criados

FILE *f_veri;

// variaveis pra salvar no arquivo .v
int  sdepth, nmioin, nuioou, nugain; // nuioin eh usado internamente pelo flex e da pau -> usar nmioin!!

void set_name (char *va){strcpy(name, va);}
void set_float(int    n){float_point = n ;}

void set_nbits (int n){nbits  = n;}
void set_nbmant(int n){nbmant = n;}
void set_nbexpo(int n){nbexpo = n;}
void set_ndstac(int n){ndstac = n;}
void set_sdepth(int n){sdepth = n;}
void set_nuioin(int n){nmioin = n;}
void set_nuioou(int n){nuioou = n;}
void set_nugain(int n){nugain = n;}
void set_fftsiz(int n){fftsiz = n;}

char *get_dname()
{
    sprintf(tmp, "%s\\%s_data.mif", hard_dir,name);
    return  tmp;
}

char *get_iname()
{
    sprintf(tmp, "%s\\%s_inst.mif", hard_dir,name);
    return  tmp;
}

char *get_vname()
{
    sprintf(tmp, "%s\\%s.v", hard_dir,name);
    return  tmp;
}

char *get_tb_name()
{
    sprintf(tmp, "%s\\%s_tb.v", hard_dir,name);
    return  tmp;
}

// gera arquivo verilog com uma instancia do processador
void build_vv_file()
{
    f_veri = fopen(get_vname(), "w");

    fprintf(f_veri, "module %s (\n", name);
    fprintf(f_veri, "input clk, rst,\n");

    int s1 = (float_point) ? nbmant-1      : nbits-1;
    int s2 = (float_point) ? nbmant+nbexpo : nbits-1;

    fprintf(f_veri, "input signed [%d:0] io_in,\n"  , s1);
    fprintf(f_veri, "output signed [%d:0] io_out,\n", s2);

    fprintf(f_veri, "output [%d:0] req_in,\n", nmioin-1);
    fprintf(f_veri, "output [%d:0] out_en,\n", nuioou-1);
    fprintf(f_veri, "input itr);\n\n");

    fprintf(f_veri, "wire signed [%d:0] in_float;\n"   , s2);
    fprintf(f_veri, "wire signed [%d:0] out_float;\n\n", s2);

    if (float_point)
    fprintf(f_veri, "int2float #(.MAN(%d),.EXP(%d)) i2f (io_in, in_float);\n\n", nbmant, nbexpo);
    else
    fprintf(f_veri, "assign in_float = io_in;\n\n");

    fprintf(f_veri, "wire proc_req_in, proc_out_en;\n");
    fprintf(f_veri, "wire [%d:0] addr_in;\n"   , (int)ceil(log2(nmioin)-1));
    fprintf(f_veri, "wire [%d:0] addr_out;\n\n", (int)ceil(log2(nuioou)-1));

    if (float_point) // exclusivo ponto flutuante
    {
        fprintf(f_veri, "proc_fl #(.NBMANT(%d),\n", nbmant);
        fprintf(f_veri, ".NBEXPO(%d),\n"          , nbexpo); // possivelmente pode ser usado em ambos;
    }
    else             // exclusivo ponto fixo
    {
        fprintf(f_veri, "proc_fx #(.NUBITS(%d),\n", nbits );
        fprintf(f_veri,           ".NUGAIN(%d),\n", nugain);
    }

    fprintf(f_veri, ".MDATAS(%d),\n", n_dat );
    fprintf(f_veri, ".MINSTS(%d),\n", n_ins );
    fprintf(f_veri, ".SDEPTH(%d),\n", sdepth);
    fprintf(f_veri, ".NUIOIN(%d),\n", nmioin);
    fprintf(f_veri, ".NUIOOU(%d),\n", nuioou);
    fprintf(f_veri, ".FFTSIZ(%d),\n", fftsiz);

    if (itr_addr != 0) fprintf(f_veri, ".ITRADD(%d),\n", itr_addr); // vai ter interrupcao

    for (int i = 0; i < m_count; i++) fprintf(f_veri, ".%s(1),\n", m_name[i]);

    fprintf(f_veri, ".DFILE(\"%s_data.mif\"),\n", name);
    fprintf(f_veri, ".IFILE(\"%s_inst.mif\")\n" , name);
    fprintf(f_veri, ") p_%s (clk, rst, in_float, out_float, addr_in, addr_out, proc_req_in, proc_out_en, itr);\n\n", name);

    if (float_point)
    fprintf(f_veri, "float2int #(.EXP(%d),.MAN(%d)) f2i (out_float, io_out);\n\n", nbexpo, nbmant);
    else
    fprintf(f_veri, "assign io_out = out_float;\n\n");

    if (nmioin == 1)
    fprintf(f_veri, "assign req_in = proc_req_in;\n");
    else
    fprintf(f_veri, "addr_dec #(%d) dec_in (proc_req_in, addr_in , req_in);\n"  , nmioin);

    if (nuioou == 1)
    fprintf(f_veri, "assign out_en = proc_out_en;\n\n");
    else
    fprintf(f_veri, "addr_dec #(%d) dec_out(proc_out_en, addr_out, out_en);\n\n", nuioou);

    fprintf(f_veri, "endmodule\n");

    fclose (f_veri);
}

void build_tb_file()
{
    double T = 1000.0/clk_frq; // periodo do clock em ns (clk_frq em MHz)

    f_veri = fopen(get_tb_name(), "w");

    fprintf(f_veri,      "`timescale 1ns/1ps\n\n", name);
    fprintf(f_veri,         "module %s_tb();\n\n", name);
    fprintf(f_veri,           "reg clk, rst;\n\n"      );
    fprintf(f_veri,           "initial begin\n\n"      );
    fprintf(f_veri, "$dumpfile(\"%s_tb.vcd\");\n", name);
    fprintf(f_veri,     "$dumpvars(0,%s_tb);\n\n", name);

    fprintf(f_veri,   "clk = 0;\n"     );
    fprintf(f_veri,   "rst = 1;\n"     );
    fprintf(f_veri,       "#%f;\n",   T);
    fprintf(f_veri, "rst = 0;\n\n"     );
    fprintf(f_veri, "#%f;\n", T*clk_num);
    fprintf(f_veri, "$finish;\n\n"     );
    fprintf(f_veri,      "end\n\n"     );

    fprintf(f_veri, "always #%f clk = ~clk;\n\n", T/2.0);

    int s1 = (float_point) ? nbmant-1      : nbits-1;
    int s2 = (float_point) ? nbmant+nbexpo : nbits-1;

    fprintf(f_veri, "reg signed [%d:0] proc_io_in = 0;\n" , s1);
    fprintf(f_veri, "wire signed [%d:0] proc_io_out;\n"   , s2);
    fprintf(f_veri, "wire [%d:0] proc_req_in;\n"    , nmioin-1);
    fprintf(f_veri, "wire [%d:0] proc_out_en;\n\n"  , nuioou-1);

    fprintf(f_veri, "%s proc(clk,rst,proc_io_in,proc_io_out,proc_req_in,proc_out_en,1'b0);\n\n", name);

    int i;
    for(i=0;i<nmioin;i++)
    fprintf(f_veri, "reg signed [%d:0] in_%d = 0; // coloque aqui a sua entrada\n", s1, i);
    fprintf(f_veri,"\n");

    for(i=0;i<nuioou;i++)
    fprintf(f_veri, "reg signed [%d:0] out_%d = 0;\n", s2, i);

    fprintf(f_veri, "\nalways @ (*) begin\n");
    fprintf(f_veri, "   proc_io_in = 0;\n");
    for(i=0;i<nmioin;i++)
    fprintf(f_veri, "   if (proc_req_in == %d) proc_io_in = in_%d;\n", (int)pow(2,i),i);
    fprintf(f_veri, "end\n");

    fprintf(f_veri, "\nalways @ (*) begin\n");
    for(i=0;i<nuioou;i++)
    fprintf(f_veri, "   if (proc_out_en == %d) out_%d <= proc_io_out;\n", (int)pow(2,i), i);

    fprintf(f_veri, "end\n");

    fprintf(f_veri, "\nendmodule\n");

    fclose (f_veri);
}

// gera arquivo verilog do program counter para simulacao
void build_pc_file()
{
    char path[1024];
    sprintf(path, "%s/pc_sim.v", temp_dir);

    FILE *input;
    FILE *output = fopen(path, "w");

    char texto[1001] = "";

    // copia o conteudo de pc.v
    sprintf(path, "%s/pc.v", hdl_dir);
    input = fopen(path, "r");
    while(fgets(texto, 1001, input) != NULL)
    {
        if(strcmp(texto, "endmodule") != 0)
        {
            fputs(texto, output);
        }
        memset(texto, 0, sizeof(char) * 1001);
    }
    fclose(input );

    int top_ins; // guarda o numero de instrucoes no inicio do arquivo .asm
    int num_ins; // guarda o num total de instrucoes

    // pega top_ins e num_ins no arquivo log.txt, depois do caractere #
    sprintf(path, "%s/log.txt", temp_dir);
    input = fopen(path, "r");
    while(fgets(texto, 1001, input) != NULL)
    {
        if(strcmp(texto, "#\n") == 0)
        {
            memset(texto, 0, sizeof(char) * 1001);
             fgets(texto, 1001, input);
            top_ins = atoi(texto);
            memset(texto, 0, sizeof(char) * 1001);
             fgets(texto, 1001, input);
            num_ins = atoi(texto);
        }
        memset(texto, 0, sizeof(char) * 1001);
    }
    fclose(input );

    int nr = 10; // numero de atrasos
    for (int i=0;i<nr;i++)
    {
        // cria nr registradores para atrasar o valor de val em pc.v
        // val contem o endereco da instrucao atualmente sendo executada
        // o programa para nr clocks apos chegar a instrucao @fim JMP fim
        fprintf(output, "reg [NBITS-1:0] valr%d=0;\n",i+1);
    }

    fprintf(output, "\n");
    // cria memoria (min) para guardar o valor da linha ...
    // no arquivo .cmm de cada instrucao gerada
    fprintf(output, "reg [19:0] min [0:%d];\n\n", num_ins-1);

    // linetab eh o valor da linha atual na tabela
    fprintf(output, "reg signed [20:0] linetab =-1;\n"  );
    // linetabs eh o valor atual registrado
    fprintf(output, "reg signed [20:0] linetabs=-1;\n\n");

    // le o arquivo gerado pelo compilador c com o conteudo da tabela
    fprintf(output, "initial $readmemb(\"in2line.txt\", min);\n\n");

    // valores abaixo de top_ins sao instrucoes iniciais
    // elas nao estao em .cmm
    fprintf(output, "always @ (posedge clk) begin\n");
    fprintf(output, "	if (val < %d)\n", top_ins);
    fprintf(output, "       linetab <= -1;\n");
    fprintf(output, "	else if (val < %d)\n", num_ins);
    fprintf(output, "		linetab <= min[val-%d];\n", top_ins);

    // faz o shift register com os atrasos de val
    fprintf(output, "	valr1 <= val;\n");
    for (int i=2;i<=nr;i++)
    {
        fprintf(output, "	valr%d <= valr%d;\n", i, i-1);
    }

    fprintf(output,"\n");
    // atrasa o conteudo da linha correspondente no .cmm
    fprintf(output, "   linetabs <= linetab;\n");
    fprintf(output, "end\n\n");

    // para a simulacao nr clocks depois de achar a instrucao @fim JMP fim
    fprintf(output, "always @ (posedge clk) begin\n");
    fprintf(output, "   if (valr%d == %d) $finish;\nend\n\n",nr,fim_addr);

    fprintf(output, "endmodule\n");

    fclose(output);
}

// gera arquivo verilog da memoria de dados para simulacao
void build_dt_file()
{
    char path[1024];
    sprintf(path, "%s/mem_data_sim.v", temp_dir);

    FILE *input;
    FILE *output = fopen(path, "w");

    char texto[1001] = "";

    // copia o conteudo de mem_data.v
    sprintf(path, "%s/mem_data.v", hdl_dir);
    input = fopen(path, "r");
    while(fgets(texto, 1001, input) != NULL)
    {
        if(strcmp(texto, "endmodule") != 0)
        {
            fputs(texto, output);
        }
        memset(texto, 0, sizeof(char) * 1001);
    }
    fclose(input );

    int i;
    // cria um registrador para cada variavel encontrada
    for (i = 0; i < v_cont; i++) fprintf(output, "reg [NBDATA-1:0] %s=0;\n", v_namo[i]);
    
    // inicia o always para registrar as variaveis
    fprintf(output, "\nalways @ (posedge clk) begin\n");

    // registra cada variavel, dependendo do endereco de cada uma
    for (i = 0; i < v_cont; i++) fprintf(output, "   if (addr_w == %d && wr) %s <= data_in;\n", v_add[i], v_namo[i]);

    fprintf(output, "end\n\n");

    // se a variavel for comp ...
    // junta a parte real e complexa em uma variavel do dobro de tamanho
    for (i = 0; i < v_cont; i++)
    {
        if (v_tipo[i] == 3)
        {
            for (int j = 0; j < v_cont; j++)
            {
                char im[64];
                sprintf(im, "%s_i", v_namo[i]);
                if (strcmp(v_namo[j],im) == 0)
                {
                    fprintf(output,"wire [NBDATA*2-1:0] comp_%s = {%s, %s};\n", v_namo[i], v_namo[i], v_namo[j]);
                }
            }
        }
    }

    fprintf(output, "\nendmodule\n");

    fclose(output);
}