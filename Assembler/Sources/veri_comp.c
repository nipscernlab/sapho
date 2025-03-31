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

// pega num de instr no arquivo cmm_log.txt, depois do caractere #
int get_num_ins()
{
    int num_ins;
    char path[1024]; sprintf(path, "%s/cmm_log.txt", temp_dir);
    FILE *input = fopen(path, "r");

    char texto[1001] = "";
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

    return num_ins;
}

// gera arquivo verilog com uma instancia do processador
void build_vv_file()
{
    // arquivo .v do processador
    f_veri = fopen(get_vname(), "w");

    fprintf(f_veri, "module %s (\n", name);
    fprintf(f_veri, "input  clk, rst,\n");

    int s1 = nbits-1;
    int s2 = nbits-1;

    fprintf(f_veri, "input  signed [%d:0] io_in ,\n", s1);
    fprintf(f_veri, "output signed [%d:0] io_out,\n", s2);

    fprintf(f_veri, "output [%d:0] req_in,\n", nmioin-1);
    fprintf(f_veri, "output [%d:0] out_en,\n", nuioou-1);
    fprintf(f_veri, "input itr);\n\n");

    fprintf(f_veri, "wire proc_req_in, proc_out_en;\n");

    int d1 = (int)ceil(log2(nmioin)-1); // ta dando problema quando eh < 2
    int d2 = (int)ceil(log2(nuioou)-1); // aqui tb (rever)

    fprintf(f_veri, "wire [%d:0] addr_in;\n"   , d1);
    fprintf(f_veri, "wire [%d:0] addr_out;\n\n", d2);

    fprintf(f_veri, "wire mem_wr;\n");
    fprintf(f_veri, "wire [%d:0] mem_addr_wr;\n" , (int)ceil(log2(n_dat)-1));
    fprintf(f_veri, "wire [%d:0] pc_sim_val;\n\n", (int)ceil(log2(n_ins)-1));

    fprintf(f_veri, "processor\n#(.NUBITS(%d),\n",nbits );
    fprintf(f_veri,              ".NBMANT(%d),\n",nbmant);
    fprintf(f_veri,              ".NBEXPO(%d),\n",nbexpo);

    int nb = (n_dat>n_ins) ? (int)ceil(log2(n_dat)) : (int)ceil(log2(n_ins));
    fprintf(f_veri, ".NBOPER(%d),\n", nb);
    fprintf(f_veri, ".NUGAIN(%d),\n", nugain);

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
    fprintf(f_veri, ") p_%s (clk, rst, io_in, io_out, addr_in, addr_out, proc_req_in, proc_out_en, itr, mem_wr, mem_addr_wr,pc_sim_val);\n\n", name);

    if (nmioin == 1)
    fprintf(f_veri, "assign req_in = proc_req_in;\n");
    else
    fprintf(f_veri, "addr_dec #(%d) dec_in (proc_req_in, addr_in , req_in);\n"  , nmioin);

    if (nuioou == 1)
    fprintf(f_veri, "assign out_en = proc_out_en;\n\n");
    else
    fprintf(f_veri, "addr_dec #(%d) dec_out(proc_out_en, addr_out, out_en);\n\n", nuioou);

    // simulacao --------------------------------------------------------------

    fprintf(f_veri, "// ----------------------------------------------------------------------------\n");
    fprintf(f_veri, "// Simulacao ------------------------------------------------------------------\n");
    fprintf(f_veri, "// ----------------------------------------------------------------------------\n\n");

    fprintf(f_veri, "`ifdef __ICARUS__\n\n");

    fprintf(f_veri, "// variaveis ------------------------------------------------------------------\n\n");

    int i;
    // cria um registrador para cada variavel encontrada
    for (i = 0; i < v_cont; i++)
    {
        if (v_tipo[i] == 2)
            fprintf(f_veri, "reg [16+%d-1:0] %s=0;\n", nbits, v_namo[i]);
        else
            fprintf(f_veri, "reg [%d-1:0] %s=0;\n"   , nbits, v_namo[i]);
    }
    
    // inicia o always para registrar as variaveis
    fprintf(f_veri, "\nalways @ (posedge clk) begin\n");

    // registra cada variavel, dependendo do endereco de cada uma
    for (i = 0; i < v_cont; i++)
    {
        if (v_tipo[i] == 2)
            fprintf(f_veri, "   if (mem_addr_wr == %d && mem_wr) %s <= {8'd%d,8'd%d,io_out};\n", v_add[i], v_namo[i], nbmant, nbexpo);
        else
            fprintf(f_veri, "   if (mem_addr_wr == %d && mem_wr) %s <= io_out;\n", v_add[i], v_namo[i]);
    }

    fprintf(f_veri, "end\n\n");

    // se a variavel for comp ...
    // junta a parte real e complexa em uma variavel do dobro de tamanho
    for (i = 0; i < v_cont; i++)
    {
        if (v_tipo[i] == 3)
        {
            for (int j = 0; j < v_cont; j++)
            {
                char ni[64],nj[64];
                strcpy(ni,v_namo[i]);
                strcpy(nj,v_namo[j]);
                ni[strlen(ni)-3] = '\0';
                nj[strlen(nj)-3] = '\0';

                char im[64];
                sprintf(im, "%s_i", ni);
                if (strcmp(nj,im) == 0)
                    fprintf(f_veri,"wire [16+%d*2-1:0] comp_%s = {8'd%d, 8'd%d, %s, %s};\n", nbits, v_namo[i], nbmant, nbexpo, v_namo[i], v_namo[j]);
            }
        }
    }

    fprintf(f_veri, "\n// instrucoes -----------------------------------------------------------------\n\n");

    int nreg = 10;
    for (int i = 0; i < nreg; i++)
    {
        fprintf(f_veri, "reg [%d-1:0] valr%d=0;\n", nbits, i+1);
    }
    fprintf(f_veri, "\n");

    fprintf(f_veri, "reg [19:0] min [0:%d-1];\n\n", get_num_ins());

    fprintf(f_veri, "reg signed [19:0] linetab =-1;\n");
    fprintf(f_veri, "reg signed [19:0] linetabs=-1;\n\n");

    fprintf(f_veri, "initial	$readmemb(\"pc_%s_mem.txt\",min);\n\n", name);

    fprintf(f_veri, "always @ (posedge clk) begin\n");
    fprintf(f_veri, "if (pc_sim_val < %d) linetab <= min[pc_sim_val];\n", get_num_ins());
    fprintf(f_veri, "valr1  <= pc_sim_val;\n");
    fprintf(f_veri, "valr2  <= valr1;\n");
    fprintf(f_veri, "valr3  <= valr2;\n");
    fprintf(f_veri, "valr4  <= valr3;\n");
    fprintf(f_veri, "valr5  <= valr4;\n");
    fprintf(f_veri, "valr6  <= valr5;\n");
    fprintf(f_veri, "valr7  <= valr6;\n");
    fprintf(f_veri, "valr8  <= valr7;\n");
    fprintf(f_veri, "valr9  <= valr8;\n");
    fprintf(f_veri, "valr10 <= valr9;\n");
    fprintf(f_veri, "linetabs <= linetab;\n");
    fprintf(f_veri, "end\n\n");

    if (sim_typ==0)
    {
    fprintf(f_veri, "always @ (posedge clk) if (valr10 == %d) begin\n", fim_addr);
    fprintf(f_veri, "$display(\"Fim do programa!\");\n");
    fprintf(f_veri, "$finish;\n");
    fprintf(f_veri, "end\n\n");
    }

    fprintf(f_veri, "`endif\n\n");

    fprintf(f_veri, "endmodule");

    fclose(f_veri);
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

    fprintf(f_veri,   "clk = 0;\n"   );
    fprintf(f_veri,   "rst = 1;\n"   );
    fprintf(f_veri,       "#%f;\n", T);
    fprintf(f_veri, "rst = 0;\n\n"   );
    fprintf(f_veri, "for (i = 10; i <= 100; i = i + 10) begin\n");
    fprintf(f_veri, "#%f;\n", T*clk_num/10);
    fprintf(f_veri, "$display(\"Progress: \%\%0d\%\%\%\% complete\", i);\n");
    fprintf(f_veri, "end\n"          );
    fprintf(f_veri, "$finish;\n\n"   );
    fprintf(f_veri,      "end\n\n"   );

    fprintf(f_veri, "always #%f clk = ~clk;\n\n", T/2.0);

    int s1 = nbits-1;
    int s2 = nbits-1;

    fprintf(f_veri, "reg signed [%d:0] proc_io_in = 0;\n", s1);
    fprintf(f_veri, "wire signed [%d:0] proc_io_out;\n"  , s2);
    fprintf(f_veri, "wire [%d:0] proc_req_in;\n"   , nmioin-1);
    fprintf(f_veri, "wire [%d:0] proc_out_en;\n\n" , nuioou-1);

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