NAME=ex29
CFLAGS=-g -O2 -Wall -Wextra -DNDEBUG $(OPTFLAGS)

SO_TARGET=build/lib$(NAME).so
TARGET=build/$(NAME)
TEST_TARGET=build/$(NAME)_test

SO_OBJS=$(patsubst %.c, %.o, $(wildcard src/lib*.c))
OBJS=$(patsubst %.c, %.o, $(wildcard src/ex*.c))
TEST_OBJS=$(patsubst %.c, %.o, $(wildcard tests/*_tests.c))

all: $(SO_TARGET) $(TARGET) $(TEST_TARGET)

$(SO_TARGET): OPTFLAGS+=-fPIC
$(SO_TARGET): build $(SO_OBJS)
	$(CC) -shared -o $@ $(SO_OBJS)

$(TARGET): build $(OBJS)
	$(CC) -o $@ $(OBJS) -ldl

$(TEST_TARGET): OPTFLAGS+=-fPIC -Isrc
$(TEST_TARGET): build $(TEST_OBJS)
	$(CC) -o $@ $(TEST_OBJS) -ldl

build:
	@mkdir -p build
	@mkdir -p bin

clean:
	rm -rf build $(OBJECTS) $(TESTS)
	rm -f tests/tests.log
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`
