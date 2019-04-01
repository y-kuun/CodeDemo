NAME=ex22

.PHONY: all clean ${NAME}

all: ${NAME}

${NAME}:
	${CC} ${CFLAGS} -o ${NAME} ${NAME}.c ${NAME}_main.c

clean:
	find . -executable -type f | xargs rm
