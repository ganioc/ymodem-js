# YMODEM sender in JS
## test

```js
npm run tx
npm run rx

```

## Ymodem protocol

```

SENDER                                                                                    RECEIVER 
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<              C
SOH 00 FF "foo.c" "1064'' NUL[118] CRC CRC >>>>>>>>>>>>>
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<            ACK
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<               C
STX 01 FE data[1024] CRC CRC>>>>>>>>>>>>>>>>>>>>>>>> 
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<             ACK
STX 02 FD data[1024] CRC CRC>>>>>>>>>>>>>>>>>>>>>>>
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<             ACK
STX 03 FC data[1024] CRC CRC>>>>>>>>>>>>>>>>>>>>>>>
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<             ACK
STX 04 FB data[1024] CRC CRC>>>>>>>>>>>>>>>>>>>>>>>
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<             ACK
SOH 05 FA data[100] 1A[28] CRC CRC>>>>>>>>>>>>>>>>>>
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<             ACK
EOT >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<             NAK
EOT>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<             ACK
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<               C
SOH 00 FF NUL[128] CRC CRC >>>>>>>>>>>>>>>>>>>>>>>
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<             ACK
```

## C version ymodem tx, same as 'npm run tx'
c-version/tx.c, posix c version ymodem tx

### Ccmake
```shell
$ mkdir build
$ cd build
$ cmake ..
$ make
$ 
$ ./tx -D /dev/ttyUSB0 -S 115200 -B ../bin/lamp421-0314-14.bin

```

* arguments parsing, getopt.h
* Linux serial port programming, 
* test on rasp pi 4, /dev/ttyUSB0


### make

```
$ make
$ ./output/tx -D /dev/ttyACM0 -S 115200 -B ../bin/lamp421-0322-ota.bin
```


## 生成OTA bin文件

