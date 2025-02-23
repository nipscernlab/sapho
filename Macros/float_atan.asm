
// Funcao arco-tangente para float --------------------------------------------

@float_atan SET my_atan_x                  // pega parametro x

LOAD 0.0                                   // if (x == 0)
EQU  my_atan_x
JZ   L1else

LOAD 0.0                                   // return 0.0;
RETURN

JMP L1end                                  // else if (abs(x) == 1.0)
@L1else LOAD my_atan_x
ABS
PLD 1.0
SEQU
JZ L2else

LOAD 0.5                                   // return sign(x,pi2*0.5);
MLT  1.57079632679
SIGN my_atan_x
RETURN
JMP  L2end

@L2else LOAD my_atan_x                     // else if (abs(x) > 1.0)
ABS
PLD 1.0
SGRE
JZ L3else

LOAD 1.57079632679                         // return sign(x,pi2) - atan(1.0/x);
SIGN my_atan_x
PLD  my_atan_x
DIV  1.0
CALL float_atan
NEG
SADD
RETURN

@L3else @L2end @L1end LOAD my_atan_x       // float termo = x;
SET my_atan_termo

LOAD my_atan_x                             // float x2 = x*x;
MLT  my_atan_x
SET  my_atan_x2

LOAD my_atan_termo                         // float resultado  = termo;
SET  my_atan_resultado

LOAD my_atan_x2                            // float tolerancia = 0.000008/x2;
DIV  0.000008
SET  my_atan_tolerancia

LOAD 3                                     // int indiceX = 3;
SET my_atan_indiceX

@L4 LOAD my_atan_termo                     // while (abs(termo) > tolerancia) {
ABS
PLD my_atan_tolerancia
SGRE
JZ L4end

LOAD -2                                    // termo = termo * (- x2 * (indiceX - 2)) / indiceX;
ADD  my_atan_indiceX
MLT  my_atan_x2
NEG
MLT  my_atan_termo
PLD  my_atan_indiceX
SDIV
SET  my_atan_termo

LOAD my_atan_termo                         // resultado = resultado + termo;
ADD  my_atan_resultado
SET  my_atan_resultado

LOAD 2                                     // indiceX = indiceX + 2;
ADD  my_atan_indiceX
SET  my_atan_indiceX

JMP L4                                     // }

@L4end LOAD my_atan_resultado              // return resultado;
RETURN