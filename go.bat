cls
echo off

:: Gera o compilador CMM ------------------------------------------------------

cd CMMComp/Sources

bison -y -d cmm2asm.y
flex cmm2asm.l
gcc -o cmm2asm.exe data_assign.c data_declar.c data_use.c diretivas.c funcoes.c labels.c lex.yy.c oper.c saltos.c stdlib.c t2t.c variaveis.c y.tab.c

rm lex.yy.c
rm y.tab.c
rm y.tab.h

:: Executa o compilador CMM ---------------------------------------------------

cmm2asm.exe ../../%1 tmp.asm

rm cmm2asm.exe
mv log.txt ../../Assembler/Sources
mv in2line.txt ../../HDL
cd..
cd..

:: Gera o compilador Assembler ------------------------------------------------

cd Assembler/Sources

flex -oasm2mif.c asm2mif.l
gcc -o asm2mif.exe asm2mif.c eval.c labels.c mnemonicos.c variaveis.c t2t.c veri_comp.c

rm asm2mif.c

:: Executa o compilador Assembler ---------------------------------------------

asm2mif.exe ../../CMMComp/Sources/tmp.asm %2 %3

rm asm2mif.exe
rm ../../CMMComp/Sources/tmp.asm
if exist ../../CMMComp/Sources/c2aux.asm rm ../../CMMComp/Sources/c2aux.asm
mv *.mif *.v ../../HDL
mv trad_opcode.txt ../../HDL

:: Gera traducao de ponto-flutuante pro GtkWave -------------------------------

gcc -o float2gtkw.exe float2gtkw.c
cd..
cd..

:: Gera o testbench com o Icarus ----------------------------------------------

set /p PROC_NAME=<Assembler/Sources/log.txt
cd HDL

iverilog -s %PROC_NAME%_tb -o %PROC_NAME% %PROC_NAME%_tb.v %PROC_NAME%.v int2float.v proc_fl.v float2int.v addr_dec.v mem_data_sim.v core_fl.v mem_instr.v pc_sim.v prefetch.v instr_dec.v stack_pointer.v ula.v float2index.v stack.v rel_addr.v ula_fl.v proc_fx.v core_fx.v ula_fx.v
vvp %PROC_NAME%
if exist config.gtkw (gtkwave config.gtkw) else (gtkwave %PROC_NAME%_tb.vcd --script=gtkwave_init.tcl)

rm %PROC_NAME%.v %PROC_NAME%_tb.v
rm %PROC_NAME% %PROC_NAME%_data.mif %PROC_NAME%_inst.mif
rm %PROC_NAME%_tb.vcd
rm trad_opcode.txt
cd..
rm Assembler/Sources/log.txt
rm Assembler/Sources/float2gtkw.exe
rm HDL/in2line.txt
rm HDL/pc_sim.v
rm HDL/trad_cmm.txt
rm HDL/mem_data_sim.v