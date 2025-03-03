// ----------------------------------------------------------------------------
// rotinas e variaveis de estado para parser de funcoes -----------------------
// ----------------------------------------------------------------------------

// variaveis de estado --------------------------------------------------------

extern char fname[512];             // nome da funcao atual sendo parseada
extern int  ret_ok;                 // diz se teve um retorno da funcao corretamente
extern int  mainok;                 // status da funcao main: 0 -> nao usada, 1 -> declarada, 2 -> chamada no inicio
extern int fun_id1;                 // guarda id da funcao sendo parseada
extern int fun_id2;                 // guarda id da funcao sendo usada
extern int  p_test;                 // identifica parametros na chamada de funcoes (parecido com OFST, mas de valor 10)

// funcoes auxiliares ---------------------------------------------------------

int   get_npar  (int par);          // calcula  numero de parametros
void  par_check (int et );          // checa se numero de parametros na declaracao eh igual na chamada

// declaracao -----------------------------------------------------------------

void  declar_fun(int id1, int id2); // tipo e nome
void  declar_fst(int id);           // primeiro/unico parametro
int   declar_par(int   t, int id ); // proximo parametro
void     set_par(int id);           // SET no parametro
void  declar_ret(int  et, int ret); // achou uma palavra-chave return
void    func_ret(int id);           // fim/return da funcao
void    void_ret(      );           // fim de uma funcao void

// utilizacao -----------------------------------------------------------------

void  par_exp    (int et );         // carrega primeiro parametro (se houver)
void  par_listexp(int et );         // carrega os proximos parametros (se houver)
void  vcall      (int id );         // CALL de funcao com retorno
int   fcall      (int id );         // CALL de funcao void