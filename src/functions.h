#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define MAGIC_LEN (sizeof(unsigned char) * 102)

#include <stdbool.h>

// Globally accessed struct (only modified by parse_args function), used to
// control program flow based on which options are activated.
struct global_opts_t {
    bool help;		/* -h, --help */
    bool info;		/* -I, --info */
    bool version;       /* -v, --version */
    bool verbose;       /* -V, --verbose */
    bool ip;		/* -i, --ip-address */
    char * arg_ip;	/* destination ip address */
    bool port;		/* -p, --port */
    char * arg_port;	/* destination port */
    int optind;
} global_opts;

int parse_args(int argc, char * argv[]);
void magic_packet(unsigned char buffer[], const char * mac_str);
void usage(void);
void help(void);

#endif
