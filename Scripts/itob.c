#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

int main(int argc, char *argv[])
{
    FILE *f_in  = fopen(argv[1], "r");
    FILE *f_out = fopen(argv[2], "w");

    int w = atoi(argv[3]);
    char num[128] = "";
    char *val;
    int x;

    while(fgets(num, 128, f_in) != NULL)
    {
        x   = atoi(num);
        val = itob(x,w);
        strcat(val,"\n");
        fputs(val, f_out);
        memset(num, 0, sizeof(char) * 128);
    }

    fclose(f_in );
    fclose(f_out);

    return 0;
}