# Makefile

# Compiler
CC = gcc

# Source files
SRC = src/main.c src/file_scanner.c src/action.c

# Executable name
TARGET = executable

# Default rule
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET)

# Rule to clean up the executable
clear:
	rm -f $(TARGET)

# Rule to clean and recompile
recompile: clear all
