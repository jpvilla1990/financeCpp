CC = g++
CFLAGS = -Wall -I src/lib/curl-8.6.0_1-win64-mingw/include -I src/lib/json/include
LIBS = -L src/lib/curl-8.6.0_1-win64-mingw/lib -lcurl
SRC_DIR = src
MODULES_DIR = src/modules
BIN_FILE = collector
BIN_DIR = bin

all: main copy_dll

copy_dll:
	copy /Y src\lib\curl-8.6.0_1-win64-mingw\bin\libcurl-x64.dll $(BIN_DIR)
main: $(SRC_DIR)/main.cpp
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$(BIN_FILE) $(MODULES_DIR)/utils.cpp $(MODULES_DIR)/config.cpp $(MODULES_DIR)/curlModule.cpp $^ $(LIBS)
clean:
	rm -f $(BIN_DIR)/*

.PHONY: all clean