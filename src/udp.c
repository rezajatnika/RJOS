#include "udp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int udp_init(udp_t *udp, const char *host, uint16_t port) {
    if (udp == NULL) {
        perror("udp_init");
        return -1;
    }
    udp->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp->sockfd < 0) {
        perror("udp_init: socket");
        return -1;
    }
    udp->host = strdup(host);
    if (udp->host == NULL) {
        perror("udp_init: strdup");
        return -1;
    }
    udp->port = port;
    return 0;
}

int udp_send(udp_t *udp, char *data, size_t len) {
    if (!udp || !data) {
        perror("udp_send");
        return -1;
    }
    struct sockaddr_in dest = {
        .sin_family = AF_INET,
        .sin_port = htons(udp->port),
        .sin_addr.s_addr = inet_addr(udp->host),
    };
    ssize_t sent = sendto(udp->sockfd, data, len, 0, (struct sockaddr *)&dest, sizeof(dest));
    if (sent < 0) {
        perror("udp_send: sendto");
        return -1;
    }
    return sent;
}

int udp_close(udp_t *udp) {
    if (!udp) {
        perror("udp_close");
        return -1;
    }
    if (udp->sockfd >= 0) {
        if (close(udp->sockfd) < 0) {
            perror("udp_close: close");
            return -1;
        }
        udp->sockfd = -1;
    }
    free(udp->host);
    udp->host = NULL;
    return 0;
}
