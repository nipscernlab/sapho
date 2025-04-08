// ----------------------------------------------------------------------------
// rotinas para geracao dos arquivos .v do processador e test bench -----------
// ----------------------------------------------------------------------------

void build_vv_file(int n_ins, int n_dat, int nbopr, int itr_addr); // cria arquivo .v com uma instancia do processador
void build_tb_file();                                              // cria arquivo de testbench