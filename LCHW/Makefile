CC=gcc
CFLAGS=-Wall -g

EXERCISE_DIR_PREFIX=exercise_
LATEST_EXERCISE=10
EXERCISE_TARGETS=$(shell seq 1 $(LATEST_EXERCISE))
CREATE_EXERCISE_TARGETS=$(addprefix c, $(EXERCISE_TARGETS))

.PHONY: all clean $(EXERCISE_TARGETS) $(CREATE_EXERCISE_TARGETS)
all: $(EXERCISE_TARGETS)

$(EXERCISE_TARGETS):
	$(info  $(EXERCISE_DIR_PREFIX)$(shell printf %03d $@))
	make -C $(EXERCISE_DIR_PREFIX)$(shell printf %03d $@)

CREATE_TARGET=$(patsubst c%,%,$@)
CREATE_DIR=$(EXERCISE_DIR_PREFIX)$(shell printf %03d $(CREATE_TARGET))
CREATE_C_FILE=./$(CREATE_DIR)/ex$(CREATE_TARGET).c
TMP_CREATE_CFILE=./templates/ex.c.temp
CREATE_MAKE_FILE=./$(CREATE_DIR)/Makefile
TMP_CREATE_MFILE=./templates/Makefile.temp
$(CREATE_EXERCISE_TARGETS):
	$(info creating targets $@)
	mkdir $(CREATE_DIR)
	cp $(TMP_CREATE_CFILE) $(CREATE_C_FILE)
	cp $(TMP_CREATE_MFILE) $(CREATE_MAKE_FILE) 
	sed -i s/{TARGET_NUM}/$(CREATE_TARGET)/g $(CREATE_MAKE_FILE)

clean:
	find . -executable -type f | xargs rm
