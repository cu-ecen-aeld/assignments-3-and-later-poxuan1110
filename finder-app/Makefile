# Compiler and flags
CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -Werror -g

# Target binary
TARGET = writer

# Source files
SRC = writer.c
OBJ = $(SRC:.c=.o)

# Default target: build the writer application
all: $(TARGET)

# Compile the writer program
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Compile object files
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean target to remove binaries and object files
clean:
	rm -f $(TARGET) $(OBJ)
