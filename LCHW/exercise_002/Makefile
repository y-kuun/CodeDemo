CC=gcc
CFLAGS=-Wall -g
NAME=ex2

.PHONY: all clean ${NAME}

all: ${NAME}

${NAME}:
	${CC} ${CFLAGS} -o ${NAME} ${NAME}.c

clean:
	ls *.exe | xargs rm
