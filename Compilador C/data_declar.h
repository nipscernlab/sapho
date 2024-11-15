#ifndef DATA_DECLAR_H_INCLUDED
#define DATA_DECLAR_H_INCLUDED

int  type_tmp; // para pegar o tipo quando uma variavel eh declarada (ver c2asm.l)

void declar_var   (int id);
void declar_img   (int id);
void declar_arr_1d(int id_var, int id_arg,           int id_fname);
void declar_arr_2d(int id_var, int id_x  , int id_y, int id_fname);

// testes com numeros complexos -----------------------------------------------
void declar_arr_1d_img(int id_var, int id_arg, int id_fname);
void declar_arr_2d_img(int id_var, int size  , int id_fname);
// fim do teste ---------------------------------------------------------------

#endif // DATA_DECLAR_H_INCLUDED
