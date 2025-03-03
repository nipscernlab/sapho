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

extern int i2f ;                                    // vai usar funcao int2float
extern int f2i ;                                    // vai usar funcao float2int
extern int fadd;                                    // vai usar funcao float_add
extern int fmlt;                                    // vai usar funcao float_mlt
extern int fdiv;                                    // vai usar funcao float_div

extern int fsqrti;                                  // vai usar macro de raiz quadrada em ponto fixo
extern int fatani;                                  // vai usar macro de arco tangente em ponto fixo
extern int fsqrt ;                                  // vai usar macro de raiz quadrada em ponto flut
extern int fatan ;                                  // vai usar macro de arco tangente em ponto flut

void mac_geni(char *fasm);                          // adiciona todo o codigo necessario no .asm (pra ponto fixo)
void mac_genf(char *fasm);                          // adiciona todo o codigo necessario no .asm (pra ponto flut)