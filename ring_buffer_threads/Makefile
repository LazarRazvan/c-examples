CC	= gcc
CFLAGS	= -Wall -Werror
# Comment if you don't want multi-threading
MULTI	= -DMULTI_THREADING
# Comment if you don't want to print information
PRINT	= -DPRINT
SFLAGS	= -fPIC
LDFLAGS = -shared
LINK	= -lring_buffer -lpthread -L.

TARGET = libring_buffer.so threads

all: $(TARGET)

libring_buffer.so: buffer.o
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

buffer.o: buffer.c
	$(CC) $(CFLAGS) $(MULTI) $(SFLAGS) -c $<

threads: threads.c
	$(CC) $(CFLAGS) $(PRINT) $< -o $@ $(LINK)
clean:
	rm $(TARGET) *.o
