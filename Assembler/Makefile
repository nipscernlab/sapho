# Nome do executável final a ser gerado pelo compilador.
TARGET = bin/myasmcomp.exe

# Diretórios onde estão localizados os arquivos fonte e cabeçalhos do projeto.
SRC_DIR = Sources
HEADER_DIR = Headers
BIN_DIR = bin
OUTPUT_DIR = Output

# Lista dos arquivos fonte principais do projeto.
SRCS = eval.c labels.c mnemonicos.c t2t.c variaveis.c veri_comp.c

# Adiciona o caminho do diretório $(SRC_DIR) antes de cada arquivo listado em $(SRCS).
SRC_FILES = $(addprefix $(SRC_DIR)/, $(SRCS))

# Arquivo fonte do Flex que define os padrões (tokens) para o analisador léxico.
FLEX_SRC = $(SRC_DIR)/asm2mif.l

# Arquivo gerado pelo Flex, que contém o código do analisador léxico (scanner).
FLEX_OUT = $(SRC_DIR)/lex.yy.c

# Arquivo de entrada que será processado pelo compilador.
INPUT_FILE = ../test.asm

# Arquivos de saída gerados pelo compilador.
OUTPUT_V_FILE = $(OUTPUT_DIR)/output.v
DATA_MIF_FILE = $(OUTPUT_DIR)/_data.mif
INST_MIF_FILE = $(OUTPUT_DIR)/_inst.mif

# Nome do compilador utilizado para compilar os arquivos fonte.
CC = gcc

# Flags de compilação.
CFLAGS = -Wall -Wextra -g -I$(HEADER_DIR)

# Regra padrão: será executada se nenhum alvo específico for passado.
# O alvo "all" agora depende apenas do executável $(TARGET).
all: $(TARGET)

# Regra para gerar o executável final $(TARGET).
$(TARGET): $(SRC_FILES) $(FLEX_OUT)
	@mkdir -p $(BIN_DIR) $(OUTPUT_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# Regra para gerar o scanner (analisador léxico) usando o Flex.
$(FLEX_OUT): $(FLEX_SRC)
	flex -o $(FLEX_OUT) $(FLEX_SRC)

# Regra para rodar o compilador com o arquivo de entrada e gerar os arquivos de saída.
run: $(TARGET)
	@echo "Executando: $(TARGET) $(INPUT_FILE)"
	./$(TARGET) $(INPUT_FILE) $(OUTPUT_V_FILE) $(DATA_MIF_FILE) $(INST_MIF_FILE)

# Regra para limpeza dos arquivos gerados.
clean:
	rm -f $(TARGET) $(FLEX_OUT) $(OUTPUT_V_FILE) $(DATA_MIF_FILE) $(INST_MIF_FILE) *.o

# Regra para limpeza completa.
distclean: clean
	rm -f *~
