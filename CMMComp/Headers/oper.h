int negacao   (int et );                                                   // uso de NEG (se necessario)
int oper_cmp  (int et1, int et2, int  type);                               // reune expressoes de comparacao
int oper_int  (int et1, int et2, int    op);
int oper_shift(int et1, int et2, int  type);
int oper_bitw (int et1, int et2, int  type);
int oper_linv (int et);
int oper_inv  (int et);
int oper_mod  (int et1, int et2);
int oper_soma (int et1, int et2);
int oper_subt (int et1, int et2);
int oper_mult (int et1, int et2);
int oper_divi (int et1, int et2);
int oper_greq (int et1, int et2);
int oper_leeq (int et1, int et2);
int oper_dife (int et1, int et2);
int oper_lanor(int et1, int et2, int type);
