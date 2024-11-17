
// Funcao sqrt para float -----------------------------------------------------

@float_sqrt SET sqrt_num     // pega parametro
@L1_sqrt    SET sqrt_x       // atualiza x

DIV sqrt_num                 // iteracao
ADD sqrt_x
MLT 0.5
SET sqrt_raiz

NEG                          // abs(x - raiz)
ADD sqrt_x
ABS                          // precisa mesmo desse ABS?

GRE 0.000008                 // menor numero possivel = 2^(m-1)*2^(-(2^(e-1)))
JZ L2else_sqrt               // para m = 16 e = 6, o num eh: 0.000007629...

LOAD sqrt_raiz               // finaliza
RETURN

@L2else_sqrt LOAD sqrt_raiz  // se nao terminou...
JMP L1_sqrt                  // volta
