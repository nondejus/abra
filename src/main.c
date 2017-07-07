#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "debug.h"      /* debug.h: logging, platform defines */
#include "functions.h"  /* functions.h: main program subroutines */
#include "validate.h"   /* validate.h: input validation, platform regex */
#include "transmit.h"   /* transmit.h: platform socket functions */

#define DEFAULT_PORT "7"
#define DEFAULT_IP "255.255.255.255"

int
main(int argc, char * argv[])
{
    // Initialize global opts struct for program flow control.
    global_opts.help = false;       global_opts.info = false;
    global_opts.version = false;    global_opts.verbose = false;
    global_opts.ip = false;         global_opts.port = false;

    // Exit on arg parse failure.
    if (parse_args(argc, argv) == EXIT_FAILURE) {
        usage();
        exit(EXIT_FAILURE);
    }

    // Log option status.
    debug_log(FNAME, "help: %d\n", global_opts.help);
    debug_log(FNAME, "info: %d\n", global_opts.info);
    debug_log(FNAME, "version: %d\n", global_opts.version);
    debug_log(FNAME, "verbose: %d\n", global_opts.verbose);
    debug_log(FNAME, "ip: %d\n", global_opts.ip);
    debug_log(FNAME, "port: %d\n", global_opts.port);

    // Handle info, version, help options.
    if (global_opts.help) {
        help();
    }
    if (global_opts.info || global_opts.version) {
        build_info();
    }

    // Exit on necessary options.
    if (global_opts.info || global_opts.version || global_opts.help) {
        exit(EXIT_SUCCESS);
    }

    // Exit if getopt didn't leave any args unparsed.
    if (global_opts.optind == argc) {
        usage();
        exit(EXIT_FAILURE);
    }

    // MAC address validation.
    // Set dest_mac to the first arg not parsed by getopt.
    char * dest_mac = argv[global_opts.optind];
    if (!validate_mac(dest_mac)) {
        fprintf(stderr, "Error: '%s' is not a valid MAC address.\n", dest_mac);
        usage();
        exit(EXIT_FAILURE);
    } 
    
    // IP address validation.
    char * dest_ip = (global_opts.ip) ? global_opts.arg_ip : DEFAULT_IP;
    if (!validate_ip(dest_ip)) {
        fprintf(stderr, "Error: '%s' is not a valid IPv4 address.\n", dest_ip);
        usage();
        exit(EXIT_FAILURE);
    }
    
    // Port number validation.
    int dest_port = validate_port((global_opts.port) 
            ? global_opts.arg_port : DEFAULT_PORT);
    if (dest_port == -1) {
        fprintf(stderr, "Error: '%s' is not a valid port number.\n",
                global_opts.arg_port);
        usage();
        exit(EXIT_FAILURE);
    }

    // Log validated inputs.
    debug_log(FNAME, "dest_ip: %s\n", dest_ip);
    debug_log(FNAME, "dest_port: %d\n", dest_port);
    debug_log(FNAME, "dest_mac: %s\n", dest_mac);

    // Construct magic packet.
    unsigned char packet[MAGIC_LEN];
    magic_packet(packet, dest_mac);

    // Log packet contents.
    if (DEBUG_MODE) {
        debug_log(FNAME, "packet:\n");
        for (int i = 0; i < (int) MAGIC_LEN; i++) {
            printf("%3d ", (int) packet[i]);
            if ((i + 1) % 6 == 0) {
                putchar('\n');
            }
        }
    }

    // Transmit packet.
    if (send_data((char *) packet, MAGIC_LEN, dest_ip, dest_port)
            == EXIT_FAILURE) {
        fprintf(stderr, "Error: magic packet could not be sent.\n");
        exit(EXIT_FAILURE);
    }

    // Output success.
    printf("abra: magic packet sent!\n");
    if (global_opts.verbose) {
        printf("Summary:\n");
        printf("  Target: %s\n", dest_mac);
        printf("  Broadcast Route: %s\n", dest_ip);
        printf("  Port Number: %d\n", dest_port);
    }

    return EXIT_SUCCESS;
}
