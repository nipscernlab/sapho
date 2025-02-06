#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

float i2mf(int ifl, int nbm, int nbe)
{
    // sinal
    int s = ifl >> (nbm+nbe);

    // expoente
    int e = ifl << (32-(nbm+nbe)    );
        e = e   >> (32-(nbm+nbe)+nbm);

    // mantissa
    int m = ((1 << nbm) - 1) & ifl;

    // gera o float
    float f = m * pow(2,e);
    if (s) f = -f;

    return f;
}

int main(int argc, char **argv)
{
    char   fname[64], man[16], exp[16];
    FILE  *flog = fopen("cmm_log.txt", "r");
    fscanf(flog,"%s %s %s", fname, man, exp);
    fclose(flog);

    int nbm = atoi(man);
    int nbe = atoi(exp);

    char bufi[1025], bufo[1025];

    while (!feof(stdin))
    {
        bufi[0] = 0;
        fscanf(stdin, "%s", bufi);
        if (bufi[0]) 
        {
            int x;
            sscanf(bufi, "%d", &x);
            float f = i2mf(x,nbm,nbe);
            sprintf(bufo, "%.3f", f);
            printf("%s\n", bufo);
            fflush(stdout);
        }
    }
    return 0;
}