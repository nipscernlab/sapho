
// Funcao arco-tangente para proc ponto fixo ----------------------------------

@float_atani SET my_atan_x        // pega parametro x

EQU  float_zero // 0.0            // if (x == 0) return 0.0;
JZ L1else_atani
LOAD float_zero // 0.0
RETURN

@L1else_atani LOAD my_atan_x     // if (abs(x) == 1.0) return sign(x,pi2*0.5);
AND  tudo_um
EQU  num_um // 1.0
JZ   L2else_atani
LOAD pi_div_2
PLD  um_div_2 // 0.5
CALL float_mult
AND  tudo_um
PLD  tudo_um
ADD  1
AND  my_atan_x
SADD
RETURN
JMP L2end_atani

@L2else_atani LOAD my_atan_x     // else if (abs(x) > 1.0) return sign(x,pi2) - atan(1.0/x);
AND  tudo_um
PLD  num_um // 1.0
CALL denorm
LOAD float_aux3
GRE  float_aux1
JZ   L3else_atani
LOAD tudo_um
ADD  1
AND  my_atan_x
PLD  pi_div_2
AND  tudo_um
SADD
PLD  num_um // 1.0
PLD  my_atan_x
CALL float_div
CALL float_atani
PLD  float_nbits
SHL  1
SADD
CALL denorm
CALL float_add
RETURN

@L3else_atani @L2end_atani LOAD my_atan_x  // float termo = x;
SET my_atan_termo

LOAD my_atan_x                             // float x2 = x*x;
PLD  my_atan_x
CALL float_mult
SET  my_atan_x2

LOAD my_atan_termo                         // float resultado = termo;
SET  my_atan_resultado

LOAD epsilon_taylor                        // float tolerancia = epslon/x2;
PLD  my_atan_x2
CALL float_div
SET  my_atan_tolerancia

LOAD 3                                     // int indiceX = 3;
SET  my_atan_indiceX

@L4_atani LOAD my_atan_termo               // while (abs(termo) > tolerancia) {
AND tudo_um
PLD my_atan_tolerancia
CALL denorm
LOAD float_aux3
GRE float_aux1
JZ L4end_atani

LOAD -2                                    // termo = termo * (- x2 * (indiceX - 2)) / indiceX;
ADD  my_atan_indiceX
CALL int2float
PLD  my_atan_x2
CALL float_mult
PLD  float_nbits
SHL  1
SADD
PLD  my_atan_termo
CALL float_mult
PLD  my_atan_indiceX
CALL int2float
CALL float_div
SET  my_atan_termo

LOAD my_atan_resultado                     // resultado = resultado + termo;
PLD  my_atan_termo
CALL denorm
CALL float_add
SET  my_atan_resultado

LOAD 2                                     // indiceX = indiceX + 2;
ADD  my_atan_indiceX
SET  my_atan_indiceX

JMP L4_atani                               // }

@L4end_atani LOAD my_atan_resultado        // return resultado;
RETURN