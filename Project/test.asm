LOAD NULL
#PRNAME proc_FFT128
#DIRNAM "."
#DATYPE 1
#NBMANT 23
#NBEXPO 8
#NDSTAC 100
#SDEPTH 100
#NUIOIN 5
#NUIOOU 6
#array data 258
CALL   main
@fim JMP fim
@fft SETP fft_nfft
SET fft_nn
LOAD 2
MLT fft_nn
SET fft_n
LOAD 1
SET fft_j
LOAD 1
SET fft_i
LOAD 0.0
SET fft_tempr
LOAD 0.0
SET fft_tempi
LOAD 0
SET fft_m
LOAD 0
SET fft_flag_fft
@L1 LOAD fft_n
LES fft_i
JZ L1end
LOAD fft_i
GRE fft_j
JZ L2else
LOAD fft_nfft
ADD fft_j
PUSH
SRF
LOAD data
SET fft_tempr
LOAD fft_nfft
ADD fft_j
PLD fft_nfft
ADD fft_i
PUSH
SRF
LOAD data
SRF
SET data
LOAD fft_nfft
ADD fft_i
PLD fft_tempr
SRF
SET data
LOAD 1
ADD fft_j
PLD fft_nfft
SADD
PUSH
SRF
LOAD data
SET fft_tempr
LOAD 1
ADD fft_j
PLD fft_nfft
SADD
PLD 1
ADD fft_i
PLD fft_nfft
SADD
PUSH
SRF
LOAD data
SRF
SET data
LOAD 1
ADD fft_i
PLD fft_nfft
SADD
PLD fft_tempr
SRF
SET data
@L2else LOAD fft_nn
SET fft_m
LOAD 1
GRE fft_m
JZ L3else
LOAD fft_m
GRE fft_j
JZ L4else
LOAD 1
SET fft_flag_fft
@L4else @L3else @L5 LOAD 1
EQU fft_flag_fft
JZ L5end
LOAD fft_m
NEG
ADD fft_j
SET fft_j
LOAD 0.5
MLT fft_m
SET fft_m
LOAD 1
GRE fft_m
JZ L6else
LOAD fft_m
GRE fft_j
JZ L7else
LOAD 1
SET fft_flag_fft
JMP L7end
@L7else LOAD 0
SET fft_flag_fft
@L7end JMP L6end
@L6else LOAD 0
SET fft_flag_fft
@L6end JMP L5
@L5end LOAD fft_m
ADD fft_j
SET fft_j
LOAD 2.0
ADD fft_i
SET fft_i
JMP L1
@L1end LOAD 2.0
SET fft_mmax
@L8 LOAD fft_mmax
GRE fft_n
JZ L8end
LOAD 2.0
MLT fft_mmax
SET fft_istep
LOAD fft_mmax
DIV 6.28318530717959
SET fft_theta
LOAD fft_theta
MLT 0.5
PLD fft_theta
MLT 0.02083333333
PLD fft_theta
SMLT
PLD fft_theta
SMLT
NEG
SADD
PLD fft_theta
MLT fft_theta
PLD fft_theta
SMLT
PLD fft_theta
SMLT
PLD fft_theta
SMLT
PLD 0.00026041666
SMLT
SADD
PLD fft_theta
MLT fft_theta
PLD fft_theta
SMLT
PLD fft_theta
SMLT
PLD fft_theta
SMLT
PLD fft_theta
SMLT
PLD fft_theta
SMLT
PLD 0.00000155009
SMLT
NEG
SADD
SET fft_wtemp
LOAD fft_wtemp
MLT 2.0
PLD fft_wtemp
SMLT
NEG
SET fft_wpr
LOAD fft_theta
MLT 0.16666666666
PLD fft_theta
SMLT
PLD fft_theta
SMLT
NEG
ADD fft_theta
PLD fft_theta
MLT 0.00833333333
PLD fft_theta
SMLT
PLD fft_theta
SMLT
PLD fft_theta
SMLT
PLD fft_theta
SMLT
SADD
PLD fft_theta
MLT fft_theta
PLD fft_theta
SMLT
PLD fft_theta
SMLT
PLD fft_theta
SMLT
PLD fft_theta
SMLT
PLD fft_theta
SMLT
PLD 0.00019841269
SMLT
NEG
SADD
SET fft_wpi
LOAD 1.0
SET fft_wr
LOAD 0.0
SET fft_wi
LOAD 1.0
SET fft_m
@L9 LOAD fft_mmax
LES fft_m
JZ L9end
LOAD fft_m
SET fft_i
@L10 LOAD 1
ADD fft_n
LES fft_i
JZ L10end
LOAD fft_mmax
ADD fft_i
SET fft_j
LOAD fft_nfft
ADD fft_j
PUSH
SRF
LOAD data
MLT fft_wr
PLD 1
ADD fft_j
PLD fft_nfft
SADD
PUSH
SRF
LOAD data
MLT fft_wi
NEG
SADD
SET fft_tempr
LOAD 1
ADD fft_j
PLD fft_nfft
SADD
PUSH
SRF
LOAD data
MLT fft_wr
PLD fft_nfft
ADD fft_j
PUSH
SRF
LOAD data
MLT fft_wi
SADD
SET fft_tempi
LOAD fft_nfft
ADD fft_j
PLD fft_nfft
ADD fft_i
PUSH
SRF
LOAD data
PLD fft_tempr
NEG
SADD
SRF
SET data
LOAD 1
ADD fft_j
PLD fft_nfft
SADD
PLD 1
ADD fft_i
PLD fft_nfft
SADD
PUSH
SRF
LOAD data
PLD fft_tempi
NEG
SADD
SRF
SET data
LOAD fft_nfft
ADD fft_i
PLD fft_nfft
ADD fft_i
PUSH
SRF
LOAD data
PLD fft_tempr
SADD
SRF
SET data
LOAD 1
ADD fft_i
PLD fft_nfft
SADD
PLD 1
ADD fft_i
PLD fft_nfft
SADD
PUSH
SRF
LOAD data
PLD fft_tempi
SADD
SRF
SET data
LOAD fft_istep
ADD fft_i
SET fft_i
JMP L10
@L10end LOAD fft_wr
SET fft_wtemp
LOAD fft_wtemp
MLT fft_wpr
PLD fft_wpi
MLT fft_wi
NEG
SADD
PLD fft_wtemp
SADD
SET fft_wr
LOAD fft_wpr
MLT fft_wi
PLD fft_wpi
MLT fft_wtemp
SADD
PLD fft_wi
SADD
SET fft_wi
LOAD 2
ADD fft_m
SET fft_m
JMP L9
@L9end LOAD fft_istep
SET fft_mmax
JMP L8
@L8end RETURN
@main LOAD 0
PUSH
IN
PLD 0
SEQU
JZ L11else
LOAD 128
SET N
JMP L11end
@L11else LOAD 0
PLD 0.0
SRF
SET data
LOAD 2
MLT N
PLD 1
SADD
PLD 0.0
SRF
SET data
LOAD 1
SET cont
@L12 LOAD 2
MLT N
PLD 1
SADD
LES cont
JZ L12end
LOAD cont
PLD 1
PUSH
IN
SRF
SET data
LOAD 1
ADD cont
PLD 0
SRF
SET data
LOAD 2
ADD cont
SET cont
JMP L12
@L12end LOAD 0
SET cont
@L13 LOAD 2
MLT N
PLD 2
SADD
LES cont
JZ L13end
LOAD 1
PLD cont
PUSH
SRF
LOAD data
OUT
LOAD 1
ADD cont
SET cont
JMP L13
@L13end LOAD 128.0
PLD 0
CALL   fft
LOAD 0
SET cont
@L14 LOAD 2
MLT N
PLD 2
SADD
LES cont
JZ L14end
LOAD 2
PLD cont
PUSH
SRF
LOAD data
OUT
LOAD 1
ADD cont
SET cont
JMP L14
@L14end @L11end LOAD 0
PLD 1
OUT
RETURN
