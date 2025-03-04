:: ****************************************************************************
:: Script para emular o SAPHO na compilacao de um projeto com varios procs
:: ****************************************************************************

:: Configura o ambiente -------------------------------------------------------

cls
echo off
set ROOT_DIR=%cd%
set TESTE_DIR=%ROOT_DIR%\Teste
rmdir %TESTE_DIR% /s /q

:: Parametros definidos pelo usuario do SAPHO para compilacao -----------------

:: nome da pasta do projeto
set PROJET=DTW
:: lista dos tipo de processadores que tem no projeto (nomes das sub-pastas do projeto)
set PROC_LIST=ProcDTW ZeroCross
:: lista das instancias que serao simuladas (um mesmo proc pode ter varias instancias)
set INST_LIST=ZeroCross_inst DTWv4_inst
:: lista do tipo de processador para cada instancia (tem que ser do mesmo tamanho de PROC_LIST)
set PROC_TYPE=ZeroCross ProcDTW
:: lista o tipo de dado da instancia do processador (int ou float)
set PROC_DATA=float int
:: nome do test bench (sem .v) a ser simulado (tem que estar na pasta TopLevel)
set TB=top_level_tb
:: nome do arquivo de visualizacao do gtkwave (se nao achar, usa o script padrao)
set GTKW=dtw.gtkw

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
set PROJ_DIR=%USER_DIR%\%PROJET%
set TOPL_DIR=%PROJ_DIR%\TopLevel

:: Gera diretorios pra teste --------------------------------------------------

mkdir %TESTE_DIR%
    mkdir %INST_DIR%
        mkdir %BIN_DIR%
        mkdir %HDL_DIR%
        mkdir %MAC_DIR%
        mkdir %SCR_DIR%
        mkdir %TMP_DIR%
    mkdir %USER_DIR%

(for %%i in (%PROC_LIST%) do (
    mkdir %TMP_DIR%\%%i
))

:: Copia os arquivos para os diretorios de teste ------------------------------

xcopy Exemplos %USER_DIR% /e /i /q>%TMP_DIR%\xcopy.txt
xcopy HDL %HDL_DIR% /q /y>%TMP_DIR%\xcopy.txt
xcopy Macros %MAC_DIR% /q /y>%TMP_DIR%\xcopy.txt
xcopy Scripts %SCR_DIR% /q /y>%TMP_DIR%\xcopy.txt

:: Gera o compilador CMM ------------------------------------------------------

cd %ROOT_DIR%\CMMComp\Sources

bison -y -d CMMComp.y
flex        CMMComp.l
gcc      -o CMMComp.exe data_assign.c data_declar.c data_use.c diretivas.c funcoes.c labels.c lex.yy.c oper.c saltos.c stdlib.c t2t.c variaveis.c array_index.c global.c y.tab.c

move CMMComp.exe %BIN_DIR%>%TMP_DIR%\xcopy.txt
del lex.yy.c
del  y.tab.c
del  y.tab.h

:: Gera o compilador Assembler ------------------------------------------------

cd %ROOT_DIR%\Assembler\Sources

flex -oASMComp.c ASMComp.l
gcc -o ASMComp.exe ASMComp.c eval.c labels.c mnemonicos.c variaveis.c t2t.c veri_comp.c

move ASMComp.exe %BIN_DIR%>%TMP_DIR%\xcopy.txt
del ASMComp.c

:: Gera tradutores de dados ---------------------------------------------------

cd %SCR_DIR%

gcc -o float2gtkw.exe float2gtkw.c
gcc -o f2i_gtkw.exe f2i_gtkw.c
gcc -o comp2gtkw.exe comp2gtkw.c

move float2gtkw.exe %BIN_DIR%>%TMP_DIR%\xcopy.txt
move f2i_gtkw.exe   %BIN_DIR%>%TMP_DIR%\xcopy.txt
move comp2gtkw.exe  %BIN_DIR%>%TMP_DIR%\xcopy.txt

:: Executa o compilador CMM ---------------------------------------------------

cd  %BIN_DIR%

(for %%i in (%PROC_LIST%) do (
    CMMComp.exe %%i %PROJ_DIR%\%%i %MAC_DIR% %TMP_DIR%\%%i
))

:: Executa o compilador Assembler ---------------------------------------------

(for %%i in (%PROC_LIST%) do (
    ASMComp.exe %PROJ_DIR%\%%i\Software\%%i.asm %PROJ_DIR%\%%i %HDL_DIR% %TMP_DIR%\%%i 0 0 1
))

:: Gera o testbench com o Icarus ----------------------------------------------

cd  %TMP_DIR%

setlocal enabledelayedexpansion

:: lista arquivos da pasta HDL
dir %HDL_DIR%\*.v /b > f_list.txt
for /f "delims=" %%a in (%TMP_DIR%\f_list.txt) do set "HDL_V=!HDL_V!%HDL_DIR%\%%a "

:: lista arquivos da pasta TopLevel
dir %TOPL_DIR%\*.v /b > f_list.txt
for /f "delims=" %%a in (%TMP_DIR%\f_list.txt) do set "TOP_V=!TOP_V!%TOPL_DIR%\%%a "

:: lista arquivos dos processadores encontrados (simulacao)
for %%a in (%PROC_LIST%) do (
    set "PRO_V=!PRO_V!%TMP_DIR%\%%a\%%a_sim.v "
    set "PRO_V=!PRO_V!%TMP_DIR%\%%a\proc_flx_%%a_sim.v "
    set "PRO_V=!PRO_V!%TMP_DIR%\%%a\core_flx_%%a_sim.v "
    set "PRO_V=!PRO_V!%TMP_DIR%\%%a\pc_%%a.v "
    set "PRO_V=!PRO_V!%TMP_DIR%\%%a\mem_data_%%a.v "
)     

iverilog -s %TB% -o %TMP_DIR%\%PROJET%.vvp %HDL_V% %PRO_V% %TOP_V%

for %%a in (%PROC_LIST%) do copy %TMP_DIR%\%%a\%%a_tb.v %PROJ_DIR%\%%a\Simulation>%TMP_DIR%\xcopy.txt

:: Roda o testbench com o vvp -------------------------------------------------

dir %TOPL_DIR%\*.txt /b > f_list.txt
for /f "delims=" %%a in (%TMP_DIR%\f_list.txt) do copy %TOPL_DIR%\%%a .\>%TMP_DIR%\xcopy.txt
for %%a in (%PROC_LIST%) do copy %PROJ_DIR%\%%a\Hardware\%%a_inst.mif .\>%TMP_DIR%\xcopy.txt
for %%a in (%PROC_LIST%) do copy %PROJ_DIR%\%%a\Hardware\%%a_data.mif .\>%TMP_DIR%\xcopy.txt
for %%a in (%PROC_LIST%) do copy %TMP_DIR%\%%a\pc_%%a_mem.txt .\>%TMP_DIR%\xcopy.txt

endlocal

del f_list.txt
del xcopy.txt

vvp %PROJET%.vvp -fst

:: Roda o GtkWave -------------------------------------------------------------

echo %INST_LIST%> tcl_infos.txt
echo %PROC_TYPE%>>tcl_infos.txt
echo %PROC_DATA%>>tcl_infos.txt
echo %TMP_DIR%>>  tcl_infos.txt
echo %BIN_DIR%>>  tcl_infos.txt

if exist %TOPL_DIR%\%GTKW% (
    gtkwave --rcvar "hide_sst on" --dark %TOPL_DIR%\%GTKW% --script=%SCR_DIR%\pos_gtkw.tcl
) else (
    gtkwave --rcvar "hide_sst on" --dark %TMP_DIR%\%TB%.vcd --script=%SCR_DIR%\gtk_proj_init.tcl
)

cd %ROOT_DIR%