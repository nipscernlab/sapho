int negacao   (int et );                                                   // uso de NEG (se necessario)
int operacoes (int et1, int et2, char *iop, char * fop, int *op, int nin); // funcao principal da ULA
int int_oper  (int  t1, int  t2, char * op, char *code, int fok);          // reune expressoes que geram int
int oper_cmp  (int et1, int et2, int    op);                               // reune expressoes de comparacao
int oper_ari  (int et1, int et2, int    op);                               // reune expressoes aritmeticas
int oper_int  (int et1, int et2, int    op);
int oper_shift(int et1, int et2, int  type);
int oper_bitw (int et1, int et2, int  type);
int oper_linv (int et);
int oper_inv  (int et);
int oper_mod  (int et1, int et2);
int oper_soma (int et1, int et2);
int oper_subt (int et1, int et2);
int oper_mult (int et1, int et2);

int oper_ari_cmp(int et1, int et2, int op);
