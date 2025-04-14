// ----------------------------------------------------------------------------
// conversao e tratamento de dados --------------------------------------------
// ----------------------------------------------------------------------------

// conversao de dados ---------------------------------------------------------

char* itob(int x, int w);                           // converte inteiro para string binaria

// funcoes auxiliares de acesso a tipos de terminais --------------------------

int  get_type   (int et);                           // pega o tipo de dado
int  get_img_id (int id);                           // pega o id da parte imaginaria
void get_cmp_cst(int et, int *et_r, int *et_i);     // gera ets para uma constante complexa
void get_cmp_ets(int et, int *et_r, int *et_i);     // pega ets de   uma variavel  complexa