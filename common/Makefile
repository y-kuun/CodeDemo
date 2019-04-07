export CC=clang
export CFLAGS=-Wall -g -fsanitize=address -O0 -fno-omit-frame-pointer -fno-optimize-sibling-calls

.PHONY: all clean zlib utils lz4
all: utils

ZLIB_DIR=build/zlib
ZHEADER_DIR=${ZLIB_DIR}/include
ZLIB_FLAGS=-L$(ZLIB_DIR) -I$(ZLIB_DIR) -I${ZHEADER_DIR} -lz
zlib: 
	if [ ! -d $(ZLIB_DIR) ]; then mkdir -p $(ZLIB_DIR); fi
	if [ ! -d $(ZHEADER_DIR) ]; then mkdir -p $(ZHEADER_DIR); fi
	cd $(ZLIB_DIR); cmake ../../zlib; make
	cp zlib/*.h ${ZHEADER_DIR}

LZ4_DIR=build/lz4
LZ4HEADER_DIR=${LZ4_DIR}/include
LZ4LIB_FLAGS=-L$(LZ4_DIR) -I${LZ4HEADER_DIR} -llz4
lz4:
	if [ ! -d $(LZ4HEADER_DIR) ]; then mkdir -p $(LZ4HEADER_DIR); fi
	if [ ! -d $(LZ4_DIR) ]; then mkdir -p $(LZ4_DIR); fi
	make -C lz4
	cp lz4/lib/*.h $(LZ4HEADER_DIR)
	cp lz4/lib/*.a $(LZ4_DIR)

UTILS_DIR=build/utils
#UTILS_FLAGS=-save-temps=$(UTILS_DIR)
utils: zlib lz4
	if [ ! -d $(UTILS_DIR) ]; then mkdir -p $(UTILS_DIR); fi
	$(CC) $(CFLAGS) -o$(UTILS_DIR)/utils utils.c $(ZLIB_FLAGS) $(UTILS_FLAGS) $(LZ4LIB_FLAGS)


clean:
	find . -executable -type f | xargs rm
	find . -name *.o | xargs rm
