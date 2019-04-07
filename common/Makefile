export CC=clang
export CFLAGS=-Wall -g -fsanitize=address -O0 -fno-omit-frame-pointer -fno-optimize-sibling-calls

.PHONY: all clean zlib utils
all: utils

ZLIB_DIR=build/zlib
ZHEADER_DIR=${ZLIB_DIR}/include
ZLIB_FLAGS=-L$(ZLIB_DIR) -I$(ZLIB_DIR) -I${ZHEADER_DIR} -lz
zlib: 
	if [ ! -d $(ZLIB_DIR) ]; then mkdir -p $(ZLIB_DIR); fi
	if [ ! -d $(ZHEADER_DIR) ]; then mkdir -p $(ZHEADER_DIR); fi
	cd $(ZLIB_DIR); cmake ../../zlib; make
	cp zlib/*.h ${ZHEADER_DIR}

UTILS_DIR=build/utils
#UTILS_FLAGS=-save-temps=$(UTILS_DIR)
utils: zlib
	if [ ! -d $(UTILS_DIR) ]; then mkdir -p $(UTILS_DIR); fi
	$(CC) $(CFLAGS) -o$(UTILS_DIR)/utils utils.c $(ZLIB_FLAGS) $(UTILS_FLAGS)

clean:
	find . -executable -type f | xargs rm
	find . -name *.o | xargs rm
