export CC=clang
export CFLAGS=-Wall -g -fsanitize=address -O1 -fno-omit-frame-pointer -fno-optimize-sibling-calls

EXERCISE_DIR_PREFIX=exercise_
LATEST_EXERCISE=20
EXERCISE_TARGETS=$(shell seq 1 $(LATEST_EXERCISE))
CREATE_EXERCISE_TARGETS=$(addprefix c, $(EXERCISE_TARGETS))
EXEC_EXERCISE_TARGETS=$(addprefix e, $(EXERCISE_TARGETS))

.PHONY: all clean $(EXERCISE_TARGETS) $(CREATE_EXERCISE_TARGETS) $(EXEC_EXERCISE_TARGETS)
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

EXEC_TARGET=$(patsubst e%,%,$@)
EXEC_FILE=$(EXERCISE_DIR_PREFIX)$(shell printf %03d/ex%d $(EXEC_TARGET) $(EXEC_TARGET))
$(EXEC_EXERCISE_TARGETS):
	$(info exec targets $@)
	@echo ========== exec result ==========
	@$(EXEC_FILE)

clean:
	find . -executable -type f | xargs rm
	find . -name *.o | xargs rm
