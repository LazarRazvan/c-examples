CC	= gcc
CFLAGS	= -Wall -Werror -DMULTI_THREADING
SFLAGS	= -fPIC
LDFLAGS = -shared
TARGET = libring_buffer.so

all: $(TARGET)

libring_buffer.so: buffer.o
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

buffer.o: buffer.c
	$(CC) $(CFLAGS) $(SFLAGS) -c $<

clean:
	rm $(TARGET) *.o
