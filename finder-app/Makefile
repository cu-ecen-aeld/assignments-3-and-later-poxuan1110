CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -Werror -g
TARGET = writer

all: $(TARGET)

$(TARGET): writer.c
	$(CC) $(CFLAGS) -o $@ $^


clean:
	rm -f $(TARGET) *.o
