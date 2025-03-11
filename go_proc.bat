:: ****************************************************************************
:: Script para emular o SAPHO na compilacao de um unico processador
:: ****************************************************************************

:: Configura o ambiente -------------------------------------------------------

cls
echo off

set ROOT_DIR=%cd%

set TESTE_DIR=%ROOT_DIR%\Teste
rmdir %TESTE_DIR% /s /q

:: Parametros definidos pelo usuario do SAPHO para compilacao -----------------

:: nome da pasta do projeto
set PROJET=FFT
:: nome do tipo de processador a ser simulado (uma sub-pasta do projeto)
set PROC=proc_fft
:: tipo de processador (int ou float)
set PROC_DATA=float
:: test_bench (sem .v) a ser simulado (tem que estar na pasta Simulation)
:: se nao achar, usa simulacao padrao
set TB=errado
:: nome do arquivo de visualizacao do gtkwave (se nao achar, usa o script padrao)
set GTKW=teste.gtkw
:: frequencia de operacao do processador em MHz
set FRE_CLK=100
:: numero de clocks a ser simulado
set NUM_CLK=20000

:: Parametros que o SAPHO tem que saber ---------------------------------------

:: Arvore de pastas apos a instalacao
set INST_DIR=%TESTE_DIR%\saphoComponents
set BIN_DIR=%INST_DIR%\bin
set HDL_DIR=%INST_DIR%\HDL
set MAC_DIR=%INST_DIR%\Macros
set SCR_DIR=%INST_DIR%\Scripts
set TMP_DIR=%INST_DIR%\Temp

:: Arvore de pastas do projeto sendo executado
set USER_DIR=%TESTE_DIR%\Projetos
set PROC_DIR=%USER_DIR%\%PROJET%\%PROC%
set SOFT_DIR=%PROC_DIR%\Software
set HARD_DIR=%PROC_DIR%\Hardware
set SIMU_DIR=%PROC_DIR%\Simulation
set TMP_PRO=%TMP_DIR%\%PROC%

:: Gera diretorios pra teste --------------------------------------------------

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

xcopy Exemplos %USER_DIR% /e /i /q>%TMP_PRO%\xcopy.txt
xcopy HDL %HDL_DIR% /q /y>%TMP_PRO%\xcopy.txt
xcopy Macros %MAC_DIR% /q /y>%TMP_PRO%\xcopy.txt
xcopy Scripts %SCR_DIR% /q /y>%TMP_PRO%\xcopy.txt

:: Gera o compilador CMM ------------------------------------------------------

cd %ROOT_DIR%\CMMComp\Sources

bison -y -d CMMComp.y
flex        CMMComp.l
gcc      -o CMMComp.exe data_assign.c data_declar.c data_use.c diretivas.c funcoes.c labels.c lex.yy.c oper.c saltos.c stdlib.c t2t.c variaveis.c array_index.c global.c y.tab.c

move CMMComp.exe %BIN_DIR%>%TMP_PRO%\xcopy.txt
del lex.yy.c
del  y.tab.c
del  y.tab.h

:: Gera o compilador Assembler ------------------------------------------------

cd %ROOT_DIR%\Assembler\Sources

flex -oASMComp.c ASMComp.l
gcc -o ASMComp.exe ASMComp.c eval.c labels.c mnemonicos.c variaveis.c t2t.c veri_comp.c

move ASMComp.exe %BIN_DIR%>%TMP_PRO%\xcopy.txt
del ASMComp.c

:: Gera tradutores para o GTKWave ---------------------------------------------

cd %SCR_DIR%

gcc -o float2gtkw.exe float2gtkw.c
gcc -o f2i_gtkw.exe f2i_gtkw.c
gcc -o comp2gtkw.exe comp2gtkw.c

move float2gtkw.exe %BIN_DIR%>%TMP_PRO%\xcopy.txt
move f2i_gtkw.exe   %BIN_DIR%>%TMP_PRO%\xcopy.txt
move comp2gtkw.exe  %BIN_DIR%>%TMP_PRO%\xcopy.txt

:: Executa o compilador CMM ---------------------------------------------------

cd %BIN_DIR%

CMMComp.exe %PROC% %PROC_DIR% %MAC_DIR% %TMP_PRO%

:: Executa o compilador Assembler ---------------------------------------------

set ASM_FILE=%SOFT_DIR%\%PROC%.asm

ASMComp.exe %ASM_FILE% %PROC_DIR% %HDL_DIR% %TMP_PRO% %FRE_CLK% %NUM_CLK% 0

:: Gera o testbench com o Icarus ----------------------------------------------

set UPROC=%HARD_DIR%\%PROC%
cd  %HDL_DIR%

if exist %SIMU_DIR%\%TB%.v (
    set TB_MOD=%TB%
) else (
    copy %TMP_PRO%\%PROC%_tb.v %SIMU_DIR%>%TMP_PRO%\xcopy.txt
    set TB_MOD=%PROC%_tb
)

iverilog -s %TB_MOD% -o %TMP_PRO%\%PROC%.vvp %SIMU_DIR%\%TB_MOD%.v %UPROC%.v %TMP_PRO%\mem_data_%PROC%.v %TMP_PRO%\pc_%PROC%.v int2float.v proc_fl.v float2int.v addr_dec.v core_fl.v mem_instr.v prefetch.v instr_dec.v stack_pointer.v ula.v float2index.v stack.v rel_addr.v ula_fl.v proc_fx.v core_fx.v ula_fx.v

:: Roda o testbench com o vvp -------------------------------------------------

copy %UPROC%_data.mif %TMP_PRO%>%TMP_PRO%\xcopy.txt
copy %UPROC%_inst.mif %TMP_PRO%>%TMP_PRO%\xcopy.txt

cd %TMP_PRO%
del xcopy.txt

vvp %PROC%.vvp -fst

:: Roda o GtkWave -------------------------------------------------------------

echo %PROC_DATA%>tcl_infos.txt
echo %TMP_PRO%>>tcl_infos.txt
echo %BIN_DIR%>>tcl_infos.txt

if exist %SIMU_DIR%\%GTKW% (
    gtkwave --rcvar "hide_sst on" --dark %SIMU_DIR%\%GTKW% --script=%SCR_DIR%\pos_gtkw.tcl
) else (
    gtkwave --rcvar "hide_sst on" --dark %TMP_PRO%\%TB_MOD%.vcd --script=%SCR_DIR%\gtk_proc_init.tcl
)

:: RTL Viewer -----------------------------------------------------------------

cd %HDL_DIR%
cp %UPROC%.v .
cp %SCR_DIR%\proc2rtl.ys .

sed -i "s/@PROC@/%PROC%/" proc2rtl.ys
yosys -s proc2rtl.ys
cmd /c "netlistsvg proc_fft.json -o proc_fft.svg"

cp  proc_fft.svg %TMP_PRO%
del proc_fft.json
del proc_fft.svg
del proc2rtl.ys
del %PROC%.v

:: Limpa a pasta de arquivos temporarios --------------------------------------

del %TMP_PRO%\cmm_log.txt
del %TMP_PRO%\mem_data_%PROC%.v
del %TMP_PRO%\pc_%PROC%_mem.txt
del %TMP_PRO%\pc_%PROC%.v
del %TMP_PRO%\%PROC%_data.mif
del %TMP_PRO%\%PROC%_inst.mif
del %TMP_PRO%\%PROC%_tb.v
del %TMP_PRO%\tcl_infos.txt
del %TMP_PRO%\%PROC%.vvp
del %TMP_PRO%\%TB_MOD%.vcd
del %TMP_PRO%\trad_cmm.txt
del %TMP_PRO%\trad_opcode.txt

cd %ROOT_DIR%