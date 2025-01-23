#ifndef VERI_COMP_H_INCLUDED
#define VERI_COMP_H_INCLUDED

int  ndstac, nbmant, nbexpo, nbits, fftsiz;
int  float_point; // diz se eh ou nao o processador em ponto flutuante
char d_name[512];

int clk_frq;
int clk_num;

void    set_name(char *va);
char  *get_dname();
char  *get_iname();

void set_nbits      (int n);
void set_nbmant     (int n);
void set_nbexpo     (int n);
void set_ndstac     (int n);
void set_sdepth     (int n);
void set_nuioin     (int n);
void set_nuioou     (int n);
void set_fftsiz     (int n);
void set_nugain     (int n);
void set_float_point(int n);
void set_dir        (char* dir);

void build_vv_file();
void build_tb_file();
void build_pc_file();
void build_dt_file();

#endif // VERI_COMP_H_INCLUDED
