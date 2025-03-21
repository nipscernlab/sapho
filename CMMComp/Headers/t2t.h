// ----------------------------------------------------------------------------
// conversao e tratamento de dados --------------------------------------------
// ----------------------------------------------------------------------------

// conversao de dados ---------------------------------------------------------

unsigned int f2mf(char     *va);                    // converte float   para meu float
char        *itob(int x, int w);                    // converte inteiro para string binaria

// funcoes auxiliares de acesso a terminais -----------------------------------

int  get_type   (int et);                           // pega o tipo de dado

int  get_img_id (int id);                           // pega o id da parte imaginaria
void get_cmp_cst(int et, int *et_r, int *et_i);     // gera ets para uma constante complexa
void get_cmp_ets(int et, int *et_r, int *et_i);     // pega ets de uma variavel complexa

// uso de macros externas -----------------------------------------------------

extern int fsqrti;                                  // vai usar macro de raiz quadrada
extern int fatani;                                  // vai usar macro de arco tangente

void mac_geni(char *fasm);                          // adiciona todo o codigo necessario no .asm (pra ponto fixo)