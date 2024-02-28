#include "veri_comp.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "eval.h"
#include "mnemonicos.h"

char name[128];
char  tmp[512];
FILE *f_veri;
int sdepth, nuioin2, nuioou, nugain; // nuioin eh usado internamente pelo flex e da pau -> usar nuioin2!!

void set_name(char *va)
{
    strcpy(name, va);
}

char *get_dname()
{
    strcpy(tmp, d_name);
    strcat(tmp, name);
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



char *barra_fix(char *dir)
{
    int i;

    for (i = 0; i < strlen(dir); i++) if (dir[i] == 92) dir[i] = 47;

    return dir;
}

void build_vfile()
{
    f_veri = fopen(get_vname(), "w");

    fprintf(f_veri, "module %s (\n", name);
    fprintf(f_veri, "input clk, rst,\n");

    int s1 = (float_point) ? nbmant-1 : nbits-1;
    int s2 = (float_point) ? nbmant+nbexpo : nbits-1;

    fprintf(f_veri, "input signed [%d:0] io_in,\n", s1);
    fprintf(f_veri, "output signed [%d:0] io_out,\n", s2);

    fprintf(f_veri, "output [%d:0] req_in,\n", nuioin2-1);
    fprintf(f_veri, "output [%d:0] out_en);\n\n", nuioou-1);

    fprintf(f_veri, "wire signed [%d:0] in_float;\n", s2);
    fprintf(f_veri, "wire signed [%d:0] out_float;\n\n", s2);
    if (float_point)
    fprintf(f_veri, "int2float #(.MAN(%d),.EXP(%d)) i2f (io_in, in_float);\n\n", nbmant, nbexpo);
    else
    fprintf(f_veri, "assign in_float = io_in;\n\n");

    fprintf(f_veri, "wire proc_req_in, proc_out_en;\n");
    fprintf(f_veri, "wire [%d:0] addr_in;\n", (int)ceil(log2(nuioin2)-1));
    fprintf(f_veri, "wire [%d:0] addr_out;\n\n", (int)ceil(log2(nuioou)-1));

    if (float_point) // exclusivo ponto flutuante
    {
    fprintf(f_veri, "proc_fl #(.NBMANT(%d),\n", nbmant);
    fprintf(f_veri, ".NBEXPO(%d),\n", nbexpo); // possivelmente pode ser usado em ambos;
    }
    else // exclusivo ponto fixo
    {
        fprintf(f_veri, "proc_fx #(.NUBITS(%d),\n", nbits);
        fprintf(f_veri, ".NUGAIN(%d),\n", nugain);
    }


    fprintf(f_veri, ".MDATAS(%d),\n", n_dat + ndstac);
    fprintf(f_veri, ".MINSTS(%d),\n", n_ins);
    fprintf(f_veri, ".SDEPTH(%d),\n", sdepth);
    fprintf(f_veri, ".NUIOIN(%d),\n", nuioin2);
    fprintf(f_veri, ".NUIOOU(%d),\n", nuioou);



    int i;
    for (i = 0; i < m_count; i++) fprintf(f_veri, ".%s(1),\n", m_name[i]);

    fprintf(f_veri, ".DFILE(\"%s%s_data.mif\"),\n", barra_fix(d_name), name);
    fprintf(f_veri, ".IFILE(\"%s%s_inst.mif\")\n", d_name, name);
    fprintf(f_veri, ") p_%s (clk, rst, in_float, out_float, addr_in, addr_out, proc_req_in, proc_out_en);\n\n", name);

    if (float_point)
    fprintf(f_veri, "float2int #(.EXP(%d),.MAN(%d)) f2i (out_float, io_out);\n\n", nbexpo, nbmant);
    else
    fprintf(f_veri, "assign io_out = out_float;\n\n");

    fprintf(f_veri, "addr_dec #(%d) dec_in (proc_req_in, addr_in , req_in);\n", nuioin2);
    fprintf(f_veri, "addr_dec #(%d) dec_out(proc_out_en, addr_out, out_en);\n\n", nuioou);

    fprintf(f_veri, "endmodule\n");

    fclose(f_veri);
}
