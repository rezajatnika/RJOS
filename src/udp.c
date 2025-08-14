#include "udp.h"

#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

/**
 * Sets the file descriptor to non-blocking mode.
 *
 * @param fd The file descriptor to be set to non-blocking mode.
 * @return Returns 0 on success, or -1 in case of failure.
 */
static int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) {
        perror("set_nonblocking: fcntl");
        return -1;
    }
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) < 0) {
        perror("set_nonblocking: fcntl");
    }
    return 0;
}

/**
 * Sets the close-on-exec (FD_CLOEXEC) flag on a file descriptor.
 * This ensures that the file descriptor will be automatically closed
 * during an exec family function call.
 *
 * @param fd The file descriptor on which to set the close-on-exec flag.
 * @return Returns 0 on success, or -1 in case of failure.
 */
static int set_cloexec(int fd) {
    int flags = fcntl(fd, F_GETFD, 0);
    if (flags < 0) {
        perror("set_cloexec: fcntl");
        return -1;
    }
    if (fcntl(fd, F_SETFD, flags | FD_CLOEXEC) < 0) {
        perror("set_cloexec: fcntl");
        return -1;
    }
    return 0;
}

int udp_init(udp_t *udp, const char *host, uint16_t port) {
    if (!udp || !host) {
        perror("udp_init");
        return -1;
    }
    udp->sockfd = -1;
    udp->host = NULL;
    udp->port = 0;

    char service[16];
    int n = snprintf(service, sizeof(service), "%u", (unsigned)port);
    if (n < 0 || (size_t)n >= sizeof(service)) {
        perror("udp_init: snprintf");
        return -1;
    }

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    struct addrinfo *res = NULL;
    int rc = getaddrinfo(host, service, &hints, &res);
    if (rc != 0) {
        perror("udp_init: getaddrinfo");
        return -1;
    }

    int fd = -1;
    for (struct addrinfo *rp = res; rp != NULL; rp = rp->ai_next) {
        fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (fd < 0) {
            continue;
        }
        set_cloexec(fd);

        if (connect(fd, rp->ai_addr, rp->ai_addrlen) == 0) {
            break;
        }
        close(fd);
        fd = -1;
    }
    freeaddrinfo(res);

    if (fd < 0) {
        perror("udp_init: socket");
        return -1;
    }

    udp->host = strdup(host);
    if (udp->host == NULL) {
        perror("udp_init: strdup");
        return -1;
    }
    udp->sockfd = fd;
    udp->port   = port;
    return 0;
}

int udp_send(udp_t *udp, char *data, size_t len) {
    if (!udp || udp->sockfd < 0 || (!data && len > 0)) {
        perror("udp_send");
        return -1;
    }
    if (len == 0) {
        return 0;
    }
    for (;;) {
        ssize_t n = send(udp->sockfd, data, len, 0);
        if (n >= 0) {
            if (n > INT32_MAX) {
                return INT32_MAX;
            }
            return n;
        }
    }
}

int udp_recv(udp_t *udp, char *data, size_t len) {
    if (!udp || udp->sockfd < 0 || (!data && len > 0)) {
        perror("udp_recv");
        return -1;
    }
    if (len == 0) {
        return 0;
    }
    for (;;) {
        ssize_t n = recv(udp->sockfd, data, len, 0);
        if (n >= 0) {
            if (n > INT32_MAX) {
                return INT32_MAX;
            }
            return n;
        }
    }
}

int udp_close(udp_t *udp) {
    if (!udp) {
        perror("udp_close");
        return -1;
    }

    int rc = 0;
    if (udp->sockfd >= 0) {
        if (close(udp->sockfd) < 0) {
            rc = -1;
        }
        udp->sockfd = -1;
    }
    if (udp->host) {
        free(udp->host);
        udp->host = NULL;
    }
    return rc;
}
