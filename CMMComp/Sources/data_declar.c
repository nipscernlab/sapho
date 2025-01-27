#include "..\Headers\data_declar.h"
#include "..\Headers\variaveis.h"
#include "..\Headers\diretivas.h"
#include "..\Headers\data_assign.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
    if (strcmp(fname,"")==0)
        strcpy(func, "global");
    else
        strcpy(func, fname);

    fprintf(f_log, "%s %s %d\n", func, rem_fname(v_name[id], fname), type_tmp);
    if (type_tmp == 3)
    fprintf(f_log, "%s %s_i %d\n", func, rem_fname(v_name[id], fname), type_tmp);

    // testes com numeros complexos -------------------------------------------
    if (type_tmp > 2) declar_img(id);
    // fim do teste -----------------------------------------------------------
}

// declara parte imaginaria da veriavel complexa
void declar_img(int id)
{
    int idi     = get_img_id(id);
    v_type[idi] = 4; // usar tipo 4 pra parte imaginaria
    v_used[idi] = 0;
    v_asgn[idi] = 0;
    v_fnid[idi] = find_var(fname);
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

    int type = type_tmp;

    // proc ponto fixo, tipo int, sem arquivo
    if ((prtype == 0) && (type == 1) && (id_fname == -1))
    {
        if (using_macro == 0) fprintf(f_asm, "#array %s %s\n", v_name[id_var], v_name[id_arg]);
    }

    // proc ponto fixo, tipo int, com arquivo
    if ((prtype == 0) && (type == 1) && (id_fname != -1))
    {
        if (using_macro == 0) fprintf(f_asm, "#arrays %s %d %s %s\n", v_name[id_var], type, v_name[id_arg], v_name[id_fname]);
    }

    // proc ponto fixo, tipo float, sem arquivo
    if ((prtype == 0) && (type == 2) && (id_fname == -1))
    {
        if (using_macro == 0) fprintf(f_asm, "#array %s %s\n", v_name[id_var], v_name[id_arg]);
    }

    // proc ponto fixo, tipo float, com arquivo
    if ((prtype == 0) && (type == 2) && (id_fname != -1))
    {
        if (using_macro == 0) fprintf(f_asm, "#arrays %s %d %s %s\n", v_name[id_var], type, v_name[id_arg], v_name[id_fname]);
    }

    // proc ponto fixo, tipo comp, sem arquivo
    if ((prtype == 0) && (type == 3) && (id_fname == -1))
    {
        if (using_macro == 0) fprintf(f_asm, "#array %s %s\n", v_name[id_var], v_name[id_arg]);
        id_var = get_img_id(id_var);
        if (using_macro == 0) fprintf(f_asm, "#array %s %s\n", v_name[id_var], v_name[id_arg]);

        v_isar[id_var] = 1; // variavel eh array 1D
        v_asgn[id_var] = 1; // array ja comeca como assigned (pois eh dificil de checar indice a indice)
    }

    // proc ponto fixo, tipo comp, com arquivo
    if ((prtype == 0) && (type == 3) && (id_fname != -1))
    {
        if (using_macro == 0) fprintf(f_asm, "#arrays %s 3 %s %s\n", v_name[id_var], v_name[id_arg], v_name[id_fname]);
        id_var = get_img_id(id_var);
        if (using_macro == 0) fprintf(f_asm, "#arrays %s 4 %s %s\n", v_name[id_var], v_name[id_arg], v_name[id_fname]);

        v_isar[id_var] = 1; // variavel eh array 1D
        v_asgn[id_var] = 1; // array ja comeca como assigned (pois eh dificil de checar indice a indice)
    }

    // proc ponto flut, tipo int, sem arquivo
    if ((prtype == 1) && (type == 1) && (id_fname == -1))
    {
        if (using_macro == 0) fprintf(f_asm, "#array %s %s\n", v_name[id_var], v_name[id_arg]);
    }

    // proc ponto flut, tipo int, com arquivo
    if ((prtype == 1) && (type == 1) && (id_fname != -1))
    {
        if (using_macro == 0) fprintf(f_asm, "#arrays %s %d %s %s\n", v_name[id_var], type, v_name[id_arg], v_name[id_fname]);
    }

    // proc ponto flut, tipo float, sem arquivo
    if ((prtype == 1) && (type == 2) && (id_fname == -1))
    {
        if (using_macro == 0) fprintf(f_asm, "#array %s %s\n", v_name[id_var], v_name[id_arg]);
    }

    // proc ponto flut, tipo float, com arquivo
    if ((prtype == 1) && (type == 2) && (id_fname != -1))
    {
        if (using_macro == 0) fprintf(f_asm, "#arrays %s %d %s %s\n", v_name[id_var], type, v_name[id_arg], v_name[id_fname]);
    }

    // proc ponto flut, tipo comp, sem arquivo
    if ((prtype == 1) && (type == 3) && (id_fname == -1))
    {
        if (using_macro == 0) fprintf(f_asm, "#array %s %s\n", v_name[id_var], v_name[id_arg]);
        id_var = get_img_id(id_var);
        if (using_macro == 0) fprintf(f_asm, "#array %s %s\n", v_name[id_var], v_name[id_arg]);

        v_isar[id_var] = 1; // variavel eh array 1D
        v_asgn[id_var] = 1; // array ja comeca como assigned (pois eh dificil de checar indice a indice)
    }

    // proc ponto flut, tipo comp, com arquivo
    if ((prtype == 1) && (type == 3) && (id_fname != -1))
    {
        if (using_macro == 0) fprintf(f_asm, "#arrays %s 3 %s %s\n", v_name[id_var], v_name[id_arg], v_name[id_fname]);
        id_var = get_img_id(id_var);
        if (using_macro == 0) fprintf(f_asm, "#arrays %s 4 %s %s\n", v_name[id_var], v_name[id_arg], v_name[id_fname]);

        v_isar[id_var] = 1; // variavel eh array 1D
        v_asgn[id_var] = 1; // array ja comeca como assigned (pois eh dificil de checar indice a indice)
    }
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

    int type = type_tmp;

    // proc ponto fixo, tipo int, sem arquivo
    if ((prtype == 0) && (type == 1) && (id_fname == -1))
    {
        if (using_macro == 0) fprintf(f_asm, "#array %s %d\n", v_name[id_var], size);
    }

    // proc ponto fixo, tipo int, com arquivo
    if ((prtype == 0) && (type == 1) && (id_fname != -1))
    {
        if (using_macro == 0) fprintf(f_asm, "#arrays %s %d %d %s\n", v_name[id_var], type, size, v_name[id_fname]);
    }

    // proc ponto fixo, tipo float, sem arquivo
    if ((prtype == 0) && (type == 2) && (id_fname == -1))
    {
        if (using_macro == 0) fprintf(f_asm, "#array %s %d\n", v_name[id_var], size);
    }

    // proc ponto fixo, tipo float, com arquivo
    if ((prtype == 0) && (type == 2) && (id_fname != -1))
    {
        if (using_macro == 0) fprintf(f_asm, "#arrays %s %d %d %s\n", v_name[id_var], type, size, v_name[id_fname]);
    }

    // proc ponto fixo, tipo comp, sem arquivo
    if ((prtype == 0) && (type == 3) && (id_fname == -1))
    {
        if (using_macro == 0) fprintf(f_asm, "#array %s %d\n", v_name[id_var], size);
        idi = get_img_id(id_var);
        if (using_macro == 0) fprintf(f_asm, "#array %s %d\n", v_name[idi], size);

        v_isar[idi] = 2; // variavel eh array 2D
        v_asgn[idi] = 1; // array ja comeca como assigned (pois eh dificil de checar indice a indice)
    }

    // proc ponto fixo, tipo comp, com arquivo
    if ((prtype == 0) && (type == 3) && (id_fname != -1))
    {
        if (using_macro == 0) fprintf(f_asm, "#arrays %s 3 %d %s\n", v_name[id_var], size, v_name[id_fname]);
        idi = get_img_id(id_var);
        if (using_macro == 0) fprintf(f_asm, "#arrays %s 4 %d %s\n", v_name[idi], size, v_name[id_fname]);

        v_isar[idi] = 2; // variavel eh array 2D
        v_asgn[idi] = 1; // array ja comeca como assigned (pois eh dificil de checar indice a indice)
    }

    // proc ponto flut, tipo int, sem arquivo
    if ((prtype == 1) && (type == 1) && (id_fname == -1))
    {
        if (using_macro == 0) fprintf(f_asm, "#array %s %d\n", v_name[id_var], size);
    }

    // proc ponto flut, tipo int, com arquivo
    if ((prtype == 1) && (type == 1) && (id_fname != -1))
    {
        if (using_macro == 0) fprintf(f_asm, "#arrays %s %d %d %s\n", v_name[id_var], type, size, v_name[id_fname]);
    }

    // proc ponto flut, tipo float, sem arquivo
    if ((prtype == 1) && (type == 2) && (id_fname == -1))
    {
        if (using_macro == 0) fprintf(f_asm, "#array %s %d\n", v_name[id_var], size);
    }

    // proc ponto flut, tipo float, com arquivo
    if ((prtype == 1) && (type == 2) && (id_fname != -1))
    {
        if (using_macro == 0) fprintf(f_asm, "#arrays %s %d %d %s\n", v_name[id_var], type, size, v_name[id_fname]);
    }

    // proc ponto flut, tipo comp, sem arquivo
    if ((prtype == 1) && (type == 3) && (id_fname == -1))
    {
        if (using_macro == 0) fprintf(f_asm, "#array %s %d\n", v_name[id_var], size);
        idi = get_img_id(id_var);
        if (using_macro == 0) fprintf(f_asm, "#array %s %d\n", v_name[idi], size);

        v_isar[idi] = 2; // variavel eh array 2D
        v_asgn[idi] = 1; // array ja comeca como assigned (pois eh dificil de checar indice a indice)
    }

    // proc ponto flut, tipo comp, com arquivo
    if ((prtype == 1) && (type == 3) && (id_fname != -1))
    {
        if (using_macro == 0) fprintf(f_asm, "#arrays %s 3 %d %s\n", v_name[id_var], size, v_name[id_fname]);
        idi = get_img_id(id_var);
        if (using_macro == 0) fprintf(f_asm, "#arrays %s 4 %d %s\n", v_name[idi], size, v_name[id_fname]);

        v_isar[idi] = 2; // variavel eh array 2D
        v_asgn[idi] = 1; // array ja comeca como assigned (pois eh dificil de checar indice a indice)
    }

    // cria uma variavel auxiliar pra guardar o tamanho da dimensao x
    if (is_macro() == 0) fprintf(f_asm, "LOAD %s\n", v_name[id_x]);
    if (is_macro() == 0) fprintf(f_asm, "SET %s_arr_size\n", v_name[id_var]);
}
