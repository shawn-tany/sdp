CFLAGS += -Wall -Werror
CFLAGS += -g
CFLAGS += -O0

compile_check :
	@if [ ! -d "$(BUILD_DIR)" ]; then mkdir $(BUILD_DIR); fi
	@if [ ! -d "$(BUILD_DIR)/obj" ]; then mkdir $(BUILD_DIR)/obj; fi
	@if [ ! -d "$(BUILD_DIR)/bin" ]; then mkdir $(BUILD_DIR)/bin; fi

build_lib_check :
	@if [ ! -d "$(CREATE_LIB_DIR)" ]; then mkdir $(CREATE_LIB_DIR); fi

install_check :
	@echo "install check"

OBJALL := $(patsubst %.c, %.o, $(SRCS))
OBJOUT := $(patsubst %.c, %.o, $(filter-out $(OUTSRCS), $(SRCS)))

compile : $(OBJALL)
	@echo "  link to $(APP)"
	@$(CC) $(OBJALL) -o $(BUILD_DIR)/bin/$(APP) $(CFLAGS) $(LINK_LIBS)
	@echo "  ## build $(APP) success!"

build_lib : $(OBJALL)
	@echo "  ## build lib $(CREATE_LIB)..."
	@$(CC) -fPIC -shared $(OBJOUT) -o $(CREATE_LIB_DIR)/$(CREATE_LIB).so
	@$(AR) rcs $(CREATE_LIB_DIR)/$(CREATE_LIB).a $(OBJOUT)

%.o: %.c
	@($(CC) $(CFLAGS) -c -fPIC -o $@ $<) && ((echo "  [SUCCESS] $<") || ((echo "  [FAILED ] $<" && (rm *.o -rf)) && (exit 1)))
 
clean_build :
	@echo "  ## clean $(APP)..."
	@rm -rf $(BUILD_DIR) $(OBJALL)
	@echo "  ## clean $(APP) success!"
