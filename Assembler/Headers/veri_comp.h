extern int  ndstac, nbmant, nbexpo, nbits, fftsiz;

extern char proc_dir[1024]; // diretorio do processador
extern char temp_dir[1024]; // diretorio da pasta Tmp
extern char  hdl_dir[1024]; // diretorio da pasta HDL

extern int clk_frq;         // frequencia do cloak (em MHz) para a simulacao
extern int clk_num;         // num de clocks maximo a simular
extern int sim_typ;         // tipo de simulacao (um proc ou multicore)

void    set_name(char *va);
char  *get_dname();
char  *get_iname();

void set_nbits (int n);
void set_nbmant(int n);
void set_nbexpo(int n);
void set_ndstac(int n);
void set_sdepth(int n);
void set_nuioin(int n);
void set_nuioou(int n);
void set_fftsiz(int n);
void set_nugain(int n);

void build_vv_file(); // cria arquivo .v com uma instancia do processador
void build_tb_file(); // cria arquivo de testbench