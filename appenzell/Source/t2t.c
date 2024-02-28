#include "t2t.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "veri_comp.h"

char *itob(int x, int w)
{
	int z;
    char *b = (char *) malloc(w+1);
    b[0] = '\0';

	int s = (w > 31) ? 31 : w;
	if (w > 31)
	for (z = 0; z < w- 31; z++)
		if (x < 0) strcat(b, "1"); else strcat(b, "0");
    for (z = pow(2,s-1); z > 0; z >>= 1)
		strcat(b, ((x & z) == z) ? "1" : "0");

    return b;
}

int f2mf(char *va)
{
    float f = atof(va);

    if (f == 0.0) return 0;

    int *ifl = &f;

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
