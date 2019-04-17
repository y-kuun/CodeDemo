LCTHW_LIB_NAME=lcthw
CC=clang
CFLAGS=-g -O2 -Wall -Wextra -fsanitize=address -O1 -fno-omit-frame-pointer -fno-optimize-sibling-calls $(OPTFLAGS)

SO_LCTHW=build/lib${LCTHW_LIB_NAME}.so
STATIC_LCTHW=build/lib$(LCTHW_LIB_NAME).a
TEST_TARGET=build/$(LCTHW_LIB_NAME)_test

LCTHW_OBJS=$(patsubst %c, %o, $(wildcard src/lcthw/*.c))
TEST_OBJS=$(patsubst %.c, %.o, $(wildcard tests/*_tests.c))

all: $(SO_LCTHW) $(STATIC_LCTHW) $(TEST_TARGET)

$(SO_LCTHW): OPTFLAGS+=-fPIC -Isrc
$(SO_LCTHW): build $(LCTHW_OBJS)
	$(CC) -shared -o $@ $(LCTHW_OBJS)

$(STATIC_LCTHW): OPTFLAGS+=-fPIC -Isrc
$(STATIC_LCTHW): build $(LCTHW_OBJS)
	ar rcs -o $@ $(LCTHW_OBJS)

$(TEST_TARGET): OPTFLAGS+=-fPIC -Isrc -Lbuild -l:liblcthw.a
$(TEST_TARGET): build $(TEST_OBJS)
	$(CC) -o $@ $(TEST_OBJS) $(CFLAGS)

build:
	@mkdir -p build
	@mkdir -p bin

clean:
	rm -rf build $(OBJECTS) $(TESTS)
	rm -f tests/tests.log
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`
