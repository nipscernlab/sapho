#PRNAME proc_sim
#DIRNAM "."
#DATYPE 1
#NUBITS 23
#NDSTAC 10
#SDEPTH 10
#NUIOIN 1
#NUIOOU 2
#NBMANT 16
#NBEXPO 6
#FFTSIZ 3
#NUGAIN 128

void main()
{
    float x,xl,y;
    float ene[512] "Valores_Interpolados_Canal_A12-L_512bins.txt";

    float r1 = 0.0;
    float r2 = 0.0;
    float r3 = 0.0;
    float r4 = 0.0;
    float r5 = 0.0;
    float r6 = 0.0;
    float r7 = 0.0;

    while (1)
    {
        // truth --------------------------------------------------------------

        if (in(0) < 300) x = ene[in(0)]; else x = 0.0;

        out(0,x*1000.0);

        // pulso --------------------------------------------------------------

      /*y =  0.0222*x + 0.1068*rx1 + 0.1894*rx2 + 0.1037*rx3 - 0.1063*rx4 - 0.1890*rx5 - 0.1053*rx6 - 0.0215*rx7
                      - 0.0056*ry1 + 0.1267*ry2 + 0.4181*ry3 + 0.4352*ry4 - 0.0709*ry5 + 0.1524*ry6 - 0.0643*ry7;*/

        xl =        x  + 0.1267*r2 + 0.4181*r3 + 0.4352*r4 +  0.1524*r6 - (0.0056*r1 + 0.0709*r5 + 0.0643*r7);
        y  = 0.0222*xl + 0.1068*r1 + 0.1894*r2 + 0.1037*r3 - (0.1063*r4 +  0.1890*r5 + 0.1053*r6 + 0.0215*r7);

        r7 = r6;
        r6 = r5;
        r5 = r4;
        r4 = r3;
        r3 = r2;
        r2 = r1;
        r1 = xl;

        out(1,y*1000.0);

        // pzc ----------------------------------------------------------------
    }
}
