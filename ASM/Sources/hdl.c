// ----------------------------------------------------------------------------
// rotinas para geracao dos arquivos .v do processador e test-bench -----------
// ----------------------------------------------------------------------------

// includes globais
#include <string.h>
#include <stdlib.h>
#include  <stdio.h>
#include   <math.h>

// includes locais
#include "..\Headers\eval.h"
#include "..\Headers\opcodes.h"
#include "..\Headers\simulacao.h"

// ----------------------------------------------------------------------------
// funcoes auxiliares ---------------------------------------------------------
// ----------------------------------------------------------------------------

// troca \ por / no em um path
void force_rightbar(char *str){while (*str) {if (*str == '\\') *str = '/'; str++;}}

// ----------------------------------------------------------------------------
// gera arquivo verilog com uma instancia do processador ----------------------
// ----------------------------------------------------------------------------

void hdl_vv_file(int n_ins, int n_dat, int nbopr, int itr_addr)
{
    // ------------------------------------------------------------------------
    // cria o arquivo .v do processador ---------------------------------------
    // ------------------------------------------------------------------------

    char    tmp[512];
    sprintf(tmp, "%s/Hardware/%s.v", proc_dir, prname);

    FILE *f_veri = fopen(tmp,"w");

    // ------------------------------------------------------------------------
    // cria cabecalho e portas do modulo --------------------------------------
    // ------------------------------------------------------------------------

    fprintf(f_veri, "module %s (\n", prname);
    fprintf(f_veri, "input  clk, rst,\n");

    fprintf(f_veri, "input  signed [%d:0] io_in ,\n", nubits-1);
    fprintf(f_veri, "output signed [%d:0] io_out,\n", nubits-1);

    fprintf(f_veri, "output [%d:0] req_in,\n", nuioin-1);
    fprintf(f_veri, "output [%d:0] out_en,\n", nuioou-1);
    fprintf(f_veri, "input itr);\n\n");

    // ------------------------------------------------------------------------
    // wires de interface com I/O ---------------------------------------------
    // ------------------------------------------------------------------------

    fprintf(f_veri, "wire proc_req_in, proc_out_en;\n");

    fprintf(f_veri, "wire [%d:0] addr_in;\n"   ,  (int)ceil(log2(nuioin)-1));
    fprintf(f_veri, "wire [%d:0] addr_out;\n\n",  (int)ceil(log2(nuioou)-1));

    // ------------------------------------------------------------------------
    // wires de interface com simulacao ---------------------------------------
    // ------------------------------------------------------------------------

    // acesso as variaveis na memoria de dados
    fprintf(f_veri, "wire mem_wr;\n");
    fprintf(f_veri, "wire [%d:0] mem_addr_wr;\n" , (int)ceil(log2(n_dat)-1));

    // acesso ao indice da instrucao no program counter
    fprintf(f_veri, "wire [%d:0] pc_sim_val;\n\n", (int)ceil(log2(n_ins)-1));

    // ------------------------------------------------------------------------
    // parametros do processador ----------------------------------------------
    // ------------------------------------------------------------------------

    fprintf(f_veri, "processor#(.NUBITS(%d),\n", nubits);
    fprintf(f_veri,            ".NBMANT(%d),\n", nbmant);
    fprintf(f_veri,            ".NBEXPO(%d),\n", nbexpo);
    fprintf(f_veri,            ".NBOPER(%d),\n", nbopr );
    fprintf(f_veri,            ".NUGAIN(%d),\n", nugain);
    fprintf(f_veri,            ".MDATAS(%d),\n", n_dat );
    fprintf(f_veri,            ".MINSTS(%d),\n", n_ins );
    fprintf(f_veri,            ".SDEPTH(%d),\n", sdepth);
    fprintf(f_veri,            ".NUIOIN(%d),\n", nuioin);
    fprintf(f_veri,            ".NUIOOU(%d),\n", nuioou);
    fprintf(f_veri,            ".FFTSIZ(%d),\n", fftsiz);

    // se tem interrupcao, coloca o endereco dela no processador
    if (itr_addr != 0) fprintf(f_veri, ".ITRADD(%d),\n", itr_addr);

    // ------------------------------------------------------------------------
    // alocacao dinamica de recursos ------------------------------------------
    // ------------------------------------------------------------------------

    for (int i = 0; i < opc_cnt(); i++) fprintf(f_veri, ".%s(1),\n", opc_get(i));

    // ------------------------------------------------------------------------
    // finaizacao da instancia do processador ---------------------------------
    // ------------------------------------------------------------------------

    fprintf(f_veri, ".DFILE(\"%s_data.mif\"),\n", prname);
    fprintf(f_veri, ".IFILE(\"%s_inst.mif\")\n" , prname);
    fprintf(f_veri, ") p_%s (clk, rst, io_in, io_out, addr_in, addr_out, proc_req_in, proc_out_en, itr, mem_wr, mem_addr_wr,pc_sim_val);\n\n", prname);

    // ------------------------------------------------------------------------
    // decodificadores de endereco de I/O -------------------------------------
    // ------------------------------------------------------------------------

    // portas de entrada
    if (nuioin == 1)
    fprintf(f_veri, "assign req_in = proc_req_in;\n");
    else
    fprintf(f_veri, "addr_dec #(%d) dec_in (proc_req_in, addr_in , req_in);\n"  , nuioin);

    // portas de saida
    if (nuioou == 1)
    fprintf(f_veri, "assign out_en = proc_out_en;\n\n");
    else
    fprintf(f_veri, "addr_dec #(%d) dec_out(proc_out_en, addr_out, out_en);\n\n", nuioou);

    // ------------------------------------------------------------------------
    // inicio de interface com simulacao (iverilog+gtkwave) -------------------
    // ------------------------------------------------------------------------

    fprintf(f_veri, "// ----------------------------------------------------------------------------\n");
    fprintf(f_veri, "// Simulacao ------------------------------------------------------------------\n");
    fprintf(f_veri, "// ----------------------------------------------------------------------------\n\n");

    fprintf(f_veri, "`ifdef __ICARUS__\n\n");

    // ------------------------------------------------------------------------
    // cadastra portas de I/O para simulacao ----------------------------------
    // ------------------------------------------------------------------------

    fprintf(f_veri, "// I/O ------------------------------------------------------------------------\n\n");

    // cadastra portas de entrada
    for(int i=0;i<nuioin;i++)
    {
    fprintf(f_veri, "reg signed [%d:0] in_sim_%d = 0;\n", nubits-1, i);
    fprintf(f_veri, "reg req_in_sim_%d = 0;\n", i);
    }
    fprintf(f_veri,"\n");

    // cadastra portas de saida
    for(int i=0;i<nuioou;i++)
    {
    fprintf(f_veri, "reg signed [%d:0] out_sig_%d = 0;\n", nubits-1, i);
    fprintf(f_veri, "reg out_en_sim_%d = 0;\n", i);
    }

    // decodifica porta de entrada
    fprintf(f_veri, "\nalways @ (*) begin\n");
    for(int i=0;i<nuioin;i++)
    {
    fprintf(f_veri, "   if (req_in == %d) in_sim_%d = io_in;\n", (int)pow(2,i),i);
    fprintf(f_veri, "   req_in_sim_%d = req_in == %d;\n",     i, (int)pow(2,i),i);
    }
    fprintf(f_veri, "end\n");

    // decodifica portas de saida
    fprintf(f_veri, "\nalways @ (*) begin\n");
    for(int i=0;i<nuioou;i++)
    {
    fprintf(f_veri, "   if (out_en == %d) out_sig_%d <= io_out;\n", (int)pow(2,i), i);
    fprintf(f_veri, "   out_en_sim_%d = out_en == %d;\n",        i, (int)pow(2,i),i);
    }
    fprintf(f_veri, "end\n\n");

    // ------------------------------------------------------------------------
    // cadastra variaveis do usuario para simulacao ---------------------------
    // ------------------------------------------------------------------------

    fprintf(f_veri, "// variaveis ------------------------------------------------------------------\n\n");

    // cria um registrador para cada variavel encontrada
    for (int i = 0; i < sim_cnt(); i++)
    {
        if (sim_type(i) == 2)
            fprintf(f_veri, "reg [16+%d-1:0] %s=0;\n", nubits, sim_name(i));
        else
            fprintf(f_veri, "reg [%d-1:0] %s=0;\n"   , nubits, sim_name(i));
    }
    
    // inicia o always para registrar as variaveis
    fprintf(f_veri, "\nalways @ (posedge clk) begin\n");
    // registra cada variavel, dependendo do endereco de cada uma
    for (int i = 0; i < sim_cnt(); i++)
    {
        if (sim_type(i) == 2)
            fprintf(f_veri, "   if (mem_addr_wr == %d && mem_wr) %s <= {8'd%d,8'd%d,io_out};\n", sim_addr(i), sim_name(i), nbmant, nbexpo);
        else
            fprintf(f_veri, "   if (mem_addr_wr == %d && mem_wr) %s <= io_out;\n"              , sim_addr(i), sim_name(i));
    }
    fprintf(f_veri, "end\n\n");

    // se a variavel for comp ...
    // junta a parte real e imag em uma variavel do dobro de tamanho
    char ni[64],nj[64],im[64];
    for (int i = 0; i < sim_cnt(); i++)
    {
        if (sim_type(i) == 3)
        {
            for (int j = 0; j < sim_cnt(); j++)
            {
                strcpy(ni,sim_name(i));
                strcpy(nj,sim_name(j));
                ni[strlen(ni)-3] = '\0';
                nj[strlen(nj)-3] = '\0';

                sprintf(im, "%s_i", ni);
                if (strcmp(nj,im) == 0)
                    fprintf(f_veri,"wire [16+%d*2-1:0] comp_%s = {8'd%d, 8'd%d, %s, %s};\n", nubits, sim_name(i), nbmant, nbexpo, sim_name(i), sim_name(j));
            }
        }
    }

    // ------------------------------------------------------------------------
    // interface com istrucoes cmm e asm --------------------------------------
    // ------------------------------------------------------------------------

    fprintf(f_veri, "\n// instrucoes -----------------------------------------------------------------\n\n");

    // cria 10 registradores para retardar a instrucao @fim
    int nreg = 10;
    for (int i = 0; i < nreg; i++)
    {
        fprintf(f_veri, "reg [%d-1:0] valr%d=0;\n", nubits, i+1);
    }
    fprintf(f_veri, "\n");

    char aux[256]; eval_get("cmm_log.txt","num_ins", aux); int num_ins = atoi(aux);

    // cria memoria que vai guardar a tabela de instrucoes
    fprintf(f_veri, "reg [19:0] min [0:%d-1];\n\n", num_ins);
    // cria interface com essa memoria
    fprintf(f_veri, "reg signed [19:0] linetab =-1;\n"  );
    fprintf(f_veri, "reg signed [19:0] linetabs=-1;\n\n");
    // inicializa a memoria com o arquivo .txt
    fprintf(f_veri, "initial	$readmemb(\"pc_%s_mem.txt\",min);\n\n"  , prname );
    // executa os registros
    fprintf(f_veri, "always @ (posedge clk) begin\n");
    fprintf(f_veri, "if (pc_sim_val < %d) linetab <= min[pc_sim_val];\n", num_ins);
    fprintf(f_veri, "linetabs <= linetab;   \n");
    fprintf(f_veri, "valr1    <= pc_sim_val;\n");
    // delays para o @fim
    fprintf(f_veri, "valr2    <= valr1;\n");
    fprintf(f_veri, "valr3    <= valr2;\n");
    fprintf(f_veri, "valr4    <= valr3;\n");
    fprintf(f_veri, "valr5    <= valr4;\n");
    fprintf(f_veri, "valr6    <= valr5;\n");
    fprintf(f_veri, "valr7    <= valr6;\n");
    fprintf(f_veri, "valr8    <= valr7;\n");
    fprintf(f_veri, "valr9    <= valr8;\n");
    fprintf(f_veri, "valr10   <= valr9;\n");

    fprintf(f_veri, "end\n\n");

    // se for simulacao tipo single, cadastra $finish no endewreco @fim
    if (sim_multi()==0)
    {
    fprintf(f_veri, "always @ (posedge clk) if (valr10 == %d) begin\n", sim_get_fim());
    fprintf(f_veri, "$display(\"Fim do programa!\");\n");
    fprintf(f_veri, "$finish;\n");
    fprintf(f_veri, "end\n\n");
    }

    // ------------------------------------------------------------------------
    // finaliza arquivo -------------------------------------------------------
    // ------------------------------------------------------------------------

    fprintf(f_veri, "`endif\n\n");
    fprintf(f_veri, "endmodule" );

    fclose(f_veri);
}

// ----------------------------------------------------------------------------
// gera arquivo verilog com o test-bench do processador -----------------------
// ----------------------------------------------------------------------------

void hdl_tb_file()
{
    // ------------------------------------------------------------------------
    // cria o arquivo .v ------------------------------------------------------
    // ------------------------------------------------------------------------

    char    tmp[512];
    sprintf(tmp, "%s/%s_tb.v", temp_dir, prname);

    FILE *f_veri = fopen(tmp,"w");

    // ------------------------------------------------------------------------
    // cria cabecalho do modulo -----------------------------------------------
    // ------------------------------------------------------------------------

    double T = 1000.0/sim_clk(); // periodo do clock em ns (clk frq em MHz)

    fprintf(f_veri,          "`timescale 1ns/1ps\n\n", prname);
    fprintf(f_veri,             "module %s_tb();\n\n", prname);
    fprintf(f_veri,               "reg clk, rst;\n\n"        );
    fprintf(f_veri,                    "integer i;\n"        );
    fprintf(f_veri,               "initial begin\n\n"        );
    fprintf(f_veri, "    $dumpfile(\"%s_tb.vcd\");\n", prname);
    fprintf(f_veri,     "    $dumpvars(0,%s_tb);\n\n", prname);

    // ------------------------------------------------------------------------
    // inicializacao (reset e $finish) ----------------------------------------
    // ------------------------------------------------------------------------

    fprintf(f_veri,   "    clk = 0;\n"   );
    fprintf(f_veri,   "    rst = 1;\n"   );
    fprintf(f_veri,       "    #%f;\n", T);
    fprintf(f_veri, "    rst = 0;\n\n"   );
    fprintf(f_veri, "    for (i = 10; i <= 100; i = i + 10) begin\n");
    fprintf(f_veri, "        #%f;\n"  , T*sim_clk_num()/10          );
    fprintf(f_veri, "        $display(\"Progress: \%\%0d\%\%\%\% complete\", i);\n");
    fprintf(f_veri, "    end\n"          );
    fprintf(f_veri, "    $finish;\n\n"   );
    fprintf(f_veri,          "end\n\n"   );

    // ------------------------------------------------------------------------
    // geracao do clock -------------------------------------------------------
    // ------------------------------------------------------------------------

    fprintf(f_veri, "always #%f clk = ~clk;\n\n", T/2.0);

    // ------------------------------------------------------------------------
    // portas de interface e instancia do processador -------------------------
    // ------------------------------------------------------------------------

    fprintf(f_veri, "reg  signed [%d:0] proc_io_in = 0;\n", nubits-1);
    fprintf(f_veri, "wire signed [%d:0] proc_io_out;\n"   , nubits-1);
    fprintf(f_veri, "wire [%d:0] proc_req_in;\n"          , nuioin-1);
    fprintf(f_veri, "wire [%d:0] proc_out_en;\n\n"        , nuioou-1);

    fprintf(f_veri, "%s proc(clk,rst,proc_io_in,proc_io_out,proc_req_in,proc_out_en,1'b0);\n\n", prname);

    // ------------------------------------------------------------------------
    // interface com portas de entrada ----------------------------------------
    // ------------------------------------------------------------------------

    // cadastra portas de entrada
    for(int i=0;i<nuioin;i++)
    {
    fprintf(f_veri, "integer data_in_%d;\n", i);
    fprintf(f_veri, "reg signed [%d:0] in_%d = 0;\n", nubits-1, i);
    fprintf(f_veri, "reg req_in_%d = 0;\n\n", i);
    }
    fprintf(f_veri,"\n");

    // inicializa portas de entrada
    fprintf(f_veri, "initial begin\n");
    for(int i=0;i<nuioin;i++)
    {
    force_rightbar(proc_dir);
    fprintf(f_veri, "    data_in_%d = $fopen(\"%s/Simulation/input_%d.txt\", \"r\"); // write your input data into this file\n",i,proc_dir,i); 
    }
    fprintf(f_veri, "end\n\n");

    // decodifica porta de entrada
    fprintf(f_veri, "integer scan_result;\n");
    fprintf(f_veri, "always @ (*) begin  \n");
    fprintf(f_veri, "    proc_io_in = 0; \n");
    for(int i=0;i<nuioin;i++)
    {
    fprintf(f_veri, "    if (proc_req_in == %d) begin\n"  ,             (int)pow(2,i)  );
    fprintf(f_veri, "        scan_result = $fscanf(data_in_%d, \"%%d\", in_%d);\n", i,i);
    fprintf(f_veri, "        proc_io_in  = in_%d;\n"                                 ,i);
    fprintf(f_veri, "    end\n"                                                        );
    fprintf(f_veri, "    req_in_%d = proc_req_in == %d;\n",          i, (int)pow(2,i),i);
    }
    fprintf(f_veri, "end\n\n");

    // ------------------------------------------------------------------------
    // interface com portas de saida ------------------------------------------
    // ------------------------------------------------------------------------

    // cadastra portas de saida
    for(int i=0;i<nuioou;i++)
    {
    fprintf(f_veri, "reg signed [%d:0] out_sig_%d = 0;\n", nubits-1, i);
    fprintf(f_veri, "reg out_en_%d = 0;\n", i);
    }

    // decodifica portas de saida
    fprintf(f_veri, "\nalways @ (*) begin\n");
    for(int i=0;i<nuioou;i++)
    {
    fprintf(f_veri, "    if (proc_out_en == %d) out_sig_%d <= proc_io_out;\n", (int)pow(2,i),i);
    fprintf(f_veri, "    out_en_%d = proc_out_en == %d;\n",                 i, (int)pow(2,i),i);
    }
    fprintf(f_veri, "end\n\n");

    // ------------------------------------------------------------------------
    // finaliza arquivo -------------------------------------------------------
    // ------------------------------------------------------------------------

    fprintf(f_veri, "endmodule\n");

    fclose (f_veri);
}