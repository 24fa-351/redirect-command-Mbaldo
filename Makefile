CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = streams

all: $(TARGET)

$(TARGET): streams.o
	$(CC) $(CFLAGS) -o $(TARGET) streams.o

streams.o: streams.c
	$(CC) $(CFLAGS) -c streams.c

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean