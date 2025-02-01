cls
echo off

:: Parametros do teste --------------------------------------------------------

set EXEMPLO=DTW
set PROC=ZeroCross
set NUM_CLK=20000

set ROOT_DIR=%cd%
set TESTE_DIR=%ROOT_DIR%\Teste

rmdir %TESTE_DIR% /s /q

:: Gera diretorios pra teste --------------------------------------------------

set INST_DIR=%TESTE_DIR%\saphoComponents
set BIN_DIR=%INST_DIR%\bin
set HDL_DIR=%INST_DIR%\HDL
set MAC_DIR=%INST_DIR%\Macros
set SCR_DIR=%INST_DIR%\Scripts
set TMP_DIR=%INST_DIR%\Temp
set TMP_PRO=%TMP_DIR%\%PROC%

set USER_DIR=%TESTE_DIR%\Projetos
set PROC_DIR=%USER_DIR%\%EXEMPLO%\%PROC%
set SOFT_DIR=%PROC_DIR%\Software
set HARD_DIR=%PROC_DIR%\Hardware
set SIMU_DIR=%PROC_DIR%\Simulation

mkdir %TESTE_DIR%
    mkdir %INST_DIR%
        mkdir %BIN_DIR%
        mkdir %HDL_DIR%
        mkdir %MAC_DIR%
        mkdir %SCR_DIR%
        mkdir %TMP_DIR%
            mkdir %TMP_PRO%
    mkdir %USER_DIR%

:: Copia os arquivos para os diretorios de teste ------------------------------

xcopy Exemplos %USER_DIR% /e /i /q
xcopy HDL %HDL_DIR% /q /y
xcopy Macros %MAC_DIR% /q /y
xcopy Scripts %SCR_DIR% /q /y

:: Gera o compilador CMM ------------------------------------------------------

cd %ROOT_DIR%\CMMComp\Sources

bison -y -d CMMComp.y
flex        CMMComp.l
gcc      -o CMMComp.exe data_assign.c data_declar.c data_use.c diretivas.c funcoes.c labels.c lex.yy.c oper.c saltos.c stdlib.c t2t.c variaveis.c y.tab.c

mv CMMComp.exe %BIN_DIR%
rm lex.yy.c
rm  y.tab.c
rm  y.tab.h

:: Gera o compilador Assembler ------------------------------------------------

cd %ROOT_DIR%\Assembler\Sources

flex -oASMComp.c ASMComp.l
gcc -o ASMComp.exe ASMComp.c eval.c labels.c mnemonicos.c variaveis.c t2t.c veri_comp.c

mv ASMComp.exe %BIN_DIR%
rm ASMComp.c

:: Gera tradutores de dados pro GtkWave ---------------------------------------

cd %SCR_DIR%

gcc -o float2gtkw.exe float2gtkw.c
gcc -o f2i_gtkw.exe f2i_gtkw.c
gcc -o comp2gtkw.exe comp2gtkw.c

mv float2gtkw.exe %BIN_DIR%
mv f2i_gtkw.exe %BIN_DIR%
mv comp2gtkw.exe %BIN_DIR%

:: Executa o compilador CMM ---------------------------------------------------

set CMM_FILE=%SOFT_DIR%\%PROC%.cmm
set ASM_FILE=%SOFT_DIR%\%PROC%.asm
cd %BIN_DIR%

CMMComp.exe %CMM_FILE% %ASM_FILE% %MAC_DIR% %TMP_PRO% %PROC%

:: Executa o compilador Assembler ---------------------------------------------

ASMComp.exe %ASM_FILE% %HARD_DIR% %HDL_DIR% %TMP_PRO% 100 %NUM_CLK%

:: Gera o testbench com o Icarus ----------------------------------------------

cd %HDL_DIR%

set UDIR=%HARD_DIR%\%PROC%

iverilog -s %PROC%_tb -o %TMP_PRO%\%PROC% %TMP_PRO%\%PROC%_tb.v %UDIR%.v %TMP_PRO%\mem_data_%PROC%.v %TMP_PRO%\pc_%PROC%.v int2float.v proc_fl.v float2int.v addr_dec.v core_fl.v mem_instr.v prefetch.v instr_dec.v stack_pointer.v ula.v float2index.v stack.v rel_addr.v ula_fl.v proc_fx.v core_fx.v ula_fx.v

cp %UDIR%_data.mif %TMP_PRO%
cp %UDIR%_inst.mif %TMP_PRO%
cd %TMP_PRO%

vvp %PROC%

:: Roda o GtkWave -------------------------------------------------------------

cp %BIN_DIR%\float2gtkw.exe %TMP_PRO%
cp %BIN_DIR%\f2i_gtkw.exe %TMP_PRO%
cp %BIN_DIR%\comp2gtkw.exe %TMP_PRO%

if exist %SIMU_DIR%\.config.gtkw (gtkwave %SIMU_DIR%\.config.gtkw) else (gtkwave %PROC%_tb.vcd --script=%SCR_DIR%\gtkwave_init.tcl)

cd %ROOT_DIR%