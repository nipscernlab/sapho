:: ****************************************************************************
:: Script para emular o SAPHO na compilacao de um projeto com varios procs
:: ****************************************************************************

:: Configura o ambiente -------------------------------------------------------

cls
::echo off
set ROOT_DIR=%cd%
set TESTE_DIR=%ROOT_DIR%\Teste
rmdir %TESTE_DIR% /s /q

:: Parametros definidos pelo usuario do SAPHO para compilacao -----------------

set PROJET=DTW
set PROC_LIST=ZeroCross ProcDTW
set TB=top_level_tb
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

:: Gera tradutores de dados ---------------------------------------------------

cd %SCR_DIR%

gcc -o float2gtkw.exe float2gtkw.c
gcc -o f2i_gtkw.exe f2i_gtkw.c
gcc -o comp2gtkw.exe comp2gtkw.c
gcc -o itob.exe itob.c

mv float2gtkw.exe %BIN_DIR%
mv f2i_gtkw.exe   %BIN_DIR%
mv comp2gtkw.exe  %BIN_DIR%
mv itob.exe       %BIN_DIR%

:: Executa o compilador CMM ---------------------------------------------------

cd  %BIN_DIR%

(for %%i in (%PROC_LIST%) do (
    CMMComp.exe %PROJ_DIR%\%%i\Software\%%i.cmm %PROJ_DIR%\%%i\Software\%%i.asm %MAC_DIR% %TMP_DIR%\%%i %%i 
))

:: Executa o compilador Assembler ---------------------------------------------

(for %%i in (%PROC_LIST%) do (
    ASMComp.exe %PROJ_DIR%\%%i\Software\%%i.asm %PROJ_DIR%\%%i\Hardware %HDL_DIR% %TMP_DIR%\%%i 0 0
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

:: lista arquivos da pasta Hardware dos processadores encontrados
for %%a in (%PROC_LIST%) do set "PRO_V=!PRO_V!%PROJ_DIR%\%%a\Hardware\%%a.v "    

iverilog -v -s %TB% -o %TMP_DIR%\%PROJET% %HDL_V% %PRO_V% %TOP_V%

:: Roda o testbench com o vvp -------------------------------------------------

dir %TOPL_DIR%\*.mif /b > f_list.txt
for /f "delims=" %%a in (%TMP_DIR%\f_list.txt) do cp %TOPL_DIR%\%%a .\
for %%a in (%PROC_LIST%) do cp %PROJ_DIR%\%%a\Hardware\%%a_inst.mif .\
for %%a in (%PROC_LIST%) do cp %PROJ_DIR%\%%a\Hardware\%%a_data.mif .\

endlocal

vvp -v %PROJET% -fst

:: Roda o GtkWave -------------------------------------------------------------

cp %BIN_DIR%\float2gtkw.exe %TMP_DIR%
cp %BIN_DIR%\f2i_gtkw.exe %TMP_DIR%
cp %BIN_DIR%\comp2gtkw.exe %TMP_DIR%

if exist %TOPL_DIR%\%GTKW% (gtkwave %TOPL_DIR%\%GTKW%) else (gtkwave %TB%.vcd)

cd %ROOT_DIR%