CC = g++
CFLAGS = -Wall -I src/lib/json/include
LIBS = -lcurl -lmysqlcppconn
SRC_DIR = src
MODULES_DIR = src/modules
BIN_FILE = collector
BIN_DIR = bin

all: main

main: $(SRC_DIR)/main.cpp
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$(BIN_FILE) $(MODULES_DIR)/utils.cpp $(MODULES_DIR)/config.cpp $(MODULES_DIR)/curlModule.cpp $(MODULES_DIR)/fileSystem.cpp $^ $(LIBS)
clean:
	rm -f $(BIN_DIR)/*

.PHONY: all clean
