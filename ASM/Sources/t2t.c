// ----------------------------------------------------------------------------
// rotinas de conversao entre tipos de dados ----------------------------------
// ----------------------------------------------------------------------------

// includes globais
#include <string.h>
#include <stdlib.h>
#include  <stdio.h>
#include   <math.h>

// includes locais
#include "..\Headers\eval.h"

// converte o inteiro x para binario de comprimento w
// tentar mudar para conseguir converter int maior que 32 bits
// uma forma de mudar eh usar ascii pra x tambem
char *itob(int x, int w)
{
	int z;
    char *b = (char *) malloc(w+1);
    b[0] = '\0';

	int s = (w > 31) ? 31 : w;
	if (w > 31)
    {
        for (z = 0; z < w- 31; z++)
            if (x < 0) strcat(b,"1");
            else       strcat(b,"0");
    }

    for (z = pow(2,s-1); z > 0; z >>= 1)
		strcat(b, ((x & z) == z) ? "1" : "0");

    return b;
}

// converte float ieee 32 bits para meu float
// tentar mudar pra converter float de 64 bits
unsigned int f2mf(char *va)
{
    float f = atof(va);

    if (f == 0.0) return 1 << (nbmant + nbexpo -1);

    int *ifl = (int*)&f;

    // checa se o numero eh menor que o menor float permitido -----------------

    float r = (f < 0) ? -f : f;                        // valor absoluto do num, em float
    float q = pow(2,nbmant-1)*pow(2,-pow(2,nbexpo-1)); // menor valor permitido pra float = 2^(m-1)*2^(-(2^(e-1)))

    // se o numero for menor do que o menor permitido pra float, printa um erro
    if ((r < q) && (r != 0))
        {fprintf (stderr, "Erro: achei %f*10^-7, mas o menor número permitido é 2^(%d)!\n", f*10000000, (int)(nbmant-1 -pow(2,nbexpo-1))); exit(EXIT_FAILURE);}

    // desempacota padrao IEEE ------------------------------------------------

    int s =  (*ifl >> 31) & 0x00000001;
    int e = ((*ifl >> 23) & 0xFF) - 127 - 22;
    int m = ((*ifl & 0x007FFFFF) + 0x00800000) >> 1;

    // sinal ------------------------------------------------------------------

    s = s << (nbmant + nbexpo);

    // expoente ---------------------------------------------------------------

    e = e + (23-nbmant);

    int sh = 0;
    while (e < -pow(2, nbexpo-1))
    {
        e   = e+1;
        sh = sh+1;
    }
    e = e & ((int)(pow(2,nbexpo)-1));
    e = e << nbmant;

    // mantissa ---------------------------------------------------------------

    if (nbmant == 23)
    {
        if (*ifl & 0x00000001) m = m+1; // arredonda
    }
    else
    {
        sh = 23-nbmant+sh;
        int carry = (m >> (sh-1)) & 0x00000001; // carry de arredondamento
        m = m >> sh;
        if (carry) m = m+1; // arredonda
    }
    
    return s + e + m;
}