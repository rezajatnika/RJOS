#ifndef UDP_H
#define UDP_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    int sockfd;
    char *host;
    uint16_t port;
} udp_t;

/**
 *  @brief Initialize UDP socket for sending.
 *
 *  This function creates a UDP socket and stores the destination address.
 *  It duplicates the host string to ensure safe ownership.
 * @param udp Pointer to udp_t structure.
 * @param host Destination IP address (e.g. "127.0.0.1").
 * @param port Destination port.
 * @return 0 on success, -1 on failure.
 */
int udp_init(udp_t *udp, const char *host, uint16_t port);

/**
 *  @brief Send data over UDP.
 *
 *  This functions sends a buffer to the configured host and port.
 * @param udp Pointer to udp_t structure.
 * @param data Pointer to data buffer.
 * @param len Length of data.
 * @return Number of bytes sent, or -1 on failure.
 */
int udp_send(udp_t *udp, char *data, size_t len);

/**
 * @brief Close UDP socket and free resources
 *
 * This function closes the socket and frees the duplicated host string.
 *
 * @param udp Pointer to udp_t
 * @return 0 on success, -1 on failure
 */
int udp_close(udp_t *udp);

#endif
