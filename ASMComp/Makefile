# Nome do executável final a ser gerado pelo compilador.
TARGET = bin/asmcomp.exe

# Diretórios onde estão localizados os arquivos fonte e cabeçalhos do projeto.
SRC_DIR = Sources
HEADER_DIR = Headers
BIN_DIR = bin

# Diretório do projeto onde o arquivo de entrada e saída estarão localizados.
PROJECT_DIR = ../Project

# Lista dos arquivos fonte principais do projeto.
SRCS = eval.c labels.c mnemonicos.c t2t.c variaveis.c veri_comp.c

# Adiciona o caminho do diretório $(SRC_DIR) antes de cada arquivo listado em $(SRCS).
# Isso resulta em uma lista completa de arquivos com seus caminhos.
SRC_FILES = $(addprefix $(SRC_DIR)/, $(SRCS))

# Arquivo fonte do Flex que define os padrões (tokens) para o analisador léxico.
FLEX_SRC = $(SRC_DIR)/asm2mif.l

# Arquivo gerado pelo Flex, que contém o código do analisador léxico (scanner).
FLEX_OUT = $(SRC_DIR)/lex.yy.c

# Arquivo de entrada que será processado pelo compilador.
INPUT_FILE = $(PROJECT_DIR)/test.asm

# Nome do compilador utilizado para compilar os arquivos fonte.
CC = gcc

# Flags de compilação:
# -Wall: Ativa todos os avisos comuns.
# -Wextra: Ativa avisos adicionais.
# -g: Inclui informações de depuração no binário gerado.
# CFLAGS = -Wall -Wextra -g -I$(HEADER_DIR)

# Regra para criar o diretório bin caso não exista.
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Regra padrão: será executada se nenhum alvo específico for passado.
# O alvo "all" agora depende apenas do executável $(TARGET).
all: $(TARGET)

# Regra para gerar o executável final $(TARGET).
$(TARGET): $(SRC_FILES) $(FLEX_OUT)
	@mkdir -p $(BIN_DIR) $(PROJECT_DIR)  
	$(CC) -I$(HEADER_DIR) -o $@ $^

# Regra para gerar o scanner (analisador léxico) usando o Flex.
$(FLEX_OUT): $(FLEX_SRC)
	flex $(FLEX_SRC)
	mv lex.yy.c $(SRC_DIR)/lex.yy.c

# Regra para rodar o compilador com o arquivo de entrada e gerar os arquivos de saída.
run: $(TARGET)
	@echo Executando: $(TARGET) $(INPUT_FILE)
	@cd $(PROJECT_DIR) && ../ASMComp/$(TARGET) $(INPUT_FILE)

# Regra para limpeza dos arquivos gerados.
clean:
	rm -f $(TARGET) $(FLEX_OUT)
	rm -f *.o

# Regra para limpeza completa.
distclean: clean
	rm -f *~