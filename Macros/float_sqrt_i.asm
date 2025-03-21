
// Funcao sqrt ----------------------------------------------------------------

@float_sqrti SET     sqrt_num                  // pega parametro
@L1_sqrti    SET     sqrt_x                    // atualiza x

             FDIV    sqrt_num                  // iteracao
             FADD    sqrt_x
             FMLT    um_div_2 // 0.5
             SET     sqrt_raiz

             FNEG                              // negacao da raiz
             FADD    sqrt_x                    // x - raiz

             FGRE    epsilon_taylor            // checa tolerancia
             JZ      L2else_sqrti

             LOD     sqrt_raiz                 // se eh, retorna o resultado
             RETURN

@L2else_sqrti LOD    sqrt_raiz                 // se nao eh, volta
              JMP    L1_sqrti
