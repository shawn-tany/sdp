CFLAGS += -Wall -Werror
CFLAGS += -g
CFLAGS += -O0

compile_check :
	@if [ ! -d "$(BUILD_DIR)" ]; then mkdir $(BUILD_DIR); fi
	@if [ ! -d "$(BUILD_DIR)/obj" ]; then mkdir $(BUILD_DIR)/obj; fi
	@if [ ! -d "$(BUILD_DIR)/bin" ]; then mkdir $(BUILD_DIR)/bin; fi

install_check :
	@echo "install check"

compile :
	@echo "  ## build $(APP)"
	@for src in $(SRCS);            \
	do                              \
	    ($(CC) $(CFLAGS) $(LIBS) $$src -c) && (echo "  [SUCCESS] $$src") || ((echo "  [FAILED ] $$src" && (rm *.o -rf)) && (exit 1)); \
	done;
	@mv *.o $(BUILD_DIR)/obj
	@echo "  link to $(APP)"
	@$(CC) $(BUILD_DIR)/obj/*.o -o $(BUILD_DIR)/bin/$(APP) $(CFLAGS) $(LIBS)
	@rm -rf $(BUILD_DIR)/obj/*
	@echo "  ## build $(APP) success!"

clean_build :
	@echo "  ## clean $(APP)..."
	@rm -rf $(BUILD_DIR)
	@echo "  ## clean $(APP) success!"
