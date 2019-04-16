NAME=ex29
CFLAGS=-g -O2 -Wall -Wextra -DNDEBUG $(OPTFLAGS)

SO_TARGET=build/lib$(NAME).so
TARGET=build/$(NAME)

SO_OBJS=$(patsubst %.c, %.o, $(wildcard lib*.c))
OBJS=$(patsubst %.c, %.o, $(wildcard ex*.c))

all: $(SO_TARGET) $(TARGET)

$(SO_TARGET): OPTFLAGS+=-fPIC
$(SO_TARGET): build $(SO_OBJS)
	$(CC) -shared -o $@ $(SO_OBJS)

$(TARGET): build $(OBJS)
	$(CC) -o $@ $(OBJS) -ldl

build:
	@mkdir -p build
	@mkdir -p bin

clean:
	rm -rf build $(OBJECTS) $(TESTS)
	rm -f tests/tests.log
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`
