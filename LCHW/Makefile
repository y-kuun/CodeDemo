CC=gcc
CFLAGS=-Wall -g -fno-builtin

EXERCISE_DIR_PREFIX=exercise_
EXERCISE_TARGETS=$(shell seq 1 3)

.PHONY: all clean
all: $(EXERCISE_TARGETS)

$(EXERCISE_TARGETS):
	$(info  $(EXERCISE_DIR_PREFIX)$(shell printf %03d $@))
	make -C $(EXERCISE_DIR_PREFIX)$(shell printf %03d $@)

ex2:
	make -C exercise_002/

clean:
	rm **/*.exe
