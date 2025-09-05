#ifndef RJOS_UDP_H
#define RJOS_UDP_H

#include <stdint.h>

#define UDP_MAX_PAYLOAD 256

/**
 * @brief Structure representing a UDP packet.
 *
 * This structure is used to encapsulate the data and metadata
 * for a UDP packet, including the source address, source port,
 * data length, and the payload.
 */
typedef struct udp_packet {
    char    *src;
    uint16_t src_port;
    size_t   len;
    uint8_t  data[UDP_MAX_PAYLOAD];
} udp_packet_t;

/**
 * @brief Structure representing a UDP connection.
 *
 * This structure encapsulates the details required to manage
 * a UDP connection, including the socket file descriptor, the
 * host address, and the port number.
 */
typedef struct udp {
    int      sockfd;
    char    *host;
    uint16_t port;
} udp_t;

/**
 *  @brief Initialize a connected UDP socket for sending (and receiving replies).
 *
 *  Resolves the given host:port, creates a UDP socket, and connects it to that peer.
 *  On success, udp->sockfd is a valid connected datagram socket and udp owns a
 *  duplicated copy of host.
 *
 *  Blocking behavior: the socket is created in blocking mode.
 *
 *  @param udp  Pointer to udp_t structure (must not be NULL).
 *  @param host Destination IP or hostname (e.g. "127.0.0.1" or "localhost").
 *  @param port Destination port.
 *  @return 0 on success, -1 on failure (errno set).
 */
int udp_init(udp_t *udp, const char *host, uint16_t port);

/**
 *  @brief Send data over a connected UDP socket.
 *
 *  @param udp  Pointer to initialized udp_t.
 *  @param data Pointer to data buffer to send.
 *  @param len  Length of data.
 *  @return Number of bytes sent (>=0), or -1 on failure (errno set).
 */
int udp_send(udp_t *udp, char *data, size_t len);

/**
 * @brief Close UDP socket and free resources.
 *
 * Closes the socket (if open) and frees the duplicated host string.
 *
 * @param udp Pointer to udp_t.
 * @return 0 on success, -1 on failure (errno set). Returns 0 if socket was already closed.
 */
int udp_close(udp_t *udp);

/**
 * @brief Receive data from the connected UDP socket.
 *
 * Receives a single datagram (up to len bytes). If the datagram is larger than len,
 * the excess is discarded by the kernel.
 *
 * @param udp  Pointer to initialized udp_t.
 * @param data Destination buffer.
 * @param len  Buffer capacity in bytes.
 * @return Number of bytes received (>=0), or -1 on failure (errno set).
 */
int udp_recv(udp_t *udp, char *data, size_t len);

#endif
