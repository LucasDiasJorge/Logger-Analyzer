# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Source files
SRC = src/main.c src/file_scanner.c src/decision_node.c src/error_map.c

# Object files
OBJ = $(SRC:.c=.o)

# Executable name
TARGET = main

# Default rule
all: $(TARGET)

# Rule to build the executable from object files
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

# Rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean up the object files and executable
clean:
	rm -f $(OBJ) $(TARGET)

# Rule to clean and recompile
recompile: clean all
