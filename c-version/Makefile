
CC     := gcc
LD     := gcc
CFLAGS := -g -Wall

INCLUDES := -I./inc

VPATH    := ./src/ \
	./inc/ \
	.

BUILD_DIR := output

.PHONE: clean

all: checkdirs tx

checkdirs: $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $@

tx: tx.o crc16.o  lib.o  packet.o  serial.o  ymodem.o
	$(CC) $(filter %.o,$^) -o $(BUILD_DIR)/$@

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

clean:
	rm -f *.o
	rm $(BUILD_DIR)/tx
