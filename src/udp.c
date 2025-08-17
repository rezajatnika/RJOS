#include "udp.h"
#include "logger.h"
#include "util/net_util.h"

#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int udp_init(udp_t *udp, const char *host, uint16_t port) {
    if (!udp || !host) {
        logger_log(LOG_LEVEL_ERROR, "udp_init: invalid arguments");
        return -1;
    }
    udp->sockfd = -1;
    udp->host = NULL;
    udp->port = 0;

    char service[16];
    int n = snprintf(service, sizeof(service), "%u", (unsigned)port);
    if (n < 0 || (size_t)n >= sizeof(service)) {
        logger_log(LOG_LEVEL_ERROR, "udp_init: snprintf failed");
        return -1;
    }

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    struct addrinfo *res = NULL;
    int rc = getaddrinfo(host, service, &hints, &res);
    if (rc != 0) {
        logger_log(LOG_LEVEL_ERROR, "udp_init: getaddrinfo failed");
        return -1;
    }

    int fd = -1;
    for (struct addrinfo *rp = res; rp != NULL; rp = rp->ai_next) {
        fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (fd < 0) {
            continue;
        }
        if (set_cloexec(fd) < 0 || connect(fd, rp->ai_addr, rp->ai_addrlen) < 0) {
            close(fd);
            fd = -1;
        }
    }
    freeaddrinfo(res);

    if (fd < 0) {
        logger_log(LOG_LEVEL_ERROR, "udp_init: failed to connect");
        return -1;
    }

    udp->host = strdup(host);
    if (udp->host == NULL) {
        logger_log(LOG_LEVEL_ERROR, "udp_init: strdup failed");
        close(fd);
        return -1;
    }
    udp->sockfd = fd;
    udp->port   = port;
    return 0;
}

int udp_send(udp_t *udp, char *data, size_t len) {
    if (!udp || udp->sockfd < 0 || (!data && len > 0)) {
        logger_log(LOG_LEVEL_ERROR, "udp_send: invalid arguments");
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
        logger_log(LOG_LEVEL_ERROR, "udp_recv: invalid arguments");
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
        logger_log(LOG_LEVEL_ERROR, "udp_close: invalid arguments");
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
