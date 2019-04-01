NAME=ex19

.PHONY: all clean ${NAME} test

all: ${NAME}

${NAME}: object.o
	${CC} ${CFLAGS} -o ${NAME} ${NAME}.c $^

test:
	python test.py

clean:
	find . -executable -type f | xargs rm
