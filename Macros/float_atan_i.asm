
// Funcao arco-tangente -------------------------------------------------------

@float_atani SET my_atan_x        // pega parametro x

EQU  float_zero // 0.0            // if (x == 0) return 0.0;
JZ   L1else_atani
LOD  float_zero // 0.0
RETURN

@L1else_atani FABSM my_atan_x     // if (abs(x) == 1.0) return sign(x,pi2*0.5);
EQU  num_um // 1.0
JZ   L2else_atani
LOD  pi_div_2
FMLT um_div_2 // 0.5
FSGN my_atan_x
RETURN
JMP  L2end_atani

@L2else_atani FABSM my_atan_x     // else if (abs(x) > 1.0) return sign(x,pi2) - atan(1.0/x);
FLES num_um // 1.0
JZ   L3else_atani
LOD  pi_div_2
FSGN my_atan_x
PLD  my_atan_x
FDIV num_um // 1.0
CALL float_atani
FNEG
SFADD
RETURN

@L3else_atani @L2end_atani LOD my_atan_x  // float termo = x;
SET my_atan_termo

LOD  my_atan_x                             // float x2 = x*x;
FMLT my_atan_x
SET  my_atan_x2

LOD  my_atan_termo                         // float resultado = termo;
SET  my_atan_resultado

LOD  my_atan_x2                            // float tolerancia = epslon/x2;
FDIV epsilon_taylor
SET  my_atan_tolerancia

LOD 3                                     // int indiceX = 3;
SET  my_atan_indiceX

@L4_atani FABSM my_atan_termo              // while (abs(termo) > tolerancia)
FLES my_atan_tolerancia
JZ L4end_atani

LOD  -2                                    // termo = termo * (- x2 * (indiceX - 2)) / indiceX;
ADD   my_atan_indiceX
IFA
FMLT  my_atan_x2
FNEG
FMLT  my_atan_termo
PIFM  my_atan_indiceX
SFDIV
SET   my_atan_termo

LOD   my_atan_resultado                    // resultado = resultado + termo;
FADD  my_atan_termo
SET   my_atan_resultado

LOD   2                                    // indiceX = indiceX + 2;
ADD   my_atan_indiceX
SET   my_atan_indiceX

JMP L4_atani                               // }

@L4end_atani LOD my_atan_resultado        // return resultado;
RETURN