#PRNAME detetor_top
#DIRNAM "C:"
#DATYPE 0
#NUBITS 29
#NDSTAC 16
#SDEPTH 16
#NUIOIN 2
#NUIOOU 2
#NUGAIN 2
@main LOAD 1 // 33.4
PLD 1
PLD float_nbits
SSHL
SADD
SET maina
#array mainarray 2
LOAD 0
PLD 1 // 22.3
SRF
SET mainarray
LOAD maina
CALL float2int
PLD 0
PUSH
SRF
LOAD mainarray
CALL float2int
SIGN
CALL int2float
SET mainb
@fim JMP fim
