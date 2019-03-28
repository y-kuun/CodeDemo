NAME=ex17

.PHONY: all clean ${NAME}

all: ${NAME}

database:
	${CC} ${CFLAGS} -c database.c 

${NAME}: database
	${CC} ${CFLAGS} -o ${NAME} ${NAME}.c $(patsubst %, %.o, $^)

clean:
	find . -name *.o | xargs rm
	find . -executable -type f | xargs rm
