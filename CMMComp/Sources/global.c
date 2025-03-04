#include "..\Headers\global.h"
#include "..\Headers\diretivas.h"
#include "..\Headers\t2t.h"

#include <stdarg.h>

// adiciona instrucao no arquivo asm
void add_instr(char *inst, ...)
{
    // adiciona instrucao -----------------------------------------------------

    va_list  args;
    va_start(args , inst);
    if (using_macro == 0) vfprintf(f_asm, inst, args);
    va_end  (args);

    // tabela para tradutor asselbly do gtkwave -------------------------------

    if (using_macro == 0) num_ins++;
    if (using_macro == 0) fprintf(f_lin, "%s\n", itob(line_num+1,20));
}

// adiciona instrucoes especiais
// type =  0 -> comentario
// type = -1 -> INTERNO
// type = -2 -> void main();
// type = -3 -> FIM
void add_sinst(int type, char *inst, ...)
{
    // adiciona instrucao -----------------------------------------------------

    va_list  args;
    va_start(args , inst);
    if (using_macro == 0) vfprintf(f_asm, inst, args);
    va_end  (args);

    // tabela para tradutor asselbly do gtkwave -------------------------------

    if (type != 0)
    {
        if (using_macro == 0) num_ins++;
        if (using_macro == 0) fprintf(f_lin, "%s\n", itob(type,20));
    }
}