#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "debug.h"
#include "validate.h"

#ifdef BUILD_PLATFORM_WIN32

//#include <winsock2.h>

/* validate_ip - Return true if arg addr is a valid ip address. */
bool
validate_ip(UNUSED const char * addr)
{
    return true; // Needs to be implemented in a Windows/MinGW compatible way.
}

#endif

#ifdef BUILD_PLATFORM_UNIX

#include <sys/socket.h>
#include <arpa/inet.h>

/* validate_ip - Return true if arg addr is a valid ipv4 address. */
bool
validate_ip(const char * addr)
{
    void * tmp;
    return inet_pton(AF_INET, addr, &tmp);
}

#endif

/* validate_mac - Return true if arg addr is a valid mac address. */
bool
validate_mac(const char * addr)
{
    if (strlen(addr) != 17) {
        return false;
    }
    for (int i = 0; addr[i]; i++) {
        char c = tolower(addr[i]);
        if (((c < '0' || c > '9') && (c < 'a' || c > 'f')) 
                && c != ':') {
            return false;
        }
    }
    return true;
}


/* validate_port - Return int version of arg port, return -1 on conversion error
 * or if port falls out of the valid range. */
int
validate_port(const char * port)
{
    char * end_ptr;
    int portno = (int) strtol(port, &end_ptr, 10);
    if (*end_ptr != '\0' || (PORT_MIN > portno || portno > PORT_MAX)) {
        return -1;
    }
    return portno;
}
