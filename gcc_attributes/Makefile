CC	= gcc
CFLAGS	= -Wall -Werror

TARGET = constructor

all: $(TARGET)

constructor: constructor.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGET)
