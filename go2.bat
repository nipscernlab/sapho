cls
::echo off

set EXEMPLO=DTW

:: gera arvore de diretorios --------------------------------------------------

rmdir Teste /s /q
mkdir Teste
mkdir Teste\Projeto

xcopy Exemplos\%EXEMPLO% Teste\Projeto /e /i /q

mkdir Teste\saphoComponents
mkdir Teste\saphoComponents\bin
mkdir Teste\saphoComponents\HDL
mkdir Teste\saphoComponents\Macros
mkdir Teste\saphoComponents\Scripts

xcopy HDL Teste\saphoComponents\HDL /e /i /q
xcopy Macros Teste\saphoComponents\Macros /e /i /q
xcopy Scripts Teste\saphoComponents\Scripts /e /i /q

mkdir Teste\saphoComponents\Temp

dir Exemplos\%EXEMPLO% /a:d /b > Teste\saphoComponents\Temp\proc_list.txt

for /f "tokens=*" %%a in (Teste\saphoComponents\Temp\proc_list.txt) do (
  mkdir Teste\saphoComponents\Temp\%%a
)

:: Gera o compilador CMM ------------------------------------------------------

set ROOT_DIR=%cd%
cd CMMComp\Sources

bison -y -d CMMComp.y
flex        CMMComp.l
gcc      -o CMMComp.exe data_assign.c data_declar.c data_use.c diretivas.c funcoes.c labels.c lex.yy.c oper.c saltos.c stdlib.c t2t.c variaveis.c y.tab.c

mv CMMComp.exe %ROOT_DIR%\Teste\saphoComponents\bin
rm lex.yy.c
rm  y.tab.c
rm  y.tab.h

:: Gera o compilador Assembler ------------------------------------------------

cd %ROOT_DIR%\Assembler\Sources

flex -oASMComp.c ASMComp.l
gcc -o ASMComp.exe ASMComp.c eval.c labels.c mnemonicos.c variaveis.c t2t.c veri_comp.c

mv ASMComp.exe %ROOT_DIR%\Teste\saphoComponents\bin
rm ASMComp.c

:: Gera tradutores de dados pro GtkWave ---------------------------------------

cd %ROOT_DIR%\Scripts

gcc -o float2gtkw.exe float2gtkw.c
gcc -o f2i_gtkw.exe f2i_gtkw.c
gcc -o comp2gtkw.exe comp2gtkw.c

mv float2gtkw.exe %ROOT_DIR%\Teste\saphoComponents\bin
mv f2i_gtkw.exe %ROOT_DIR%\Teste\saphoComponents\bin
mv comp2gtkw.exe %ROOT_DIR%\Teste\saphoComponents\bin

set MAC_DIR=%ROOT_DIR%\Teste\saphoComponents\Macros

for /f "tokens=*" %%b in (%ROOT_DIR%\Teste\saphoComponents\Temp\proc_list.txt) do (

    cd %ROOT_DIR%\Teste\saphoComponents\bin

    CMMComp.exe %ROOT_DIR%\Teste\Projeto\%%b\Software\%%b.cmm %ROOT_DIR%\Teste\Projeto\%%b\Software\%%b.asm %MAC_DIR% %ROOT_DIR%\Teste\saphoComponents\Temp\%%b %%b

    ASMComp.exe %ROOT_DIR%\Teste\Projeto\%%b\Software\%%b.asm %ROOT_DIR%\Teste\Projeto\%%b\Hardware %ROOT_DIR%\Teste\saphoComponents\HDL %ROOT_DIR%\Teste\saphoComponents\Temp\%%b 100 2000
)

:: icarus ---------------------------------------------------------------------

cd %ROOT_DIR%\Teste\saphoComponents\HDL

iverilog -s %1_tb -o %ROOT_DIR%\Teste\saphoComponents\Temp\%1 %ROOT_DIR%\Teste\Projeto\%1_tb.v %ROOT_DIR%\Teste\Projeto\%1.v %ROOT_DIR%\Teste\Projeto\myFIFO.v %ROOT_DIR%\Teste\Projeto\maq_estados.v %ROOT_DIR%\Teste\Projeto\ProcZeroCross.v %ROOT_DIR%\Teste\Projeto\ProcDTWv4.v %ROOT_DIR%\Teste\Projeto\ProcDTW\Hardware\ProcDTW.v %ROOT_DIR%\Teste\Projeto\ZeroCross\Hardware\ZeroCross.v int2float.v proc_fl.v float2int.v addr_dec.v core_fl.v mem_instr.v prefetch.v instr_dec.v stack_pointer.v ula.v float2index.v stack.v rel_addr.v ula_fl.v proc_fx.v core_fx.v ula_fx.v mem_data.v pc.v

cd %ROOT_DIR%\Teste\saphoComponents\Temp

cp %ROOT_DIR%\Teste\Projeto\ProcDTW\Hardware\ProcDTW_data.mif .\
cp %ROOT_DIR%\Teste\Projeto\ProcDTW\Hardware\ProcDTW_inst.mif .\
cp %ROOT_DIR%\Teste\Projeto\ZeroCross\Hardware\ZeroCross_data.mif .\
cp %ROOT_DIR%\Teste\Projeto\ZeroCross\Hardware\ZeroCross_inst.mif .\

vvp %1 -n

cd %ROOT_DIR%