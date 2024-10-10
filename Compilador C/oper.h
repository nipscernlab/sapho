#ifndef OPER_H_INCLUDED
#define OPER_H_INCLUDED

int negacao  (int et );                                          // uso de NEG (se necessario)
int operacoes(int et1, int et2, char *iop, char * fop, int *op); // funcao principal da ULA
int int_oper (int  t1, int  t2, char * op, char *code, int fok); // reune expressoes que geram int
int oper_cmp (int et1, int et2, int    op);                      // reune expressoes de comparacao
int oper_ari (int et1, int et2, int    op);                      // reune expressoes aritmeticas

#endif // OPER_H_INCLUDED
