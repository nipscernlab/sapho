#ifndef VERI_COMP_H_INCLUDED
#define VERI_COMP_H_INCLUDED

int  ndstac, nbmant, nbexpo, nbits;
int  float_point;
char d_name[512];

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
void set_nugain     (int n);
void set_float_point(int n);
void set_dir        (char* dir);

void build_vfile();

#endif // VERI_COMP_H_INCLUDED
