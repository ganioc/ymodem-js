CC := gcc

.PHONY: all clean

all: tx

tx: tx.c
	$(CC) -g -O0 $< -o $@

clean:
	-rm -f tx.o tx 
