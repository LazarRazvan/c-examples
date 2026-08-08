CC	= gcc
CFLAGS	= -Wall -Werror

TARGET = constructor fn_table ifunc

all: $(TARGET)

constructor: constructor.c
	$(CC) $(CFLAGS) $< -o $@

fn_table: fn_table.c fn_table_main.c fn_table_test1.c fn_table_test2.c
	$(CC) $(CFLAGS) $^ -o $@

ifunc: ifunc.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGET)
