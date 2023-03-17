#include <stdio.h>
#include <getopt.h>
#include <signal.h>
#include <stdlib.h>

#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <time.h>


#define SUCCESS        0
#define FAILURE_UART   2
#define FAILURE_ARGS   3
#define FAILURE_YMODEM 4
#define FAILURE_CONFIG -1

/* uart parameters */
static char *device = "/dev/ttyUSB0";
static int speed = 115200;
static int hardflow = 0;
static int verbose = 0;
static char *bin = "../bin/filename.bin.bin.bin.bin";
static FILE *fp;



static const struct option lopts[] = {
	{ "device", required_argument, 0, 'D' },
	{ "speed", optional_argument, 0, 'S' },
	{ "verbose", optional_argument, 0, 'v' },
	{ "hardflow", required_argument, 0, 'f' },
    { "bin", required_argument, 0, 'b'},
	{ NULL, 0, 0, 0 },
};

static void print_usage(const char *prog)
{
	printf("Usage: %s [-DSvfb]\n", prog);
	puts("  -D --device    tty device to use\n"
		 "  -S --speed     uart speed\n"
		 "  -v --verbose   Verbose (show rx buffer)\n"
		 "  -f --hardflow  open hardware flowcontrol\n"
         "  -b --bin       bin file\n");
	exit(1);
}


static void parse_opts(int argc, char *argv[])
{
	int c;
	
	while (1) {
		c = getopt_long(argc, argv, "D:S:vfbh", lopts, NULL);
		if (c == -1) {
			break;
		}
		switch (c) {
		case 'D':
			if (optarg != NULL)
				device = optarg;
			break;
		case 'S':
			if (optarg != NULL)
				speed = atoi(optarg);
			break;
		case 'v':
			verbose = 1;
			break;
		case 'f':
			hardflow = 1;
			break;	
        case 'b':
            if (optarg != NULL)
                bin = optarg;
            break;
		case 'h':
		default:
			print_usage(argv[0]);
			break;
		}
	}
}

/**
 * libtty_setcustombaudrate - set baud rate of tty device
 * @fd: device handle
 * @speed: baud rate to set
 *
 * The function return 0 if success, or -1 if fail.
 */
// static int libtty_setcustombaudrate(int fd, int baudrate)
// {
// 	struct termios2 tio;

// 	if (ioctl(fd, TCGETS2, &tio)) {
// 		perror("TCGETS2");
// 		return -1;
// 	}

// 	tio.c_cflag &= ~CBAUD;
// 	tio.c_cflag |= BOTHER;
// 	tio.c_ispeed = baudrate;
// 	tio.c_ospeed = baudrate;

// 	if (ioctl(fd, TCSETS2, &tio)) {
// 		perror("TCSETS2");
// 		return -1;
// 	}

// 	if (ioctl(fd, TCGETS2, &tio)) {
// 		perror("TCGETS2");
// 		return -1;
// 	}

// 	return 0;
// }

/**
 * libtty_setcustombaudrate - set baud rate of tty device
 * @fd: device handle
 * @speed: baud rate to set
 *
 * The function return 0 if success, or -1 if fail.
 */
/**
 * libtty_setopt - config tty device
 * @fd: device handle
 * @speed: baud rate to set
 * @databits: data bits to set
 * @stopbits: stop bits to set
 * @parity: parity to set
 * @hardflow: hardflow to set
 *
 * The function return 0 if success, or -1 if fail.
 */
static int libtty_setopt(int fd, int speed, int databits, int stopbits, char parity, char hardflow)
{
	struct termios newtio;
	struct termios oldtio;
	int i;
	speed_t baudrate;
	
	bzero(&newtio, sizeof(newtio));
	bzero(&oldtio, sizeof(oldtio));
	
	if (tcgetattr(fd, &oldtio) != 0) {
		perror("tcgetattr");    
		return -1; 
	}
	newtio.c_cflag |= CLOCAL | CREAD;
	newtio.c_cflag &= ~CSIZE;
 
	/* set data bits */
	switch (databits) {
	case 5:                
		newtio.c_cflag |= CS5;
		break;
	case 6:                
		newtio.c_cflag |= CS6;
		break;
	case 7:                
		newtio.c_cflag |= CS7;
		break;
	case 8:    
		newtio.c_cflag |= CS8;
		break;  
	default:   
		fprintf(stderr, "unsupported data size\n");
		return -1; 
	}
	
	/* set parity */
	switch (parity) {  
	case 'n':
	case 'N':
		newtio.c_cflag &= ~PARENB;    /* Clear parity enable */
		newtio.c_iflag &= ~INPCK;     /* Disable input parity check */
		break; 
	case 'o':  
	case 'O':    
		newtio.c_cflag |= (PARODD | PARENB); /* Odd parity instead of even */
		newtio.c_iflag |= INPCK;     /* Enable input parity check */
		break; 
	case 'e': 
	case 'E':  
		newtio.c_cflag |= PARENB;    /* Enable parity */   
		newtio.c_cflag &= ~PARODD;   /* Even parity instead of odd */  
		newtio.c_iflag |= INPCK;     /* Enable input parity check */
		break;
	default:  
		fprintf(stderr, "unsupported parity\n");
		return -1; 
	} 
	
	/* set stop bits */ 
	switch (stopbits) {  
	case 1:   
		newtio.c_cflag &= ~CSTOPB; 
		break;
	case 2:   
		newtio.c_cflag |= CSTOPB; 
		break;
	default:   
		perror("unsupported stop bits\n"); 
		return -1;
	}
 
	if (hardflow)
		newtio.c_cflag |= CRTSCTS;
	else
		newtio.c_cflag &= ~CRTSCTS;
 
	newtio.c_cc[VTIME] = 10;	/* Time-out value (tenths of a second) [!ICANON]. */
	newtio.c_cc[VMIN] = 0;	/* Minimum number of bytes read at once [!ICANON]. */

	switch(speed){
		case 9600:
			baudrate = B9600;
			break;
		case 115200:
		default:
			baudrate = B115200;
			break;
	}

	cfsetispeed(&newtio, baudrate);
	cfsetospeed(&newtio, baudrate);
	
	tcflush(fd, TCIOFLUSH);  
	
	if (tcsetattr(fd, TCSANOW, &newtio) != 0) {
		perror("tcsetattr");
		return -1;
	}

	/* set tty speed */
	// if (libtty_setcustombaudrate(fd, speed) != 0) {
	// 	perror("setbaudrate");
	// 	return -1;
	// }




	return 0;
}

static int libtty_open(const char *devname)
{
	int fd = open(devname, O_RDWR | O_NOCTTY | O_NDELAY); 
	int flags = 0;
	
	if (fd < 0) {                        
		perror("open device failed");
		return -1;            
	}
	
	flags = fcntl(fd, F_GETFL, 0);
	flags &= ~O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flags) < 0) {
		printf("fcntl failed.\n");
		return -1;
	}
		
	if (isatty(fd) == 0) {
		printf("not tty device.\n");
		return -1;
	}
	else
		printf("tty device test ok.\n");
	
	return fd;
}
static void sig_handler(int signo)
{
    printf("capture sign no:%d\n",signo);
	if (fp != NULL) {
		fflush(fp);
		fsync(fileno(fp));
		fclose(fp);
	}
	exit(0);
}

int main(int argc, char *argv[]){
    int fd;
	int ret;
	int times = 50;

    printf("hello tx ymodem\n");
    printf("parse input args:\n");

    parse_opts(argc, argv);

    printf("device: %s\n", device);
    printf("speed: %d\n",  speed);
    printf("verbose: %d\n", verbose);
    printf("hardflow: %d\n", hardflow);
    printf("bin: %s\n", bin);

    signal(SIGINT, sig_handler); 

    fd = libtty_open(device);
	if (fd < 0) {
		printf("libtty_open: %s error.\n", device);
		exit(FAILURE_UART);
	}

	ret = libtty_setopt(fd, speed, 8, 1, 'n', hardflow);
	if (ret != 0) {
		printf("libtty_setopt error.\n");
		exit(FAILURE_UART);
	}

	while(times--){
		printf("send a byte\n");
		write(fd, "33", 2);
		sleep(1);
	}


    return SUCCESS;
}