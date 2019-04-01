NAME=ex20

override CFLAGS += -D NDEBUG
#export CFLAGS

.PHONY: all clean ${NAME}

all: ${NAME}

${NAME}:
	${CC} ${CFLAGS} -o ${NAME} ${NAME}.c

clean:
	find . -executable -type f | xargs rm
