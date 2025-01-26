cls
echo off
set EXEMPLO=proc_fft
set ROOT_DIR=%cd%
set TESTE_DIR=%ROOT_DIR%\Teste

rmdir %TESTE_DIR% /s /q

:: Gera diretorios pra teste --------------------------------------------------

set INST_DIR=%TESTE_DIR%\Instalacao
set BIN_DIR=%INST_DIR%\bin
set HDL_DIR=%INST_DIR%\hdl
set MAC_DIR=%INST_DIR%\macros
set SCR_DIR=%INST_DIR%\script

set USER_DIR=%TESTE_DIR%\Projeto
set PROC_DIR=%USER_DIR%\%EXEMPLO%
set SOFT_DIR=%PROC_DIR%\Software
set HARD_DIR=%PROC_DIR%\Hardware

set TMP_DIR=%TESTE_DIR%\Tmp

mkdir %TESTE_DIR%
mkdir %INST_DIR%
mkdir %BIN_DIR%
mkdir %HDL_DIR%
mkdir %MAC_DIR%
mkdir %SCR_DIR%
mkdir %USER_DIR%
mkdir %PROC_DIR%
mkdir %SOFT_DIR%
mkdir %HARD_DIR%
mkdir %TMP_DIR%

:: Copia os arquivos para os diretorios de teste ------------------------------

set CMM_FILE=%ROOT_DIR%\Exemplos\%EXEMPLO%.cmm

xcopy HDL %HDL_DIR% /q /y
xcopy Macros %MAC_DIR% /q /y
xcopy Scripts %SCR_DIR% /q /y
cp %CMM_FILE% %SOFT_DIR%

:: Gera o compilador CMM ------------------------------------------------------

cd %ROOT_DIR%\CMMComp\Sources

bison -y -d cmm2asm.y
flex        cmm2asm.l
gcc      -o cmm2asm.exe data_assign.c data_declar.c data_use.c diretivas.c funcoes.c labels.c lex.yy.c oper.c saltos.c stdlib.c t2t.c variaveis.c y.tab.c

mv cmm2asm.exe %BIN_DIR%
rm lex.yy.c
rm  y.tab.c
rm  y.tab.h

:: Gera o compilador Assembler ------------------------------------------------

cd %ROOT_DIR%\Assembler\Sources

flex -oasm2mif.c asm2mif.l
gcc -o asm2mif.exe asm2mif.c eval.c labels.c mnemonicos.c variaveis.c t2t.c veri_comp.c

mv asm2mif.exe %BIN_DIR%
rm asm2mif.c

:: Gera tradutores de dados pro GtkWave ---------------------------------------

cd %SCR_DIR%

gcc -o float2gtkw.exe float2gtkw.c
gcc -o f2i_gtkw.exe f2i_gtkw.c
gcc -o comp2gtkw.exe comp2gtkw.c

mv float2gtkw.exe %BIN_DIR%
mv f2i_gtkw.exe %BIN_DIR%
mv comp2gtkw.exe %BIN_DIR%

:: Executa o compilador CMM ---------------------------------------------------

set ASM_FILE=%SOFT_DIR%\%EXEMPLO%.asm
cd %BIN_DIR%

cmm2asm.exe %CMM_FILE% %ASM_FILE% %MAC_DIR% %TMP_DIR%

:: Executa o compilador Assembler ---------------------------------------------

asm2mif.exe %ASM_FILE% %HARD_DIR% %HDL_DIR% %TMP_DIR% 100 %1

:: Gera o testbench com o Icarus ----------------------------------------------

cd %HDL_DIR%

set UDIR=%HARD_DIR%\%EXEMPLO%

iverilog -s %EXEMPLO%_tb -o %TMP_DIR%\%EXEMPLO% %UDIR%_tb.v %UDIR%.v %TMP_DIR%\mem_data_sim.v %TMP_DIR%\pc_sim.v int2float.v proc_fl.v float2int.v addr_dec.v core_fl.v mem_instr.v prefetch.v instr_dec.v stack_pointer.v ula.v float2index.v stack.v rel_addr.v ula_fl.v proc_fx.v core_fx.v ula_fx.v

cp %UDIR%_data.mif %TMP_DIR%
cp %UDIR%_inst.mif %TMP_DIR%
cd %TMP_DIR%
vvp %EXEMPLO%

:: Roda o GtkWave -------------------------------------------------------------

cp %BIN_DIR%\float2gtkw.exe %TMP_DIR%
cp %BIN_DIR%\f2i_gtkw.exe %TMP_DIR%
cp %BIN_DIR%\comp2gtkw.exe %TMP_DIR%

if exist %HARD_DIR%\.config.gtkw (gtkwave %HARD_DIR%\.config.gtkw) else (gtkwave %EXEMPLO%_tb.vcd --script=%SCR_DIR%\gtkwave_init.tcl)

cd %ROOT_DIR%