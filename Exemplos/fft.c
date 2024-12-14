#PRNAME proc_sim
#DIRNAM "."
#DATYPE 1
#NUBITS 32
#NDSTAC 10
#SDEPTH 10
#NUIOIN 1
#NUIOOU 2
#NBMANT 23
#NBEXPO 8
#FFTSIZ 3
#NUGAIN 128

comp data[8];
comp  wpv[3];

void fft(int n)
{
    int j,k,istep,m;

    comp w,temp;

    int mmax = 1;
    int ind  = 0;

    while (mmax < n)
    {
        istep = mmax*2;
        w = 1;
        m = 0;
        while (m < mmax)
        {
            k = m;
            while (k < n)
            {
                j       = k+mmax;
                temp    = w*data[j);
                data[j) =   data[k) - temp;
                data[k) =   data[k) + temp;
                k       = k+istep;
            }
            w = w*wpv[ind];
            m++;
        }
        ind++;
        mmax = istep;
    }
}

void main()
{
    wpv[0] = -1 + 0i;
    wpv[1] =  0 + 1i;
    wpv[2] =  0.7071067811 + 0.7071067811i;

    data[0] = 1;
    data[1] = 2;
    data[2] = 3;
    data[3] = 4;
    data[4] = 5;
    data[5] = 6;
    data[6] = 7;
    data[7] = 8;

    fft(8);

    out(0,1000*imag(data[0)));
    out(0,1000*imag(data[1)));
    out(0,1000*imag(data[2)));
    out(0,1000*imag(data[3)));
    out(0,1000*imag(data[4)));
    out(0,1000*imag(data[5)));
    out(0,1000*imag(data[6)));
    out(0,1000*imag(data[7)));
}

