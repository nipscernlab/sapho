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

// funcoes auxiliares para setar e pegar variaveis globais
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

char *get_dname  (){sprintf(tmp, "%s/Hardware/%s_data.mif", proc_dir,name); return tmp;}
char *get_iname  (){sprintf(tmp, "%s/Hardware/%s_inst.mif", proc_dir,name); return tmp;}
char *get_vname  (){sprintf(tmp, "%s/Hardware/%s.v"       , proc_dir,name); return tmp;}
char *get_tb_name(){sprintf(tmp, "%s/%s_tb.v"             , temp_dir,name); return tmp;}

// cria arquivo de simulacao para o core_fx ou core_fl
// eh usado com multicore
void build_core_flx()
{
    char path[1024];
    sprintf(path, "%s/core_flx_%s_sim.v",temp_dir,name);

    FILE *input;
    FILE *output = fopen(path, "w");

    char texto[1001] = "";

    if (float_point) sprintf(path, "%s/core_fl.v", hdl_dir);
    else             sprintf(path, "%s/core_fx.v", hdl_dir);
    input = fopen(path, "r");

    fgets(texto, 1001, input);
    fprintf(output, "module core_flx_%s_sim\n", name);

    // copia o conteudo do processador
    while(fgets(texto, 1001, input) != NULL)
    {
        if(strstr(texto,     "pc #(MINSTW) pc (clk, rst, pc_load, pcl, pc_addr);") != NULL)
          fprintf(output, "pc_%s #(MINSTW) pc (clk, rst, pc_load, pcl, pc_addr);\n",  name);
        else
            fputs(texto, output);
        memset(texto, 0, sizeof(char) * 1001);
    }

    fclose(input );
    fclose(output);    
}

// cria arquivo de simulacao para o proc_fx ou proc_fl
// eh usado com multicore
void build_proc_flx()
{
    char path[1024];
    sprintf(path, "%s/proc_flx_%s_sim.v",temp_dir,name);

    FILE *input;
    FILE *output = fopen(path, "w");

    char texto[1001] = "";

    if (float_point) sprintf(path, "%s/proc_fl.v", hdl_dir);
    else             sprintf(path, "%s/proc_fx.v", hdl_dir);
    input = fopen(path, "r");

    fgets(texto, 1001, input);
    fprintf(output, "module proc_flx_%s_sim\n", name);

    // copia o conteudo do processador
    while(fgets(texto, 1001, input) != NULL)
    {
        if ( strstr(texto,          "core_fx #(.NUBITS(NUBITS),") != NULL)
            fprintf(output, "core_flx_%s_sim #(.NUBITS(NUBITS),\n",  name);
        else if (strstr(texto,      "core_fl #(.NBMANT(NBMANT),")    != 0)
            fprintf(output, "core_flx_%s_sim #(.NBMANT(NBMANT),\n",  name);
        else if (strstr(texto,    "mem_data # (.NADDRE(MDATAS),")    != 0)
            fprintf(output,    "mem_data_%s # (.NADDRE(MDATAS),\n",  name);
        else
            fputs(texto, output);
        memset(texto, 0, sizeof(char) * 1001);
    }

    fclose(input );
    fclose(output);    
}

// cria arquivo de simulacao para o proc
// eh usado com multicore
void build_proc_sim()
{
    char path[1024];
    sprintf(path, "%s/%s_sim.v", temp_dir,name);

    FILE *input;
    FILE *output = fopen(path, "w");

    char texto[1001] = "";

    // copia o conteudo do processador
    sprintf(path, "%s/Hardware/%s.v", proc_dir, name);
    input = fopen(path, "r");
    while(fgets(texto, 1001, input) != NULL)
    {
        if (((strstr(texto, "proc_fx") != NULL) || (strstr(texto, "proc_fl") != NULL)) && (sim_typ == 1))
            fprintf(f_veri, "proc_flx_%s_sim\n", name);
        else if(strcmp(texto, "endmodule") != 0)
            fputs(texto, output);
           memset(texto, 0, sizeof(char) * 1001);
    }
    fclose(input );

    int s1 = (float_point) ? nbmant-1      : nbits-1;
    int s2 = (float_point) ? nbmant+nbexpo : nbits-1;

    fprintf(output, "// Simulacao ------------------------------------------------------------------\n\n");

    int i;

    for(i=0;i<nmioin;i++)
    {
    fprintf(output, "reg signed [%d:0] in_sim_%d = 0;\n", s1, i);
    fprintf(output, "reg req_in_sim_%d = 0;\n", i);
    }
    fprintf(output,"\n");

    for(i=0;i<nuioou;i++)
    {
    fprintf(output, "reg signed [%d:0] out_sig_%d = 0;\n", s2, i);
    fprintf(output, "reg out_en_sim_%d = 0;\n", i);
    }

    fprintf(output, "\nalways @ (*) begin\n");
    for(i=0;i<nmioin;i++)
    {
    fprintf(output, "   if (req_in == %d) in_sim_%d = io_in;\n", (int)pow(2,i),i);
    fprintf(output, "   req_in_sim_%d = req_in == %d;\n", i, (int)pow(2,i),i);
    }
    fprintf(output, "end\n");

    fprintf(output, "\nalways @ (*) begin\n");
    for(i=0;i<nuioou;i++)
    {
    fprintf(output, "   if (out_en == %d) out_sig_%d <= io_out;\n", (int)pow(2,i), i);
    fprintf(output, "   out_en_sim_%d = out_en == %d;\n", i, (int)pow(2,i),i);
    }
    fprintf(output, "end\n\n");

    fprintf(output, "endmodule");

    fclose(output);
}

// gera arquivo verilog com uma instancia do processador
void build_vv_file()
{
    // arquivo .v do processador
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

    if (float_point)
    {
        fprintf(f_veri, "proc_fl\n#(.NBMANT(%d),\n",nbmant);
        fprintf(f_veri,            ".NBEXPO(%d),\n",nbexpo);
    }
    else
    {
        fprintf(f_veri, "proc_fx\n#(.NUBITS(%d),\n",nbits );
        fprintf(f_veri,            ".NBMANT(%d),\n",nbmant);
        fprintf(f_veri,            ".NBEXPO(%d),\n",nbexpo);
        fprintf(f_veri,            ".NUGAIN(%d),\n",nugain);
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

    fprintf(f_veri, "endmodule");

    fclose(f_veri);

    // se for multicore, criar arquivos de simulacao para cada tipo de proc
    if (sim_typ==1)
    {
        build_proc_sim();
        build_proc_flx();
        build_core_flx();
    }
}

void build_tb_file()
{
    double T = 1000.0/clk_frq; // periodo do clock em ns (clk_frq em MHz)

    f_veri = fopen(get_tb_name(), "w");

    fprintf(f_veri,      "`timescale 1ns/1ps\n\n", name);
    fprintf(f_veri,         "module %s_tb();\n\n", name);
    fprintf(f_veri,           "reg clk, rst;\n\n"      );
    fprintf(f_veri,              "integer i;\n"        );
    fprintf(f_veri,           "initial begin\n\n"      );
    fprintf(f_veri, "$dumpfile(\"%s_tb.vcd\");\n", name);
    fprintf(f_veri,     "$dumpvars(0,%s_tb);\n\n", name);

    fprintf(f_veri,   "clk = 0;\n"     );
    fprintf(f_veri,   "rst = 1;\n"     );
    fprintf(f_veri,       "#%f;\n",   T);
    fprintf(f_veri, "rst = 0;\n\n"     );
    fprintf(f_veri, "for (i = 10; i <= 100; i = i + 10) begin\n");
    fprintf(f_veri, "#%f;\n", T*clk_num/10);
    fprintf(f_veri, "$display(\"Progress: \%\%0d\%\%\%\% complete\", i);\n");
    fprintf(f_veri, "end\n"            );
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
    {
    fprintf(f_veri, "reg signed [%d:0] in_%d = 0; // coloque aqui a sua entrada\n", s1, i);
    fprintf(f_veri, "reg req_in_%d = 0;\n", i);
    }
    fprintf(f_veri,"\n");

    for(i=0;i<nuioou;i++)
    {
    fprintf(f_veri, "reg signed [%d:0] out_sig_%d = 0;\n", s2, i);
    fprintf(f_veri, "reg out_en_%d = 0;\n", i);
    }

    fprintf(f_veri, "\nalways @ (*) begin\n");
    fprintf(f_veri, "   proc_io_in = 0;\n");
    for(i=0;i<nmioin;i++)
    {
    fprintf(f_veri, "   if (proc_req_in == %d) proc_io_in = in_%d;\n", (int)pow(2,i),i);
    fprintf(f_veri, "   req_in_%d = proc_req_in == %d;\n", i, (int)pow(2,i),i);
    }
    fprintf(f_veri, "end\n");

    fprintf(f_veri, "\nalways @ (*) begin\n");
    for(i=0;i<nuioou;i++)
    {
    fprintf(f_veri, "   if (proc_out_en == %d) out_sig_%d <= proc_io_out;\n", (int)pow(2,i), i);
    fprintf(f_veri, "   out_en_%d = proc_out_en == %d;\n", i, (int)pow(2,i),i);
    }
    fprintf(f_veri, "end\n");

    fprintf(f_veri, "\nendmodule\n");

    fclose (f_veri);
}

// gera arquivo verilog do program counter para simulacao
void build_pc_file()
{
    char path[1024];
    sprintf(path, "%s/pc_%s.v", temp_dir,name);

    FILE *input;
    FILE *output = fopen(path, "w");

    char texto[1001] = "";

    // copia o conteudo de pc.v
    sprintf(path, "%s/pc.v", hdl_dir);
    input = fopen(path, "r");

    if (sim_typ == 1)
    {
        fgets  (texto , 1001, input);
        fprintf(output, "module pc_%s\n", name);
    }
    while(fgets(texto, 1001, input) != NULL)
    {
        if(strcmp(texto, "endmodule") != 0)
            fputs(texto, output);
           memset(texto, 0, sizeof(char) * 1001);
    }
    fclose(input );

    int num_ins; // guarda o num total de instrucoes

    // pega num_ins no arquivo cmm_log.txt, depois do caractere #
    sprintf(path, "%s/cmm_log.txt", temp_dir);
    input = fopen(path, "r");

    while(fgets(texto, 1001, input) != NULL)
    {
        if(strstr(texto, "#") != NULL)
        {
            memset(texto, 0, sizeof(char) * 1001);
             fgets(texto, 1001, input);
            num_ins = atoi(texto);
        }
        memset(texto, 0, sizeof(char) * 1001);
    }
    fclose(input );

    fprintf(output, "// Simulacao ------------------------------------------------------------------\n\n");

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
    fprintf(output, "reg signed [19:0] linetab =-1;\n");
    // linetabs eh o valor atual registrado
    fprintf(output, "reg signed [19:0] linetabs=-1;\n\n");

    // le o arquivo gerado pelo compilador c com o conteudo da tabela
    fprintf(output, "initial $readmemb(\"pc_%s_mem.txt\", min);\n\n", name);

    fprintf(output, "always @ (posedge clk) begin\n");
    fprintf(output, "    if (val < %d) linetab <= min[val];\n", num_ins);

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

    if (sim_typ == 0)
    {
    // para a simulacao nr clocks depois de achar a instrucao @fim JMP fim
    fprintf(output, "always @ (posedge clk) begin\n");
    fprintf(output, "if (valr%d == %d) begin\n",nr,fim_addr);
    fprintf(f_veri, "$display(\"Fim do programa!\");\n");
    fprintf(f_veri, "$finish;\n");
    fprintf(output, "end\nend\n\n");
    }

    fprintf(output, "endmodule\n");

    fclose(output);
}

// gera arquivo verilog da memoria de dados para simulacao
void build_dt_file()
{
    char path[1024];
    sprintf(path, "%s/mem_data_%s.v", temp_dir,name);

    FILE *input;
    FILE *output = fopen(path, "w");

    char texto[1001] = "";

    // abre mem_data.v para leitura
    sprintf(path, "%s/mem_data.v", hdl_dir);
    input = fopen(path, "r");

    // soh muda o nome do modulo se for simulacao multicore
    if (sim_typ == 1)
    {
        fgets(texto, 1001, input);
        fprintf(output, "module mem_data_%s\n", name);
    }

    // copia ate achar endmodule
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
    for (i = 0; i < v_cont; i++)
    {
        if ((v_tipo[i] == 2) || ((v_tipo[i] == 1) && float_point))
            fprintf(output, "reg [16+NBDATA-1:0] %s=0;\n", v_namo[i]);
        else
            fprintf(output, "reg [NBDATA-1:0] %s=0;\n"   , v_namo[i]);
    }
    
    // inicia o always para registrar as variaveis
    fprintf(output, "\nalways @ (posedge clk) begin\n");

    // registra cada variavel, dependendo do endereco de cada uma
    for (i = 0; i < v_cont; i++)
    {
        if ((v_tipo[i] == 2) || ((v_tipo[i] == 1) && float_point))
            fprintf(output, "   if (addr_w == %d && wr) %s <= {8'd%d,8'd%d,data_in};\n", v_add[i], v_namo[i], nbmant, nbexpo);
        else
            fprintf(output, "   if (addr_w == %d && wr) %s <= data_in;\n", v_add[i], v_namo[i]);
    }

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
                    fprintf(output,"wire [16+NBDATA*2-1:0] comp_%s = {8'd%d, 8'd%d, %s, %s};\n", v_namo[i], nbmant, nbexpo, v_namo[i], v_namo[j]);
            }
        }
    }

    fprintf(output, "\nendmodule\n");

    fclose(output);
}