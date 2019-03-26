CC=gcc
CFLAGS=-Wall -g
NAME=ex10

.PHONY: all clean ${NAME}

all: ${NAME}

${NAME}:
	${CC} ${CFLAGS} -o ${NAME} ${NAME}.c

clean:
	find . -executable -type f | xargs rm
