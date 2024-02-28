LOAD 0
#PRNAME teste
#DIRNAM "C:\Users\luciano\Desktop\20-Final\Quartus"
#DATYPE 0
#NUBITS 32
#NBMANT 23
#NBEXPO 8
#NDSTAC 16
#SDEPTH 5
#NUIOIN 10
#NUIOOU 10
@main LOAD 2
CALL int2float
PLD 1977614336 // 3.0
CALL denorm
CALL float_add
SET mainx
LOAD 0
PLD mainx
CALL float2int
OUT
@fim JMP fim
