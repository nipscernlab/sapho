#PRNAME testeASM
#DIRNAM "C:\Users\Lucca\Desktop\TesteASM"
#DATYPE 0
#NUBITS 20
#NDSTAC 12
#SDEPTH 12
#NUIOIN 2
#NUIOOU 2
#NUGAIN 4

@main LOAD 32
SET maina
LOAD 16
SET mainb
LOAD maina
PLD mainb
SIGN
SET mainc
@fim JMP fim
