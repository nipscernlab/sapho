LOAD NULL
#PRNAME proc_fft
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
#array data 8
#array data_i 8
#array wpv 3
#array wpv_i 3
CALL main
@fim JMP fim
@fft SET fft_n
LOAD 1
SET fft_mmax
LOAD 0
SET fft_ind
@L1 LOAD fft_n
LES fft_mmax
JZ L1end
LOAD 2
MLT fft_mmax
SET fft_istep
LOAD 1.000000
SET fft_w
LOAD 0.000000
SET fft_w_i
LOAD 0
SET fft_m
@L2 LOAD fft_mmax
LES fft_m
JZ L2end
LOAD fft_m
SET fft_k
@L3 LOAD fft_n
LES fft_k
JZ L3end
LOAD fft_mmax
ADD fft_k
SET fft_j
LOAD fft_j
ILDI data
PLD fft_j
ILDI data_i
SETP fft_aux_cmp_i
SET  fft_aux_cmp_r
LOAD fft_aux_cmp_r
MLT fft_w
PLD fft_aux_cmp_i
MLT fft_w_i
NEG
SADD
PLD fft_aux_cmp_i
MLT fft_w
PLD fft_aux_cmp_r
MLT fft_w_i
SADD
SETP fft_temp_i
SET fft_temp
LOAD fft_j
PLD fft_k
ILDI data
PLD fft_k
ILDI data_i
SETP fft_aux_cmp_i
SET  fft_aux_cmp_r
LOAD fft_temp
NEG
ADD fft_aux_cmp_r
PLD fft_temp_i
NEG
ADD fft_aux_cmp_i
SETP aux_tmp_i
SETP aux_tmp_r
SET aux_index
PLD aux_tmp_r
ISRF
SET data
LOAD aux_index
PLD aux_tmp_i
ISRF
SET data_i
LOAD fft_k
PLD fft_k
ILDI data
PLD fft_k
ILDI data_i
SETP fft_aux_cmp_i
SET  fft_aux_cmp_r
LOAD fft_temp
ADD fft_aux_cmp_r
PLD fft_temp_i
ADD fft_aux_cmp_i
SETP aux_tmp_i
SETP aux_tmp_r
SET aux_index
PLD aux_tmp_r
ISRF
SET data
LOAD aux_index
PLD aux_tmp_i
ISRF
SET data_i
LOAD fft_istep
ADD fft_k
SET fft_k
JMP L3
@L3end LOAD fft_ind
LDI wpv
PLD fft_ind
LDI wpv_i
SETP fft_aux_cmp_i
SET  fft_aux_cmp_r
LOAD fft_aux_cmp_r
MLT fft_w
PLD fft_aux_cmp_i
MLT fft_w_i
NEG
SADD
PLD fft_aux_cmp_i
MLT fft_w
PLD fft_aux_cmp_r
MLT fft_w_i
SADD
SETP fft_w_i
SET fft_w
LOAD 1
ADD fft_m
SET fft_m
JMP L2
@L2end LOAD 1
ADD fft_ind
SET fft_ind
LOAD fft_istep
SET fft_mmax
JMP L1
@L1end RETURN
@main LOAD 0
SET aux_index
PLD -1.000000
SRF
SET wpv
LOAD aux_index
PLD 0.000000
SRF
SET wpv_i
LOAD 1
SET aux_index
PLD 0.000000
SRF
SET wpv
LOAD aux_index
PLD 1.000000
SRF
SET wpv_i
LOAD 2
SET aux_index
PLD 0.707107
SRF
SET wpv
LOAD aux_index
PLD 0.707107
SRF
SET wpv_i
LOAD 0
SET aux_index
PLD 1.000000
SRF
SET data
LOAD aux_index
PLD 0.000000
SRF
SET data_i
LOAD 1
SET aux_index
PLD 2.000000
SRF
SET data
LOAD aux_index
PLD 0.000000
SRF
SET data_i
LOAD 2
SET aux_index
PLD 3.000000
SRF
SET data
LOAD aux_index
PLD 0.000000
SRF
SET data_i
LOAD 3
SET aux_index
PLD 4.000000
SRF
SET data
LOAD aux_index
PLD 0.000000
SRF
SET data_i
LOAD 4
SET aux_index
PLD 5.000000
SRF
SET data
LOAD aux_index
PLD 0.000000
SRF
SET data_i
LOAD 5
SET aux_index
PLD 6.000000
SRF
SET data
LOAD aux_index
PLD 0.000000
SRF
SET data_i
LOAD 6
SET aux_index
PLD 7.000000
SRF
SET data
LOAD aux_index
PLD 0.000000
SRF
SET data_i
LOAD 7
SET aux_index
PLD 8.000000
SRF
SET data
LOAD aux_index
PLD 0.000000
SRF
SET data_i
LOAD 8
CALL fft
LOAD 0
PLD 0
ILDI data
PLD 0
ILDI data_i
SETP aux_cmp
MLT 1000.0
OUT
LOAD 1
PLD 0
ILDI data
PLD 0
ILDI data_i
SETP aux_cmp
LOAD aux_cmp
MLT 1000.0
OUT
LOAD 0
PLD 1
ILDI data
PLD 1
ILDI data_i
SETP aux_cmp
MLT 1000.0
OUT
LOAD 1
PLD 1
ILDI data
PLD 1
ILDI data_i
SETP aux_cmp
LOAD aux_cmp
MLT 1000.0
OUT
LOAD 0
PLD 2
ILDI data
PLD 2
ILDI data_i
SETP aux_cmp
MLT 1000.0
OUT
LOAD 1
PLD 2
ILDI data
PLD 2
ILDI data_i
SETP aux_cmp
LOAD aux_cmp
MLT 1000.0
OUT
LOAD 0
PLD 3
ILDI data
PLD 3
ILDI data_i
SETP aux_cmp
MLT 1000.0
OUT
LOAD 1
PLD 3
ILDI data
PLD 3
ILDI data_i
SETP aux_cmp
LOAD aux_cmp
MLT 1000.0
OUT
LOAD 0
PLD 4
ILDI data
PLD 4
ILDI data_i
SETP aux_cmp
MLT 1000.0
OUT
LOAD 1
PLD 4
ILDI data
PLD 4
ILDI data_i
SETP aux_cmp
LOAD aux_cmp
MLT 1000.0
OUT
LOAD 0
PLD 5
ILDI data
PLD 5
ILDI data_i
SETP aux_cmp
MLT 1000.0
OUT
LOAD 1
PLD 5
ILDI data
PLD 5
ILDI data_i
SETP aux_cmp
LOAD aux_cmp
MLT 1000.0
OUT
LOAD 0
PLD 6
ILDI data
PLD 6
ILDI data_i
SETP aux_cmp
MLT 1000.0
OUT
LOAD 1
PLD 6
ILDI data
PLD 6
ILDI data_i
SETP aux_cmp
LOAD aux_cmp
MLT 1000.0
OUT
LOAD 0
PLD 7
ILDI data
PLD 7
ILDI data_i
SETP aux_cmp
MLT 1000.0
OUT
LOAD 1
PLD 7
ILDI data
PLD 7
ILDI data_i
SETP aux_cmp
LOAD aux_cmp
MLT 1000.0
OUT
RETURN
