#include <stdio.h>
#include <getopt.h>
#include <signal.h>
#include <stdlib.h>

#define SUCCESS        0
#define FAILURE_UART   1
#define FAILURE_ARGS   2
#define FAILURE_YMODEM 3


/* uart parameters */
static const char *device = "/dev/ttyUSB0";
static int speed = 115200;
static int hardflow = 0;
static int verbose = 0;
static FILE *fp;



static const struct option lopts[] = {
	{ "device", required_argument, 0, 'D' },
	{ "speed", optional_argument, 0, 'S' },
	{ "verbose", optional_argument, 0, 'v' },
	{ "hardflow", required_argument, 0, 'f' },
	{ NULL, 0, 0, 0 },
};

static void print_usage(const char *prog)
{
	printf("Usage: %s [-DSvf]\n", prog);
	puts("  -D --device    tty device to use\n"
		 "  -S --speed     uart speed\n"
		 "  -v --verbose   Verbose (show rx buffer)\n"
		 "  -f --hardflow  open hardware flowcontrol\n");
	exit(1);
}


static void parse_opts(int argc, char *argv[])
{
	int c;
	
	while (1) {
		c = getopt_long(argc, argv, "D:S:vfh", lopts, NULL);
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
		case 'h':
		default:
			print_usage(argv[0]);
			break;
		}
	}
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
    printf("hello tx ymodem\n");
    printf("parse input args:\n");

    parse_opts(argc, argv);

    printf("device: %s\n", device);
    printf("speed: %d\n",  speed);
    printf("verbose: %d\n", verbose);
    printf("hardflow: %d\n", hardflow);

    signal(SIGINT, sig_handler); 


    return SUCCESS;
}