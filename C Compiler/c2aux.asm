#PRNAME testeProc
#DIRNAM "C:\Users\chrys\Desktop\teste\Hardware\testeProc_H"
#DATYPE 0
#NUBITS 16
#NUGAIN 4
#NDSTAC 4
#SDEPTH 4
#NUIOIN 2
#NUIOOU 2
@main LOAD 0
SET maini
LOAD 0
SET maina
@L1 LOAD 1 // 4.0
CALL float2int
SET maini
LOAD maini
CALL int2float
PLD 1 // 8.0
CALL denorm
LOAD float_aux3
LES float_aux1
PLD maini
CALL int2float
PLD 1 // 1.0
CALL denorm
CALL float_add
CALL float2int
SET maini
LOAD maina
CALL int2float
PLD 1 // 1.0
CALL denorm
CALL float_add
CALL float2int
SET maina
LOAD maini
JZ L1end
