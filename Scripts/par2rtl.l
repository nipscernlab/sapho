%{

#include <stdio.h>

FILE *bat;
char param[100];

%}

%%

"NBMANT" {strcpy(param, yytext);}
"NBEXPO" {strcpy(param, yytext);}
"NUIOOU" {strcpy(param, yytext);}
"NUIOIN" {strcpy(param, yytext);}
"CAL 1"  {fprintf(bat, "sed -i \"s/CAL 0/CAL 1/\" %%TMP_PRO%%/rtl.ys\n");}
"SRF 1"  {fprintf(bat, "sed -i \"s/SRF 0/SRF 1/\" %%TMP_PRO%%/rtl.ys\n");}
"LDI 1"  {fprintf(bat, "sed -i \"s/LDI 0/LDI 1/\" %%TMP_PRO%%/rtl.ys\n");}
"FFT 1"  {fprintf(bat, "sed -i \"s/FFT 0/FFT 1/\" %%TMP_PRO%%/rtl.ys\n");}
"ADD 1"  {fprintf(bat, "sed -i \"s/ADD 0/ADD 1/\" %%TMP_PRO%%/rtl.ys\n");}
"MLT 1"  {fprintf(bat, "sed -i \"s/MLT 0/MLT 1/\" %%TMP_PRO%%/rtl.ys\n");}
"DIV 1"  {fprintf(bat, "sed -i \"s/DIV 0/DIV 1/\" %%TMP_PRO%%/rtl.ys\n");}
"MOD 1"  {fprintf(bat, "sed -i \"s/MOD 0/MOD 1/\" %%TMP_PRO%%/rtl.ys\n");}
"ABS 1"  {fprintf(bat, "sed -i \"s/ABS 0/ABS 1/\" %%TMP_PRO%%/rtl.ys\n");}
"NRM 1"  {fprintf(bat, "sed -i \"s/NRM 0/NRM 1/\" %%TMP_PRO%%/rtl.ys\n");}
"PST 1"  {fprintf(bat, "sed -i \"s/PST 0/PST 1/\" %%TMP_PRO%%/rtl.ys\n");}
"SGN 1"  {fprintf(bat, "sed -i \"s/SGN 0/SGN 1/\" %%TMP_PRO%%/rtl.ys\n");}
"NEG 1"  {fprintf(bat, "sed -i \"s/NEG 0/NEG 1/\" %%TMP_PRO%%/rtl.ys\n");}
"OR 1"   {fprintf(bat, "sed -i \"s/OR 0/OR 1/\" %%TMP_PRO%%/rtl.ys\n");}
"AND 1"  {fprintf(bat, "sed -i \"s/AND 0/AND 1/\" %%TMP_PRO%%/rtl.ys\n");}
"INV 1"  {fprintf(bat, "sed -i \"s/INV 0/INV 1/\" %%TMP_PRO%%/rtl.ys\n");}
"XOR 1"  {fprintf(bat, "sed -i \"s/XOR 0/XOR 1/\" %%TMP_PRO%%/rtl.ys\n");}
"SHL 1"  {fprintf(bat, "sed -i \"s/SHL 0/SHL 1/\" %%TMP_PRO%%/rtl.ys\n");}
"SHR 1"  {fprintf(bat, "sed -i \"s/SHR 0/SHR 1/\" %%TMP_PRO%%/rtl.ys\n");}
"SRS 1"  {fprintf(bat, "sed -i \"s/SRS 0/SRS 1/\" %%TMP_PRO%%/rtl.ys\n");}
"LOR 1"  {fprintf(bat, "sed -i \"s/LOR 0/LOR 1/\" %%TMP_PRO%%/rtl.ys\n");}
"LAN 1"  {fprintf(bat, "sed -i \"s/LAN 0/LAN 1/\" %%TMP_PRO%%/rtl.ys\n");}
"LIN 1"  {fprintf(bat, "sed -i \"s/LIN 0/LIN 1/\" %%TMP_PRO%%/rtl.ys\n");}
"GRE 1"  {fprintf(bat, "sed -i \"s/GRE 0/GRE 1/\" %%TMP_PRO%%/rtl.ys\n");}
"LES 1"  {fprintf(bat, "sed -i \"s/LES 0/LES 1/\" %%TMP_PRO%%/rtl.ys\n");}
"EQU 1"  {fprintf(bat, "sed -i \"s/EQU 0/EQU 1/\" %%TMP_PRO%%/rtl.ys\n");}

[0-9]+ { fprintf(bat, "sed -i \"s/@%s@/%s/\" %%TMP_PRO%%/rtl.ys\n", param, yytext); }

.|\n ;

%%

int main(int argc, char *argv[])
{
	yyin = fopen(argv[1], "r");
    bat  = fopen(argv[2], "w");
    fprintf(bat, "sed -i \"s/@PROC@/%%PROC%%/\" %%TMP_PRO%%/rtl.ys\n");
    yylex();
    fprintf(bat, "yosys -s %%TMP_PRO%%/rtl.ys\n");
	fclose(yyin);
    fclose(bat );

    return 0;
}

int yywrap(void) {return 1;}
