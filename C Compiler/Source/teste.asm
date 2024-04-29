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
@L1 LOAD 2
SET maini
LOAD 6
LES maini
PLD 1
ADD maini
SET maini
JZ L1end
LOAD 1
ADD maina
SET maina
JMP L1
@L1end @fim JMP fim
