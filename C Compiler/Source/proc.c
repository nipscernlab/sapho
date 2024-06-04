#PRNAME proc
#DIRNAM "C:\Users\LAPTELUSER\Documents\Igor\Atividades_SAPHO\Atividade1_SAPHO\Hardware\proc_H"
#DATYPE 1
#NBMANT 23
#NBEXPO 8
#NDSTAC 100
#SDEPTH 100
#NUIOIN 5
#NUIOOU 5
float a;

void main() 
{
    if(in(0)==0) // Inicialização
    {
        out(3,1.0);
    }
    else
    {
        a = 2.0;
        while(a<10)
        {
            out(1,a);
            a = a + 1;
        }
        out(3,2.0);
    }
    out(0,1.0);
}
