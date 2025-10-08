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
unsigned int f2mf(char *va, float *delta)
{
    float f = atof(va);

    if (f == 0.0) return 1 << (nbmant + nbexpo -1);

    int *ifl = (int*)&f;

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

    // calcula residuo --------------------------------------------------------
    
    float num = (atof(va)<0.0) ? -atof(va) : atof(va); // valor do numero em modulo
    *delta = m*pow(2,e)-num;

    // junta tudo -------------------------------------------------------------
    
    e = e & ((int)(pow(2,nbexpo)-1));
    e = e << nbmant;

    return s + e + m;
}

// converte meu float (em ascii) para float
float mf2f(char *ifl)
{
    // sinal ------------------------------------------------------------------

    int s = ifl[0] == '1';

    // expoente ---------------------------------------------------------------

    char exb[64]; for (int i=0;i<nbexpo;i++) exb[i] = ifl[i+1]; exb[nbexpo]=0;

    int es = exb[0] == '1';
    if (es) for (int i=0;i<nbexpo;i++) exb[i] = (exb[i] == '1') ? '0' : '1';

    char *endp;
    int e = strtol(exb,&endp,2);
    if (es) e = -(e+1);

    // mantissa ---------------------------------------------------------------

    char mab[64]; for (int i=0;i<nbmant;i++) mab[i] = ifl[nbexpo+1+i]; mab[nbmant]=0;

    int  m = strtol(mab,&endp,2);

    // gera o float -----------------------------------------------------------

    float  f = m * pow(2,e);
    if (s) f = -f;

    return f;
}