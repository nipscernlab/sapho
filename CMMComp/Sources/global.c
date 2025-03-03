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
    vfprintf(f_asm, inst, args);
    va_end  (args);

    // tabela para tradutor asselbly do gtkwave -------------------------------

    num_ins++;
    if (using_macro == 0) fprintf(f_lin, "%s\n", itob(line_num+1,20));
}