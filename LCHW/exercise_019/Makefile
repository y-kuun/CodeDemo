NAME=ex19

.PHONY: all clean ${NAME}

all: ${NAME}

${NAME}: object.o
	${CC} ${CFLAGS} -o ${NAME} ${NAME}.c $^

clean:
	find . -executable -type f | xargs rm
