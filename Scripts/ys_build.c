#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    FILE *rtlys = fopen("rtl.ys", "w");

    char proc[1024];
    strcpy(proc, argv[1]);

    fprintf(rtlys, "read_verilog %s.v\n", proc);
    fprintf(rtlys, "read_verilog addr_dec.v\n");
    fprintf(rtlys, "read_verilog core_fl.v\n");
    fprintf(rtlys, "read_verilog core_fx.v\n");
    fprintf(rtlys, "read_verilog float2index.v\n");
    fprintf(rtlys, "read_verilog float2int.v\n");
    fprintf(rtlys, "read_verilog instr_dec.v\n");
    fprintf(rtlys, "read_verilog int2float.v\n");
    fprintf(rtlys, "read_verilog mem_data.v\n");
    fprintf(rtlys, "read_verilog mem_instr.v\n");
    fprintf(rtlys, "read_verilog pc.v\n");
    fprintf(rtlys, "read_verilog prefetch.v\n");
    fprintf(rtlys, "read_verilog proc_fl.v\n");
    fprintf(rtlys, "read_verilog proc_fx.v\n");
    fprintf(rtlys, "read_verilog rel_addr.v\n");
    fprintf(rtlys, "read_verilog stack_pointer.v\n");
    fprintf(rtlys, "read_verilog stack.v\n");
    fprintf(rtlys, "read_verilog ula_fl.v\n");
    fprintf(rtlys, "read_verilog ula_fx.v\n");
    fprintf(rtlys, "read_verilog ula.v\n");

    char modulo[1024];
    strcpy(modulo, argv[2]);

    for (int i = 3; i < argc; i++) {

        FILE *par = fopen("proc_par.txt", "r");

        char rparam[1024], wparam[1024];
        sscanf(argv[i], "%[^/]/%s", rparam, wparam);

        char linha[1024];
        int val;
        while (fgets(linha, sizeof(linha), par) != NULL) 
        {
            if (strstr(linha, rparam))
            {
                sscanf(linha, "%s %d", rparam, &val);
                fprintf(rtlys, "chparam -set %s %d %s\n", wparam, val, modulo);
            }
        }

        fclose(par);
    }
    
    fprintf(rtlys, "prep -top %s\n", modulo);
    fprintf(rtlys, "write_json %s.json\n", modulo);
    fprintf(rtlys, "select -list > %s.txt\n", modulo);

    fclose(rtlys);
    return 0;
}