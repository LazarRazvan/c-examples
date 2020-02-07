CC	= gcc
CFLAGS	= -Wall -Werror
SFLAGS	= -fPIC
LDFLAGS = -shared
LINK	= -lnamed_pipes -L.

TARGET = libnamed_pipes.so server client

all: $(TARGET)

libnamed_pipes.so: named_pipes_api.o
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

named_pipes_api.o: named_pipes_api.c
	$(CC) $(CFLAGS) $(SFLAGS) -c $<

server: named_pipes_server.c
	$(CC) $(CFLAGS) $< -o $@ $(LINK)

client: named_pipes_client.c
	$(CC) $(CFLAGS) $< -o $@ $(LINK)

clean:
	rm $(TARGET) *.o
