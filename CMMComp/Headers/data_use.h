// ----------------------------------------------------------------------------
// rotinas para reducao exp ---------------------------------------------------
// ----------------------------------------------------------------------------

// retorno pra exp (ver regra pra exp no .y)
//   OFST    -> inteiro reduzido
// 2*OFST    -> float   reduzido
// 3*OFST    -> comp    reduzido
//   OFTS+id -> identificador int
// 2*OFST+id -> identificador float
// 3*OFST+id -> identificador comp
// 5*OFST+id -> identificador const comp
#define OFST 1000000

int      num2exp(int id, int dtype);         // reduz de um numero        para exp
int       id2exp(int id);                    // reduz de um identificador para exp
int    pplus2exp(int et);                    // reduz de um i++           para exp
int  pplus1d2exp(int id, int et);            // reduz de um x[i]++        para exp
int  pplus2d2exp(int id, int et1, int et2);  // reduz de um x[i][j]++     para exp