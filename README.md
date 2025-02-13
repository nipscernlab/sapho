# SAPHO Compilers  

<p align="center">
  <img src="https://github.com/nipscernlab/nipscernweb/blob/main/assets/icons/icon_home_psychology.svg" alt="SAPHO Compilers" width="200">
</p>

## Sobre o Projeto  
O **SAPHO Compilers** é um projeto que contém o código-fonte de dois compiladores desenvolvidos pela **NIPSCERN Lab** na **Universidade Federal de Juiz de Fora (UFJF), Brasil**. Este projeto utiliza **Flex** e **Bison**, com suporte para **Cygwin64**.  

### Compiladores:  
1. **CMMComp**: Traduz código em **CMM** (uma linguagem criada pela NIPSCERN Lab) para **código ASM** com instruções específicas.  
2. **ASMComp**: Compila o código **ASM** gerado pelo CMMComp para um arquivo **.v** (Verilog) e gera um **.mif** (Memory Initialization File).  

## Requisitos  
- **Cygwin64** com Flex e Bison instalados.  
- As pastas **Hardware**, **Software** e **Simulation** devem existir na raiz do projeto.  

## Instalação  
```bash
git clone https://github.com/nipscernlab/sapho-p.git
cd sapho-p
