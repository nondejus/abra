#ifndef VALIDATE_H
#define VALIDATE_H

#include <stdbool.h>
#include "debug.h"

// Valid port range constants.
#define PORT_MIN 1
#define PORT_MAX 65535

bool validate_mac(const char * addr);
bool validate_ip(const char * addr);
int validate_port(const char * port);

#endif
