#include "data_declar.h"
#include "variaveis.h"
#include "diretivas.h"
#include "data_assign.h"
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

    v_type[id] = type_tmp;        // o tipo da variavel esta em type_tmp (ver no flex quando acha int, float ou comp)
    v_used[id] = 0;               // acabou de ser declarada, entao ainda nao foi usada (lado direito do =)
    v_asgn[id] = 0;               // acabou de ser declarada, entao ainda nao tem valor (lado esquerd do =)
    v_fnid[id] = find_var(fname); // guarda em que funcao ela esta

    // testes com numeros complexos -------------------------------------------
    if (type_tmp > 2) declar_img(id);
    // fim do teste -----------------------------------------------------------
}

// declara parte imaginaria da veriavel complexa
void declar_img(int id)
{
    int idi = get_img_id(id);
    v_type[idi] = 4; // usar tipo 4 pra parte imaginaria
    v_used[idi] = 0;
    v_asgn[idi] = 0;
    v_fnid[idi] = find_var(fname);
}

// declara array 1D
void declar_arr_1d(int id_var, int id_arg, int id_fname)
{
    declar_var(id_var);

    if(id_fname == -1) // declara array sem arquivo de inicializacao
    {
        if (using_macro == 0) fprintf(f_asm, "#array %s %s\n", v_name[id_var], v_name[id_arg]);
    }
    else               // declara array com arquivo de inicializacao
    {
        if ((prtype == 0) && (v_type[id_var] != 1))
        {
            fprintf (stderr, "Erro na linha %d: inicialização de array, no processador em ponto fixo, só pra int por enquanto. Se vira!\n", line_num+1);
        }

        if (using_macro == 0) fprintf(f_asm, "#arrays %s %s %s\n", v_name[id_var], v_name[id_arg], v_name[id_fname]);
    }

    v_isar[id_var] = 1; // variavel é array 1D
    v_asgn[id_var] = 1; // array ja comeca como assigned (pois eh dificil de checar indice a indice)

    // teste com numeros complexos --------------------------------------------
    if (type_tmp == 3) declar_arr_1d_img(id_var,id_arg,id_fname);
    // fim do teste -----------------------------------------------------------
}

// declara parte imagin de um array complexo
void declar_arr_1d_img(int id_var, int id_arg, int id_fname)
{
    id_var = get_img_id(id_var);

    if(id_fname == -1) // declara array sem arquivo de inicializacao
    {   // ainda nao fiz inicializacao pra complexo
        if (using_macro == 0) fprintf(f_asm, "#array %s %s\n", v_name[id_var], v_name[id_arg]);
    }
    else               // declara array com arquivo de inicializacao
    {
        if ((prtype == 0) && (v_type[id_var] > 1))
        {   // preciso implementar isso!
            fprintf (stderr, "Erro na linha %d: inicialização de array, no processador em ponto fixo, só pra int por enquanto. Se vira!\n", line_num+1);
        }

        if (using_macro == 0) fprintf(f_asm, "#arrays %s %s %s\n", v_name[id_var], v_name[id_arg], v_name[id_fname]);
    }

    v_isar[id_var] = 1; // variavel é array 1D
    v_asgn[id_var] = 1; // array ja comeca como assigned (pois eh dificil de checar indice a indice)
}

// declara array 2D
void declar_arr_2d(int id_var, int id_x, int id_y, int id_fname)
{
    declar_var(id_var);

    // tamanho do array 1D equivalente
    int size = atoi(v_name[id_x])*atoi(v_name[id_y]);

    if (id_fname == -1) // declara array sem arquivo de inicializacao
    {
        if (using_macro == 0) fprintf(f_asm, "#array %s %d\n" , v_name[id_var], size);
    }
    else                // declara array com arquivo de inicializacao
    {
        if ((prtype == 0) && (v_type[id_var] != 1))
        {
            fprintf (stderr, "Erro na linha %d: inicialização de array, no processador em ponto fixo, só pra int por enquanto. Se vira!\n", line_num+1);
        }

        if (using_macro == 0) fprintf(f_asm, "#arrays %s %d %s\n", v_name[id_var], size, v_name[id_fname]);
    }

    // cria uma variavel auxiliar pra guardar o tamanho da dimensao x
    fprintf(f_asm, "LOAD %s\nSET %s_arr_size\n", v_name[id_x], v_name[id_var]);

    v_isar[id_var] = 2; // variavel é array 2D
    v_asgn[id_var] = 1; // array ja comeca como assigned (pois eh dificil de checar um a um)

    // teste com numeros complexos --------------------------------------------
    if (type_tmp == 3) declar_arr_2d_img(id_var,size,id_fname);
    // fim do teste -----------------------------------------------------------
}

// declara parte imag de um array 2d complexo
void declar_arr_2d_img(int id_var, int size, int id_fname)
{
    id_var = get_img_id(id_var);

    if(id_fname == -1) // declara array sem arquivo de inicializacao
    {   // ainda nao fiz inicializacao pra complexo
        if (using_macro == 0) fprintf(f_asm, "#array %s %d\n", v_name[id_var], size);
    }
    else               // declara array com arquivo de inicializacao
    {
        if ((prtype == 0) && (v_type[id_var] > 1))
        {   // preciso implementar isso!
            fprintf (stderr, "Erro na linha %d: inicialização de array, no processador em ponto fixo, só pra int por enquanto. Se vira!\n", line_num+1);
        }

        if (using_macro == 0) fprintf(f_asm, "#arrays %s %d %s\n", v_name[id_var], size, v_name[id_fname]);
    }

    v_isar[id_var] = 1; // variavel é array 1D
    v_asgn[id_var] = 1; // array ja comeca como assigned (pois eh dificil de checar indice a indice)
}
