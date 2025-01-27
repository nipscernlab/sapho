int  ndstac, nbmant, nbexpo, nbits, fftsiz;
int  float_point;    // diz se o processador eh com ponto flutuante

char hard_dir[1024]; // diretorio da pasta Hardware
char temp_dir[1024]; // diretorio da pasta Tmp
char  hdl_dir[1024]; // diretorio da pasta HDL

int clk_frq;         // frequencia do cloak (em MHz) para a simulacao
int clk_num;         // num de clocks maximo a simular

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
void set_float (int n);

void build_vv_file(); // cria arquivo .v com uma instancia do processador
void build_tb_file(); // cria arquivo de testbench
void build_pc_file(); // cria arquivo pc_sim.v para (program couter para simulacao)
void build_dt_file(); // cria arquivo data_mem_sim.v