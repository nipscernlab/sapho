# Nome do executável final
TARGET = mycmmcomp

# Diretório onde estão os arquivos fonte
SRC_DIR = ./CMMComp

# Arquivos fonte principais
SRCS = data_assign.c data_declar.c data_use.c diretivas.c funcoes.c \
       labels.c oper.c saltos.c stdlib.c t2t.c variaveis.c

# Adiciona o caminho do diretório aos arquivos fonte
SRC_FILES = $(addprefix $(SRC_DIR)/, $(SRCS))

# Arquivos do Bison e Flex
BISON_SRC = $(SRC_DIR)/c2asm.y
FLEX_SRC = $(SRC_DIR)/c2asm.l

# Arquivos gerados
BISON_OUT = $(SRC_DIR)/c2asm.tab.c
BISON_HEADER = $(SRC_DIR)/c2asm.tab.h
FLEX_OUT = $(SRC_DIR)/lex.yy.c

# Arquivo de entrada e saída
INPUT_FILE = test.cmm
OUTPUT_FILE = test.asm

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Regras padrão
all: $(TARGET)

# Regra para gerar o executável final
$(TARGET): $(SRC_FILES) $(BISON_OUT) $(FLEX_OUT)
	$(CC) $(CFLAGS) -o $@ $^

# Regra para gerar o parser com o Bison
$(BISON_OUT) $(BISON_HEADER): $(BISON_SRC)
	bison -d -o $(BISON_OUT) $(BISON_SRC)

# Regra para gerar o scanner com o Flex
$(FLEX_OUT): $(FLEX_SRC) $(BISON_HEADER)
	flex -o $(FLEX_OUT) $(FLEX_SRC)

# Regra para rodar o compilador com o arquivo de entrada e gerar o arquivo de saída
run: $(TARGET)
	@echo "Executando compilador: ./$(TARGET) $(INPUT_FILE) > $(OUTPUT_FILE)"
	./$(TARGET) $(INPUT_FILE) > $(OUTPUT_FILE)


# Limpeza dos arquivos gerados
clean:
	rm -f $(TARGET) $(BISON_OUT) $(BISON_HEADER) $(FLEX_OUT) *.o

# Limpeza completa
distclean: clean
	rm -f *~
