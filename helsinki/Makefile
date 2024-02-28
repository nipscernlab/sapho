CC = gcc
CFLAGS = -Wall -g
INCLUDES = -IHeaders

# Diretórios
SRC_DIR = Source
OBJ_DIR = obj/Debug
BIN_DIR = bin/Debug

# Arquivos fonte
SOURCES = labels.c lex.yy.c t2t.c variaveis.c y.tab.c

# Lista de objetos
OBJECTS = $(patsubst %.c,${OBJ_DIR}/%.o,${SOURCES})

# Nome do executável
EXECUTABLE = ${BIN_DIR}/myccomp.exe

# Arquivo de entrada e saída
INPUT_FILE = ${SRC_DIR}/teste.c
OUTPUT_FILE = ${SRC_DIR}/teste.asm

all: ${EXECUTABLE}
	@${EXECUTABLE} ${INPUT_FILE} ${OUTPUT_FILE}

${EXECUTABLE}: ${OBJECTS}
	@mkdir -p ${BIN_DIR}
	${CC} ${CFLAGS} -o ${EXECUTABLE} ${OBJECTS}

${OBJ_DIR}/%.o: ${SRC_DIR}/%.c
	@mkdir -p $(@D)
	${CC} ${CFLAGS} ${INCLUDES} -c $< -o $@
