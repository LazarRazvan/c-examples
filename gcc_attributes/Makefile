CC	= gcc
CFLAGS	= -Wall -Werror

TARGET = constructor fn_table ifunc ifunc_vs_fptr

all: $(TARGET)

constructor: constructor.c
	$(CC) $(CFLAGS) $< -o $@

fn_table: fn_table.c fn_table_main.c fn_table_test1.c fn_table_test2.c
	$(CC) $(CFLAGS) $^ -o $@

ifunc: ifunc.c
	$(CC) $(CFLAGS) $< -o $@

# -O2: call-mechanism overhead is meaningless at -O0
# -fno-unroll-loops: keeps the three benchmark loops structurally comparable
ifunc_vs_fptr: ifunc_vs_fptr.c
	$(CC) $(CFLAGS) -O2 -fno-unroll-loops $< -o $@

clean:
	rm -f $(TARGET)
