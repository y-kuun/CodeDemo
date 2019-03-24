CC=gcc
CFLAGS=-Wall -g -fno-builtin

EXERCISE_DIR_PREFIX=exercise_
LATEST_EXERCISE=4
EXERCISE_TARGETS=$(shell seq 1 $(LATEST_EXERCISE))

.PHONY: all clean
all: $(EXERCISE_TARGETS)

$(EXERCISE_TARGETS):
	$(info  $(EXERCISE_DIR_PREFIX)$(shell printf %03d $@))
	make -C $(EXERCISE_DIR_PREFIX)$(shell printf %03d $@)

clean:
	find . -executable -type f | xargs rm
