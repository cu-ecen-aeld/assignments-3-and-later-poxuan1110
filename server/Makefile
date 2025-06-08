# server/Makefile

TARGET = aesdsocket
SRCS   = aesdsocket.c

CC     ?= $(CROSS_COMPILE)gcc
CFLAGS ?= -Wall -Werror -pthread

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)
