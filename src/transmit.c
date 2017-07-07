#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
#include "transmit.h"

// Windows function.
#ifdef BUILD_PLATFORM_WIN32

#include <winsock2.h>

/* send_data - Transmits arg data through a Windows socket. */
int
send_data(const char * data, const int data_len, 
        const char * address, const int port)
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        fprintf(stderr, "Windows Socket could not be initialized.\n");
        return EXIT_FAILURE;
    }

    SOCKET sock;
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
        fprintf(stderr, "Windows Socket could not be initialized.\n");
        return EXIT_FAILURE;
    }

    struct sockaddr_in dest;
    dest.sin_addr.s_addr = inet_addr(address);
    dest.sin_family = AF_INET;
    dest.sin_port = htons(port);

    debug_log(FNAME, "dest.sin_addr.s_addr: %s\n", inet_ntoa(dest.sin_addr));
    debug_log(FNAME, "dest.sin_family: %d\n", dest.sin_family);
    debug_log(FNAME, "dest.sin_port: %d\n", ntohs(dest.sin_port));

    if (connect(sock, (struct sockaddr *) &dest, sizeof(dest))
            == SOCKET_ERROR) {
        fprintf(stderr, "Windows Socket connection could not be "
                "established.\n");
        return EXIT_FAILURE;
    }

    int bytes_sent;
    if ((bytes_sent = send(sock, data, data_len, 0)) == SOCKET_ERROR) {
        fprintf(stderr, "Data could not be sent through the "
                "socket.\n");
        return EXIT_FAILURE;
    } else if (bytes_sent < data_len) {
        fprintf(stderr, "Only %d bytes (of %d) were sent.\n",
                bytes_sent, data_len);
        return EXIT_FAILURE;
    }

    closesocket(sock);

    return EXIT_SUCCESS;
}

#endif

// UNIX function.
#ifdef BUILD_PLATFORM_UNIX

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

/* send_data - Transmits arg data through a UNIX socket. */
int
send_data(const char * data, const int data_len,
        const char * address, const int port)
{
    int sock;
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("UNIX Socket could not be initialized");
        return EXIT_FAILURE;
    }

    int mode = 1; // Enable send to broadcast address.
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST,
                &mode, sizeof(mode)) == -1) {
        perror("UNIX Socket broadcast option could not be enabled");
        return EXIT_FAILURE;
    }

    struct sockaddr_in dest;
    dest.sin_addr.s_addr = inet_addr(address);
    dest.sin_family = AF_INET;
    dest.sin_port = htons(port);

    debug_log(FNAME, "dest.sin_addr.s_addr: %s\n", inet_ntoa(dest.sin_addr));
    debug_log(FNAME, "dest.sin_family: %d\n", dest.sin_family);
    debug_log(FNAME, "dest.sin_port: %d\n", ntohs(dest.sin_port));

    if ((connect(sock, (struct sockaddr *) &dest, sizeof(dest))) == -1) {
        perror("UNIX Socket connection could not be established");
        return EXIT_FAILURE;
    }

    int bytes_sent;
    if ((bytes_sent = send(sock, data, data_len, 0)) == -1) {
        perror("Data could not be sent through the socket");
        return EXIT_FAILURE;
    } else if (bytes_sent < data_len) {
        fprintf(stderr, "Only %d bytes (of %d) were sent.\n",
                bytes_sent, data_len);
        return EXIT_FAILURE;
    }

    close(sock);

    return EXIT_SUCCESS;
}

#endif
