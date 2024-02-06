CC = g++
CFLAGS = -Wall -I src/lib/curl-8.6.0_1-win64-mingw/include
LIBS = -L src/lib/curl-8.6.0_1-win64-mingw/lib -lcurl
SRC_DIR = src
BIN_DIR = bin

all: main copy_dll

copy_dll:
	copy /Y src\lib\curl-8.6.0_1-win64-mingw\bin\libcurl-x64.dll $(BIN_DIR)
main: $(SRC_DIR)/main.cpp
	$(CC) $(CFLAGS) -o $(BIN_DIR)/main $^ $(LIBS)
clean:
	rm -f $(BIN_DIR)/*

.PHONY: all clean