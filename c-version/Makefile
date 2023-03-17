CC := gcc

PROJ = tx

.PHONY: all clean

all: $(PROJ)

$(PROJ): $(PROJ).c
	$(CC) -g -O0 $< -o $@

clean:
	-rm -f $(PROJ).o $(PROJ) 
