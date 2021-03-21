include CONFIG.cfg

CC = gcc
LD = gcc
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
	@for n in $^; \
	do \
  		if echo 2 | cmp -s $$n; then \
 	  		exit 2; \
  	  	fi; \
	done
	
$(LOG): $(TEST_DIR)/%.log: $(TEST_DIR)/%.in $(TEST_DIR)/%.out $(EXEC)
    @if $(EXEC) $< | cmp -s $(TEST_DIR)/$*.out $@; then \
		echo Test $* - was successful; \
        echo 1 > $@; \
	else \
		echo Test $* - was failed; \
		echo 2 > $@; \
	fi

clean:
	$(RM) $(OBJ) $(LOG) $(EXEC) 