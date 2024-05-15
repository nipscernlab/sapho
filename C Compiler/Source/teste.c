#PRNAME proc
#DIRNAM "C:\Users\LCOM\Downloads\Quartus 22.1.2\Atividade1_SAPHO\Hardware\proc_H"
#DATYPE 1
#NBMANT 23
#NBEXPO 8
#NDSTAC 100
#SDEPTH 100
#NUIOIN 5
#NUIOOU 5

float i;

void main() 
{

        if(in(0)==0) // Inicialização
    {
        out(3,1.0);
    }
    else
    {
        
           i = 5.0;

           i++
           out(4, i);
           i++
           out(4, i);
           i++
           out(4, i);
           i++
           out(4, i);
                
        out(3,2.0);
    }
    out(0,1.0); //finalização
}