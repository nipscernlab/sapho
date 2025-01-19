#include "..\Headers\veri_comp.h"
#include "..\Headers\eval.h"
#include "..\Headers\mnemonicos.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

char name[128]; // nome do processador
char  tmp[512]; // guarda, temporariamente, os nomes de arquivos .v, .mif criados

FILE *f_veri;

// variaveis pra salvar no arquivo .v
int  sdepth, nuioin2, nuioou, nugain; // nuioin eh usado internamente pelo flex e da pau -> usar nuioin2!!

void set_name(char *va)
{
    strcpy(name, va);
}

char *get_dname()
{
    strcpy(tmp,      d_name);
    strcat(tmp,        name);
    strcat(tmp, "_data.mif");

    return tmp;
}

char *get_iname()
{
    strcpy(tmp, d_name);
    strcat(tmp, name);
    strcat(tmp, "_inst.mif");

    return tmp;
}

char *get_vname()
{
    strcpy(tmp, d_name);
    strcat(tmp, name);
    strcat(tmp, ".v");

    return tmp;
}

char *get_tb_name()
{
    strcpy(tmp, d_name);
    strcat(tmp, name);
    strcat(tmp, "_tb.v");

    return tmp;
}

void set_nbits(int n)
{
    nbits = n;
}

void set_nbmant(int n)
{
    nbmant = n;
}

void set_nbexpo(int n)
{
    nbexpo = n;
}

void set_ndstac(int n)
{
    ndstac = n;
}

void set_sdepth(int n)
{
    sdepth = n;
}

void set_nuioin(int n)
{
    nuioin2 = n;
}

void set_nuioou(int n)
{
    nuioou = n;
}

void set_float_point(int n)
{
    float_point = n;
}

void set_dir(char* dir)
{
    strcpy(d_name, dir);
}

void set_nugain(int n)
{
    nugain = n;
}

void set_fftsiz(int n)
{
    fftsiz = n;
}

// troca as barras numa string
char *barra_fix(char *dir)
{
    for (int i = 0; i < strlen(dir); i++) if (dir[i] == 92) dir[i] = 47;

    return dir;
}

void build_vfile()
{
    f_veri = fopen(get_vname(), "w");

    fprintf(f_veri, "module %s (\n", name);
    fprintf(f_veri, "input clk, rst,\n");

    int s1 = (float_point) ? nbmant-1      : nbits-1;
    int s2 = (float_point) ? nbmant+nbexpo : nbits-1;

    fprintf(f_veri, "input signed [%d:0] io_in,\n"  , s1);
    fprintf(f_veri, "output signed [%d:0] io_out,\n", s2);

    fprintf(f_veri, "output [%d:0] req_in,\n", nuioin2-1);
    fprintf(f_veri, "output [%d:0] out_en,\n", nuioou -1);
    fprintf(f_veri, "input itr);\n\n");

    fprintf(f_veri, "wire signed [%d:0] in_float;\n"   , s2);
    fprintf(f_veri, "wire signed [%d:0] out_float;\n\n", s2);

    if (float_point)
    fprintf(f_veri, "int2float #(.MAN(%d),.EXP(%d)) i2f (io_in, in_float);\n\n", nbmant, nbexpo);
    else
    fprintf(f_veri, "assign in_float = io_in;\n\n");

    fprintf(f_veri, "wire proc_req_in, proc_out_en;\n");
    fprintf(f_veri, "wire [%d:0] addr_in;\n"   , (int)ceil(log2(nuioin2)-1));
    fprintf(f_veri, "wire [%d:0] addr_out;\n\n", (int)ceil(log2(nuioou )-1));

    if (float_point) // exclusivo ponto flutuante
    {
        fprintf(f_veri, "proc_fl #(.NBMANT(%d),\n", nbmant);
        fprintf(f_veri, ".NBEXPO(%d),\n", nbexpo); // possivelmente pode ser usado em ambos;
    }
    else // exclusivo ponto fixo
    {
        fprintf(f_veri, "proc_fx #(.NUBITS(%d),\n", nbits );
        fprintf(f_veri,           ".NUGAIN(%d),\n", nugain);
    }

    fprintf(f_veri, ".MDATAS(%d),\n", n_dat  );
    fprintf(f_veri, ".MINSTS(%d),\n", n_ins  );
    fprintf(f_veri, ".SDEPTH(%d),\n", sdepth );
    fprintf(f_veri, ".NUIOIN(%d),\n", nuioin2);
    fprintf(f_veri, ".NUIOOU(%d),\n", nuioou );
    fprintf(f_veri, ".FFTSIZ(%d),\n", fftsiz );

    if (itr_addr != 0) fprintf(f_veri, ".ITRADD(%d),\n", itr_addr); // vai ter interrupcao

    for (int i = 0; i < m_count; i++) fprintf(f_veri, ".%s(1),\n", m_name[i]);

    fprintf(f_veri, ".DFILE(\"%s%s_data.mif\"),\n", barra_fix(d_name), name);
    fprintf(f_veri, ".IFILE(\"%s%s_inst.mif\")\n", d_name, name);
    fprintf(f_veri, ") p_%s (clk, rst, in_float, out_float, addr_in, addr_out, proc_req_in, proc_out_en, itr);\n\n", name);

    if (float_point)
    fprintf(f_veri, "float2int #(.EXP(%d),.MAN(%d)) f2i (out_float, io_out);\n\n", nbexpo, nbmant);
    else
    fprintf(f_veri, "assign io_out = out_float;\n\n");

    if (nuioin2 == 1)
    fprintf(f_veri, "assign req_in = proc_req_in;\n");
    else
    fprintf(f_veri, "addr_dec #(%d) dec_in (proc_req_in, addr_in , req_in);\n"  , nuioin2);

    if (nuioou == 1)
    fprintf(f_veri, "assign out_en = proc_out_en;\n\n");
    else
    fprintf(f_veri, "addr_dec #(%d) dec_out(proc_out_en, addr_out, out_en);\n\n", nuioou );

    fprintf(f_veri, "endmodule\n");

    fclose (f_veri);
}

void build_tb_file()
{
    double T = 1000.0/clk_frq; // periodo do clock em ns (clk_frq em kHz)

    f_veri = fopen(get_tb_name(), "w");

    fprintf(f_veri, "`timescale 1ns/1ps\n\n", name);
    fprintf(f_veri, "module %s_tb();\n\n", name);
    fprintf(f_veri, "reg clk, rst;\n\n");
    fprintf(f_veri, "initial begin\n\n");
    fprintf(f_veri, "$dumpfile(\"%s_tb.vcd\");\n", name);
    fprintf(f_veri, "$dumpvars(0,%s_tb);\n\n", name);

    fprintf(f_veri, "clk = 0;\n");
    fprintf(f_veri, "rst = 1;\n");
    fprintf(f_veri, "#%f;\n",T);
    fprintf(f_veri, "rst = 0;\n\n");
    fprintf(f_veri, "#%f;\n", T*clk_num);
    fprintf(f_veri, "$finish;\n\n");
    fprintf(f_veri, "end\n\n");

    fprintf(f_veri, "always #%f clk = ~clk;\n\n", T/2.0);

    int s1 = (float_point) ? nbmant-1      : nbits-1;
    int s2 = (float_point) ? nbmant+nbexpo : nbits-1;

    fprintf(f_veri, "wire signed [%d:0] proc_io_out;\n", s2);
    fprintf(f_veri, "wire [%d:0] proc_req_in;\n", nuioin2-1);
    fprintf(f_veri, "wire [%d:0] proc_out_en;\n\n", nuioou -1);

    fprintf(f_veri, "%s proc(clk,rst,%d'd0,proc_io_out,proc_req_in,proc_out_en,1'b0);\n\n", name, s1+1);

    fprintf(f_veri, "endmodule\n");

    fclose (f_veri);
}