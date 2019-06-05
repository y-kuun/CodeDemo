LCTHW_LIB_NAME=lcthw
CC=clang
export CFLAGS=-g -O0 -Wall -Wextra -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls $(OPTFLAGS) $(DEBUG)

SO_LCTHW=build/lib${LCTHW_LIB_NAME}.so
STATIC_LCTHW=build/lib$(LCTHW_LIB_NAME).a

LCTHW_OBJS=$(patsubst %c, %o, $(wildcard src/lcthw/*.c))
TEST_OBJS=$(patsubst %.c, %.o, $(wildcard tests/*_tests.c))
TEST_TARGET=$(patsubst tests/%.c, %, $(wildcard tests/*_tests.c))
PROGRAMS_OBJS=$(patsubst %.c, %.o, $(wildcard bin/*.c))
PROGRAMS=$(patsubst bin/%.c, %, $(wildcard bin/*.c))

all: $(SO_LCTHW) $(STATIC_LCTHW) $(TEST_TARGET) $(PROGRAMS)

$(SO_LCTHW): OPTFLAGS+=-fPIC -Isrc
$(SO_LCTHW): build $(LCTHW_OBJS)
	$(CC) -shared -o $@ $(LCTHW_OBJS) $(CFLAGS)

$(STATIC_LCTHW): OPTFLAGS+=-fPIC -Isrc
$(STATIC_LCTHW): build $(LCTHW_OBJS)
	ar rcs -o $@ $(LCTHW_OBJS)

# each xxx_tests.c will generat an execution file
$(TEST_TARGET): OPTFLAGS+=-fPIC -Isrc -Lbuild -l:liblcthw.a
$(TEST_TARGET): $(TEST_OBJS)
	$(CC) -o build/$@ tests/$@.o $(CFLAGS)

$(PROGRAMS): OPTFLAGS+=-fPIC -Isrc -Lbuild -l:liblcthw.a
$(PROGRAMS): $(PROGRAMS_OBJS)
	$(CC) -o bin/$@ bin/$@.o $(CFLAGS)
build:
	@mkdir -p build
	@mkdir -p bin

clean:
	rm -rf build $(OBJECTS) $(TESTS) $(PROGRAMS)
	rm -f tests/tests.log
	find . -name "*.gc*" -exec rm {} \;
	find . -name "*.o" -exec rm {} \;
	find . -name "*.dSYM" -exec rm {} \;;
	find . -regex ".*~" -exec rm {} \;
	@echo "Please run `git clean -X` to remove fellowing files"
	git clean -X -n
