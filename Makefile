include CONFIG.cfg

CC = gcc
LD = gcc
PASSED = ok
FAILED= fail
EXEC = $(BUILD_DIR)/$(NAME)
OBJ = $(BUILD_DIR)/sorter.o  
LOG = $(patsubst $(TEST_DIR)/%.in, $(TEST_DIR)/%.log, $(wildcard $(TEST_DIR)/*.in))

.PHONY: all check clean

all: $(EXEC)

$(EXEC) : $(OBJ)
	$(LD) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c | $(BUILD_DIR)
	$(CC) -c $< -o $@

$(BUILD_DIR):	
	@mkdir -p $@ 

check: $(LOG)
	@for test in $^ ; \
	do \
  		if [ "$$(cat $${test})" != "$(PASSED)" ] ; then \
 	  		exit 1; \
  	  	fi; \
	done
	
$(TEST_DIR)/%.log: $(TEST_DIR)/%.in $(TEST_DIR)/%.out $(EXEC)
    @if [ "$$(./$(EXEC) ./$<)" = "$$(cat $(word 2, $^))" ] ; then \
		echo Test $* - was successful; \
        echo "$(PASSED)" > $@ ; \
	else \
		echo Test $* - was failed; \
		echo "$(FAILED)" > $@ ;
	fi

clean:
	$(RM) $(OBJ) $(LOG) $(EXEC) 