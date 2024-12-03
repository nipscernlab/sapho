# Nome do executável final a ser gerado pelo compilador.
TARGET = bin/mycmmcomp.exe

# Diretórios onde estão localizados os arquivos fonte e cabeçalhos do projeto.
SRC_DIR = Sources
HEADER_DIR = Headers
BIN_DIR = bin

# Diretório do projeto onde o arquivo de entrada e saída estarão localizados.
PROJECT_DIR = ../Project

# Lista dos arquivos fonte principais do projeto. 
# Cada arquivo contém uma parte do código responsável por diferentes funcionalidades.
SRCS = data_assign.c data_declar.c data_use.c diretivas.c funcoes.c \
       labels.c oper.c saltos.c stdlib.c t2t.c variaveis.c

# Adiciona o caminho do diretório $(SRC_DIR) antes de cada arquivo listado em $(SRCS). 
# Isso resulta em uma lista completa de arquivos com seus caminhos.
SRC_FILES = $(addprefix $(SRC_DIR)/, $(SRCS))

# Arquivo fonte do Bison que define a gramática (regras) do compilador.
BISON_SRC = $(SRC_DIR)/c2asm.y

# Arquivo fonte do Flex que define os padrões (tokens) para o analisador léxico.
FLEX_SRC = $(SRC_DIR)/c2asm.l

# Arquivo gerado pelo Bison que contém o código do analisador sintático (parser).
BISON_OUT = $(SRC_DIR)/y.tab.c

# Arquivo de cabeçalho gerado pelo Bison, que inclui definições de símbolos usados no parser.
BISON_HEADER = $(HEADER_DIR)/y.tab.h

# Arquivo gerado pelo Flex, que contém o código do analisador léxico (scanner).
FLEX_OUT = $(SRC_DIR)/lex.yy.c

# Arquivo de entrada que será processado pelo compilador (modificado para refletir o caminho correto).
INPUT_FILE = $(PROJECT_DIR)/test.cmm

# Arquivo de saída que será gerado pelo compilador com o código assembly (modificado para refletir o caminho correto).
OUTPUT_FILE = $(PROJECT_DIR)/test.asm

# Nome do compilador utilizado para compilar os arquivos fonte.
CC = gcc

# Flags de compilação:
# -Wall: Ativa todos os avisos comuns.
# -Wextra: Ativa avisos adicionais.
# -g: Inclui informações de depuração no binário gerado.
CFLAGS = -Wall -Wextra -g -I$(HEADER_DIR)

# Regra padrão: será executada se nenhum alvo específico for passado.
# O alvo "all" depende do executável $(TARGET), que será gerado se necessário.
all: $(TARGET)

# Regra para gerar o executável final $(TARGET).
# Depende dos arquivos fonte $(SRC_FILES) e dos arquivos gerados pelo Bison e Flex.
$(TARGET): $(SRC_FILES) $(BISON_OUT) $(FLEX_OUT)
	$(CC) $(CFLAGS) -o $@ $^

# Regra para gerar o parser (analisador sintático) usando o Bison.
# Os arquivos de saída $(BISON_OUT) e $(BISON_HEADER) dependem do arquivo fonte $(BISON_SRC).
$(BISON_OUT) $(BISON_HEADER): $(BISON_SRC)
	bison -d -o $(BISON_OUT) $(BISON_SRC)
	mv $(SRC_DIR)/y.tab.h $(HEADER_DIR)/y.tab.h

# Regra para gerar o scanner (analisador léxico) usando o Flex.
# O arquivo de saída $(FLEX_OUT) depende do arquivo fonte $(FLEX_SRC) e do cabeçalho $(BISON_HEADER).
$(FLEX_OUT): $(FLEX_SRC) $(BISON_HEADER)
	flex -o $(FLEX_OUT) $(FLEX_SRC)

# Regra para rodar o compilador com o arquivo de entrada e gerar o arquivo de saída.
run: $(TARGET)
	@echo Executando: $(TARGET) $(INPUT_FILE) $(OUTPUT_FILE)
	./$(TARGET) $(INPUT_FILE) $(OUTPUT_FILE)

# Regra para limpeza dos arquivos gerados.
# Remove o executável, os arquivos gerados pelo Bison e Flex e qualquer arquivo objeto (*.o).
clean:
	rm -f $(TARGET) $(BISON_OUT) $(BISON_HEADER) $(FLEX_OUT) $(OUTPUT_FILE) *.o

# Regra para limpeza completa.
# Além dos arquivos removidos por "clean", remove também arquivos temporários como *~.
distclean: clean
	rm -f *~
