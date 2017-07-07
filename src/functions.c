#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include "debug.h"
#include "functions.h"

// getopt and getopt_long constants.
static const char * short_opts = "hvIVi:p:"; /* Defines short opts/args */
static const struct option long_opts[] = {
    { "help", no_argument, NULL, 'h' },              /* -h, --help */
    { "info", no_argument, NULL, 'I' },              /* -I, --info */
    { "verbose", no_argument, NULL, 'V' },           /* -V, --verbose */
    { "version", no_argument, NULL, 'v' },           /* -v, --version */
    { "ip-address", required_argument, NULL, 'i' },  /* -i, --ip-address */
    { "port", required_argument, NULL, 'p' },        /* -p, --port */
    { NULL, no_argument, NULL, 0 }
};

/* parse_args - Iterates through command line arg list, turns on necessary
 * booleans in the global_opts struct (used throughout program to change flow),
 * finally assigns the first optionless arg that wasn't parsed by getopt to
 * global_opts.arg_mac variable. */
int
parse_args(int argc, char * argv[])
{
    int opt = 0, long_index = 0, count = 0;
    while ((opt = getopt_long(argc, argv, short_opts,
                    long_opts, &long_index)) != -1) {
        count++;
        switch (opt) {
            case 'h':		/* -h, --help */
                global_opts.help = true;
                break;

            case 'I':		/* -I, --info */
                global_opts.info = true;
                break;

            case 'v':           /* -v, --version */
                global_opts.version = true;
                break;

            case 'V':           /* -V, --verbose */
                global_opts.verbose = true;
                break;

            case 'i':		/* -i, --ip-address */
                global_opts.ip = true;
                global_opts.arg_ip = optarg;
                break;

            case 'p':		/* -p, --port */
                global_opts.port = true;
                global_opts.arg_port = optarg;
                break;

            default: return EXIT_FAILURE;
        }	
    }

    // Set struct var to position of first arg not parsed by getopt.
    global_opts.optind = optind;

    return EXIT_SUCCESS;
}

/* magic_packet - Fills arg buffer with the necessary magic packet contents
 * (first 6 bytes FF, then 16 repetitions of arg mac_str's hex values in
 * int form). */
void
magic_packet(unsigned char buffer[], const char * mac_str)
{
    unsigned int mac_bytes[6];
    sscanf(mac_str, "%x:%x:%x:%x:%x:%x", &mac_bytes[0], &mac_bytes[1],
            &mac_bytes[2], &mac_bytes[3], &mac_bytes[4], &mac_bytes[5]);

    for (int i = 0; i < 6; i++) {
        buffer[i] = 0xFF;
    }
    int offset = 6;
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 6; j++) {
            buffer[i * 6 + j + offset] = mac_bytes[j];
        }
    }
}

/* usage - Displays usage instructions to stdout. */
void
usage(void)
{
    puts("\
Usage: abra [-hvI] [-i ip-address] [-p port] mac-address\n\
Try 'abra --help' for more more information.\n");
}

/* help - Display help instructions to stdout. */
void
help(void)
{
    puts("\
Abra - Send a magic packet.\n\
\n\
Usage:\n\
  abra [-hvI] [-i ip-address] [-p port] mac-address\n\
\n\
Mandatory Arguments:\n\
  mac-address                 48-bit target MAC address.\n\
\n\
Optional Arguments:\n\
  -i, --ip-address=ADDRESS    Target broadcast address (ipv4),\n\
                              defaults to 255.255.255.255.\n\
  -p, --port=PORT             Target port,\n\
                              defaults to 7.\n\
\n\
Options:\n\
  -h, --help                  Display help information and exit.\n\
  -v, --version               Display build information and exit.\n\
  -I, --info                  Same as -V, --version.\n\
  -V, --verbose               Display a detailed summary on success.\n\
\n\
Example:\n\
  abra -i 192.168.1.255 -p 9 E9:2B:99:3C:00:15\n\
\n");
}
