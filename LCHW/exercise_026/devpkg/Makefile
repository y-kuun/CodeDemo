PREFIX?=/usr/local
CFLAGS=-g -Wall -I${PREFIX}/apr/include/apr-1
LDFLAGS=-L${PREFIX}/apr/lib -lapr-1 -pthread -laprutils-1

.PHONY: all
all: devpkg

devpkg: bstrlib.o db.o shell.o commands.o

install: all
	install -d $(DESTDIR)/${PREFIX}/bin/
	install devpkg $(DESTDIR)/${PREFIX}/bin/

clean:
	rm -f *.o
	rm -f devpkg
	rm -f *.dSYM
