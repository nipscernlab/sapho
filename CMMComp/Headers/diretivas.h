// ----------------------------------------------------------------------------
// reune funcoes e variaveis acionadas por diretivas --------------------------
// ----------------------------------------------------------------------------

// somente essas 3 sao usadas pelo compilador cmm
extern char prname[128]; // nome do processador
extern int  nbmant;      // numero de bits de mantissa
extern int  nbexpo;      // numero de bits de expoente

// parse das diretivas
void dire_exec (char *dir, int id, int t);