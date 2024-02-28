CC = gcc
CFLAGS = -Wall -g
INCLUDES = -IHeaders

# Diretórios
SRC_DIR = Source
OBJ_DIR = obj/Debug
BIN_DIR = bin/Debug

# Arquivos fonte
SOURCES = asm2mif.c eval.c labels.c mnemonicos.c t2t.c variaveis.c veri_comp.c
# Lista de objetos
OBJECTS = $(patsubst %.c,${OBJ_DIR}/%.o,${SOURCES})

# Nome do executável
EXECUTABLE = ${BIN_DIR}/asm2mif.exe

# Arquivos de saída
OUTPUT_V_FILE = ${BIN_DIR}/output.v
DATA_MIF_FILE = ${BIN_DIR}/_data.mif
INST_MIF_FILE = ${BIN_DIR}/_inst.mif

# Arquivo de entrada
INPUT_FILE = ${SRC_DIR}/teste.asm

all: ${EXECUTABLE} ${OUTPUT_V_FILE} ${DATA_MIF_FILE} ${INST_MIF_FILE}
	@echo "Executando ${EXECUTABLE} com ${INPUT_FILE}"

${EXECUTABLE}: ${OBJECTS}
	@mkdir -p ${BIN_DIR}
	${CC} ${CFLAGS} -o ${EXECUTABLE} ${OBJECTS}

${OUTPUT_V_FILE} ${DATA_MIF_FILE} ${INST_MIF_FILE}: ${EXECUTABLE} ${INPUT_FILE}
	@echo "Gerando arquivos de saida..."
	@echo | ${EXECUTABLE} ${INPUT_FILE}
	@echo "Concluido."

${OBJ_DIR}/%.o: ${SRC_DIR}/%.c
	@mkdir -p $(@D)
	${CC} ${CFLAGS} ${INCLUDES} -c $< -o $@
