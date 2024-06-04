#PRNAME proc
#DIRNAM "C:\Users\LCOM\Desktop\Atividade1_SAPHO\Hardware\proc_H"
#DATYPE 1
#NBMANT 23
#NBEXPO 8
#NDSTAC 100
#SDEPTH 100
#NUIOIN 5
#NUIOOU 5
float a;
float b;

void main() 
{
    if(in(0)==0) // Inicialização
    {
        out(3,88.0);
    }
    else
    {
      
        for(a = 33.0; a < 10; a = a + 1.0)
        {
            out(46,a);
            b = b + 1.0;
        }
        out(3,66.0);
    }
    out(0,77.0);
}