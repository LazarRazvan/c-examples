CC	= gcc
CFLAGS	= -Wall -Werror

TARGET = pipes

all: $(TARGET)

pipes: anonymous_pipes.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(TARGET)
