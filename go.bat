:: Gera o compilador CMM ------------------------------------------------------

cd CMMComp/Sources

bison -y -d cmm2asm.y
flex cmm2asm.l
gcc -o cmm2asm.exe data_assign.c data_declar.c data_use.c diretivas.c funcoes.c labels.c lex.yy.c oper.c saltos.c stdlib.c t2t.c variaveis.c y.tab.c

rm lex.yy.c
rm y.tab.c
rm y.tab.h

:: Executa o compilador CMM ---------------------------------------------------

cmm2asm.exe ../../Exemplos/fft.cmm tmp.asm

rm cmm2asm.exe
cd..
cd..

:: Gera o compilador Assembler ------------------------------------------------

cd Assembler/Sources

flex -oasm2mif.c asm2mif.l
gcc -o asm2mif.exe asm2mif.c eval.c labels.c mnemonicos.c variaveis.c t2t.c veri_comp.c

rm asm2mif.c

:: Executa o compilador Assembler ---------------------------------------------

asm2mif.exe ../../CMMComp/Sources/tmp.asm

rm asm2mif.exe
rm ../../CMMComp/Sources/tmp.asm
rm ../../CMMComp/Sources/c2aux.asm
mv *.mif *.v ../../HDL
cd..
cd..

:: Gera o testbench com o Icarus ----------------------------------------------

cd HDL

iverilog -s %1_tb -o %1 %1_tb.v %1.v int2float.v proc_fl.v float2int.v addr_dec.v mem_data.v core_fl.v mem_instr.v pc.v prefetch.v instr_dec.v stack_pointer.v ula.v float2index.v stack.v rel_addr.v ula_fl.v proc_fx.v core_fx.v ula_fx.v
vvp %1
gtkwave %1_tb.vcd --script=gtkwave_init.tcl

rm %1.v %1_tb.v
rm %1 %1_data.mif %1_inst.mif
rm %1_tb.vcd
cd..