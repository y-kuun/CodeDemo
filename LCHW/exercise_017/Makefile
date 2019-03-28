NAME=ex17

.PHONY: all clean ${NAME}

all: ${NAME}

database:
	${CC} ${CFLAGS} -c database.c 

${NAME}: database
	${CC} ${CFLAGS} -o ${NAME} ${NAME}.c database.o
	rm database.o

clean:
	find . -executable -type f | xargs rm
