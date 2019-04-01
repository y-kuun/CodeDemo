NAME=ex17

.PHONY: all clean ${NAME}

all: ${NAME}

database:
	${CC} ${CFLAGS} -c $@.c 

simple_stack:
	${CC} ${CFLAGS} -c $@.c

${NAME}: database simple_stack
	${CC} ${CFLAGS} -o ${NAME} ${NAME}.c $(patsubst %, %.o, $^)

clean:
	find . -name *.o | xargs rm
	find . -executable -type f | xargs rm
