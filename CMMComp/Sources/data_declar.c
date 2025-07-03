// ----------------------------------------------------------------------------
// implementa declaracao de variaveis -----------------------------------------
// ----------------------------------------------------------------------------

// includes globais
#include <string.h>
#include <stdlib.h>
#include  <stdio.h>

// includes locais
#include "..\Headers\t2t.h"
#include "..\Headers\global.h"
#include "..\Headers\stdlib.h"
#include "..\Headers\funcoes.h"
#include "..\Headers\variaveis.h"

// ----------------------------------------------------------------------------
// redeclaracao de variaveis globais ------------------------------------------
// ----------------------------------------------------------------------------

int type_tmp; // para pegar o tipo quando uma variavel eh declarada (ver c2asm.l)

// ----------------------------------------------------------------------------
// declaracoes ----------------------------------------------------------------
// ----------------------------------------------------------------------------

// declara variavel (sem ser array)
void declar_var(int id)
{
    if (v_type[id] != 0) // variavel ja existe
    {
        fprintf (stderr, "Erro na linha %d: puts, a variável %s já existe, tá doido?\n", line_num+1, rem_fname(v_name[id], fname));
        return;
    }

    v_type[id] = type_tmp;               // o tipo da variavel esta em type_tmp (ver no flex quando acha int, float ou comp)
    v_asgn[id] = strcmp(fname,"") == 0;  // variavel global deve ser marcada como assigned
    v_used[id] = 0;                      // acabou de ser declarada, entao ainda nao foi usada
    v_fnid[id] = find_var(fname);        // guarda em que funcao ela esta

    char func[256];
    if (strcmp(fname,"")==0) strcpy(func, "global"); else strcpy(func, fname);

    fprintf(f_log, "%s %s %d\n"  , func, rem_fname(v_name[id], fname), type_tmp);
    if (type_tmp == 3)
    {
    fprintf(f_log, "%s %s_i %d\n", func, rem_fname(v_name[id], fname), type_tmp);
    }

    // declara parte imaginaria da veriavel complexa
    if (type_tmp > 2)
    {
        int idi     = get_img_id(id);
        v_type[idi] = 4; // usar tipo 4 pra parte imaginaria
        v_used[idi] = 0;
        v_asgn[idi] = 0;
        v_fnid[idi] = find_var(fname);
    }
}

// declara array 1D
void declar_arr_1d(int id_var, int id_arg, int id_fname)
{
    if (v_type[id_var] != 0) // variavel ja existe
    {
        fprintf (stderr, "Erro na linha %d: puts, a variável %s já existe, tá doido?\n", line_num+1, rem_fname(v_name[id_var], fname));
        return;
    }

    v_type[id_var] = type_tmp;               // o tipo da variavel esta em type_tmp (ver no flex quando acha int, float ou comp)
    v_used[id_var] = 0;                      // acabou de ser declarada, entao ainda nao foi usada
    v_fnid[id_var] = find_var(fname);        // guarda em que funcao ela esta
    v_isar[id_var] = 1;                      // variavel eh array 1D
    v_asgn[id_var] = 1;                      // array ja comeca como assigned (pois eh dificil de checar indice a indice)
    v_size[id_var] = atoi(v_name[id_arg]);   // guarda o tamanho do array

    int type = type_tmp;

    // tipo int, sem arquivo
    if ((type == 1) && (id_fname == -1))
    {
        add_sinst(0, "#array %s %s\n", v_name[id_var], v_name[id_arg]);
    }

    // tipo int, com arquivo
    if ((type == 1) && (id_fname != -1))
    {
        add_sinst(0, "#arrays %s %d %s %s\n", v_name[id_var], type, v_name[id_arg], v_name[id_fname]);
    }

    // tipo float, sem arquivo
    if ((type == 2) && (id_fname == -1))
    {
        add_sinst(0, "#array %s %s\n", v_name[id_var], v_name[id_arg]);
    }

    // tipo float, com arquivo
    if ((type == 2) && (id_fname != -1))
    {
        add_sinst(0, "#arrays %s %d %s %s\n", v_name[id_var], type, v_name[id_arg], v_name[id_fname]);
    }

    // tipo comp, sem arquivo
    if ((type == 3) && (id_fname == -1))
    {
        add_sinst(0, "#array %s %s\n", v_name[id_var], v_name[id_arg]);
        id_var = get_img_id(id_var);
        add_sinst(0, "#array %s %s\n", v_name[id_var], v_name[id_arg]);

        v_isar[id_var] = 1; // variavel eh array 1D
        v_asgn[id_var] = 1; // array ja comeca como assigned (pois eh dificil de checar indice a indice)
    }

    // tipo comp, com arquivo
    if ((type == 3) && (id_fname != -1))
    {
        add_sinst(0, "#arrays %s 3 %s %s\n", v_name[id_var], v_name[id_arg], v_name[id_fname]);
        id_var = get_img_id(id_var);
        add_sinst(0, "#arrays %s 4 %s %s\n", v_name[id_var], v_name[id_arg], v_name[id_fname]);

        v_isar[id_var] = 1; // variavel eh array 1D
        v_asgn[id_var] = 1; // array ja comeca como assigned (pois eh dificil de checar indice a indice)
    }
}

// declara array 1d como um produto entre matriz e vetor
void declar_Mv(int id_name, int id_N, int id_M, int id_v)
{
    declar_arr_1d(id_name,id_N,  -1);
    exec_Mv      (id_name,id_M,id_v);
}

// declara array 1d como um produto entre constante e vetor
void declar_cv(int id_name, int id_N, int id_c, int id_v)
{
    declar_arr_1d(id_name,id_N,  -1);
    exec_cv      (id_name,id_c,id_v);
}

// declara array 2D
void declar_arr_2d(int id_var, int id_x, int id_y, int id_fname)
{
    int idi;

    // tamanho do array 1D equivalente
    int size = atoi(v_name[id_x])*atoi(v_name[id_y]);

    if (v_type[id_var] != 0) // variavel ja existe
    {
        fprintf (stderr, "Erro na linha %d: puts, a variável %s já existe, tá doido?\n", line_num+1, rem_fname(v_name[id_var], fname));
        return;
    }

    v_type[id_var] = type_tmp;               // o tipo da variavel esta em type_tmp (ver no flex quando acha int, float ou comp)
    v_used[id_var] = 0;                      // acabou de ser declarada, entao ainda nao foi usada
    v_fnid[id_var] = find_var(fname);        // guarda em que funcao ela esta
    v_isar[id_var] = 2;                      // variavel eh array 2D
    v_asgn[id_var] = 1;                      // array ja comeca como assigned (pois eh dificil de checar indice a indice)
    v_size[id_var] = atoi(v_name[id_x]);     // guarda o tamanho da dimensao i
    v_siz2[id_var] = atoi(v_name[id_y]);     // guarda o tamanho da dimensao j

    int type = type_tmp;

    // tipo int, sem arquivo
    if ((type == 1) && (id_fname == -1))
    {
        add_sinst(0, "#array %s %d\n", v_name[id_var], size);
    }

    // tipo int, com arquivo
    if ((type == 1) && (id_fname != -1))
    {
        add_sinst(0, "#arrays %s %d %d %s\n", v_name[id_var], type, size, v_name[id_fname]);
    }

    // tipo float, sem arquivo
    if ((type == 2) && (id_fname == -1))
    {
        add_sinst(0, "#array %s %d\n", v_name[id_var], size);
    }

    // tipo float, com arquivo
    if ((type == 2) && (id_fname != -1))
    {
        add_sinst(0, "#arrays %s %d %d %s\n", v_name[id_var], type, size, v_name[id_fname]);
    }

    // tipo comp, sem arquivo
    if ((type == 3) && (id_fname == -1))
    {
        add_sinst(0, "#array %s %d\n", v_name[id_var], size);
        idi = get_img_id(id_var);
        add_sinst(0, "#array %s %d\n", v_name[idi], size);

        v_isar[idi] = 2; // variavel eh array 2D
        v_asgn[idi] = 1; // array ja comeca como assigned (pois eh dificil de checar indice a indice)
    }

    // tipo comp, com arquivo
    if ((type == 3) && (id_fname != -1))
    {
        add_sinst(0, "#arrays %s 3 %d %s\n", v_name[id_var], size, v_name[id_fname]);
        idi = get_img_id(id_var);
        add_sinst(0, "#arrays %s 4 %d %s\n", v_name[idi], size, v_name[id_fname]);

        v_isar[idi] = 2; // variavel eh array 2D
        v_asgn[idi] = 1; // array ja comeca como assigned (pois eh dificil de checar indice a indice)
    }

    // cria uma variavel auxiliar pra guardar o tamanho da dimensao x
    add_instr("LOD %s\n",           v_name[id_x  ]);
    add_instr("SET  %s_arr_size\n", v_name[id_var]);
}
