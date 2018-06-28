CC=g++
OBJ := main.o utils.o http.o
LIB :=-ljson_linux-gcc-4.8.5_libmt -lcurl

TOP_DIR := $(PWD)
INC_DIR := $(TOP_DIR)/include
LIB_DIR := $(TOP_DIR)/lib
OBJ_DIR := $(TOP_DIR)/obj
BIN_DIR := $(TOP_DIR)/bin
BIN:= out
SUB_DIR := main \
		utils \
		http \
 		obj
export CC OBJ TOP_DIR OBJ_DIR BIN_DIR BIN  INC_DIR LIB_DIR LIB utils http 
all:CHECKDIR $(SUB_DIR)
CHECKDIR:
	mkdir -p $(SUB_DIR) $(BIN_DIR)
$(SUB_DIR):ECHO
	make -C $@
ECHO:
	@echo $(SUB_DIR)
	@echo begin compile
clean:
	rm -rf $(OBJ_DIR)/*o $(BIN_DIR)


